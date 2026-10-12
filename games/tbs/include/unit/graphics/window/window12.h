#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_WINDOW12_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_WINDOW12_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct UiWindow;

void Scheduler_ScheduleCallbackCAfterFrames(void);
void Menu_DrawSelectionRow(struct UiWindow *work, s32 first, const s16 *second);
s16 Menu_RunSelection(void);
s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode);
s32 Menu_RunSelectionWithCursorObject(void);
void Text_FormatHex(u32 value, u32 count, u8 *out);
void Menu_DrawFlagBitTable(s32 window, s32 start_flag);
s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor);
void Menu_ReservedNoOp(void);
void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index);
s32 Menu_RunWorkspaceResultLoop(void);
s32 DebugMenu_BrowseIcons(void);
s32 DebugMenu_BrowseEntryGlyphs(void);

#endif
