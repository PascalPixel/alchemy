@ Far-call stub table heading the window and text code at 0x08015000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08015000
	.thumb_func
Func_08015000:
	overlay_veneer UiWork_InitializeWithResourceCounters
	overlay_veneer UiWork_Initialize
	.global UiWindow_CreateFar
	.thumb_func
UiWindow_CreateFar:
	overlay_veneer UiWindow_Create
	.global UiWork_FinalizeFar
	.thumb_func
UiWork_FinalizeFar:
	overlay_veneer UiWork_Finalize
	.global Party_Apply
	.thumb_func
Party_Apply:
	.global Ui_AdjustValueWithoutLimitFar
	.thumb_func
Ui_AdjustValueWithoutLimitFar:
	overlay_veneer Ui_AdjustValueWithoutLimit
	overlay_veneer UiText_DecodeMessage
	.global UiText_CopyMessageStringFar
	.thumb_func
UiText_CopyMessageStringFar:
	overlay_veneer UiText_CopyMessageString
	.global UiText_OpenMessageWindowFar
	.thumb_func
UiText_OpenMessageWindowFar:
	overlay_veneer UiText_OpenMessageWindow
	.global UiText_DrawMessage
	.thumb_func
UiText_DrawMessage:
	.global UiText_ShowPositionedMessageAndWaitFar
	.thumb_func
UiText_ShowPositionedMessageAndWaitFar:
	overlay_veneer UiText_ShowPositionedMessageAndWait
	.global UiWork_IsCompleteFar
	.thumb_func
UiWork_IsCompleteFar:
	overlay_veneer UiWork_IsComplete
	.global UiWork_IsIdleFar
	.thumb_func
UiWork_IsIdleFar:
	overlay_veneer UiWork_IsIdle
	.global UiWork_SetBusyFlagsFar
	.thumb_func
UiWork_SetBusyFlagsFar:
	overlay_veneer UiWork_SetBusyFlags
	.global UiWindow_Clear
	.thumb_func
UiWindow_Clear:
	.global RenderOutput_PrepareForRedrawFar
	.thumb_func
RenderOutput_PrepareForRedrawFar:
	overlay_veneer RenderOutput_PrepareForRedraw
	.global UiWindow_ClearInteriorTilesFar
	.thumb_func
UiWindow_ClearInteriorTilesFar:
	overlay_veneer UiWindow_ClearInteriorTiles
	.global UiWindow_DrawDividerLineFar
	.thumb_func
UiWindow_DrawDividerLineFar:
	overlay_veneer UiWindow_DrawDividerLine
	.global Func_08015078
	.thumb_func
Func_08015078:
	.global UiText_DrawMessageAt
	.thumb_func
UiText_DrawMessageAt:
	.global UiText_DrawResourceFar
	.thumb_func
UiText_DrawResourceFar:
	overlay_veneer UiText_DrawResource
	.global UiText_DrawAt
	.thumb_func
UiText_DrawAt:
	.global UiText_DrawCharacterAtOffsetFar
	.thumb_func
UiText_DrawCharacterAtOffsetFar:
	overlay_veneer UiText_DrawCharacterAtOffset
	.global UiText_DrawStringFar
	.thumb_func
UiText_DrawStringFar:
	overlay_veneer UiText_DrawString
	.global UiText_DrawStringAtOffsetFar
	.global Func_08015090
	.thumb_func
UiText_DrawStringAtOffsetFar:
Func_08015090:
	overlay_veneer UiText_DrawStringAtOffset
	.global UiText_DrawStringInWindowFar
	.thumb_func
UiText_DrawStringInWindowFar:
	overlay_veneer UiText_DrawStringInWindow
	.global UiText_DrawNumberFar
	.thumb_func
UiText_DrawNumberFar:
	overlay_veneer UiText_DrawNumber
	.global UiText_DrawNumberAtOffsetFar
	.global Func_080150a8
	.thumb_func
UiText_DrawNumberAtOffsetFar:
Func_080150a8:
	overlay_veneer UiText_DrawNumberAtOffset
	.global UiNumber_DrawAt
	.thumb_func
UiNumber_DrawAt:
	.global UiText_DrawNumberInWindowFar
	.thumb_func
UiText_DrawNumberInWindowFar:
	overlay_veneer UiText_DrawNumberInWindow
	.global UiWork_SetParamNibbleFar
	.thumb_func
UiWork_SetParamNibbleFar:
	overlay_veneer UiWork_SetParamNibble
	overlay_veneer UiWork_SetRenderWord
	.global RenderOutput_CreateFar
	.global Func_080150c8
	.thumb_func
RenderOutput_CreateFar:
Func_080150c8:
	overlay_veneer RenderOutput_Create
	.global RenderOutput_CreateFromResourceFar
	.thumb_func
RenderOutput_CreateFromResourceFar:
	overlay_veneer RenderOutput_CreateFromResource
	.global SideObject_CreateFar
	.thumb_func
SideObject_CreateFar:
	overlay_veneer CreateSideObject
	.global Ui_LoadCharacterEntryForSlotFar
	.thumb_func
Ui_LoadCharacterEntryForSlotFar:
	overlay_veneer Ui_LoadCharacterEntryForSlot
	overlay_veneer UiWork_SetByte5AndResetHalfwordC
	overlay_veneer UiWork_SetByte15Inverted
	.global UiWindow_CreateWithSideObjectFar
	.global Func_080150f8
	.thumb_func
UiWindow_CreateWithSideObjectFar:
Func_080150f8:
	overlay_veneer UiWindow_CreateWithSideObject
	.global UiWork_FinalizeEntityMatchingLocalizedIdFar
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedIdFar:
	overlay_veneer UiWork_FinalizeEntityMatchingLocalizedId
	.global Func_08015108
	.thumb_func
Func_08015108:
	overlay_veneer UiText_GetResourceDimensions
	overlay_veneer UiText_GetResourceDimensionsAlt
	.global UiWork_ClearValueNameTablesFar
	.thumb_func
UiWork_ClearValueNameTablesFar:
	overlay_veneer UiWork_ClearValueNameTables
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	.global UiWork_PushValueSlotFar
	.thumb_func
UiWork_PushValueSlotFar:
	.global Func_08015120
	.thumb_func
Func_08015120:
	overlay_veneer UiWork_PushValueSlot
	overlay_veneer UiWindow_CreateWithLayoutBounds
	.global UiWindow_DrawPartyStatusContentsFar
	.thumb_func
UiWindow_DrawPartyStatusContentsFar:
	overlay_veneer UiWindow_DrawPartyStatusContents
	overlay_veneer UiWork_FinalizeAndReleaseBlock16
	.global UiWork_FinalizePendingCoreFar
	.thumb_func
UiWork_FinalizePendingCoreFar:
	overlay_veneer UiWork_FinalizePendingCore
	overlay_veneer UiWork_DrainPending
	overlay_veneer UiGlyph_ResetWorkState
	overlay_veneer Resource_ClearOwnerListAndCounters
	overlay_veneer Resource_PushPendingPair
	overlay_veneer Resource_ScheduleOwnerResetDelayed
	overlay_veneer Resource_ScheduleOwnerReset
	overlay_veneer Menu_OpenSelectionWindow
	overlay_veneer Resource_ResetOwnerEntries
	overlay_veneer Menu_SetupSelectionBothSides
	overlay_veneer Menu_SelectionLoop
	overlay_veneer Menu_WaitForSelectionInput
	overlay_veneer UiTextResource_NoOpCallbackBank
	overlay_veneer UiTextResource_NoOpCallback1
	overlay_veneer MenuSelection_BuildEntries
	overlay_veneer UiWork_ReservedNoOp
	.global Battle_CollectPartyCommandsFar
	.global Func_080151c0
	.thumb_func
Battle_CollectPartyCommandsFar:
Func_080151c0:
	overlay_veneer Battle_CollectPartyCommands
	.global UiText_ShowMessageAndWaitCoreFar
	.thumb_func
UiText_ShowMessageAndWaitCoreFar:
	overlay_veneer UiText_ShowMessageAndWaitCore
	.global UiText_PrepareMessageWorkFar
	.thumb_func
UiText_PrepareMessageWorkFar:
	overlay_veneer UiText_PrepareMessageWork
	overlay_veneer UiTextResource_NoOpCallback4
	.global Func_080151e0
	.thumb_func
Func_080151e0:
	overlay_veneer Localization_LookupEntryId
	overlay_veneer Menu_RunTopSelection
	.global Menu_SelectSaveSlotActionFar
	.thumb_func
Menu_SelectSaveSlotActionFar:
	overlay_veneer Menu_SelectSaveSlotAction
	.global Menu_SelectResourceLayoutFar
	.thumb_func
Menu_SelectResourceLayoutFar:
	overlay_veneer Menu_SelectResourceLayout
	.global Func_08015200
	.thumb_func
Func_08015200:
	overlay_veneer Func_0801c34c
	.global UiTimedNotice_CloseIfActiveFar
	.thumb_func
UiTimedNotice_CloseIfActiveFar:
	overlay_veneer UiTimedNotice_CloseIfActive
	.global UiText_ShowCenteredMessageFar
	.thumb_func
UiText_ShowCenteredMessageFar:
	overlay_veneer UiText_ShowCenteredMessage
	.global UiWork_FinalizeSharedSlotFar
	.global Func_08015218
	.thumb_func
UiWork_FinalizeSharedSlotFar:
Func_08015218:
	overlay_veneer UiWork_FinalizeSharedSlot
	.global UiWork_ResetFreeChannelFar
	.thumb_func
UiWork_ResetFreeChannelFar:
	overlay_veneer UiWork_ResetFreeChannel
	.global UiTextResource_InitializeFar
	.thumb_func
UiTextResource_InitializeFar:
	overlay_veneer UiTextResource_Initialize
	.global UiTextResource_SetPositionFar
	.thumb_func
UiTextResource_SetPositionFar:
	overlay_veneer UiTextResource_SetPosition
	.global UiTextResource_ReleaseFar
	.thumb_func
UiTextResource_ReleaseFar:
	overlay_veneer UiTextResource_Release
	overlay_veneer Menu_RunSelectionHook
	overlay_veneer Item_CallHandler48
	.global ItemIcon_LoadTilesFar
	.global Func_08015250
	.thumb_func
ItemIcon_LoadTilesFar:
Func_08015250:
	.global Ui_PrepareTransferForItemFar
	.thumb_func
Ui_PrepareTransferForItemFar:
	overlay_veneer Ui_PrepareTransferForItem
	.global UiText_RenderWideStringAtOffsetFar
	.thumb_func
UiText_RenderWideStringAtOffsetFar:
	overlay_veneer UiText_RenderWideStringAtOffset
	overlay_veneer GraphicsPalette_SelectionNoOp
	.global Func_08015268
	.thumb_func
Func_08015268:
	overlay_veneer UiWindow_FillFromScene
	.global RenderOutput_RedrawSavedRectFar
	.thumb_func
RenderOutput_RedrawSavedRectFar:
	.global UiWindow_Commit
	.thumb_func
UiWindow_Commit:
	overlay_veneer RenderOutput_RedrawSavedRect
	.global RenderOutput_ClearListFar
	.thumb_func
RenderOutput_ClearListFar:
	overlay_veneer RenderOutput_ClearList
	.global UiWindow_SetTilemapEntryFar
	.thumb_func
UiWindow_SetTilemapEntryFar:
	overlay_veneer UiWindow_SetTilemapEntry
	overlay_veneer Menu_RunSelection
	overlay_veneer Menu_RunSelectionWithCursorObject
	.global Func_08015298
	.thumb_func
Func_08015298:
	.global Resource_LoadByModeIntoSlotFar
	.thumb_func
Resource_LoadByModeIntoSlotFar:
	overlay_veneer Resource_LoadByModeIntoSlot
	overlay_veneer Debug_SelectAbilityPair
	.global Func_080152a8
	.thumb_func
Func_080152a8:
	overlay_veneer UiWindow_MarkVisibleTileAttributes
	overlay_veneer ItemIcon_Compose
	.global BattleLayout_HighlightPartyPanelsFar
	.global Func_080152b8
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
Func_080152b8:
	overlay_veneer BattleLayout_HighlightPartyPanels
	overlay_veneer UiIcon_BuildItemIconTiles
	overlay_veneer UiIcon_BuildAbilityIconTiles
	.global Func_080152d0
	.thumb_func
Func_080152d0:
	.global UiIcon_Draw
	.thumb_func
UiIcon_Draw:
	overlay_veneer RenderOutput_CreateLoaded
	overlay_veneer RenderOutput_CreateFromTable
	overlay_veneer SaveState_WriteCurrentSlotPair
	.global Save_WriteSelectedSlotFar
	.thumb_func
Save_WriteSelectedSlotFar:
	overlay_veneer Save_WriteSelectedSlot
	.global SaveState_LoadRecordIntoWorkFar
	.thumb_func
SaveState_LoadRecordIntoWorkFar:
	overlay_veneer SaveState_LoadRecordIntoWork
	.global SaveState_CopySlotPairFar
	.thumb_func
SaveState_CopySlotPairFar:
	overlay_veneer SaveState_CopySlotPair
	.global SaveState_DeleteSelectedSlotFar
	.thumb_func
SaveState_DeleteSelectedSlotFar:
	overlay_veneer SaveState_DeleteSelectedSlot
	.global SaveState_ProcessSelectedSlotFar
	.thumb_func
SaveState_ProcessSelectedSlotFar:
	overlay_veneer SaveState_ProcessSelectedSlot
	overlay_veneer SaveState_WriteSlotPair
	.global SaveState_ScanRecordFlagsFar
	.thumb_func
SaveState_ScanRecordFlagsFar:
	overlay_veneer SaveState_ScanRecordFlags
	.global NameEntry_EditOwnerNameFar
	.thumb_func
NameEntry_EditOwnerNameFar:
	overlay_veneer NameEntry_EditOwnerName
	overlay_veneer Resource_CopyFixedBlockB
	overlay_veneer Resource_CopyFixedBlockA
	overlay_veneer Menu_SetNodeCoordinates
	.global DebugMenu_BrowseIconsFar
	.thumb_func
DebugMenu_BrowseIconsFar:
	overlay_veneer DebugMenu_BrowseIcons
	.global DebugMenu_BrowseEntryGlyphsFar
	.thumb_func
DebugMenu_BrowseEntryGlyphsFar:
	overlay_veneer DebugMenu_BrowseEntryGlyphs
	overlay_veneer Item_ReturnTrue
	.global Djinn_ShowJoinedMessageFar
	.global Func_08015358
	.thumb_func
Djinn_ShowJoinedMessageFar:
Func_08015358:
	overlay_veneer Djinn_ShowJoinedMessage
	.global PaletteGlow_UpdateFar
	.thumb_func
PaletteGlow_UpdateFar:
	overlay_veneer PaletteGlow_Update
	overlay_veneer Menu_RunWorkspaceOptions
	.global Func_08015370
	.thumb_func
Func_08015370:
	overlay_veneer Menu_RunWorkspaceResultLoop
	overlay_veneer UiWindow_FillScreenBlockRect
	.global Func_08015380
	.thumb_func
Func_08015380:
	overlay_veneer Menu_SelectEntry11To14
	.global Menu_SelectEntry19To1cFar
	.thumb_func
Menu_SelectEntry19To1cFar:
	overlay_veneer Menu_SelectEntry19To1c
	.global Inventory_RequestMode
	.thumb_func
Inventory_RequestMode:
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer Menu_RunConfirmSelection
	.global Menu_SelectEntry20To21Far
	.thumb_func
Menu_SelectEntry20To21Far:
	overlay_veneer Menu_SelectEntry20To21
	.global PartyTalkMenu_ChooseFar
	.thumb_func
PartyTalkMenu_ChooseFar:
	overlay_veneer PartyTalkMenu_Choose
	overlay_veneer RenderResource_LoadTableEntry
	.global Menu_RunConfirmSelectionAtFar
	.global Func_080153b0
	.thumb_func
Menu_RunConfirmSelectionAtFar:
Func_080153b0:
	overlay_veneer Menu_RunConfirmSelectionAt
	.global Func_080153b8
	.thumb_func
Func_080153b8:
	.global Resource_LoadFixedBlockBIntoFreeSlotFar
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlotFar:
	overlay_veneer Resource_LoadFixedBlockBIntoFreeSlot
	overlay_veneer UiText_MeasureResourceEntries
	.global UiWindow_SetPaletteBitRectFar
	.thumb_func
UiWindow_SetPaletteBitRectFar:
	overlay_veneer Ui_SetRectHighlight
	.global Func_080153d0
	.thumb_func
Func_080153d0:
	overlay_veneer Ability_LoadGlyph
	.global Func_080153d8
	.thumb_func
Func_080153d8:
	.global Graphics_ExpandVramTilesByColorTableFar
	.thumb_func
Graphics_ExpandVramTilesByColorTableFar:
	overlay_veneer Graphics_ExpandVramTilesByColorTable
	.global Func_080153e0
	.thumb_func
Func_080153e0:
	overlay_veneer UiWork_SetAltFlagAndClearTable
	.global Menu_AnimateSelectionToEntryFar
	.thumb_func
Menu_AnimateSelectionToEntryFar:
	overlay_veneer Menu_AnimateSelectionToEntry
	.global Func_080153f0
	.thumb_func
Func_080153f0:
	overlay_veneer Ui_GetTableWordZero
	overlay_veneer UiText_OpenEntryMessage
	.global SaveState_CountRecordsExcludingFlaggedFar
	.thumb_func
SaveState_CountRecordsExcludingFlaggedFar:
	overlay_veneer SaveState_CountRecordsExcludingFlagged
	.global UiWindow_DrawFrameFar
	.global Func_08015408
	.thumb_func
UiWindow_DrawFrameFar:
Func_08015408:
	overlay_veneer UiWindow_DrawFrame
	.global UiWindow_EraseBorderRectFar
	.thumb_func
UiWindow_EraseBorderRectFar:
	overlay_veneer UiWindow_EraseBorderRect
	.global Link_DrawShiftedTilePairFar
	.thumb_func
Link_DrawShiftedTilePairFar:
	overlay_veneer Link_DrawShiftedTilePair
	.global Party_ShowJoinedMessageFar
	.thumb_func
Party_ShowJoinedMessageFar:
	overlay_veneer Party_ShowJoinedMessage
	.global Party_ShowPairJoinedMessageFar
	.thumb_func
Party_ShowPairJoinedMessageFar:
	overlay_veneer Party_ShowPairJoinedMessage