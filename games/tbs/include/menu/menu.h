#ifndef MENU_H
#define MENU_H

/// @file
/// The menus' list orders, sign strings and routines.

#include "types.h"
#include "edition.h"
#include "farcall.h"

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
