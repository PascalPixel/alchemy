#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_WINTILE_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_WINTILE_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct RenderInput;

void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap);
s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x);
/* UiWindow_ClearTileAttributesInRect is called before its definition with no prototype in scope. */
void UiWindow_MarkVisibleTileAttributes(void);
void UiWork_SetAltFlagAndClearTable(s32 flag);
void UiRender_ReservedNoOp(void);
void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2, u32 y2);
void UiWork_SetParamNibble(s32 param);
void UiWork_SetRenderWord(u16 value);
void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void UiText_DrawCharacterAtOffset(s32 character, struct RenderInput *position, u32 offset_x, u32 offset_y);
void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3);
void UiText_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offset_x, s32 offset_y);
void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void UiText_DrawNumberAtOffset(s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4);
void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
/* UiText_DrawPrefixedNumberAtOffset is called before its definition with no prototype in scope. */

#endif
