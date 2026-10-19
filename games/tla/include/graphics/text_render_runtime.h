#ifndef ALCHEMY_TEXT_RENDER_RUNTIME_H
#define ALCHEMY_TEXT_RENDER_RUNTIME_H

#include "edition.h"
#include "render_input.h"

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x);

void UiText_DrawCharacterAtOffset(s32 character, struct RenderInput *work,
                                  u32 offsetX, u32 offsetY);

void UiText_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                               s32 offsetY);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *work,
                               s32 offsetX, s32 offsetY);

#endif
