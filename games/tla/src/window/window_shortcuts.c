// The window that shows the area's name, and removing an item from the party's inventory. Part of
// window.c.

void mnAreaNameWindowUpdate(void);

// Shows the area's name in a window near the top of the screen for AREA_NAME_WINDOW_TICKS, kept in
// the field state until mnAreaNameWindowUpdate closes it.
void mnAreaNameWindowShow(void)
{
	FIELD_STATE        *fieldState;
	Sint32              height;
	Sint32              width;
	Sint32              y;
	Sint32              x;
	Sint32              areaMsg;
	WINDOW             *nameWindow;
	MAIN_ROUTINE_STATE *state;

	fieldState = FieldStateWork;
	state      = &MainRoutineStateWork;
	x          = 8;
	y          = 8;
	areaMsg    = fdAreaGetNameIndexStub(state->next, state->entrance) + (Sint32)&MsgDebugAreaName;
	mnTextGetMessageSize(areaMsg, &x, &y, &width, &height);
	x                        = (DISPLAY_TILE_WIDTH - width) >> 1;
	y                        = (10 - height) >> 1;
	nameWindow               = mnWindowCreate(x, y, width, height, WINDOW_FLAG_INSTANT);
	fieldState->noticeWindow = nameWindow;
	mnTextDrawMessage(areaMsg, (Sint32)nameWindow, 0, 0);
	fieldState->noticeFrame = AREA_NAME_WINDOW_TICKS;
	sySetEvent(mnAreaNameWindowUpdate, EVENT_PASS_TICK);
}

// Counts the area name down, and closes it at 0.
void mnAreaNameWindowUpdate(void)
{
	FIELD_STATE *state;
	Uint16       ticksLeft;

	state              = FieldStateWork;
	state->noticeFrame = ticksLeft = state->noticeFrame - 1;
	if (ticksLeft == 0)
	{
		mnWindowClose(state->noticeWindow, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

// Closes the area name at once, if it is shown.
void mnAreaNameWindowHide(void)
{
	WINDOW *window;

	window = FieldStateWork->noticeWindow;
	if (window != NULL && window->flags != 0)
	{
		mnWindowClose(window, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

void   gmPartyInventoryRemoveStub(Sint32 item);

// Removes itemId from the party's inventory and returns 0.
Sint32 mnItemRemoveFromInventory(Sint32 unused, Sint32 itemId)
{
	gmPartyInventoryRemoveStub(itemId);
	return 0;
}

#undef FieldStateWork
