/* FarCall_WindowTable: one entry per 8-byte stub, in table order. */
FAR_CALL(
	UiWork_InitializeWithResourceCountersFar, UiWork_InitializeWithResourceCounters, void, (void))
FAR_CALL(UiWork_InitializeFar, UiWork_Initialize, void, (s32 mode))
#ifndef OWN_UiWindow_CreateFar
FAR_CALL(UiWindow_CreateFar, UiWindow_Create, s32, (s32 x, s32 y, s32 width, s32 height, s32 style))
#endif
FAR_ENTRY(UiWork_FinalizeFar, UiWork_Finalize)
FAR_CALL(Ui_AdjustValueWithoutLimitFar, Ui_AdjustValueWithoutLimit, void, (s32, u16 *))
FAR_CALL(
	UiText_DecodeMessageFar, UiText_DecodeMessage, void, (s32 message, u16 *buffer, s32 length))
FAR_CALL(UiText_CopyMessageStringFar, UiText_CopyMessageString, void,
	(s32 message_id, void *buffer, s32 length))
FAR_ENTRY(UiText_OpenMessageWindowFar, UiText_OpenMessageWindow)
FAR_ENTRY(UiText_ShowPositionedMessageAndWaitFar, UiText_ShowPositionedMessageAndWait)
FAR_CALL(UiWork_IsCompleteFar, UiWork_IsComplete, s32, (void))
FAR_CALL(UiWork_IsIdleFar, UiWork_IsIdle, s32, (struct UiWindow * window))
FAR_CALL(UiWork_SetBusyFlagsFar, UiWork_SetBusyFlags, void, (s32))
FAR_CALL(RenderOutput_PrepareForRedrawFar, RenderOutput_PrepareForRedraw, void, (s32 window))
FAR_CALL(UiWindow_ClearInteriorTilesFar, UiWindow_ClearInteriorTiles, void,
	(const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom))
FAR_ENTRY(UiWindow_DrawDividerLineFar, UiWindow_DrawDividerLine)
FAR_CALL(UiText_DrawResourceFar, UiText_DrawResource, void, (s32 message, s32 window, s32 x, s32 y))
#ifndef OWN_UiText_DrawCharacterAtOffsetFar
FAR_CALL(UiText_DrawCharacterAtOffsetFar, UiText_DrawCharacterAtOffset, void,
	(s32 message, s32 window, s32 x, s32 y))
#endif
FAR_CALL(UiText_DrawStringFar, UiText_DrawString, void, (u8 * text, s32 arg1, s32 arg2, s32 arg3))
FAR_ENTRY(UiText_DrawStringAtOffsetFar, UiText_DrawStringAtOffset)
FAR_CALL(UiText_DrawStringInWindowFar, UiText_DrawStringInWindow, void,
	(u8 * text, s32 arg1, u32 x, u32 y))
FAR_CALL(UiText_DrawNumberFar, UiText_DrawNumber, void,
	(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4))
FAR_ENTRY(UiText_DrawNumberAtOffsetFar, UiText_DrawNumberAtOffset)
FAR_CALL(UiText_DrawNumberInWindowFar, UiText_DrawNumberInWindow, void,
	(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4))
FAR_ENTRY(UiWork_SetParamNibbleFar, UiWork_SetParamNibble)
FAR_SLOT(UiWork_SetRenderWord)
FAR_CALL(RenderOutput_CreateFar, RenderOutput_Create, struct RenderOutput *,
	(s32 resource, s32 flags, struct RenderInput *window, s32 x, s32 y))
FAR_CALL(RenderOutput_CreateFromResourceFar, RenderOutput_CreateFromResource, struct RenderOutput *,
	(s32 mode, s32 resource, struct RenderInput *input, s32 x, s32 y))
FAR_CALL(SideObject_CreateFar, CreateSideObject, struct RenderOutput *,
	(s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5))
FAR_CALL(Ui_LoadCharacterEntryForSlotFar, Ui_LoadCharacterEntryForSlot, void, (s32 a, s32 b, s32 c))
FAR_SLOT(RenderOutput_SetAnimation)
FAR_SLOT(RenderOutput_SetInvertedSentinel)
FAR_CALL(UiWindow_CreateWithSideObjectFar, UiWindow_CreateWithSideObject, s32,
	(s32 arg0, s32 arg1, s32 x, s32 y))
FAR_ENTRY(UiWork_FinalizeEntityMatchingLocalizedIdFar, UiWork_FinalizeEntityMatchingLocalizedId)
FAR_CALL(UiText_GetResourceDimensionsFar, UiText_GetResourceDimensions, s32,
	(s32 no, s32 *x, s32 *y, u32 *width, u32 *height))
FAR_CALL(UiText_GetResourceDimensionsAltFar, UiText_GetResourceDimensionsAlt, s32,
	(s32 message, s32 *x, s32 *y, u32 *width, u32 *height))
FAR_CALL(UiWork_ClearValueNameTablesFar, UiWork_ClearValueNameTables, void, (void))
FAR_ENTRY(UiWork_PushValueSlotFar, UiWork_PushValueSlot)
FAR_CALL(UiWindow_CreateWithLayoutBoundsFar, UiWindow_CreateWithLayoutBounds, void, (s32 value))
FAR_CALL(PtyStDrawFar, PtyStDraw, void, (s32 flags))
FAR_CALL(UiWork_FinalizeAndReleaseBlock16Far, UiWork_FinalizeAndReleaseBlock16, void, (void))
FAR_ENTRY(UiWork_FinalizePendingCoreFar, UiWork_FinalizePendingCore)
FAR_CALL(UiWork_DrainPendingFar, UiWork_DrainPending, void, (s32))
FAR_SLOT(UiGlyph_ResetWorkState)
FAR_SLOT(Resource_ClearOwnerListAndCounters)
FAR_SLOT(Resource_PushPendingPair)
FAR_SLOT(Resource_ScheduleOwnerResetDelayed)
FAR_SLOT(Resource_ScheduleOwnerReset)
FAR_SLOT(Menu_OpenSelectionWindow)
FAR_SLOT(Resource_ResetOwnerEntries)
FAR_SLOT(Menu_SetupSelectionBothSides)
FAR_SLOT(Menu_SelectionLoop)
FAR_SLOT(Menu_WaitForSelectionInput)
FAR_SLOT(UiTextResource_NoOpCallbackBank)
FAR_SLOT(UiTextResource_NoOpCallback1)
FAR_SLOT(MenuSelection_BuildEntries)
FAR_SLOT(UiWork_ReservedNoOp)
FAR_CALL(Battle_CollectPartyCommandsFar, Battle_CollectPartyCommands, s32,
	(void *entries, u16 *excluded_units, s32 excluded_count))
FAR_CALL(UiText_ShowMessageAndWaitCoreFar, UiText_ShowMessageAndWaitCore, void, (s32 argument))
FAR_CALL(UiText_PrepareMessageWorkFar, UiText_PrepareMessageWork, void, (s32 message))
FAR_SLOT(UiTextResource_NoOpCallback4)
FAR_CALL(Localization_LookupEntryIdFar, Localization_LookupEntryId, s32, (u32 speaker))
FAR_CALL(Menu_RunTopSelectionFar, Menu_RunTopSelection, void, (void))
FAR_CALL(Menu_SelectSaveSlotActionFar, Menu_SelectSaveSlotAction, s32, (void))
FAR_CALL(Menu_SelectResourceLayoutFar, Menu_SelectResourceLayout, s32, (s32 mode))
FAR_CALL(UiTimedNotice_CreateFar, UiTimedNotice_Create, void, (s32 scene))
FAR_CALL(UiTimedNotice_CloseIfActiveFar, UiTimedNotice_CloseIfActive, void, (void))
FAR_CALL(UiText_ShowCenteredMessageFar, UiText_ShowCenteredMessage, void,
	(s32 message, s32 mode, s32 y_offset))
FAR_CALL(UiWork_FinalizeSharedSlotFar, UiWork_FinalizeSharedSlot, void, (void))
FAR_CALL(UiWork_ResetFreeChannelFar, UiWork_ResetFreeChannel, void, (void))
FAR_CALL(UiTextResource_InitializeFar, UiTextResource_Initialize, void,
	(struct MenuSprite * object, s32 *slot))
FAR_CALL(UiTextResource_SetPositionFar, UiTextResource_SetPosition, void,
	(struct MenuSprite * obj, s32 x, s32 y))
FAR_CALL(UiTextResource_ReleaseFar, UiTextResource_Release, void, (s32 value))
FAR_SLOT(Menu_RunSelectionHook)
FAR_SLOT(Item_CallHandler48)
FAR_CALL(Ui_PrepareTransferForItemFar, Ui_PrepareTransferForItem, void, (s32 code))
FAR_CALL(UiText_RenderWideStringAtOffsetFar, UiText_RenderWideStringAtOffset, void,
	(void *buffer, struct UiWindow *destination, s32 offset, s32 terminator))
FAR_SLOT(GraphicsPalette_SelectionNoOp)
FAR_CALL(UiWindow_FillFromSceneFar, UiWindow_FillFromScene, void, (u16 * lines))
FAR_CALL(
	RenderOutput_RedrawSavedRectFar, RenderOutput_RedrawSavedRect, void, (struct UiWindow * window))
FAR_CALL(RenderOutput_ClearListFar, RenderOutput_ClearList, void, (struct UiWindow * window))
FAR_ENTRY(UiWindow_SetTilemapEntryFar, UiWindow_SetTilemapEntry)
FAR_CALL(Menu_RunSelectionFar, Menu_RunSelection, void, (void))
FAR_CALL(Menu_RunSelectionWithCursorObjectFar, Menu_RunSelectionWithCursorObject, void, (void))
#ifndef OWN_Resource_LoadByModeIntoSlotFar
FAR_CALL(Resource_LoadByModeIntoSlotFar, Resource_LoadByModeIntoSlot, s32,
	(s32 style, s32 item, s32 target, s32 flags))
#endif
FAR_SLOT(Debug_SelectAbilityPair)
FAR_CALL(UiWindow_MarkVisibleTileAttributesFar, UiWindow_MarkVisibleTileAttributes, void, (void))
FAR_SLOT(ItemIcon_Compose)
FAR_CALL(BattleLayout_HighlightPartyPanelsFar, BattleLayout_HighlightPartyPanels, s32, (u16 * ids))
FAR_SLOT(UiIcon_BuildItemIconTiles)
FAR_SLOT(UiIcon_BuildAbilityIconTiles)
FAR_CALL(RenderOutput_CreateLoadedFar, RenderOutput_CreateLoaded, struct RenderOutput *,
	(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y))
FAR_SLOT(RenderOutput_CreateFromTable)
FAR_SLOT(SaveState_WriteCurrentSlotPair)
FAR_CALL(Save_WriteSelectedSlotFar, Save_WriteSelectedSlot, s32, (void))
FAR_CALL(SaveState_LoadRecordIntoWorkFar, SaveState_LoadRecordIntoWork, s32, (s32 arg))
FAR_CALL(SaveState_CopySlotPairFar, SaveState_CopySlotPair, s32, (void))
FAR_CALL(SaveState_DeleteSelectedSlotFar, SaveState_DeleteSelectedSlot, s32, (void))
FAR_CALL(SaveState_ProcessSelectedSlotFar, SaveState_ProcessSelectedSlot, s32, (void))
FAR_SLOT(SaveState_WriteSlotPair)
FAR_CALL(SaveState_ScanRecordFlagsFar, SaveState_ScanRecordFlags, s32, (void))
FAR_CALL(NameEntry_EditOwnerNameFar, NameEntry_EditOwnerName, s32, (s32 entry))
FAR_SLOT(Resource_CopyFixedBlockB)
FAR_SLOT(Resource_CopyFixedBlockA)
FAR_SLOT(Menu_SetNodeCoordinates)
FAR_CALL(DebugMenu_BrowseIconsFar, DebugMenu_BrowseIcons, s32, (void))
FAR_CALL(DebugMenu_BrowseEntryGlyphsFar, DebugMenu_BrowseEntryGlyphs, s32, (void))
FAR_SLOT(Item_ReturnTrue)
FAR_CALL(Djinn_ShowJoinedMessageFar, Djinn_ShowJoinedMessage, void, (s32, s32, void *))
FAR_CALL(PaletteGlow_UpdateFar, PaletteGlow_Update, void, (s32 phase, s32 brightness))
FAR_SLOT(Menu_RunWorkspaceOptions)
FAR_CALL(Menu_RunWorkspaceResultLoopFar, Menu_RunWorkspaceResultLoop, void, (s32 mode))
FAR_SLOT(UiWindow_FillScreenBlockRect)
FAR_CALL(Menu_SelectEntry11To14Far, Menu_SelectEntry11To14, s32, (s32 choice))
FAR_CALL(Menu_SelectEntry19To1cFar, Menu_SelectEntry19To1c, s32, (s32 prev))
FAR_CALL(
	Menu_RunConfirmSelectionFar, Menu_RunConfirmSelection, s32, (s32 value, s32 x, s32 y, s32 z))
FAR_CALL(Menu_SelectEntry20To21Far, Menu_SelectEntry20To21, s32, (s32 sel))
FAR_CALL(PartyTalkMenu_ChooseFar, PartyTalkMenu_Choose, s32, (s32 unused))
FAR_SLOT(RenderResource_LoadTableEntry)
FAR_CALL(Menu_RunConfirmSelectionAtFar, Menu_RunConfirmSelectionAt, s32, (s32, s32, s32))
FAR_CALL(Resource_LoadFixedBlockBIntoFreeSlotFar, Resource_LoadFixedBlockBIntoFreeSlot, u16, (void))
FAR_CALL(UiText_MeasureResourceEntriesFar, UiText_MeasureResourceEntries, void,
	(s32 message, s32 *width, s32 *height))
FAR_CALL(UiWindow_SetPaletteBitRectFar, Ui_SetRectHighlight, void, (s32, s32, s32, s32, s32))
FAR_CALL(Ability_LoadGlyphFar, Ability_LoadGlyph, s32, (s32, s32, s32 *, s32 *, s32))
FAR_CALL(
	Graphics_ExpandVramTilesByColorTableFar, Graphics_ExpandVramTilesByColorTable, void, (void *))
FAR_CALL(UiWork_SetAltFlagAndClearTableFar, UiWork_SetAltFlagAndClearTable, void, (s32 value))
FAR_CALL(Menu_AnimateSelectionToEntryFar, Menu_AnimateSelectionToEntry, s32, (s32 arg0, s32 arg1))
FAR_CALL(Ui_GetTableWordZeroFar, Ui_GetTableWordZero, s32, (s32 index))
FAR_CALL(UiText_OpenEntryMessageFar, UiText_OpenEntryMessage, s32, (s32 window, s32 message))
FAR_CALL(SaveState_CountRecordsExcludingFlaggedFar, SaveState_CountRecordsExcludingFlagged, s32,
	(s32 flag))
FAR_CALL(UiWindow_DrawFrameFar, UiWindow_DrawFrame, void, (s32 x, s32 y, s32 width, s32 height))
FAR_CALL(UiWindow_EraseBorderRectFar, UiWindow_EraseBorderRect, void,
	(s32 x, s32 y, s32 width, s32 height))
FAR_CALL(Link_DrawShiftedTilePairFar, Link_DrawShiftedTilePair, void, (s32 offset))
FAR_CALL(Party_ShowJoinedMessageFar, Party_ShowJoinedMessage, s32, (s32 event_id, s32 value))
FAR_CALL(Party_ShowPairJoinedMessageFar, Party_ShowPairJoinedMessage, s32, (s32 first, s32 second))
