#ifndef MENU_H
#define MENU_H

// MENU

#include "types.h"
#include "edition.h"

extern u8 gCharacterMenu_CursorWidths[];
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
void *AffineEffect_InitializeWork(void);
void ItemMenu_PosCategory(void);
void Menu_AppendResourceEntry(s32 no);
void Menu_EndResourceSelection(void);
void Resource_ResetPendingTransfer(void);
s32 Shop_CanServe(s32 entry_id, s32 kind);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);

struct Obj;
struct Object_0801c0dc;
struct RenderInput;
struct RenderOutput;
struct UiPanel;

extern u8 gData_08105948[];
extern u16 gShop_GlyphRowOffsets[];
extern u16 gShop_SelectorOffsets[];
void DjinnMenu_DrawPage(struct UiPanel *panel, s32 first);
void Event_DelayEffectFramesFar(void);
void Event_FinishScreenTransitionFar(void);
void Func_080c8368(s32, s32);
void BattleFx_ApplyColorToSourceBufferFar(s32);
void BattlePalette_StopBlendAndReleaseBufferFar(void);
void BattleFx_SetPrimaryBufferValueFar(void);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_DrawEquippedItemNames(s32 window, u8 *items);
void Menu_DrawFlagBitTable(s32, s32);
s32 Menu_HandleFlagGridInput(s32, s32 *, s32 *);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h);
s32 Menu_RunResourceSelectionLoop(s32);
void Party_AdjustCoinsFar(s32);
s32 Party_ListActiveOwnersFar(s16 *);
s32 Party_SumDjinnCountsFar(s32);
#ifndef OWN_RenderOutput_CreateFromResourceFar
struct RenderOutput *
RenderOutput_CreateFromResourceFar(s32 arg0, s32 arg1, struct RenderInput *arg2,
                                   s32 arg3, s32 arg4);
#endif
void Resource_ScheduleOwnerReset(void);
void *Text_FormatPlayTime(s32, u8 *);
void Ui_SetRectHighlightFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void UiTextResource_Initialize(struct Object_0801c0dc *obj, s32 *slot);
void UiTextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2);
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

#endif
