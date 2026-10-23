/*
 * resource.h
 *
 * Private interfaces between resource recipe families. Parsers borrow their
 * input; each returned Buffer or Strings list belongs to its caller.
 */
#ifndef AGS_RESOURCE_H
#define AGS_RESOURCE_H
#include "graphics.h"

Buffer readSibling(const char *input, const char *name);
Strings linesOf(const Buffer *input);
Buffer table(const Buffer *input);
Buffer plane(const Buffer *input);
Buffer grid(const Buffer *input);
Buffer script(const Buffer *input);
Buffer blocks(const Buffer *input);
Buffer imageForm(const char *input, const char *form, const Buffer *data);
Buffer mappedPart(const char *input, const char *path, const char *form,
				  const Strings *fields);
#endif
