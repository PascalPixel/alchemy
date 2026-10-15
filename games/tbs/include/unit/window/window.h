#ifndef ALCHEMY_UNIT_WINDOW_WINDOW_H
#define ALCHEMY_UNIT_WINDOW_WINDOW_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct AbilityListEntry;
struct BattleActionRecord;
struct BtlAct;
struct GlyphPalette;
struct MenuSprite;
struct NodeChainNode;
struct NodeChainState;
struct OwnerActionSlot;
struct RenderInput;
struct RenderOutput;
struct RenderOutputList;
struct SaveSummary;
struct SelectionNode;
struct SelectionScreen;
struct SelectionWork;
struct TextReader;
struct UiChannelSlot;
struct UiWindow;
struct WorkspaceWork;

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_ReleaseFree(struct RenderOutput *out);
void UiWork_InitFreeList(void);
void UiWork_InitializeWithResourceCounters(void);
void UiWork_Initialize(s32 kind);
void UiWork_UploadDirtyBlocks(void);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(struct UiWindow *window);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void UiWork_WaitWindowDuration(struct UiWindow *window);
void UiWork_Finalize(struct UiWindow *work, s32 release);
void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void RenderOutput_ClearList(void *work);
void UiWindow_ClearInteriorTiles(const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom);
void UiWork_UpdateListTail(struct RenderOutputList *list);
/* RenderOutput_AppendToList is called from battle_command.c with no prototype in scope: in one file its definition would come before RenderOutput_Create and DjinnMenu_ShowChangePreview, and RenderOutput_Create changes (2 of 61 instructions in tbs-en). Split evidence for window|btlcmd. */
void RenderOutput_Release(struct RenderOutput *entry);
/* UiText_QueueRenderEntries is called from battle_command.c with no prototype in scope: Party_ShowJoinedMessage, Party_ShowPairJoinedMessage and Djinn_ShowJoinedMessage pass five of its six arguments, and in one file, with its definition first, they would not compile. Split evidence for window|btlcmd. */
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve);
s32 Ui_ClearVramBlock(void);
s32 Ui_FillVramBlockPattern(void);
void UiWork_ResetFreeChannel(void);
void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel);
void UiWork_ResetChannelTransition(struct UiChannelSlot *channel);
void UiWork_ShiftPanelRowsLeft(s32 step);
void UiWork_ProcessRenderChannels(void);
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch);
void UiWork_ProcessDirectWork(void);
void UiWindow_UpdateInterpolatedGeometry(void *arg, s32 save_position);
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(struct UiWindow *window);
void UiWork_ResetCounters();
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_InitCountersAndScheduleRefresh(s32 initialize);
void UiWork_FinalizeSharedSlot(void);
void UiText_PrepareMessageWork(s32 argument);
void UiText_ShowMessageAndWaitCore(s32 argument);
s32 UiText_OpenEntryMessage(s32 window, s32 argument);
void UiWork_SetBusyFlags(s32 flags);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags);
void UiWork_ProcessAll(void);
#if EDITION_INTERNATIONAL
s32 UiText_RenderGlyphPair(s32 code, u32 *out);

#endif
#if !(EDITION_INTERNATIONAL)
s32 UiText_RenderGlyphPair(u32 code, u32 *out);

#endif
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y);
void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer, const struct RenderInput *work, s32 offset_x, s32 offset_y);
/* UiText_RenderWideStringInWindow is called from battle_command.c with no prototype in scope: in one file its definition would come before DjinnMenu_ShowChangePreview's 17 calls, and that function changes by 18 bytes in tbs-en. Split evidence for window|btlcmd. */
u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width);
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix);
s32 UiText_BuildRenderEntries(s32 script, s32 clear);
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, u16 *spacing, u32 flags);
void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y);
s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 UiText_GetResourceDimensionsAlt(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing);
void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height, u16 *spacing);
s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y, s32 mode);
/* UiWindow_PutGlyph is called from battle_command.c with no prototype in scope, as in window.c above its definition: PtyStDraw, SummonMenu_SelectSummon and Battle_CollectPartyCommands pass four of its five arguments, and in one file, with its definition first, they would not compile. Split evidence for window|btlcmd. */
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode);
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output);
void UiWork_AnimateSpriteSlots(void);
void Ui_AdjustValueWithoutLimit(s32 mes, s32 buf);
s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity);
void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity);
s32 *Runtime_ClearWordIfSet(s32 *p);
void UiWork_DrainPending(s32 unused);
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *ch);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(u32 value, u32 kind);
u32 UiRender_LookupNamedValue(u32 kind, u32 clear);
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch);
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch);
void UiWork_FinalizePendingCore(void);
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset);
s32 UiText_BuildRenderEntriesMode1(s32 arg0);
void UiText_LookupMessage(struct TextReader *reader, s32 message);
s32 UiText_DecodeSymbol(struct TextReader *reader);
void UiWork_SetTwoEntriesTo999(void);
s32 Localization_LookupEntryId(u32 value);
s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y);
void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry);

#define UI_WORK ((struct UiRenderWork *)gWindowWork[0])

#define ACTION_ID_MASK 0x3fff

// no の表示エントリを作り、UiWindow_FitOnScreen に flags をつけてわたす。
// エントリがなければ 0 を返す
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)               \
	s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)     \
	{                                                             \
		struct UiRenderWork *work;                                \
		s32					 idx;                                 \
                                                                  \
		work = UI_WORK;                                           \
		idx = UiText_BuildRenderEntries(no, 0);                   \
		if (work->entries[idx] == 0)                              \
		{                                                         \
			return 0;                                             \
		}                                                         \
		UiWindow_FitOnScreen(idx, x, y, width, height, 0, flags); \
		return 1;                                                 \
	}

s32 Ui_CountIconTableEntries(void);
s32 Ui_CountSecondTableEntries(void);
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag);
void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
s32 ItemIcon_Compose(u32 code, u32 layers);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot);
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot);
void Ui_PrepareTransferForItem(s32 code);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
void Ui_PrepareTransferFromTableEntry(u32 index);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);
void UiGlyph_ReservedNoOp(void);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);

void UiGlyph_ResetWorkState(void);
void Resource_ClearOwnerListAndCounters(void);
void Resource_PushPendingPair(u32 first, u32 second);
void MenuSelection_BuildEntries(u32 unused);
void UiWork_ReservedNoOp(void);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void Resource_ScheduleOwnerResetDelayed(void);
void Resource_ScheduleOwnerReset(void);
void MenuSelection_DrawFrame(void);
void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index);
void Menu_OpenSelectionWindow(s32 mode, u32 count);
void Resource_ResetOwnerEntries(void);
void Menu_SetNodeCoordinates(u32 first, u32 second);
void Menu_SetupSelectionBothSides(void);
void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index);
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st);
s32 Menu_SelectionLoop(s32 mode);
u32 Menu_WaitForSelectionInput(u32 mode);
void Menu_MoveSelectionForward(struct SelectionScreen *screen);
void Menu_MoveSelectionBackward(struct SelectionScreen *screen);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_StepLeft(struct SelectionScreen *state);
void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
s32 Resource_LoadByMode(s32 mode, s32 value);
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void UiTextResource_NoOpCallback(void);
void UiTextResource_NoOpCallbackBank(void);
void UiTextResource_NoOpCallback1(void);
void UiTextResource_NoOpCallback2(void);
void UiTextResource_NoOpCallback3(void);
void UiTextResource_NoOpCallback4(void);
void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot);
void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void Menu_LoadSelectedResource(void);
void Resource_ResetPendingTransfer(void);
s32 Menu_RunTopSelection(void);
void UiWindow_OpenMode1AndWaitFrame(void);
/* UiWork_CloseAndRelease is called before its definition with no prototype in scope. */
void Ui_ClearWorkStateAndWaitFrame(void);
u32 Menu_RunSelectionForValue(u32 value);

void UiTimedNotice_Create(void);
void UiTimedNotice_Tick(void);
void UiTimedNotice_CloseIfActive(void);
s32 Item_CallHandler48(s32 arg0, s32 arg1);
s32 Item_ReturnTrue(void);
void Party_AdjustPaletteGlowPhase(s32 arg0);
void Debug_SelectAbilityPair(void);
s32 Object_CollectResources(struct AbilityListEntry *output);
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index, const struct AbilityListEntry *entries);
void Menu_RunSelectionHook(void);

void Menu_AllocateSelectionWorkspace(void);
void Ui_FinalizeWorkAndReleaseHeap13(void);
void GraphicsPalette_ReservedNoOpC9BC(void);
void GraphicsPalette_ReservedNoOpC9C0(void);
void GraphicsPalette_ReservedNoOpC9C4(void);
void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work);
void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work);
void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work);
u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b);
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);
void PaletteGlow_Update(s32 phase, s32 brightness);
void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work);
void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work);
void GraphicsPalette_DecrementSelectedCounter(struct WorkspaceWork *work);
void GraphicsPalette_AdjustSelectionCounter(struct WorkspaceWork *work);
void GraphicsPalette_SelectionNoOp(void);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);
void OptionMenu_InitializeWork(void);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);
struct RenderInput *Menu_OpenWorkspaceOptions(void);
s32 Menu_RunWorkspaceOptions(void);
void Menu_RunSelectedWorkspaceEntry(void);
void Menu_InitializeSelectedWorkspace(void);
void Runtime_ScheduleCallbackAndReleaseBlock20A(void);
struct RenderInput *Menu_CreateWorkspaceWindows(void);
s32 Menu_RunWorkspaceSelectionLoop(void);

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
/* UiText_DrawPrefixedNumberAtOffset is called from battle_command.c with no prototype in scope: in one file its definition would come before PtyStDraw, and PtyStDraw changes (2 of 435 instructions in tbs-en). Split evidence for window|btlcmd. */

#endif
