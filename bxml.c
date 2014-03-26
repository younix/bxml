/*
 * Copyright (c) 2014 Jan Klemkow <j.klemkow@wemelug.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bxml.h"

struct bxml_ctx *
bxml_ctx_init(void (*callback)(char *))
{
	struct bxml_ctx *ctx = calloc(1, sizeof *ctx);

	ctx->depth = 0;
	ctx->block_depth = 0;
	ctx->status = OUT;
	if ((ctx->buf = calloc(1, BUFSIZ)) == NULL) {
		free(ctx);
		return NULL;
	}
	ctx->size = BUFSIZ;
	ctx->idx = 0;
	ctx->callback = callback;

	return ctx;
}

void
bxml_ctx_free(struct bxml_ctx* ctx)
{
	if (ctx == NULL)
		return;

	if (ctx->buf != NULL)
		free(ctx->buf);

	free(ctx);
}

bool
bxml_add_str(struct bxml_ctx *ctx, char *string)
{
	static char q; /* saves quoting character */

	for (char *c = &string[0]; *c != '\0'; c++) {

		/* keep enough space for char and \0 */
		if (ctx->size - ctx->idx < 2) {
			char *tmp;
			if ((tmp = realloc(ctx->buf, ctx->size * 2)) == NULL)
				return false;
			ctx->buf = tmp;
			ctx->size *= 2;
		}

		/* add current char and terminating \0 to buffer */
		ctx->buf[ctx->idx] = *c;
		ctx->idx++;
		ctx->buf[ctx->idx] = '\0';

		switch (ctx->status) {
		case OUT:
			if (*c == '<') ctx->status = IN;
			break;
		case IN:
			switch (*c) {
			case '?': ctx->status = IN_DECL;	break;
			case '!': ctx->status = IN_PRE_COMMENT;	break;
			case '/': ctx->status = IN_TAG_CLOSING;	break;
			default:  ctx->status = IN_TAG;		break;
			}
			break;
		case IN_TAG:
			switch (*c) {
			case '>': ctx->status = OUT; ctx->depth++; break;
			case '"': ctx->status = IN_QUOTE; q=*c;    break;
			case '\'':ctx->status = IN_QUOTE; q=*c;    break;
			case '/': ctx->status = IN_TAG_SELFCLOSE;  break;
			}
			break;
		case IN_TAG_CLOSING:
			if (*c != '>') break;
			ctx->depth--;
		case IN_TAG_SELFCLOSE:
			if (*c == '>') {
				if (ctx->depth == ctx->block_depth) {
					(*ctx->callback)(ctx->buf);
					ctx->idx = 0;
				}
				ctx->status = OUT;
			} else {
				ctx->status = IN_TAG;
			}
			break;
		case IN_PRE_COMMENT:
			/* XXX: we should check <!--> */
		case IN_COMMENT:
			if (*c == '-') ctx->status = IN_COMMENT_MINUS;
			break;
		case IN_COMMENT_MINUS:
			if (*c == '-') ctx->status = IN_COMMENT_MINUS_MINUS;
			else           ctx->status = IN_COMMENT;
			break;
		case IN_COMMENT_MINUS_MINUS:
			if (*c == '>') ctx->status = OUT;
			else           ctx->status = IN_COMMENT;
			break;
		case IN_DECL:
			if (*c == '?') ctx->status = IN_DECL_QUESTIONMARK;
			break;
		case IN_DECL_QUESTIONMARK:
			if (*c == '>') ctx->status = OUT;
			else           ctx->status = IN_DECL;
			break;
		case IN_QUOTE:
			if (*c == q)    ctx->status = IN_TAG;
			if (*c == '\\') ctx->status = IN_QUOTE_ESC;
			break;
		case IN_QUOTE_ESC:
			ctx->status = IN_QUOTE;
			break;
		}
	}

	return true;
}
