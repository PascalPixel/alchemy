#ifndef ALCHEMY_TEXT_RENDER_RUNTIME_H
#define ALCHEMY_TEXT_RENDER_RUNTIME_H

#include "types.h"

struct TextRenderWork {
    u8 unknown_00[12];
    u16 x;
    u16 y;
};

void UiText_DrawStringAtOffset(
    u8 *text,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y);
void UiText_DrawNumberAtOffset(
    s32 value,
    s32 format,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y);

#endif
