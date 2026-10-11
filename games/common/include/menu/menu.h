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
void	  UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
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
extern u8		  Clear_ActorTable[];
extern u16		  Clear_BlendFrame;
extern s16		  Clear_CodeHeld;
extern s16		  Clear_CodeProgress;
extern u16		  Clear_CodeSequence[];
extern s16		  Clear_CodeUnlocked;
extern u8		  Clear_EffectTable[];
extern s16		  Clear_ExtraCodeHeld;
extern s16		  Clear_ExtraCodeProgress;
extern u16		  Clear_ExtraCodeSequence[];
extern s16		  Clear_ExtraCodeUnlocked;
extern u8		  Clear_MessageTable[];
extern const u16  Clear_PasswordFlags[6];
extern const u16  Clear_PasswordKeyItems[8];
extern const s32  Clear_PasswordMembers[4];
extern const u16  Clear_PasswordStackItems[23];
extern u8		  Clear_ScriptTable[];
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
extern struct SceneEntrance					   gTitleEntrances[];
extern struct SceneEvent					   gTitleEvents[];
extern u32					   gTitleExits[];
#ifndef OWN_gTitleExtraOptionEnabled
extern u8 gTitleExtraOptionEnabled[];
#endif
extern struct ScenePlacement		 gTitlePlacements[];
extern s8		 Inn_PriceMultipliers[];
extern u8		 ItemMenu_ArrangeKeysString[];
extern s32		 ItemMenu_CommandColumnXTable[];
extern u8		 ItemMenu_EquipmentKeyString[];
extern u8	 LinkLobby_SlotColumns[];
extern const s32 LinkLobby_SlotValues[];
extern const u8	 Menu_BackdropFrameTile[];
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
s32	 AbilityMenu_BuildAvailableList(void);
void AffineEffect_UpdateFrame(void);
s32	 Audio_PlayCueReturnOne(s32 cue);
void AudioCommand_WaitForStateByteClear(void);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
void BattleUnit_ResetStateByMode(s32 id, s32 mode);
s32	 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);
void Clear_UpdateBlend(void);
void Debug_SelectAbilityPair(void);
#ifndef OWN_Djinn_ListOwnerEntries
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element);
#endif
void DjinnMenu_DrawElementList(struct DjinnListTable *tbl);
#ifndef OWN_DjinnMenu_DrawStatPreview
s32 DjinnMenu_DrawStatPreview(
	s32 window, s32 x, s32 y, s32 owner, s32 give, s32 take, s32 mode, s32 page, s32 side);
#endif
#ifndef OWN_DjinnMenu_ShowCurrentList
s32 DjinnMenu_ShowCurrentList(void);
#endif
void EventTable_ApplyRowAbilities(s32 row_no);
s32	 EventTable_CopyRowHeader(s32 row_no, s16 *output);
s32	 EventTable_GetRowLimit(void);
s32	 EventTable_GetRowType(s32 index);
void Graphics_AdjustPaletteBank(s32 arg0);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);
#ifndef OWN_Inn_CheckIn
s32 Inn_CheckIn(s32 mode, s32 object_id);
#endif
void Inn_Cleanup(void);
void Inn_PlaySleep(s32 room_price);
s32	 Inn_RoomPrice(s32 mode);
#ifndef OWN_InventoryMenu_ShowModalMessage
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
#endif
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode);
s32 Item_FindSlot(s32 unit_id, s32 item_id);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
s32	 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
s32	 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state);
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);
void ItemMenu_ResetCategory(void);
s32 LinkLobby_ExchangePartyRecords(void);
s32 LinkLobby_PartyContains(s32 id);
s32 LinkLobby_PeerSlotMatches(s32 slot);
#ifndef OWN_LinkLobby_PollPeerReady
s32 LinkLobby_PollPeerReady(void);
#endif
s32					LinkLobby_ReceivePartyRecords(void);
s32					LinkLobby_SendPartyRecords(void);
void				LinkLobby_WriteSlotValue(s32 slot);
void				Menu_BuildPatternTiles(void);
void				Menu_CancelSoundReset(void);
void				Menu_CancelSoundTick(void);
void				Menu_ClearFirstObjectRowAndScheduleUpdate(void);
void				Menu_ClearSecondObjectRowAndScheduleUpdate(void);
s32					Menu_ComputeEntryValues(void *tbl);
u32					Menu_ConfirmSelection(struct SelectionScreen *screen);
void				Menu_CopyListOrder(s32 mode, u8 *order);
s32					Menu_CreateEightEntryObjects(s32 resource);
struct RenderInput *Menu_CreateWorkspaceWindows(void);
void				Menu_DrawFlagBitTable(s32 window, s32 start_flag);
void				Menu_DrawModeIndicator(void);
void				Menu_DrawModeLabel(void);
#ifndef OWN_Menu_DrawPageIndicator
void Menu_DrawPageIndicator(
	s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
#endif
#ifndef OWN_Menu_DrawSelectionRow
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second);
#endif
void Menu_EnsureCancelSound(void);
s32	 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor);
s32	 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg);
void Menu_InitializeSelectedWorkspace(void);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base);
void Menu_LoadSelectedResource(void);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index);
s32	 Menu_OpenBackdropScreen(void);
#ifndef OWN_Menu_OpenSelectionWindow
void Menu_OpenSelectionWindow(s32 mode, u32 count);
#endif
void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase);
void Menu_ReleaseEntryObjects(void);
void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index);
void Menu_ResetTwoResourceEntries(void);
s32	 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action);
s32	 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32	 Menu_RunResourceSelectionLoop(s32 initial);
void Menu_RunSelectedWorkspaceEntry(void);
s16	 Menu_RunSelection(void);
s32	 Menu_RunWorkspaceOptions(void);
s32	 Menu_RunWorkspaceSelectionLoop(void);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
s32	 Menu_SelectEntry20To21(s32 arg0);
s32	 Menu_SelectResource(s32 start, s32 goal);
s32	 Menu_SelectTopEntry(s32 sel);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_SetupSelectionBothSides(void);
void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index);
#ifndef OWN_Menu_SpawnFourObjectsAtOrigin
void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y);
#endif
void Menu_StepLeft(struct SelectionScreen *state);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_UpdateEntryObjectTransforms(void);
void Menu_UpdateFirstObjectRowPositions(void);
void Menu_UpdateSecondObjectRowPositions(void);
u32	 Menu_WaitForSelectionInput(u32 mode);
void MenuSelection_DrawFrame(void);
void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index);
#ifndef OWN_ObjectPlacement_CreateGroup
void ObjectPlacement_CreateGroup(
	struct RenderInput *origin, s32 x, s32 y, struct PlacementDescriptor *descriptor);
#endif
void OptionMenu_InitializeWork(void);
#ifndef OWN_OwnerAction_DiffSlots
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b, u16 *out,
	s32 *first_count, s32 *second_count);
#endif
void Palette_CopyObjectBankToBackground14(void);
void Palette_LightenBankHighlight(s32 bank);
s32	 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);
#ifndef OWN_PartyTalkMenu_Choose
s32 PartyTalkMenu_Choose(void);
#endif
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state);
#ifndef OWN_PsynergyMenu_DrawListPage
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *res);
#endif
#ifndef OWN_PsynergyMenu_DrawRange
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused);
#endif
s32	 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state);
s32	 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);
void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns);
s32	 PsynergyMenu_RunList(s32 pane);
s32	 PsynergyMenu_SelectAction(void);
s32	 PsynergyMenu_SelectOwner(void);
s32	 PsynergyMenu_SelectTarget(s32 mode);
void Resource_LoadPairedBlocksIfAvailable(void);
void Resource_ResetOwnerEntries(void);
void Runtime_ScheduleCallbackAndReleaseBlock20A(void);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);
s32	 Sanctum_RunPartyService(void);
void ScrollFar_Entry00(s32 mode);
#ifndef OWN_Shop_BuySpecialItem
void Shop_BuySpecialItem(void *window, s32 item_window);
#endif
void				 Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2);
s32					 Shop_CountUnits(void);
struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y);
void				 Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id);
s32					 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
void				 Shop_DrawSelMsg(s32 target, s32 selection);
#ifndef OWN_Shop_FillSelector
void Shop_FillSelector(s32 count, s32 selector, u8 *base);
#endif
void Shop_HiliteUnit(s32 enabled, s32 selected);
void Shop_InitEffect(void);
void Shop_InitializeCursorWork(void);
s32	 Shop_MsgByMode(s32 value);
void Shop_RepairItem(s32 unit_id, s32 slot);
void Shop_ResetEffects(void);
void Shop_RestoreSceneTiles(s32 layer);
#ifndef OWN_Shop_Run
s32 Shop_Run(s32 row, s32 keeper_id);
#endif
void Shop_RunPartyMemberIconBurst(s32 member);
s32	 Shop_SelBuy(void);
void Shop_SellItem(s32 unit_id, s32 slot, s32 count);
s32	 Shop_SelSell(s32 unit_id);
s32	 Shop_SelUse(s32 actor);
s32	 Shop_ServicePrice(s32 entry_no, s32 kind);
#ifndef OWN_Shop_SetCursor
void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);
#endif
void Shop_StepCursor(void);
#ifndef OWN_StatusMenu_DrawCharacterSummary
void StatusMenu_DrawCharacterSummary(s32 surface, u8 *st);
#endif
s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot);
s32	 UiMenu_CreateCursor(void *source);
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMessage_ShowAndRestoreState(s32 message_id);
void UiMessage_ShowAndWait(s32 arg0);
s32	 UiMessage_ShowChoice(s32 arg0);
s32	 UiMessage_ShowChoiceVariant(s32 arg0);
void UiMessage_ShowResolvedAndRestoreState(s32 arg0);
void UiMessage_ShowResolvedAndWait(s32 value);
void UiText_DrawAt(s32, s32, s32, s32);
void UiWindow_Clear(s32 window);
void UiWindow_Commit(s32 window);
void UiWindow_OpenMode1AndWaitFrame(void);
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
