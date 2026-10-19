// Window construction, text drawing and selection interfaces.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/WINDOW.S.
void UiWork_InitializeWithResourceCountersFar(void);
void UiWork_InitializeFar(s32 kind);
#ifndef OWN_UiWindow_CreateFar
struct UiWindow *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                    s32 attrs);
#endif
void UiWork_FinalizeFar(struct UiWindow *work, s32 release);
void Ui_AdjustValueWithoutLimitFar(s32 message, u16 *buf);
void UiText_DecodeMessageFar(s32 message, u16 *text, s32 capacity);
s32 UiText_CopyMessageStringFar(s32 key, u16 *destination, u32 capacity);
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y,
                                             u32 packed);
void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);
s32 UiWork_IsCompleteFar(void);
s32 UiWork_IsIdleFar(struct UiWindow *window);
void UiWork_SetBusyFlagsFar(s32 flags);
void RenderOutput_PrepareForRedrawFar(struct UiWindow *window);
void UiWindow_ClearInteriorTilesFar(const struct UiWindow *window, u32 left,
                                    u32 top, u32 right, u32 bottom);
void UiWindow_DrawDividerLineFar(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                                 u32 y2);
void UiText_DrawResourceFar(s32 arg0, struct UiWindow *arg1, s32 arg2,
                            s32 arg3);
#ifndef OWN_UiText_DrawCharacterAtOffsetFar
void UiText_DrawCharacterAtOffsetFar(s32 character,
                                     const struct UiWindow *position,
                                     u32 offsetX, u32 offsetY);
#endif
void UiText_DrawStringFar(const u8 *text, struct UiWindow *arg1, s32 arg2,
                          s32 arg3);
void UiText_DrawStringAtOffsetFar(const u8 *text, const struct UiWindow *work,
                                  s32 offsetX, s32 offsetY);
void UiText_DrawStringInWindowFar(const u8 *text, struct UiWindow *arg1, u32 x,
                                  u32 y);
void UiText_DrawNumberFar(s32 arg0, s32 arg1, struct UiWindow *arg2, s32 arg3,
                          s32 arg4);
void UiText_DrawNumberAtOffsetFar(s32 arg0, s32 arg1,
                                  const struct UiWindow *arg2, s32 arg3,
                                  s32 arg4);
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, struct UiWindow *arg2,
                                  s32 arg3, s32 arg4);
void UiWork_SetParamNibbleFar(s32 param);
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 flags,
                                            struct UiWindow *input, s32 offsetX,
                                            s32 offsetY);
struct RenderOutput *RenderOutput_CreateFromResourceFar(s32 mode, s32 resource,
                                                        struct UiWindow *input,
                                                        s32 x, s32 y);
struct RenderOutput *SideObject_CreateFar(s32 object_kind, s32 position,
                                          s32 side, struct UiWindow *arg3,
                                          s32 arg4, s32 arg5);
void Ui_LoadCharacterEntryForSlotFar(u32 slot, s32 character, s32 value);
struct UiWindow *UiWindow_CreateWithSideObjectFar(s32 arg0, s32 arg1, s32 x,
                                                  s32 y);
void UiWork_FinalizeEntityMatchingLocalizedIdFar(u32 entry);
s32 UiText_GetResourceDimensionsFar(s32 no, s32 *x, s32 *y, u32 *width,
                                    u32 *height);
s32 UiText_GetResourceDimensionsAltFar(s32 no, s32 *x, s32 *y, u32 *width,
                                       u32 *height);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(u32 value, u32 kind);
void UiWindow_CreateWithLayoutBoundsFar(s32 flags);
void PartyStatusDrawFar(s32 flags);
void PartyStatus_CloseFar(void);
void UiWork_FinalizePendingCoreFar(void);
void UiWork_DrainPendingFar(s32 unused);
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *out, u16 *in,
                                   s32 count);
void UiText_ShowMessageAndWaitCoreFar(s32 argument);
void UiText_PrepareMessageWorkFar(s32 argument);
s32 Localization_LookupEntryIdFar(u32 value);
s32 Menu_RunTopSelectionFar(void);
s32 Menu_SelectSaveSlotActionFar(void);
s32 Menu_SelectResourceLayoutFar(s32 mode);
void UiTimedNotice_CreateFar();
void UiTimedNotice_CloseIfActiveFar(void);
void UiText_ShowCenteredMessageFar(s32 message, s32 mode, s32 yOffset);
void UiWork_FinalizeSharedSlotFar(void);
void UiWork_ResetFreeChannelFar(void);
void UiTextResource_InitializeFar(struct MenuSprite *object, s32 *slot);
void UiTextResource_SetPositionFar(struct MenuSprite *obj, s32 x, s32 y);
void UiTextResource_ReleaseFar(s32 value);
void Ui_PrepareTransferForItemFar(s32 code);
void UiText_RenderWideStringAtOffsetFar(u16 *text, struct UiWindow *window,
                                        s32 x, s32 y);
void UiWindow_FillFromSceneFar(void *dst);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(void *work);
void UiWindow_SetTilemapEntryFar(struct UiWindow *window, s32 value, s32 x,
                                 s32 y, u32 mode);
#ifdef UBFIX
void Menu_RunSelectionFar(void);
#else
s16 Menu_RunSelectionFar(void);
#endif
s32 Menu_RunSelectionWithCursorObjectFar(void);
#ifndef OWN_Resource_LoadByModeIntoSlotFar
s32 Resource_LoadByModeIntoSlotFar(s32 mode, s32 value, s32 result, s32 option);
#endif
void UiWindow_MarkVisibleTileAttributesFar(void);
s32 BattleLayout_HighlightPartyPanelsFar(u16 *ids);
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers,
                                                  struct UiWindow *input, s32 x,
                                                  s32 y);
s32 Save_WriteSelectedSlotFar(void);
s32 SaveState_LoadRecordIntoWorkFar(s32 arg);
s32 SaveState_CopySlotPairFar(void);
s32 SaveState_DeleteSelectedSlotFar(void);
s32 SaveState_ProcessSelectedSlotFar(void);
s32 SaveState_ScanRecordFlagsFar(void);
s32 NameEntry_EditOwnerNameFar(s32 entry);
s32 DebugMenu_BrowseIconsFar(void);
s32 DebugMenu_BrowseEntryGlyphsFar(void);
void Djinn_ShowJoinedMessageFar(s32 pc, s32 element, s32 djinn);
void PaletteGlow_UpdateFar(s32 phase, s32 brightness);
s32 Menu_RunWorkspaceResultLoopFar();
s32 Menu_SelectEntry11To14Far(s32 selection);
s32 Menu_SelectEntry19To1cFar(s32 selection);
s32 Menu_RunConfirmSelectionFar(s32 low, s32 message, s32 w, s32 selection);
s32 Menu_SelectEntry20To21Far(s32 selection);
s32 PartyTalkMenu_ChooseFar(s32 unused);
s32 Menu_RunConfirmSelectionAtFar(s32 x, s32 y, s32 selection);
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
void UiText_MeasureResourceEntriesFar(s32 no, s32 *x, s32 *y);
void UiWindow_SetPaletteBitRectFar(s32 x, s32 y, s32 width, s32 height,
                                   s32 alt);
void Ability_LoadGlyphFar(s32 action, s32 with_base, s32 *slot, s32 *tile,
                          s32 reuse);
void Graphics_ExpandVramTilesByColorTableFar(u16 *dst);
void UiWork_SetAltFlagAndClearTableFar(s32 flag);
s32 Menu_AnimateSelectionToEntryFar(s32 arg0, s32 arg1);
s32 Ui_GetTableWordZeroFar(s32 no);
s32 UiText_OpenEntryMessageFar(struct UiWindow *window, s32 argument);
s32 SaveState_CountRecordsExcludingFlaggedFar(s32 flag);
void UiWindow_DrawFrameFar(s32 x, s32 y, u32 width, u32 height);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, u32 width, u32 height);
void Link_DrawShiftedTilePairFar(s32 offset);
void Party_ShowJoinedMessageFar();
void Party_ShowPairJoinedMessageFar(s32 first, s32 second);
