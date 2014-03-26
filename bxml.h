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

enum xml_state {
	OUT,			/*		*/
	IN,			/* <		*/
	IN_TAG,			/* <  ...	*/
	IN_TAG_CLOSING,		/* </		*/
	IN_TAG_SELFCLOSE,	/* <  ... /	*/
	IN_PRE_COMMENT,		/* NOT IMPLEMENTED!!! */
	IN_COMMENT,		/* <!		*/
	IN_COMMENT_MINUS,	/* <! ... -	*/
	IN_COMMENT_MINUS_MINUS, /* <! ... --	*/
	IN_DECL,		/* <?		*/
	IN_DECL_QUESTIONMARK,	/* <? ... ?	*/
	IN_QUOTE,		/* <  ... "	*/
	IN_QUOTE_ESC		/* <  ... "...\	*/
};

struct xml_ctx {
	int depth;
	int block_depth;
	enum xml_state status;
	char *buf;
	size_t size;
	unsigned int idx;
	void (*callback)(char *);
};

struct xml_ctx *xml_ctx_init(void (*callback)(char *));
void		xml_ctx_free(struct xml_ctx *ctx);
bool		xml_add_str (struct xml_ctx *ctx, char *string);
