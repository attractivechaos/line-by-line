/* The MIT License

   Copyright (c) 2008, 2009, 2011 Attractive Chaos <attractor@live.co.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/* Last Modified: 2017-02-11 */

#ifndef AC_KSEQ_H
#define AC_KSEQ_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define KS_SEP_SPACE 0 // isspace(): \t, \n, \v, \f, \r
#define KS_SEP_TAB   1 // isspace() && !' '
#define KS_SEP_LINE  2 // line separator: "\n" (Unix) or "\r\n" (Windows)
#define KS_SEP_MAX   2

#define __KS_TYPE(type_t) \
	typedef struct __kstream_t { \
		unsigned char *buf; \
		int begin, end, is_eof; \
		type_t f; \
	} kstream_t;

#define ks_err(ks) ((ks)->end < 0)
#define ks_eof(ks) ((ks)->is_eof && (ks)->begin >= (ks)->end)
#define ks_rewind(ks) ((ks)->is_eof = (ks)->begin = (ks)->end = 0)

#define __KS_BASIC(type_t, __bufsize) \
	static inline kstream_t *ks_init(type_t f) \
	{ \
		kstream_t *ks = (kstream_t*)calloc(1, sizeof(kstream_t)); \
		ks->f = f; \
		ks->buf = (unsigned char*)malloc(__bufsize); \
		return ks; \
	} \
	static inline void ks_destroy(kstream_t *ks) \
	{ \
		if (ks) { \
			free(ks->buf); \
			free(ks); \
		} \
	}

#define __KS_GETC(__read, __bufsize) \
	static inline int ks_getc(kstream_t *ks) \
	{ \
		if (ks_err(ks)) return -3; \
		if (ks_eof(ks)) return -1; \
		if (ks->begin >= ks->end) { \
			ks->begin = 0; \
			ks->end = __read(ks->f, ks->buf, __bufsize); \
			if (ks->end == 0) { ks->is_eof = 1; return -1; } \
			else if (ks->end < 0) { ks->is_eof = 1; return -3; } \
		} \
		return (int)ks->buf[ks->begin++]; \
	}

#ifndef KSTRING_T
#define KSTRING_T kstring_t
typedef struct __kstring_t {
	size_t l, m;
	char *s;
} kstring_t;
#endif

#ifndef kroundup32
#define kroundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))
#endif

#ifndef kroundup64
#define kroundup64(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, (x)|=(x)>>32, ++(x))
#endif

#define __KS_GETUNTIL(__read, __bufsize) \
	static int64_t ks_getuntil2(kstream_t *ks, int delimiter, kstring_t *str, int *dret, int append) \
	{ \
		int gotany = 0; \
		if (dret) *dret = 0; \
		str->l = append? str->l : 0; \
		for (;;) { \
			int i; \
			if (ks_err(ks)) return -3; \
			if (ks->begin >= ks->end) { \
				if (!ks->is_eof) { \
					ks->begin = 0; \
					ks->end = __read(ks->f, ks->buf, __bufsize); \
					if (ks->end == 0) { ks->is_eof = 1; break; } \
					if (ks->end == -1) { ks->is_eof = 1; return -3; } \
				} else break; \
			} \
			if (delimiter == KS_SEP_LINE) { \
				unsigned char *sep = (unsigned char*)memchr(ks->buf + ks->begin, '\n', ks->end - ks->begin); \
				i = sep != NULL ? sep - ks->buf : ks->end; \
			} else if (delimiter > KS_SEP_MAX) { \
				for (i = ks->begin; i < ks->end; ++i) \
					if (ks->buf[i] == delimiter) break; \
			} else if (delimiter == KS_SEP_SPACE) { \
				for (i = ks->begin; i < ks->end; ++i) \
					if (isspace(ks->buf[i])) break; \
			} else if (delimiter == KS_SEP_TAB) { \
				for (i = ks->begin; i < ks->end; ++i) \
					if (isspace(ks->buf[i]) && ks->buf[i] != ' ') break; \
			} else i = 0; /* never come to here! */ \
			if (str->m - str->l < (size_t)(i - ks->begin + 1)) { \
				str->m = str->l + (i - ks->begin) + 1; \
				kroundup64(str->m); \
				str->s = (char*)realloc(str->s, str->m); \
			} \
			gotany = 1; \
			memcpy(str->s + str->l, ks->buf + ks->begin, i - ks->begin); \
			str->l = str->l + (i - ks->begin); \
			ks->begin = i + 1; \
			if (i < ks->end) { \
				if (dret) *dret = ks->buf[i]; \
				break; \
			} \
		} \
		if (!gotany && ks_eof(ks)) return -1; \
		if (str->s == 0) { \
			str->m = 1; \
			str->s = (char*)calloc(1, 1); \
		} else if (delimiter == KS_SEP_LINE && str->l > 1 && str->s[str->l-1] == '\r') --str->l; \
		str->s[str->l] = '\0'; \
		return str->l; \
	} \
	static inline int ks_getuntil(kstream_t *ks, int delimiter, kstring_t *str, int *dret) \
	{ return ks_getuntil2(ks, delimiter, str, dret, 0); }

#define KSTREAM_INIT(type_t, __read, __bufsize) \
	__KS_TYPE(type_t) \
	__KS_BASIC(type_t, __bufsize) \
	__KS_GETC(__read, __bufsize) \
	__KS_GETUNTIL(__read, __bufsize)

#endif
