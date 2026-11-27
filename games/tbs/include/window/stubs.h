#ifndef ALCHEMY_WINDOW_STUBS_H
#define ALCHEMY_WINDOW_STUBS_H

/* The window module's stub calls: windows, text and portraits, the menus and
   screens built from them (Pause menu, File Screen, Yes/No), and the debug
   selectors. */

void		   mnWindowSystemInitFieldStub(void);
void		   mnWindowSystemInitBattleStub(Sint32 loadGlyphTiles);
struct WINDOW *mnWindowCreateStub(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 attrs);
void   mnWindowCloseStub(struct WINDOW *win, Sint32 instant);
void   mnMessageDecodeRawUnboundedStub(Sint32 msg, Uint16 *text);
void   mnMessageDecodeRawStub(Sint32 msg, Uint16 *text, Sint32 capacity);
Sint32 mnMessageCopyExpandedStub(Sint32 msg, Uint16 *dst, Uint32 capacity);
struct WINDOW *mnMessageOpenWindowStub(
	Sint32 msg, Sint32 x, Sint32 y, Uint32 packed);
void   mnFieldMessageShowStub(Sint32 msg, Sint32 flg);
Sint32 mnMessageAllTypedStub(void);
Sint32 mnWindowIsClosedStub(struct WINDOW *win);
void   mnMessageSetDecodeFlagsStub(Sint32 flg);
void   mnWindowSprPrepareForRedrawStub(struct WINDOW *win);
void   mnWindowClearInteriorTilesStub(const struct WINDOW *win, Uint32 left,
	  Uint32 top, Uint32 right, Uint32 bottom);
void   mnWindowDrawDividerLineStub(
	  struct WINDOW *win, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2);
void mnTextDrawMessageStub(Sint32 msg, struct WINDOW *win, Sint32 x, Sint32 y);
void mnTextDrawMessageAsTilesStub(
	Sint32 msg, const struct WINDOW *win, Uint32 ofsX, Uint32 ofsY);
void mnTextDrawStringStub(
	const Uint8 *text, struct WINDOW *win, Sint32 x, Sint32 y);
void mnTextDrawStringAtOffsetStub(
	const Uint8 *text, const struct WINDOW *win, Sint32 ofsX, Sint32 ofsY);
void mnTextDrawStringInWindowStub(
	const Uint8 *text, struct WINDOW *win, Uint32 x, Uint32 y);
void mnTextDrawNumStub(
	Sint32 value, Sint32 width, struct WINDOW *win, Sint32 x, Sint32 y);
void mnTextDrawNumAtOffsetStub(
	Sint32 value, Sint32 width, const struct WINDOW *win, Sint32 x, Sint32 y);
void mnTextDrawNumInWindowStub(
	Sint32 value, Sint32 width, struct WINDOW *win, Sint32 x, Sint32 y);
void			   mnTextSetColStub(Sint32 col);
struct WINDOW_SPR *mnWindowSprCreateStub(
	Sint32 slot, Sint32 oamFlg, struct WINDOW *win, Sint32 ofsX, Sint32 ofsY);
struct WINDOW_SPR *mnWindowSprCreateFromResourceStub(
	Sint32 mode, Sint32 resource, struct WINDOW *win, Sint32 x, Sint32 y);
struct WINDOW_SPR *mnPortraitCreateStub(Sint32 character, Sint32 unused,
	Sint32 side, struct WINDOW *win, Sint32 x, Sint32 y);
void mnPortraitReloadStub(Uint32 side, Sint32 character, Sint32 unused);
struct WINDOW *mnPortraitOpenWindowStub(
	Sint32 speaker, Sint32 portraitOption, Sint32 x, Sint32 y);
void mnPortraitCloseWindowStub(Uint32 speaker);
/* Decodes `msg` and fits a window for its first page on the screen near
   tile (*x, *y), returning its place and size in tiles. */
Sint32 mnTextGetMessageSizeStub(
	Sint32 msg, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height);
/* As mnTextGetMessageSize, but measuring every page of the message; the field's
   speaker windows use it. */
Sint32 mnTextGetMessageSizeAllPagesStub(
	Sint32 msg, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height);
void   mnMessageArgumentClearAllStub(void);
void   mnMessageArgumentPushStub(Uint32 value, Uint32 kind);
void   mnPartyStatCreateWindowStub(Sint32 flg);
void   mnPartyStatDrawStub(Sint32 flg);
void   mnPartyStatCloseStub(void);
void   mnMessageCloseFinishedWindowsStub(void);
void   mnWindowCloseAllStub(Sint32 unused);
Sint32 mnBattleCollectPartyCommandsStub(
	struct BATTLE_ACTION *commands, Uint16 *actors, Sint32 commandCnt);
void   mnBattleMessageShowAndWaitStub(Sint32 msg);
void   mnBattleMessageOpenStub(Sint32 msg);
Sint32 mnPortraitGetForSpeakerStub(Uint32 speaker);
Sint32 mnFieldMenuRunStub(void);
Sint32 mnFileListSelectActionStub(void);
Sint32 mnIconMenuChooseSendOptionStub(Sint32 mode);
void   mnAreaNameWindowShowStub();
void   mnAreaNameWindowHideStub(void);
void   mnFieldMessageShowCenteredStub(Sint32 msg, Sint32 mode, Sint32 yOfs);
void   mnBattleMessageCloseStub(void);
void   mnMessageResetIdleStub(void);
void   mnArrowCreateStub(struct MENU_SPR *spr, Sint32 *slot);
void   mnArrowSetPosStub(struct MENU_SPR *spr, Sint32 x, Sint32 y);
void   mnArrowReleaseStub(Sint32 slot);
void   mnIconDrawItemStub(Sint32 code);
void   mnTextDrawWideStringAtOffsetStub(
	  Uint16 *text, struct WINDOW *win, Sint32 x, Sint32 y);
void mnScanlineWindowFillSpansStub(void *spans);
void mnWindowSprRedrawSavedRectStub(struct WINDOW *win);
void mnWindowSprClearListStub(void *sprs);
void mnWindowSetTilemapEntryStub(
	struct WINDOW *win, Sint32 value, Sint32 x, Sint32 y, Uint32 mode);
#if FIX_FUNCTION_SIGNATURES
/* The debug scene selector, returning nothing under UBFIX. */
void mnDebugSceneSelectRunStub(void);
#else
/* The debug scene selector: A leaves for the chosen scene and entrance, B
   closes it. In debug mode, B held as the Pause menu opens brings it up. */
Sint16 mnDebugSceneSelectRunStub(void);
#endif
Sint32 mnDebugFlagEditorRunStub(void);
Sint32 mnSelectionIconLoadIntoSlotStub(
	Sint32 kind, Sint32 entry, Sint32 slot, Sint32 baseIcon);
void			   mnWindowMarkVisibleTextTilesStub(void);
Sint32			   mnPartyStatHighlightMembersStub(Uint16 *ids);
struct WINDOW_SPR *mnWindowSprCreateLoadedStub(
	Sint32 icon, Sint32 layers, struct WINDOW *win, Sint32 x, Sint32 y);
Sint32 mnSaveToChosenFileStub(void);
Sint32 mnSaveChooseAndLoadFileStub(Sint32 mode);
Sint32 mnSaveCopyFileStub(void);
Sint32 mnSaveEraseSelectedFileStub(void);
Sint32 mnSaveWriteLinkTalliesStub(void);
Sint32 mnSaveScanFileSummariesStub(void);
Sint32 mnNameEntryEditMemberNameStub(Sint32 member);
Sint32 mnDebugBrowserIconsStub(void);
Sint32 mnDebugBrowserPortraitsStub(void);
void mnDjinnShowJoinedMessageStub(Sint32 member, Sint32 element, Sint32 djinn);
void mnWindowColApplyStub(Sint32 phase, Sint32 brightness);
Sint32 mnPauseMenuRunStub();
Sint32 mnSelectShopCommandStub(Sint32 selection);
Sint32 mnSelectSanctumServiceStub(Sint32 selection);
Sint32 mnYesNoMenuRunStub(
	Sint32 bottom, Sint32 msg, Sint32 labelWidth, Sint32 selection);
Sint32 mnSelectCoinsOrMedalsStub(Sint32 selection);
Sint32 mnColossoMenuChooseStub(Sint32 unused);
Sint32 mnYesNoMenuRunAtStub(Sint32 x, Sint32 y, Sint32 selection);
Sint32 mnPointingHandLoadIntoFreeSlotStub(void);
void   mnTextMeasureMessageStub(Sint32 msg, Sint32 *x, Sint32 *y);
void   mnWindowHighlightRectStub(
	  Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 paletteBit);
void mnAbilityBuildIconTilesStub(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse);
void   mnGraphicsExpandVRAMTilesByColTableStub(Uint16 *dst);
void   mnTextUseExtraTilesStub(Sint32 allowAlternateTiles);
Sint32 mnAnimateSelectionToEntryStub(Sint32 start, Sint32 command);
Sint32 mnWindowSprGetPointerTilesStub(Sint32 idx);
Sint32 mnMessageOpenDecodedStub(struct WINDOW *win, Sint32 msg);
Sint32 mnSaveCountFilesExcludingSentStub(Sint32 excludeSent);
void   mnWindowDrawBorderStub(Sint32 x, Sint32 y, Uint32 width, Uint32 height);
void   mnWindowEraseBorderRectStub(
	  Sint32 x, Sint32 y, Uint32 width, Uint32 height);
void mnPageArrowsDrawStub(Sint32 ofs);
void mnPartyShowJoinedMessageStub();
void mnPartyShowPairJoinedMessageStub(Sint32 first, Sint32 second);

#endif
