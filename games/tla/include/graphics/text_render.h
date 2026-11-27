#ifndef ALCHEMY_TEXT_RENDER_H
#define ALCHEMY_TEXT_RENDER_H

/* Drawing text into tiles of its own in a window, rather than as sprites. */

#include "edition.h"
#include "render_input.h"

Sint32 mnTextDrawStringAsTiles(
	Uint16 *text, Uint16 *tiles, Uint16 *mirror, Sint32 x);

#endif
