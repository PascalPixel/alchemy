#ifndef ALCHEMY_UNIT_WINDOW_PTYSTAT_H
#define ALCHEMY_UNIT_WINDOW_PTYSTAT_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct RenderInput;

void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_CreateWithLayoutBounds(s32 flags);
void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags);
s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value);
void PtyStDraw(s32 flags);
void UiWork_FinalizeAndReleaseBlock16(void);
void UiWindow_SetTileAttributeBitRect(const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field);

#endif
