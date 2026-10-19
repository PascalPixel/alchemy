/// @file
/// Window construction, text drawing and selection interfaces.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/WINDOW.S.

/// Far call to UiWork_InitializeWithResourceCounters.
void UiWork_InitializeWithResourceCountersFar(void);
/// Far call to UiWork_Initialize.
void UiWork_InitializeFar(s32 kind);
#ifndef OWN_UiWindow_CreateFar
/// Far call to UiWindow_Create.
struct UiWindow *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                    s32 attrs);
#endif
/// Far call to UiWork_Finalize.
void UiWork_FinalizeFar(struct UiWindow *work, s32 release);
/// Far call to Ui_AdjustValueWithoutLimit.
void Ui_AdjustValueWithoutLimitFar(s32 message, u16 *buf);
/// Far call to UiText_DecodeMessage.
void UiText_DecodeMessageFar(s32 message, u16 *text, s32 capacity);
/// Far call to UiText_CopyMessageString.
s32 UiText_CopyMessageStringFar(s32 key, u16 *destination, u32 capacity);
/// Far call to UiText_OpenMessageWindow.
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y,
                                             u32 packed);
/// Far call to UiText_ShowPositionedMessageAndWait.
void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);
/// Far call to UiWork_IsComplete.
s32 UiWork_IsCompleteFar(void);
/// Far call to UiWork_IsIdle.
s32 UiWork_IsIdleFar(struct UiWindow *window);
/// Far call to UiWork_SetBusyFlags.
void UiWork_SetBusyFlagsFar(s32 flags);
/// Far call to RenderOutput_PrepareForRedraw.
void RenderOutput_PrepareForRedrawFar(struct UiWindow *window);
/// Far call to UiWindow_ClearInteriorTiles.
void UiWindow_ClearInteriorTilesFar(const struct UiWindow *window, u32 left,
                                    u32 top, u32 right, u32 bottom);
/// Far call to UiWindow_DrawDividerLine.
void UiWindow_DrawDividerLineFar(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                                 u32 y2);
/// Far call to UiText_DrawResource.
void UiText_DrawResourceFar(s32 arg0, struct UiWindow *arg1, s32 arg2,
                            s32 arg3);
#ifndef OWN_UiText_DrawCharacterAtOffsetFar
/// Far call to UiText_DrawCharacterAtOffset.
void UiText_DrawCharacterAtOffsetFar(s32 character,
                                     const struct UiWindow *position,
                                     u32 offsetX, u32 offsetY);
#endif
/// Far call to UiText_DrawString.
void UiText_DrawStringFar(const u8 *text, struct UiWindow *arg1, s32 arg2,
                          s32 arg3);
/// Far call to UiText_DrawStringAtOffset.
void UiText_DrawStringAtOffsetFar(const u8 *text, const struct UiWindow *work,
                                  s32 offsetX, s32 offsetY);
/// Far call to UiText_DrawStringInWindow.
void UiText_DrawStringInWindowFar(const u8 *text, struct UiWindow *arg1, u32 x,
                                  u32 y);
/// Far call to UiText_DrawNumber.
void UiText_DrawNumberFar(s32 arg0, s32 arg1, struct UiWindow *arg2, s32 arg3,
                          s32 arg4);
/// Far call to UiText_DrawNumberAtOffset.
void UiText_DrawNumberAtOffsetFar(s32 arg0, s32 arg1,
                                  const struct UiWindow *arg2, s32 arg3,
                                  s32 arg4);
/// Far call to UiText_DrawNumberInWindow.
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, struct UiWindow *arg2,
                                  s32 arg3, s32 arg4);
/// Far call to UiWork_SetParamNibble.
void UiWork_SetParamNibbleFar(s32 param);
/// Far call to RenderOutput_Create.
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 flags,
                                            struct UiWindow *input, s32 offsetX,
                                            s32 offsetY);
/// Far call to RenderOutput_CreateFromResource.
struct RenderOutput *RenderOutput_CreateFromResourceFar(s32 mode, s32 resource,
                                                        struct UiWindow *input,
                                                        s32 x, s32 y);
/// Far call to CreateSideObject.
struct RenderOutput *SideObject_CreateFar(s32 object_kind, s32 position,
                                          s32 side, struct UiWindow *arg3,
                                          s32 arg4, s32 arg5);
/// Far call to Ui_LoadCharacterEntryForSlot.
void Ui_LoadCharacterEntryForSlotFar(u32 slot, s32 character, s32 value);
/// Far call to UiWindow_CreateWithSideObject.
struct UiWindow *UiWindow_CreateWithSideObjectFar(s32 arg0, s32 arg1, s32 x,
                                                  s32 y);
/// Far call to UiWork_FinalizeEntityMatchingLocalizedId.
void UiWork_FinalizeEntityMatchingLocalizedIdFar(u32 entry);
/// Far call to UiText_GetResourceDimensions.
s32 UiText_GetResourceDimensionsFar(s32 no, s32 *x, s32 *y, u32 *width,
                                    u32 *height);
/// Far call to UiText_GetResourceDimensionsAlt.
s32 UiText_GetResourceDimensionsAltFar(s32 no, s32 *x, s32 *y, u32 *width,
                                       u32 *height);
/// Far call to UiWork_ClearValueNameTables.
void UiWork_ClearValueNameTablesFar(void);
/// Far call to UiWork_PushValueSlot.
void UiWork_PushValueSlotFar(u32 value, u32 kind);
/// Far call to UiWindow_CreateWithLayoutBounds.
void UiWindow_CreateWithLayoutBoundsFar(s32 flags);
/// Far call to PartyStatusDraw.
void PartyStatusDrawFar(s32 flags);
/// Far call to PartyStatus_Close.
void PartyStatus_CloseFar(void);
/// Far call to UiWork_FinalizePendingCore.
void UiWork_FinalizePendingCoreFar(void);
/// Far call to UiWork_DrainPending.
void UiWork_DrainPendingFar(s32 unused);
/// Far call to Battle_CollectPartyCommands.
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *out, u16 *in,
                                   s32 count);
/// Far call to UiText_ShowMessageAndWaitCore.
void UiText_ShowMessageAndWaitCoreFar(s32 argument);
/// Far call to UiText_PrepareMessageWork.
void UiText_PrepareMessageWorkFar(s32 argument);
/// Far call to Localization_LookupEntryId.
s32 Localization_LookupEntryIdFar(u32 value);
/// Far call to Menu_RunTopSelection.
s32 Menu_RunTopSelectionFar(void);
/// Far call to Menu_SelectSaveSlotAction.
s32 Menu_SelectSaveSlotActionFar(void);
/// Far call to Menu_SelectResourceLayout.
s32 Menu_SelectResourceLayoutFar(s32 mode);
/// Far call to UiTimedNotice_Create.
void UiTimedNotice_CreateFar();
/// Far call to UiTimedNotice_CloseIfActive.
void UiTimedNotice_CloseIfActiveFar(void);
/// Far call to UiText_ShowCenteredMessage.
void UiText_ShowCenteredMessageFar(s32 message, s32 mode, s32 yOffset);
/// Far call to UiWork_FinalizeSharedSlot.
void UiWork_FinalizeSharedSlotFar(void);
/// Far call to UiWork_ResetFreeChannel.
void UiWork_ResetFreeChannelFar(void);
/// Far call to UiTextResource_Initialize.
void UiTextResource_InitializeFar(struct MenuSprite *object, s32 *slot);
/// Far call to UiTextResource_SetPosition.
void UiTextResource_SetPositionFar(struct MenuSprite *obj, s32 x, s32 y);
/// Far call to UiTextResource_Release.
void UiTextResource_ReleaseFar(s32 value);
/// Far call to Ui_PrepareTransferForItem.
void Ui_PrepareTransferForItemFar(s32 code);
/// Far call to UiText_RenderWideStringAtOffset.
void UiText_RenderWideStringAtOffsetFar(u16 *text, struct UiWindow *window,
                                        s32 x, s32 y);
/// Far call to UiWindow_FillFromScene.
void UiWindow_FillFromSceneFar(void *dst);
/// Far call to RenderOutput_RedrawSavedRect.
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
/// Far call to RenderOutput_ClearList.
void RenderOutput_ClearListFar(void *work);
/// Far call to UiWindow_SetTilemapEntry.
void UiWindow_SetTilemapEntryFar(struct UiWindow *window, s32 value, s32 x,
                                 s32 y, u32 mode);
#ifdef UBFIX
/// Far call to Menu_RunSelection.
void Menu_RunSelectionFar(void);
#else
/// Far call to Menu_RunSelection.
s16 Menu_RunSelectionFar(void);
#endif
/// Far call to Menu_RunSelectionWithCursorObject.
s32 Menu_RunSelectionWithCursorObjectFar(void);
#ifndef OWN_Resource_LoadByModeIntoSlotFar
/// Far call to Resource_LoadByModeIntoSlot.
s32 Resource_LoadByModeIntoSlotFar(s32 mode, s32 value, s32 result, s32 option);
#endif
/// Far call to UiWindow_MarkVisibleTileAttributes.
void UiWindow_MarkVisibleTileAttributesFar(void);
/// Far call to BattleLayout_HighlightPartyPanels.
s32 BattleLayout_HighlightPartyPanelsFar(u16 *ids);
/// Far call to RenderOutput_CreateLoaded.
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers,
                                                  struct UiWindow *input, s32 x,
                                                  s32 y);
/// Far call to Save_WriteSelectedSlot.
s32 Save_WriteSelectedSlotFar(void);
/// Far call to SaveState_LoadRecordIntoWork.
s32 SaveState_LoadRecordIntoWorkFar(s32 arg);
/// Far call to SaveState_CopySlotPair.
s32 SaveState_CopySlotPairFar(void);
/// Far call to SaveState_DeleteSelectedSlot.
s32 SaveState_DeleteSelectedSlotFar(void);
/// Far call to SaveState_ProcessSelectedSlot.
s32 SaveState_ProcessSelectedSlotFar(void);
/// Far call to SaveState_ScanRecordFlags.
s32 SaveState_ScanRecordFlagsFar(void);
/// Far call to NameEntry_EditOwnerName.
s32 NameEntry_EditOwnerNameFar(s32 entry);
/// Far call to DebugMenu_BrowseIcons.
s32 DebugMenu_BrowseIconsFar(void);
/// Far call to DebugMenu_BrowseEntryGlyphs.
s32 DebugMenu_BrowseEntryGlyphsFar(void);
/// Far call to Djinn_ShowJoinedMessage.
void Djinn_ShowJoinedMessageFar(s32 pc, s32 element, s32 djinn);
/// Far call to PaletteGlow_Update.
void PaletteGlow_UpdateFar(s32 phase, s32 brightness);
/// Far call to Menu_RunWorkspaceResultLoop.
s32 Menu_RunWorkspaceResultLoopFar();
/// Far call to Menu_SelectShopCommand.
s32 Menu_SelectShopCommandFar(s32 selection);
/// Far call to Menu_SelectSanctumService.
s32 Menu_SelectSanctumServiceFar(s32 selection);
/// Far call to Menu_RunConfirmSelection.
s32 Menu_RunConfirmSelectionFar(s32 low, s32 message, s32 w, s32 selection);
/// Far call to Menu_SelectCoinsOrMedals.
s32 Menu_SelectCoinsOrMedalsFar(s32 selection);
/// Far call to PartyTalkMenu_Choose.
s32 PartyTalkMenu_ChooseFar(s32 unused);
/// Far call to Menu_RunConfirmSelectionAt.
s32 Menu_RunConfirmSelectionAtFar(s32 x, s32 y, s32 selection);
/// Far call to Resource_LoadFixedBlockBIntoFreeSlot.
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
/// Far call to UiText_MeasureResourceEntries.
void UiText_MeasureResourceEntriesFar(s32 no, s32 *x, s32 *y);
/// Far call to Ui_SetRectHighlight.
void UiWindow_SetPaletteBitRectFar(s32 x, s32 y, s32 width, s32 height,
                                   s32 alt);
/// Far call to Ability_LoadGlyph.
void Ability_LoadGlyphFar(s32 action, s32 with_base, s32 *slot, s32 *tile,
                          s32 reuse);
/// Far call to Graphics_ExpandVramTilesByColorTable.
void Graphics_ExpandVramTilesByColorTableFar(u16 *dst);
/// Far call to UiWork_SetAltFlagAndClearTable.
void UiWork_SetAltFlagAndClearTableFar(s32 flag);
/// Far call to Menu_AnimateSelectionToEntry.
s32 Menu_AnimateSelectionToEntryFar(s32 arg0, s32 arg1);
/// Far call to Ui_GetTableWordZero.
s32 Ui_GetTableWordZeroFar(s32 no);
/// Far call to UiText_OpenEntryMessage.
s32 UiText_OpenEntryMessageFar(struct UiWindow *window, s32 argument);
/// Far call to SaveState_CountRecordsExcludingFlagged.
s32 SaveState_CountRecordsExcludingFlaggedFar(s32 flag);
/// Far call to UiWindow_DrawFrame.
void UiWindow_DrawFrameFar(s32 x, s32 y, u32 width, u32 height);
/// Far call to UiWindow_EraseBorderRect.
void UiWindow_EraseBorderRectFar(s32 x, s32 y, u32 width, u32 height);
/// Far call to Link_DrawShiftedTilePair.
void Link_DrawShiftedTilePairFar(s32 offset);
/// Far call to Party_ShowJoinedMessage.
void Party_ShowJoinedMessageFar();
/// Far call to Party_ShowPairJoinedMessage.
void Party_ShowPairJoinedMessageFar(s32 first, s32 second);
