/* @file */
/* The window that shows the area's name, and removing an item from the party's
 */
/* inventory. */
/* Part of window.c. */

void mnAreaNameWindowUpdate(void);

/* Shows the area's name in a window near the top of the screen for */
/* AREA_NAME_WINDOW_FRAMES, kept in the field state until */
/* mnAreaNameWindowUpdate closes it. */
void mnAreaNameWindowShow(void) {
	FIELD_STATE *fieldState;
	s32 height;
	s32 width;
	s32 y;
	s32 x;
	s32 areaMessage;
	WINDOW *nameWindow;
	MAIN_ROUTINE_STATE *state;

	fieldState = FieldStateWork;
	state = &MainRoutineStateWork;
	x = 8;
	y = 8;
	areaMessage = fdAreaGetNameIndexStub(state->next, state->entrance) +
				  (s32)&MsgDebugAreaName;
	mnTextGetMessageSize(areaMessage, &x, &y, &width, &height);
	x = (DISPLAY_TILE_WIDTH - width) >> 1;
	y = (10 - height) >> 1;
	nameWindow = mnWindowCreate(x, y, width, height, WINDOW_FLAG_INSTANT);
	fieldState->noticeWin = nameWindow;
	mnTextDrawMessage(areaMessage, (s32)nameWindow, 0, 0);
	fieldState->noticeFrm = AREA_NAME_WINDOW_FRAMES;
	sySetEvent(mnAreaNameWindowUpdate, TASK_PASS_FRAME);
}

/* Counts the area name down, and closes it at 0. */
void mnAreaNameWindowUpdate(void) {
	FIELD_STATE *state;
	u16 framesLeft;

	state = FieldStateWork;
	state->noticeFrm = framesLeft = state->noticeFrm - 1;
	if(framesLeft == 0) {
		mnWindowClose(state->noticeWin, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

/* Closes the area name at once, if it is shown. */
void mnAreaNameWindowHide(void) {
	WINDOW *window;

	window = FieldStateWork->noticeWin;
	if(window != NULL && window->flg != 0) {
		mnWindowClose(window, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

void gmPartyInventoryRemoveStub(s32 item);

/* Removes itemId from the party's inventory and returns 0. */
s32 mnItemRemoveFromInventory(s32 unused, s32 itemId) {
	gmPartyInventoryRemoveStub(itemId);
	return 0;
}

#undef FieldStateWork
