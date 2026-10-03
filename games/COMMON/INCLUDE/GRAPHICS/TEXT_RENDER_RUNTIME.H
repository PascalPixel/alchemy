#ifndef ALCHEMY_TEXT_RENDER_RUNTIME_H
#define ALCHEMY_TEXT_RENDER_RUNTIME_H

#include "RENDER_INPUT.H"

void UiText_DrawCharacterAtOffset(
    s32 character,
    struct RenderInput *work,
    u32 offset_x,
    u32 offset_y);

void UiText_DrawStringAtOffset(
    u8 *text,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y);
void UiText_DrawNumberAtOffset(
    s32 value,
    s32 format,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y);

#endif
