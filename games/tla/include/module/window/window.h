#ifndef ALCHEMY_MODULE_WINDOW_WINDOW_H
#define ALCHEMY_MODULE_WINDOW_WINDOW_H

/// @file
/// What games/tla/src/window/ declares for the files that use it, written once.

#include "types.h"

struct NameCursorMotion;
struct Window;

void Message_CloseFinishedWindows(void);

s32 Message_AllTyped(void);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void WindowSprite_RedrawSavedRect(struct Window *window);

void MessageArgument_ClearAll(void);

extern const u8 gData_0805c9c4[];

extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];

void Text_SetColor(s32 param);

void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);

s32 Text_GetWideStringWidth(u16 *text);

s32 PauseMenu_Choose(s32 initial);

s32 Func_08043690(void);

s32 SettingsMenu_Run(void);

void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse);

s32 Func_0804b8b8(s32 slot);

void BattleParty_CountStandbyDjinnFar(s32 kind, u8 *counts);

void Window_MarkVisibleTextTiles(void);

s32 Func_0804519c(void);

void WindowSprite_PrepareForRedraw(struct Window *);

extern s16 gData_020036d8;
extern s16 gData_02005350;

s32 Func_080432a4(void);

extern s32 gKeyState;

void Text_UseExtraTiles(s32 alt);

s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
s32 Message_DecodeMode1(s32 message);
void Func_0803cf6c(s32 message);

void MenuCursor_AdvanceFar(struct NameCursorMotion *cursor);
void MenuCursor_MoveTowardTargetFar(struct NameCursorMotion *cursor);

void Portrait_CloseWindow(s32 id);

u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);

void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);

void MessageArgument_Push(u32 value, u32 kind);

void Func_08043b70(void);

extern const u8 gResource_FixedBlockBTiles[];

void Text_FillCanvasBackground(void);

#endif
