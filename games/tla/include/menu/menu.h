#ifndef MENU_H
#define MENU_H

/// @file
/// MENU

#include "types.h"
#include "edition.h"

extern u8 gStatusScreen_CursorWidths[];
extern u8 gMenu_ListOrderDefault[];
extern u8 gMenu_ListOrderMode0[];
extern u8 gMenu_ListOrderMode1[];
extern u8 gMenu_ListOrderMode2[];
extern u8 gMenu_MinusSignString;
extern u8 gMenu_PlusSignString;
extern s8 gMenu_TopEntryCommandByPosition[];
extern s8 gMenu_TopEntryPositionByCommand[];
extern u8 gShop_GlyphBytes[];
extern u8 gStatusMenu_LevelLetterString[];
void *IconMenu_Open(void);
void ItemMenu_PositionCategory(void);
void IconMenu_AddIcon(s32 no);
void IconMenu_Close(void);
void Resource_ResetPendingTransfer(void);
s32 Sanctum_NeedsService(s32 entry_id, s32 kind);
void Text_DrawMessageFar(s32 message, s32 window, s32 x, s32 y);

struct Obj;
struct Actor_0801c0dc;
struct RenderInput;
struct WindowSprite;
struct DjinnMenuBackdrop;

extern u8 gData_08105948[];
extern u16 gShop_GlyphRowOffsets[];
extern u16 gShop_SelectorOffsets[];
void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 first);
void Event_FinishScreenTransitionFar(void);
void Scene_TalkToActorFar(s32, s32);
void BattleEffect_ApplyColorToSourceBufferFar(s32);
void BattlePalette_StopBlendAndReleaseBufferFar(void);
void BattleEffect_SetPrimaryBufferValueFar(void);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
void DebugFlagEditor_DrawTable(s32, s32);
s32 DebugFlagEditor_HandleInput(s32, s32 *, s32 *);
void IconMenu_LayoutIcons(s32 x, s32 y, s32 w, s32 h);
s32 IconMenu_Run(s32);
void Party_AdjustCoinsFar(s32);
s32 Party_ListMembersFar(s16 *);
s32 Party_SumDjinnCountsFar(s32);
#ifndef OWN_WindowSprite_CreateFromResourceFar
struct WindowSprite *
WindowSprite_CreateFromResourceFar(s32 arg0, s32 arg1, struct RenderInput *arg2,
                                   s32 arg3, s32 arg4);
#endif
void SelectionMenu_StopDrawing(void);
void *String_FormatPlayTime(s32, u8 *);
void Window_HighlightRectFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Icon_PrepareActor(struct WindowSprite *icon);
#ifndef OWN_UiTextResource_Initialize
void MenuArrow_Create(struct Actor_0801c0dc *obj, s32 *slot);
#endif
#ifndef OWN_UiTextResource_SetPosition
void MenuArrow_SetPosition(struct Obj *obj, s32 arg1, s32 arg2);
#endif
void Window_SetTilemapEntryFar(s32, s32, s32, s32, s32);

extern const u8 gMenu_ColonString[];
extern const u8 gMenu_HexDigitsString[];
extern u8 gMenu_SelectionStepDelays[];

#endif
