#ifndef ALCHEMY_WINDOW_STUBS_H
#define ALCHEMY_WINDOW_STUBS_H

/* @file */
/* The window module's stub calls: windows, text and portraits, the menus and */
/* screens built from them (Pause menu, File Screen, Yes/No), and the debug */
/* selectors. */

void mnWindowSystemInitFieldStub(void);
void mnWindowSystemInitBattleStub(s32 loadGlyphTiles);
struct WINDOW *mnWindowCreateStub(s32 x, s32 y, s32 width, s32 height,
								  s32 attrs);
void mnWindowCloseStub(struct WINDOW *window, s32 instant);
void mnMessageDecodeRawUnboundedStub(s32 message, u16 *text);
void mnMessageDecodeRawStub(s32 message, u16 *text, s32 capacity);
s32 mnMessageCopyExpandedStub(s32 message, u16 *destination, u32 capacity);
struct WINDOW *mnMessageOpenWindowStub(s32 message, s32 x, s32 y, u32 packed);
void mnFieldMessageShowStub(s32 message, s32 flags);
s32 mnMessageAllTypedStub(void);
s32 mnWindowIsClosedStub(struct WINDOW *window);
void mnMessageSetDecodeFlagsStub(s32 flags);
void mnWindowSprPrepareForRedrawStub(struct WINDOW *window);
void mnWindowClearInteriorTilesStub(const struct WINDOW *window, u32 left,
									u32 top, u32 right, u32 bottom);
void mnWindowDrawDividerLineStub(struct WINDOW *win, u32 x1, u32 y1, u32 x2,
								 u32 y2);
void mnTextDrawMessageStub(s32 message, struct WINDOW *window, s32 x, s32 y);
void mnTextDrawMessageAsTilesStub(s32 message, const struct WINDOW *window,
								  u32 offsetX, u32 offsetY);
void mnTextDrawStringStub(const u8 *text, struct WINDOW *window, s32 x, s32 y);
void mnTextDrawStringAtOffsetStub(const u8 *text, const struct WINDOW *window,
								  s32 offsetX, s32 offsetY);
void mnTextDrawStringInWindowStub(const u8 *text, struct WINDOW *window, u32 x,
								  u32 y);
void mnTextDrawNumStub(s32 value, s32 width, struct WINDOW *window, s32 x,
					   s32 y);
void mnTextDrawNumAtOffsetStub(s32 value, s32 width,
							   const struct WINDOW *window, s32 x, s32 y);
void mnTextDrawNumInWindowStub(s32 value, s32 width, struct WINDOW *window,
							   s32 x, s32 y);
void mnTextSetColStub(s32 color);
struct WINDOW_SPR *mnWindowSprCreateStub(s32 slot, s32 oamFlags,
										 struct WINDOW *window, s32 offsetX,
										 s32 offsetY);
struct WINDOW_SPR *mnWindowSprCreateFromResourceStub(s32 mode, s32 resource,
													 struct WINDOW *window,
													 s32 x, s32 y);
struct WINDOW_SPR *mnPortraitCreateStub(s32 character, s32 unused, s32 side,
										struct WINDOW *window, s32 x, s32 y);
void mnPortraitReloadStub(u32 side, s32 character, s32 unused);
struct WINDOW *mnPortraitOpenWindowStub(s32 speaker, s32 portraitOption, s32 x,
										s32 y);
void mnPortraitCloseWindowStub(u32 speaker);
/* Decodes `message` and fits a window for its first page on the screen near */
/* tile (*x, *y), returning its place and size in tiles. */
s32 mnTextGetMessageSizeStub(s32 message, s32 *x, s32 *y, u32 *width,
							 u32 *height);
/* As mnTextGetMessageSize, but measuring every page of the message; */
/* the field's speaker windows use it. */
s32 mnTextGetMessageSizeAllPagesStub(s32 message, s32 *x, s32 *y, u32 *width,
									 u32 *height);
void mnMessageArgumentClearAllStub(void);
void mnMessageArgumentPushStub(u32 value, u32 kind);
void mnPartyStatCreateWindowStub(s32 flags);
void mnPartyStatDrawStub(s32 flags);
void mnPartyStatCloseStub(void);
void mnMessageCloseFinishedWindowsStub(void);
void mnWindowCloseAllStub(s32 unused);
s32 btCollectPartyCommandsStub(struct BATTLE_ACTION *commands, u16 *actors,
							   s32 commandCount);
void mnBattleMessageShowAndWaitStub(s32 message);
void mnBattleMessageOpenStub(s32 message);
s32 mnPortraitGetForSpeakerStub(u32 speaker);
s32 mnFieldMenuRunStub(void);
s32 mnFileListSelectActionStub(void);
s32 mnIconMenuChooseSendOptionStub(s32 mode);
void mnAreaNameWindowShowStub();
void mnAreaNameWindowHideStub(void);
void mnFieldMessageShowCenteredStub(s32 message, s32 mode, s32 yOffset);
void mnBattleMessageCloseStub(void);
void mnMessageResetIdleStub(void);
void mnArrowCreateStub(struct MENU_SPR *sprite, s32 *slot);
void mnArrowSetPosStub(struct MENU_SPR *sprite, s32 x, s32 y);
void mnArrowReleaseStub(s32 slot);
void mnIconDrawItemStub(s32 code);
void mnTextDrawWideStringAtOffsetStub(u16 *text, struct WINDOW *window, s32 x,
									  s32 y);
void mnScanlineWindowFillSpansStub(void *spans);
void mnWindowSprRedrawSavedRectStub(struct WINDOW *window);
void mnWindowSprClearListStub(void *sprites);
void mnWindowSetTilemapEntryStub(struct WINDOW *window, s32 value, s32 x, s32 y,
								 u32 mode);
#ifdef UBFIX
/* The debug scene selector, returning nothing under UBFIX. */
void mnDebugSceneSelectRunStub(void);
#else
/* The debug scene selector: A leaves for the chosen scene and entrance, B */
/* closes it. In debug mode, B held as the Pause menu opens brings it up. */
s16 mnDebugSceneSelectRunStub(void);
#endif
s32 mnDebugFlagEditorRunStub(void);
s32 mnSelectionIconLoadIntoSlotStub(s32 kind, s32 entry, s32 slot,
									s32 baseIcon);
void mnWindowMarkVisibleTextTilesStub(void);
s32 mnPartyStatHighlightMembersStub(u16 *ids);
struct WINDOW_SPR *mnWindowSprCreateLoadedStub(s32 icon, s32 layers,
											   struct WINDOW *window, s32 x,
											   s32 y);
s32 sySaveToChosenFileStub(void);
s32 sySaveChooseAndLoadFileStub(s32 mode);
s32 sySaveCopyFileStub(void);
s32 sySaveEraseSelectedFileStub(void);
s32 sySaveWriteLinkTalliesStub(void);
s32 sySaveScanFileSummariesStub(void);
s32 mnNameEntryEditMemberNameStub(s32 member);
s32 mnDebugBrowserIconsStub(void);
s32 mnDebugBrowserPortraitsStub(void);
void gmDjinnShowJoinedMessageStub(s32 member, s32 element, s32 djinn);
void mnWindowColApplyStub(s32 phase, s32 brightness);
s32 mnPauseMenuRunStub();
s32 mnSelectShopCommandStub(s32 selection);
s32 mnSelectSanctumServiceStub(s32 selection);
s32 mnYesNoMenuRunStub(s32 bottom, s32 message, s32 labelWidth, s32 selection);
s32 mnSelectCoinsOrMedalsStub(s32 selection);
s32 mnColossoMenuChooseStub(s32 unused);
s32 mnYesNoMenuRunAtStub(s32 x, s32 y, s32 selection);
s32 mnPointingHandLoadIntoFreeSlotStub(void);
void mnTextMeasureMessageStub(s32 message, s32 *x, s32 *y);
void mnWindowHighlightRectStub(s32 x, s32 y, s32 width, s32 height,
							   s32 paletteBit);
void mnAbilityBuildIconTilesStub(s32 action, s32 withBase, s32 *slot, s32 *tile,
								 s32 reuse);
void mnGraphicsExpandVramTilesByColTableStub(u16 *dst);
void mnTextUseExtraTilesStub(s32 allowAlternateTiles);
s32 mnAnimateSelectionToEntryStub(s32 start, s32 command);
s32 mnWindowSprGetPointerTilesStub(s32 index);
s32 mnMessageOpenDecodedStub(struct WINDOW *window, s32 message);
s32 sySaveCountFilesExcludingSentStub(s32 excludeSent);
void mnWindowDrawBorderStub(s32 x, s32 y, u32 width, u32 height);
void mnWindowEraseBorderRectStub(s32 x, s32 y, u32 width, u32 height);
void mnPageArrowsDrawStub(s32 offset);
void gmPartyShowJoinedMessageStub();
void gmPartyShowPairJoinedMessageStub(s32 first, s32 second);

#endif
