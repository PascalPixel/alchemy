#ifndef ALCHEMY_FAR_CALL_WINDOW_H
#define ALCHEMY_FAR_CALL_WINDOW_H

/// @file
/// The window bank's far calls: windows, text and portraits, the menus and
/// screens built from them (Pause menu, File Screen, Yes/No), and the debug
/// selectors.

void WindowSystem_InitFieldFar(void);
void WindowSystem_InitBattleFar(s32 loadGlyphTiles);
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_CloseFar(struct Window *window, s32 instant);
void Message_DecodeRawUnboundedFar(s32 message, u16 *text);
void Message_DecodeRawFar(s32 message, u16 *text, s32 capacity);
s32 Message_CopyExpandedFar(s32 message, u16 *destination, u32 capacity);
struct Window *Message_OpenWindowFar(s32 message, s32 x, s32 y, u32 packed);
void FieldMessage_ShowFar(s32 message, s32 flags);
s32 Message_AllTypedFar(void);
s32 Window_IsClosedFar(struct Window *window);
void Message_SetDecodeFlagsFar(s32 flags);
void WindowSprite_PrepareForRedrawFar(struct Window *window);
void Window_ClearInteriorTilesFar(const struct Window *window, u32 left,
                                  u32 top, u32 right, u32 bottom);
void Window_DrawDividerLineFar(struct Window *win, u32 x1, u32 y1, u32 x2,
                               u32 y2);
void Text_DrawMessageFar(s32 message, struct Window *window, s32 x, s32 y);
void Text_DrawMessageAsTilesFar(s32 message, const struct Window *window,
                                u32 offsetX, u32 offsetY);
void Text_DrawStringFar(const u8 *text, struct Window *window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(const u8 *text, const struct Window *window,
                                s32 offsetX, s32 offsetY);
void Text_DrawStringInWindowFar(const u8 *text, struct Window *window, u32 x,
                                u32 y);
void Text_DrawNumberFar(s32 value, s32 width, struct Window *window, s32 x,
                        s32 y);
void Text_DrawNumberAtOffsetFar(s32 value, s32 width,
                                const struct Window *window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 width, struct Window *window,
                                s32 x, s32 y);
void Text_SetColorFar(s32 color);
struct WindowSprite *WindowSprite_CreateFar(s32 slot, s32 oamFlags,
                                            struct Window *window, s32 offsetX,
                                            s32 offsetY);
struct WindowSprite *WindowSprite_CreateFromResourceFar(s32 mode, s32 resource,
                                                        struct Window *window,
                                                        s32 x, s32 y);
struct WindowSprite *Portrait_CreateFar(s32 character, s32 unused, s32 side,
                                        struct Window *window, s32 x, s32 y);
void Portrait_ReloadFar(u32 side, s32 character, s32 unused);
struct Window *Portrait_OpenWindowFar(s32 speaker, s32 portraitOption, s32 x,
                                      s32 y);
void Portrait_CloseWindowFar(u32 speaker);
/// Decodes `message` and fits a window for its first page on the screen near
/// tile (*x, *y), returning its place and size in tiles.
s32 Text_GetMessageSizeFar(s32 message, s32 *x, s32 *y, u32 *width,
                           u32 *height);
/// As Text_GetMessageSize, but measuring every page of the message;
/// the field's speaker windows use it.
s32 Text_GetMessageSizeAllPagesFar(s32 message, s32 *x, s32 *y, u32 *width,
                                   u32 *height);
void MessageArgument_ClearAllFar(void);
void MessageArgument_PushFar(u32 value, u32 kind);
void PartyStatus_CreateWindowFar(s32 flags);
void PartyStatus_DrawFar(s32 flags);
void PartyStatus_CloseFar(void);
void Message_CloseFinishedWindowsFar(void);
void Window_CloseAllFar(s32 unused);
s32 Battle_CollectPartyCommandsFar(struct BattleAction *commands, u16 *actors,
                                   s32 commandCount);
void BattleMessage_ShowAndWaitFar(s32 argument);
void BattleMessage_OpenFar(s32 argument);
s32 Portrait_GetForSpeakerFar(u32 value);
s32 FieldMenu_RunFar(void);
s32 FileList_SelectActionFar(void);
s32 IconMenu_ChooseSendOptionFar(s32 mode);
void AreaNameWindow_ShowFar();
void AreaNameWindow_HideFar(void);
void FieldMessage_ShowCenteredFar(s32 message, s32 mode, s32 yOffset);
void BattleMessage_CloseFar(void);
void Message_ResetIdleFar(void);
void MenuArrow_CreateFar(struct MenuSprite *sprite, s32 *slot);
void MenuArrow_SetPositionFar(struct MenuSprite *sprite, s32 x, s32 y);
void MenuArrow_ReleaseFar(s32 value);
void Icon_DrawItemFar(s32 code);
void Text_DrawWideStringAtOffsetFar(u16 *text, struct Window *window, s32 x,
                                    s32 y);
void ScanlineWindow_FillSpansFar(void *spans);
void WindowSprite_RedrawSavedRectFar(struct Window *window);
void WindowSprite_ClearListFar(void *sprites);
void Window_SetTilemapEntryFar(struct Window *window, s32 value, s32 x, s32 y,
                               u32 mode);
#ifdef UBFIX
/// The debug scene selector, returning nothing under UBFIX.
void DebugSceneSelect_RunFar(void);
#else
/// The debug scene selector: A leaves for the chosen scene and entrance, B
/// closes it. In debug mode, B held as the Pause menu opens brings it up.
s16 DebugSceneSelect_RunFar(void);
#endif
s32 DebugFlagEditor_RunFar(void);
s32 Resource_LoadByModeIntoSlotFar(s32 mode, s32 value, s32 result, s32 option);
void Window_MarkVisibleTextTilesFar(void);
s32 PartyStatus_HighlightMembersFar(u16 *ids);
struct WindowSprite *WindowSprite_CreateLoadedFar(s32 icon, s32 layers,
                                                  struct Window *window, s32 x,
                                                  s32 y);
s32 Save_ToChosenFileFar(void);
s32 Save_ChooseAndLoadFileFar(s32 mode);
s32 Save_CopyFileFar(void);
s32 Save_EraseSelectedFileFar(void);
s32 Save_ProcessSelectedFileFar(void);
s32 Save_ScanFileSummariesFar(void);
s32 NameEntry_EditMemberNameFar(s32 member);
s32 DebugBrowser_IconsFar(void);
s32 DebugBrowser_PortraitsFar(void);
void Djinn_ShowJoinedMessageFar(s32 member, s32 element, s32 djinn);
void WindowColor_ApplyFar(s32 phase, s32 brightness);
s32 PauseMenu_RunFar();
s32 Menu_SelectShopCommandFar(s32 selection);
s32 Menu_SelectSanctumServiceFar(s32 selection);
s32 YesNoMenu_RunFar(s32 bottom, s32 message, s32 labelWidth, s32 selection);
s32 Menu_SelectCoinsOrMedalsFar(s32 selection);
s32 ColossoMenu_ChooseFar(s32 unused);
s32 YesNoMenu_RunAtFar(s32 x, s32 y, s32 selection);
s32 PointingHand_LoadIntoFreeSlotFar(void);
void Text_MeasureMessageFar(s32 message, s32 *x, s32 *y);
void Window_HighlightRectFar(s32 x, s32 y, s32 width, s32 height,
                             s32 paletteBit);
void Ability_BuildIconTilesFar(s32 action, s32 withBase, s32 *slot, s32 *tile,
                               s32 reuse);
void Graphics_ExpandVramTilesByColorTableFar(u16 *dst);
void Text_UseExtraTilesFar(s32 allowAlternateTiles);
s32 Menu_AnimateSelectionToEntryFar(s32 start, s32 command);
s32 WindowSprite_GetPointerTilesFar(s32 no);
s32 Message_OpenDecodedFar(struct Window *window, s32 message);
s32 Save_CountFilesExcludingSentFar(s32 excludeSent);
void Window_DrawBorderFar(s32 x, s32 y, u32 width, u32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, u32 width, u32 height);
void PageArrows_DrawFar(s32 offset);
void Party_ShowJoinedMessageFar();
void Party_ShowPairJoinedMessageFar(s32 first, s32 second);

#endif
