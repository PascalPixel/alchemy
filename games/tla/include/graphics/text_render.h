#ifndef ALCHEMY_TEXT_RENDER_H
#define ALCHEMY_TEXT_RENDER_H

/// @file
/// Drawing text into tiles of its own in a window, rather than as sprites.

#include "edition.h"
#include "render_input.h"

s32 Text_DrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x);

void Text_DrawMessageAsTiles(s32 message, struct Window *window, u32 offsetX,
                             u32 offsetY);

void Text_DrawStringAtOffset(u8 *text, struct Window *window, s32 offsetX,
                             s32 offsetY);
void Text_DrawNumberAtOffset(s32 value, s32 width, struct Window *window,
                             s32 offsetX, s32 offsetY);

#endif
