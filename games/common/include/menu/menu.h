#ifndef MENU_H
#define MENU_H

/* MENU */

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

extern u8 CharacterMenu_CursorWidths[];
extern u8 Menu_ListOrderDefault[];
extern u8 Menu_ListOrderMode0[];
extern u8 Menu_ListOrderMode1[];
extern u8 Menu_ListOrderMode2[];
extern u8 Menu_MinusSignString;
extern u8 Menu_PlusSignString;
extern s8 Menu_TopEntryCommandByPosition[];
extern s8 Menu_TopEntryPositionByCommand[];
extern u8 Shop_GlyphBytes[];
extern u8 StatusMenu_LevelLetterString[];
void	 *AffineEffect_InitializeWork(void);
void	  ItemMenu_PosCategory(void);
void	  Menu_AppendResourceEntry(s32 no);
void	  Menu_EndResourceSelection(void);
void	  Resource_ResetPendingTransfer(void);
s32		  Shop_CanServe(s32 entry_no, s32 kind);
#if GAME_TLA
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
#endif

#if !GAME_TLA
struct AnimationObject;
struct BtlAct;
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

struct ScrollPair
{
	u16 x;
	u16 y;
};
extern struct ScenePlacement Clear_ActorTable[];
extern s16 Clear_BlendFrame;
extern s16		  Clear_CodeHeld;
extern s16		  Clear_CodeProgress;
extern const u16 Clear_CodeSequence[];
extern s16		  Clear_CodeUnlocked;
extern struct SceneEvent Clear_EffectTable[];
extern s32 Clear_ExtraCodeHeld;
extern s16		  Clear_ExtraCodeProgress;
extern const u16 Clear_ExtraCodeSequence[];
extern s16		  Clear_ExtraCodeUnlocked;
extern u32 Clear_MessageTable[];
extern u32 Clear_PasswordFlags[];
extern u32 Clear_PasswordKeyItems[];
extern u32 Clear_PasswordMembers[];
extern u16 Clear_PasswordStackItems[];
extern struct SceneEntrance Clear_ScriptTable[];
extern u8		  Menu_SmallDownArrowTiles[];
extern u8		  Menu_ArrowTiles[];
extern u8		  Menu_PairSpriteATiles[];
extern u8		  Menu_PairSpriteBTiles[];
extern const u8	  Menu_QuantityTiles[];
extern s8		  Shop_PartyActionCues[];
extern const char DjinnMenu_TextLevel[];
extern const char DjinnMenu_TextSlash[];
extern s16		  Shop_StockTable[][33];
extern u32		  gLinkLobbyCallFrames;
#ifndef OWN_gMenuCtrlWork
extern struct ObjectSystemWork *gMenuCtrlWork;
#endif
extern u8					   gNumberTextBuffer[];
extern struct SelectionScreen *gResQueueWork;
extern struct SceneEntrance	   gTitleEntrances[];
extern struct SceneEvent	   gTitleEvents[];
extern u32					   gTitleExits[];
#ifndef OWN_gTitleExtraOptionEnabled
extern u8 gTitleExtraOptionEnabled[];
#endif
extern struct ScenePlacement gTitlePlacements[];
extern s8					 Inn_PriceMultipliers[];
extern u8					 ItemMenu_ArrangeKeysString[];
extern s32					 ItemMenu_CommandColumnXTable[];
extern u8					 ItemMenu_EquipmentKeyString[];
extern u8					 LinkLobby_SlotColumns[];
extern const s32			 LinkLobby_SlotValues[];
extern const u8				 Menu_BackdropFrameTile[];
#ifndef OWN_Menu_ColonString
extern const u8 Menu_ColonString[];
#endif
extern s8		Menu_CursorBobOffsets[];
extern u8		Menu_CursorLeftObjectTiles[];
extern const u8 Menu_HexDigitsString[];
extern const u8 Menu_HpString[];
#ifndef OWN_Menu_LvString
extern const u8 Menu_LvString[];
#endif
extern const s32 Menu_PartySpriteResourceIds[4];
extern u8		 Menu_PointString[];
extern const u8	 Menu_PpString[];
extern s8		 Menu_SaveSlotActionByPosition[];
extern u8		 Menu_SelectionStepDelays[];
extern const u8	 Menu_ShortcutLColonString[];
extern const u8	 Menu_ShortcutLString[];
extern const u8	 Menu_ShortcutRColonString[];
extern const u8	 Menu_ShortcutRString[];
extern const u8	 Menu_SlashString[];
extern u8		 Menu_TimesString[];
#ifndef OWN_Menu_WorkspaceIconFrames
extern s8 Menu_WorkspaceIconFrames[];
#endif
extern u8		 OptionMenu_FirstColorPresets[];
extern u8		 OptionMenu_SecondColorPresets[];
extern s8		 OptionMenu_SleepFrames[];
extern s8		 OptionMenu_SpeechFrames[];
extern s8		 OptionMenu_SpeedFrames[];
extern u16		 Shop_SelectorTileOffsets[];
extern u16		 Shop_GlyphOffsets[];
extern const s16 Shop_SpecialItemPrices[];
extern u8		 Ui_LvString[];
extern u8		 UiMenu_CursorBobX[];
extern u8		 UiMenu_CursorBobY[];
extern u8		 Workspace_EnergyTitle[];
extern u8		 Workspace_ItemTitle[];
extern u8		 Workspace_PageLabel[];
extern u8		 Workspace_RangeDash[];
extern u8		 Workspace_StatusTitle[];
extern const u16 WorkspaceOptions_SliderPalette[];
extern const u8	 WorkspaceOptions_SliderTiles[];
#ifndef OWN_Ability_GetData
struct BtlAct *Ability_GetData(s32 action_id);
#endif
#ifndef OWN_Djinn_ListOwnerEntries
#endif
#ifndef OWN_DjinnMenu_DrawStatPreview
#endif
#ifndef OWN_DjinnMenu_ShowCurrentList
#endif
#ifndef OWN_Inn_CheckIn
#endif
#ifndef OWN_InventoryMenu_ShowModalMessage
#endif
s32	 Item_FindSlot(s32 unit_id, s32 item_id);
#ifndef OWN_LinkLobby_PollPeerReady
#endif
#ifndef OWN_Menu_DrawPageIndicator
#endif
#ifndef OWN_Menu_DrawSelectionRow
#endif
#ifndef OWN_Menu_OpenSelectionWindow
#endif
#ifndef OWN_Menu_SpawnFourObjectsAtOrigin
#endif
#ifndef OWN_ObjectPlacement_CreateGroup
#endif
#ifndef OWN_OwnerAction_DiffSlots
#endif
s32	 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);
#ifndef OWN_PartyTalkMenu_Choose
#endif
#ifndef OWN_PsynergyMenu_DrawListPage
#endif
#ifndef OWN_PsynergyMenu_DrawRange
#endif
void ScrollFar_Entry00(s32 mode);
#ifndef OWN_Shop_BuySpecialItem
#endif
#ifndef OWN_Shop_FillSelector
#endif
#ifndef OWN_Shop_Run
#endif
#ifndef OWN_Shop_SetCursor
#endif
#ifndef OWN_StatusMenu_DrawCharacterSummary
#endif
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

extern u8  Data_08105948[];
extern u16 Shop_GlyphRowOffsets[];
extern u16 Shop_SelectorOffsets[];
void	   DjinnMenu_DrawPage(struct UiPanel *panel, s32 first);
void	   Event_DelayEffectFramesFar(void);
void	   Event_FinishScreenTransitionFar(void);
void	   Func_080c8368(s32, s32);
void	   BattleFx_ApplyColorToSourceBufferFar(s32);
void	   Runtime_ScheduleCallbackAndReleaseBlock32BFar(void);
void	   BattleFx_SetPrimaryBufferValueFar(void);
void	   ItemMenu_ArrangeCategoryItemIcons(u8 *items);
void	   ItemMenu_DrawEquippedItemNames(s32 window, u8 *items);
void	   Menu_DrawFlagBitTable(s32, s32);
s32		   Menu_HandleFlagGridInput(s32, s32 *, s32 *);
void	   Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h);
s32		   Menu_RunResourceSelectionLoop(s32);
void	   Party_AdjustCoinsFar(s32);
s32		   Party_ListActiveOwnersFar(s16 *);
s32		   Party_SumDjinnCountsFar(s32);
#ifndef OWN_RenderOutput_CreateFromResourceFar
struct RenderOutput *RenderOutput_CreateFromResourceFar(
	s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4);
#endif
void  Resource_ScheduleOwnerReset(void);
void *Text_FormatPlayTime(s32, u8 *);
void  Ui_SetRectHighlightFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void  UiIcon_PrepareObject(struct RenderOutput *icon);
void  UiTextResource_Initialize(struct Object_0801c0dc *obj, s32 *slot);
void  UiTextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2);
void  UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);
#endif

#endif
