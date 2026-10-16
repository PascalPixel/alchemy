#ifndef MENU_H
#define MENU_H

// MENU

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

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
#if GAME_TLA
void *AffineEffect_InitializeWork(void);
void ItemMenu_PosCategory(void);
void Menu_AppendResourceEntry(s32 no);
void Menu_EndResourceSelection(void);
void Resource_ResetPendingTransfer(void);
s32 Shop_CanServe(s32 entry_id, s32 kind);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
#endif

#if !GAME_TLA
struct AnimationObject;
struct BattleAct;
struct DjinnListTable;
struct EffectSlot;
struct MenuResult;
struct ObjectSystemWork;
struct OwnerActionSlot;
struct PlacementDescriptor;
struct RenderInput;
struct RenderOutput;
struct SelectionScreen;
struct ShopCursor;
struct UiWindow;
struct Work;

struct ScrollPair {
  u16 x;
  u16 y;
};
extern u8 gMenu_SmallDownArrowTiles[];
extern u8 gMenu_ArrowTiles[];
extern u8 gMenu_PairSpriteATiles[];
extern u8 gMenu_PairSpriteBTiles[];
extern const u8 gMenu_QuantityTiles[];
extern const char gDjinnMenu_TextLevel[];
extern const char gDjinnMenu_TextSlash[];
#ifndef OWN_gMenuCtrlWork
extern struct ObjectSystemWork *gMenuCtrlWork;
#endif
extern u8 gNumberTextBuffer[];
extern struct SelectionScreen *gResQueueWork;
#ifndef OWN_gTitleExtraOptionEnabled
extern u8 gTitleExtraOptionEnabled[];
#endif
extern u8 gItemMenu_ArrangeKeysString[];
extern s32 gItemMenu_CommandColumnXTable[];
extern u8 gItemMenu_EquipmentKeyString[];
extern const u8 gMenu_BackdropFrameTile[];
#ifndef OWN_Menu_ColonString
extern const u8 gMenu_ColonString[];
#endif
extern u8 gMenu_CursorLeftObjectTiles[];
extern const u8 gMenu_HexDigitsString[];
extern const u8 gMenu_HpString[];
#ifndef OWN_Menu_LvString
extern const u8 gMenu_LvString[];
#endif
extern const s32 gMenu_PartySpriteResourceIds[4];
extern u8 gMenu_PointString[];
extern const u8 gMenu_PpString[];
extern s8 gMenu_SaveSlotActionByPosition[];
extern u8 gMenu_SelectionStepDelays[];
extern const u8 gMenu_ShortcutLColonString[];
extern const u8 gMenu_ShortcutLString[];
extern const u8 gMenu_ShortcutRColonString[];
extern const u8 gMenu_ShortcutRString[];
extern const u8 gMenu_SlashString[];
extern u8 gMenu_TimesString[];
extern s8 gOptionMenu_SleepFrames[];
extern s8 gOptionMenu_SpeechFrames[];
extern s8 gOptionMenu_SpeedFrames[];
extern u8 gUi_LvString[];
extern u8 gUiMenu_CursorBobX[];
extern u8 gUiMenu_CursorBobY[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];
extern u8 gWorkspace_StatusTitle[];
extern const u16 gWorkspaceOptions_SliderPalette[];
extern const u8 gWorkspaceOptions_SliderTiles[];
#ifndef OWN_Ability_GetData
struct BattleAct *Ability_GetData(s32 actionId);
#endif
s32 Item_FindSlot(s32 unitId, s32 item_id);
s32 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);
void ScrollFar_Entry00(s32 mode);
void UiText_DrawAt(s32, s32, s32, s32);
void UiWindow_Clear(s32 window);
void UiWindow_Commit(s32 window);
#endif

#if GAME_TLA
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
void Runtime_ScheduleCallbackAndReleaseBlock32BFar(void);
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

#endif
