#ifndef ALCHEMY_TEXT_RENDER_H
#define ALCHEMY_TEXT_RENDER_H

#include "edition.h"
#include "render_input.h"

s32 Text_DrawStringAsTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x);

void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *work,
                                u32 offsetX, u32 offsetY);

void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                             s32 offsetY);
void Text_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *work,
                             s32 offsetX, s32 offsetY);

#endif
