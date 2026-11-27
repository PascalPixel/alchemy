#ifndef ALCHEMY_TEXT_RENDER_H
#define ALCHEMY_TEXT_RENDER_H

/* @file */
/* Drawing text into tiles of its own in a window, rather than as sprites. */

#include "edition.h"
#include "render_input.h"

s32 mnTextDrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x);

#endif
