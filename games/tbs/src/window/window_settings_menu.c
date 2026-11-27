/* Window colours, the settings menu and the Pause menu. Part of window.c. */

/* The pixels a settings slider's marker runs across, from OPTION_SLIDER_X. */
#define OPTION_SLIDER_WIDTH 60

/* The headings of MenuTitlePictures (graphics/ui/menu_titles.s) these menus
   show above their windows. */
enum {
	MENU_TITLE_PAUSE = 6,	 /* "Pause" */
	MENU_TITLE_SETTINGS = 7, /* "Settings" */
};

/* The settings menu's rows, in the order of their help messages (MsgOptionHelp
   on) and of SETTINGS_MENU_STATE.option. */
typedef enum SETTINGS_MENU_ROW {
	SETTINGS_ROW_WINDOW_COLOR = 0, /* "Window color" */
	SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
	SETTINGS_ROW_MESSAGE_SPEED = 2,
	SETTINGS_ROW_SPEECH = 3, /* "Turn voice sounds on or off." */
	SETTINGS_ROW_AUTO_SLEEP = 4,
	SETTINGS_ROW_COUNT = 5
} SETTINGS_MENU_ROW;

/* The window colours the window colour settings set: WINDOW_PALETTE's colours
   from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6. */
#define WINDOW_SHADE(n) \
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))

/* Unused: three empty functions. */
UNUSED void mnSettingsMenuReservedNoOperationA(void)
{
}

UNUSED void mnSettingsMenuReservedNoOperationB(void)
{
}

UNUSED void mnSettingsMenuReservedNoOperationC(void)
{
}

/* One of the seven entries of the settings menu's state
   (SETTINGS_MENU_STATE.entries); only its state is known, and only the unused
   mnSettingsMenuResetEntries writes it. */
typedef struct OPTION_MENU_ENTRY {
	Uint8 unused[10];
	Uint16
		/* cleared to 0 by mnSettingsMenuResetEntries; nothing reads it */
		  state;
	Uint8 unusedAfterState[40];
} OPTION_MENU_ENTRY;

/* Clears the settings menu's seven entry states, its row, its tint and its
   cursor animation. */
UNUSED void mnSettingsMenuResetEntries(SETTINGS_MENU_STATE *optionMenuState)
{
	OPTION_MENU_ENTRY *entry = (OPTION_MENU_ENTRY *)optionMenuState->entries;

	entry->state = 0;
	entry++;
	entry->state = 0;
	entry++;
	entry->state = 0;
	entry++;
	entry->state = 0;
	entry++;
	entry->state = 0;
	entry++;
	entry->state = 0;
	entry++;
	entry->state = 0;
	optionMenuState->page = 0;
	optionMenuState->tint[0] = 0;
	optionMenuState->tint[1] = 0;
	optionMenuState->tint[2] = 0;
	optionMenuState->cursorFrm = 0;
}

/* Works out the window tint (red, green and blue, 0 to 31) from the window
   colour settings: the window color picks a point on the colour wheel and the
   brightness shifts all three channels. */
UNUSED void mnWindowColCalcTint(SETTINGS_MENU_STATE *state)
{
	Sint16 phase;
	Sint32 bias;
	Sint32 blue;
	Sint32 red;
	Sint32 green;

	phase = (MainRoutineStateWork.winHue + WINDOW_COLOR_COUNT / 2) %
		WINDOW_COLOR_COUNT * 4;
	bias = MainRoutineStateWork.winBrightness - WINDOW_BRIGHTNESS_NEUTRAL;
	red = WindowColorWaveTable[(phase % WINDOW_COLOR_WHEEL_STEPS)];
	green = WindowColorWaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS / 3) %
		WINDOW_COLOR_WHEEL_STEPS];
	blue = WindowColorWaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
		WINDOW_COLOR_WHEEL_STEPS];
	red += bias;
	green += bias;
	blue += bias;
	if(red < 0)
		red = 0;
	if(green < 0)
		green = 0;
	if(blue < 0)
		blue = 0;
	if(red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;
	if(green > RGB_LEVEL_MAX)
		green = RGB_LEVEL_MAX;
	if(blue > RGB_LEVEL_MAX)
		blue = RGB_LEVEL_MAX;
	state->tint[0] = red;
	state->tint[1] = green;
	state->tint[2] = blue;
}

/* Writes the window colours (shades 0 to 6 of WINDOW_SHADE), each a fixed scale
   of the window tint. */
UNUSED void mnWindowColWritePalette(SETTINGS_MENU_STATE *state)
{
	*(Sint16 *)&WINDOW_SHADE(0) = mnWindowColScaleTint(
		state, toFIXED(14) / 15, toFIXED(4) / 5, toFIXED(1.06666));
	*(Sint16 *)&WINDOW_SHADE(1) = mnWindowColScaleTint(
		state, toFIXED(5) / 6, toFIXED(11) / 15, toFIXED(14) / 15);
	*(Sint16 *)&WINDOW_SHADE(2) = mnWindowColScaleTint(
		state, toFIXED(11) / 15, toFIXED(2) / 3, toFIXED(4) / 5);
	*(Sint16 *)&WINDOW_SHADE(3) = mnWindowColScaleTint(
		state, toFIXED(0.63333), toFIXED(3) / 5, toFIXED(2) / 3);
	*(Sint16 *)&WINDOW_SHADE(4) = mnWindowColScaleTint(
		state, toFIXED(31) / 30, toFIXED(13) / 15, toFIXED(6) / 5);
	*(Sint16 *)&WINDOW_SHADE(5) = mnWindowColScaleTint(
		state, toFIXED(1.13333), toFIXED(14) / 15, toFIXED(4) / 3);
	*(Sint16 *)&WINDOW_SHADE(6) = mnWindowColScaleTint(
		state, toFIXED(37) / 30, toFIXED(1), toFIXED(22) / 15);
}

/* Returns the window tint with each channel scaled (16.16 fixed point) and
   clamped to 0 to 31, as a BGR555 colour. */
Uint16 mnWindowColScaleTint(SETTINGS_MENU_STATE *state, FIXED redScale,
	FIXED greenScale, FIXED blueScale)
{
	Sint32 red;
	Sint32 green;
	Sint32 blue;

	red = FIXEDtoINT(syMulFX(toFIXED(state->tint[0]), redScale));
	green = FIXEDtoINT(syMulFX(toFIXED(state->tint[1]), greenScale));
	blue = FIXEDtoINT(syMulFX(toFIXED(state->tint[2]), blueScale));
	if(red < 0)
		red = 0;
	if(green < 0)
		green = 0;
	if(blue < 0)
		blue = 0;
	if(red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;
	if(green > RGB_LEVEL_MAX)
		green = RGB_LEVEL_MAX;
	if(blue > RGB_LEVEL_MAX)
		blue = RGB_LEVEL_MAX;
	return red + ((blue << 10) + (green << 5));
}

/* Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
   31, as a BGR555 colour. rgb holds three components, not an already packed
   palette word. */
Uint16 mnColScaleComponents(
	Sint16 *rgb, FIXED redScale, FIXED greenScale, FIXED blueScale)
{
	Sint32 red;
	Sint32 green;
	Sint32 blue;

	red = FIXEDtoINT(syMulFX(toFIXED(rgb[0]), redScale));
	green = FIXEDtoINT(syMulFX(toFIXED(rgb[1]), greenScale));
	blue = FIXEDtoINT(syMulFX(toFIXED(rgb[2]), blueScale));
	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	return red + ((blue << 10) + (green << 5));
}

/* Sets the window colours (WINDOW_SHADE) for a window color (0 to 23) and
   brightness (0 to 14, 7 neutral). */
void mnWindowColApply(Sint32 phase, Sint32 brightness)
{
	Sint16 rgb[3];
	Sint16 step;
	Sint16 red;
	Sint16 green;
	Sint16 blue;
	Sint16 ofs;

	/* The 24 window colors are every fourth step of the 96-step wheel, color 0
	   at step 48. */
	step = ((phase + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
	ofs = brightness - WINDOW_BRIGHTNESS_NEUTRAL;
	red = WindowColorWaveTable[(step % WINDOW_COLOR_WHEEL_STEPS)] + ofs;
	green = WindowColorWaveTable[(step + WINDOW_COLOR_WHEEL_STEPS / 3) %
				WINDOW_COLOR_WHEEL_STEPS] +
		ofs;
	blue = WindowColorWaveTable[(step + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
			   WINDOW_COLOR_WHEEL_STEPS] +
		ofs;
	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	/* Each shade scales the tint by channel, in 16.16 (toFIXED(1) is 1): shades
	   0 to 3 grow darker, from about 0.93, 0.8 and 1.07 to 0.63, 0.6 and 0.67,
	   and 4 to 6 lighter, up to about 1.23, 1 and 1.47. */
	WINDOW_SHADE(0) = mnColScaleComponents(
		rgb, toFIXED(14) / 15, toFIXED(4) / 5, toFIXED(1.06666));
	WINDOW_SHADE(1) = mnColScaleComponents(
		rgb, toFIXED(5) / 6, toFIXED(11) / 15, toFIXED(14) / 15);
	WINDOW_SHADE(2) = mnColScaleComponents(
		rgb, toFIXED(11) / 15, toFIXED(2) / 3, toFIXED(4) / 5);
	WINDOW_SHADE(3) = mnColScaleComponents(
		rgb, toFIXED(0.63333), toFIXED(3) / 5, toFIXED(2) / 3);
	WINDOW_SHADE(4) = mnColScaleComponents(
		rgb, toFIXED(31) / 30, toFIXED(13) / 15, toFIXED(6) / 5);
	WINDOW_SHADE(5) = mnColScaleComponents(
		rgb, toFIXED(1.13333), toFIXED(14) / 15, toFIXED(4) / 3);
	WINDOW_SHADE(6) = mnColScaleComponents(
		rgb, toFIXED(37) / 30, toFIXED(1), toFIXED(22) / 15);
}

/* The rows of the unused three-row settings cursor
   (SETTINGS_MENU_STATE.page). */
typedef enum OPTION_MENU_ROW {
	OPTION_ROW_MESSAGE_SPEED = 0,
	OPTION_ROW_WINDOW_COLOR = 1,
	OPTION_ROW_WINDOW_BRIGHTNESS = 2
} OPTION_MENU_ROW;

/* The five-row settings menu mnSettingsMenuRun runs uses none of these four. */

/* Moves the three-row settings cursor up, wrapping to the bottom. The three
   rows are those of mnSettingsMenuLowerSetting. */
UNUSED void mnSettingsMenuPreviousRow(SETTINGS_MENU_STATE *state)
{
	if(state->page == OPTION_ROW_MESSAGE_SPEED)
		state->page = OPTION_ROW_WINDOW_BRIGHTNESS;
	else
		state->page--;
}

/* Moves the three-row settings cursor down, wrapping to the top. */
UNUSED void mnSettingsMenuNextRow(SETTINGS_MENU_STATE *state)
{
	if(++state->page > OPTION_ROW_WINDOW_BRIGHTNESS)
		state->page = OPTION_ROW_MESSAGE_SPEED;
}

/* Lowers the setting on the three-row cursor's row: message speed, window color
   or window brightness. */
UNUSED void mnSettingsMenuLowerSetting(SETTINGS_MENU_STATE *state)
{
	switch(state->page) {
	case OPTION_ROW_MESSAGE_SPEED:
		if(MainRoutineStateWork.msgSpeed != 0)
			MainRoutineStateWork.msgSpeed--;
		break;
	case OPTION_ROW_WINDOW_COLOR:
		if(MainRoutineStateWork.winHue != 0)
			MainRoutineStateWork.winHue--;
		break;
	case OPTION_ROW_WINDOW_BRIGHTNESS:
		if(MainRoutineStateWork.winBrightness != 0)
			MainRoutineStateWork.winBrightness--;
		break;
	}
}

/* Raises the setting on the three-row cursor's row: message speed, window color
   or window brightness. */
UNUSED void mnSettingsMenuRaiseSetting(SETTINGS_MENU_STATE *state)
{
	switch(state->page) {
	case OPTION_ROW_MESSAGE_SPEED:
		if(MainRoutineStateWork.msgSpeed < MESSAGE_SPEED_FAST)
			MainRoutineStateWork.msgSpeed++;
		break;
	case OPTION_ROW_WINDOW_COLOR:
		if(MainRoutineStateWork.winHue <= WINDOW_COLOR_COUNT - 1)
			MainRoutineStateWork.winHue++;
		break;
	case OPTION_ROW_WINDOW_BRIGHTNESS:
		if(MainRoutineStateWork.winBrightness <= WINDOW_BRIGHTNESS_COUNT - 1)
			MainRoutineStateWork.winBrightness++;
		break;
	}
}

/* An empty function, reached only through a bare entry of the window stub
   table. */
void mnSettingsMenuSelectionNoOperation(void)
{
}

/* The settings menu's per-tick event: moves the cursor and the two slider
   markers, animates the marker of the slider in use, and pulses the chosen
   setting's icon. */
void mnSettingsMenuAnimateCursors(void)
{
	Sint32				 cursorResource;
	Sint32				 markerResource;
	Sint32				 selectedOption;
	SETTINGS_MENU_STATE *state;

	state = OptionMenuState;
	selectedOption = state->page;
	mnCursorAdvanceStub(&state->cursor.cursor);
	mnCursorMoveTowardTargetStub(&state->marker[0].cursor);
	mnCursorMoveTowardTargetStub(&state->marker[1].cursor);

	if(selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
		cursorResource = (state->cursorFrm & 7) + RESOURCE_MENU_CURSOR_PICTURES;
	} else {
		cursorResource = RESOURCE_MENU_CURSOR_PICTURES;
	}
	syVRAMCacheLoad((Uint8)state->marker[0].spr->idx, 256,
		syResourceGetTableEntry(cursorResource));

	if(selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
		markerResource = (state->cursorFrm & 7) + RESOURCE_MENU_CURSOR_PICTURES;
	} else {
		markerResource = RESOURCE_MENU_CURSOR_PICTURES;
	}
	syVRAMCacheLoad((Uint8)state->marker[1].spr->idx, 256,
		syResourceGetTableEntry(markerResource));

	if(selectedOption > SETTINGS_ROW_WINDOW_BRIGHTNESS)
		mnWindowSprPulse(state->choiceIcons[selectedOption -
			SETTINGS_ROW_MESSAGE_SPEED][state->option[selectedOption]]);
	state->cursorFrm++;
}

/* Allocates the settings menu's state block, copies the settings into it with
   how many values each takes, and starts its per-tick event. */
void mnSettingsMenuInitState(void)
{
	SETTINGS_MENU_STATE *state;

	state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));
	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	state->option[SETTINGS_ROW_WINDOW_COLOR] = MainRoutineStateWork.winHue;
	state->optionCnt[SETTINGS_ROW_WINDOW_COLOR] = WINDOW_COLOR_COUNT;
	state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] =
		MainRoutineStateWork.winBrightness;
	state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS] = WINDOW_BRIGHTNESS_COUNT;
	state->option[SETTINGS_ROW_MESSAGE_SPEED] = MainRoutineStateWork.msgSpeed;
	state->optionCnt[SETTINGS_ROW_MESSAGE_SPEED] = MESSAGE_SPEED_FAST + 1;
	state->option[SETTINGS_ROW_SPEECH] = MainRoutineStateWork.speech;
	state->optionCnt[SETTINGS_ROW_SPEECH] = 2;
	state->option[SETTINGS_ROW_AUTO_SLEEP] =
		MainRoutineStateWork.idleSleepEnabled;
	state->optionCnt[SETTINGS_ROW_AUTO_SLEEP] = 2;
	sySetEvent(mnSettingsMenuAnimateCursors, EVENT_PASS_TICK);
}

/* Stops the settings menu's per-tick event and frees its state block. The
   event goes first, since it reads the state block. */
void mnSettingsMenuRelease(void)
{
	syCloseEvent(mnSettingsMenuAnimateCursors);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* The sprite palettes of the sliders' ends, which SettingsMenu_SliderPalette
   fills. */
#define WINDOW_COLOR_SLIDER_PALETTE		 14
#define WINDOW_BRIGHTNESS_SLIDER_PALETTE (WINDOW_COLOR_SLIDER_PALETTE + 1)

/* Opens the settings menu's window: the labels, the window color and brightness
   sliders with their markers, the cursor and the setting icons. Returns the
   window. */
WINDOW *mnSettingsMenuOpen(void)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*win;
	WINDOW_SPR			*marker;
	Sint32				 cacheSlot;
	Sint32				 x;
	Sint32				 y;
	Sint32				 labelMsg;

	state = OptionMenuState;
	win = mnWindowCreate(
		OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, WINDOW_FLAG_INSTANT);
	mnWindowDrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
	mnWindowDrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
	mnWindowDrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
	mnWindowDrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
	labelMsg = MsgWindowColorLabel;
	mnTextDrawMessageAsTiles(labelMsg, win, 8, 0);
	labelMsg++;
	mnTextDrawMessageAsTiles(labelMsg, win, 8, 16);
#if LANGUAGE != LANGUAGE_JAPANESE
	labelMsg = MsgMessageSpeedLabel;
	mnTextDrawMessageAsTiles(labelMsg, win, 8, 32);
	labelMsg++;
	mnTextDrawMessageAsTiles(labelMsg, win, 32, 40);
#else
	mnTextDrawMessageAsTiles(MsgMessageSpeedLabel, win, 8, 40);
#endif
	mnTextDrawMessageAsTiles(MsgSpeechLabel, win, 8, 64);
	mnTextDrawMessageAsTiles(MsgAutoSleepLabel, win, 8, 88);

	cacheSlot = syVRAMCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(cacheSlot, 128, PointingHandTiles);
		marker =
			mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		state->cursor.spr = marker;
		x = win->x * 8;
		y = win->y * 8 + 12;
		mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
	}

	/* The ends of both sliders: OUTPUT_SIZE_32X8 sprites, the right end 4 tiles
	   into the picture, each slider in its own palette. */
	cacheSlot = syVRAMCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syDMAXCopy(SettingsMenu_SliderPalette,
			(void *)&OBJ_PLTT_COLOR(WINDOW_COLOR_SLIDER_PALETTE, 0),
			DMA_COPY_HALFWORDS(2 * PALETTE_COLORS), REG_DMA3);
		syVRAMCacheLoad(cacheSlot, 256, SettingsMenu_SliderTiles);
		y = 0;
		marker = mnWindowSprCreate(
			cacheSlot, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X - 6, y);
		marker->spr.oam.pal = WINDOW_COLOR_SLIDER_PALETTE;
		marker = mnWindowSprCreate(
			cacheSlot, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X + 26, y);
		marker->spr.oam.idx += 4;
		marker->spr.oam.pal = WINDOW_COLOR_SLIDER_PALETTE;
		y = 16;
		marker = mnWindowSprCreate(
			cacheSlot, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X - 6, y);
		marker->spr.oam.pal = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
		marker = mnWindowSprCreate(
			cacheSlot, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X + 26, y);
		marker->spr.oam.idx += 4;
		marker->spr.oam.pal = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
	}

	/* The two markers: 16x16 sprites of 256 colours, whose 256 bytes
	   mnSettingsMenuAnimateCursors fills each tick. */
	cacheSlot = syVRAMCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(cacheSlot, 256, 0);
		marker =
			mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		marker->spr.oam.fullCol = 1;
		state->marker[0].spr = marker;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
			state->optionCnt[SETTINGS_ROW_WINDOW_COLOR];
		y = win->y * 8 + 4;
		mnCursorSetPosImmediateStub(&state->marker[0].cursor, x, y);
	}

	cacheSlot = syVRAMCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(cacheSlot, 256, 0);
		marker =
			mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		marker->spr.oam.fullCol = 1;
		state->marker[1].spr = marker;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] *
			OPTION_SLIDER_WIDTH /
			state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		y = win->y * 8 + 20;
		mnCursorSetPosImmediateStub(&state->marker[1].cursor, x, y);
	}

	y = 28;
	state->choiceIcons[0][0] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[0], 0, win, OPTION_CHOICE_X, y);
	state->choiceIcons[0][1] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[1], 0, win, OPTION_CHOICE_X + 24, y);
	state->choiceIcons[0][2] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[2], 0, win, OPTION_CHOICE_X + 48, y);
	y = 52;
	state->choiceIcons[1][0] =
		mnCommandIconCreate(SettingsMenuSpeechIcons[0], 0, win, 100, y);
	state->choiceIcons[1][1] =
		mnCommandIconCreate(SettingsMenuSpeechIcons[1], 0, win, 124, y);
	y = 76;
	state->choiceIcons[2][0] =
		mnCommandIconCreate(SettingsMenuSleepIcons[0], 0, win, 100, y);
	state->choiceIcons[2][1] =
		mnCommandIconCreate(SettingsMenuSleepIcons[1], 0, win, 124, y);
	return win;
}

/* Runs the settings menu (window colour and brightness, message speed, speech,
   auto-sleep) until A or START keeps the settings (returns 0) or B drops them
   (returns -1). */
Sint32 mnSettingsMenuRun(void)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*settingsWin;
	WINDOW				*helpWin;
	WINDOW_SPR			*pair;
	Sint8				*winCol;
	Sint8				*brightness;
	Bool				 redraw;
	Sint32				 selectedOption;
	Sint32				 i;
	Bool				 dimmed;
	Sint32				 resourceIdx;
	Sint32				 x;
	Sint32				 y;
	Sint32				 result;
	WINDOW_SPR			*frm;

	redraw = TRUE;
	selectedOption = 0;
	mnSettingsMenuInitState();
	state = OptionMenuState;
	helpWin = mnWindowCreate(
		OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
	settingsWin = mnSettingsMenuOpen();
	pair = mnTitleCreate(MENU_TITLE_SETTINGS, settingsWin, 64, -48);
	sySynch(1);
	winCol = &state->option[SETTINGS_ROW_WINDOW_COLOR];
	brightness = &state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];

	for(;;) {
		if(redraw != 0) {
			redraw = FALSE;
			selectedOption =
				(selectedOption + SETTINGS_ROW_COUNT) % SETTINGS_ROW_COUNT;
			state->option[selectedOption] =
				(state->option[selectedOption] +
					state->optionCnt[selectedOption]) %
				state->optionCnt[selectedOption];
			state->page = selectedOption;
			if(SleepDisabled != 0)
				state->option[SETTINGS_ROW_AUTO_SLEEP] = 0;

			/* Redraw each choice's icon at OAM_DEPTH_FRONT - 4, dimmed unless
			   it is the row's setting. */
			for(i = 0; i <= 2; i++) {
				frm = state->choiceIcons[0][i];
				frm->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frm);
				resourceIdx = (Uint8)state->choiceIcons[0][i]->idx;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_MESSAGE_SPEED])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSpeedIcons[i], resourceIdx, dimmed);
			}
			for(i = 0; i <= 1; i++) {
				frm = state->choiceIcons[1][i];
				frm->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frm);
				resourceIdx = (Uint8)state->choiceIcons[1][i]->idx;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_SPEECH])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSpeechIcons[i], resourceIdx, dimmed);
			}
			for(i = 0; i <= 1; i++) {
				frm = state->choiceIcons[2][i];
				frm->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frm);
				resourceIdx = (Uint8)state->choiceIcons[2][i]->idx;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_AUTO_SLEEP])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSleepIcons[i], resourceIdx, dimmed);
			}

			x = settingsWin->x * 8 + OPTION_SLIDER_X;
			x += (*winCol * OPTION_SLIDER_WIDTH) /
				state->optionCnt[SETTINGS_ROW_WINDOW_COLOR];
			y = settingsWin->y * 8 + 4;
			mnCursorSetStub(&state->marker[0].cursor, x, y, 1);
			x = settingsWin->x * 8 + OPTION_SLIDER_X;
			x += (*brightness * OPTION_SLIDER_WIDTH) /
				state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS];
			y = settingsWin->y * 8 + 20;
			mnCursorSetStub(&state->marker[1].cursor, x, y, 1);

			x = state->option[SETTINGS_ROW_MESSAGE_SPEED] +
				MsgMessageSpeedSetting;
			mnWindowClearInteriorTiles(
				settingsWin, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
			mnTextDrawMessageAsTiles(x, settingsWin, OPTION_SETTING_X, 40);
			x = state->option[SETTINGS_ROW_SPEECH] + MsgSpeechSetting;
			mnWindowClearInteriorTiles(
				settingsWin, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
			mnTextDrawMessageAsTiles(x, settingsWin, OPTION_SETTING_X, 64);
			x = state->option[SETTINGS_ROW_AUTO_SLEEP] + MsgAutoSleepSetting;
			mnWindowClearInteriorTiles(
				settingsWin, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
			mnTextDrawMessageAsTiles(x, settingsWin, OPTION_SETTING_X, 88);
			/* The windows show the edited colours at once. */
			mnWindowColApply(*winCol, *brightness);

			/* The cursor goes 3 tiles a row down the window's left edge, the
			   first row's a tile lower; the help window says what the row sets
			   (`Change the window's color.` for the first). */
			x = settingsWin->x * 8;
			y = (selectedOption * 3 + settingsWin->y) * 8 + 4;
			if(selectedOption == SETTINGS_ROW_WINDOW_COLOR)
				y += 8;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			mnWindowSprClearList(helpWin);
			mnTextDrawMessage(selectedOption + MsgOptionHelp, helpWin, 0, 0);
		}
		mnTitleBob(pair);
		sySynch(1);

		/* SELECT steps through the colour presets (window/scales.c). */
		if(PerPush & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CONFIRM);
			state->preset++;
			redraw = TRUE;
			if(state->preset > ARRAY_COUNT(SettingsMenuHuePresets) - 1)
				state->preset = 0;
			*winCol = SettingsMenuHuePresets[state->preset];
			*brightness = SettingsMenuBrightnessPresets[state->preset];
			continue;
		}
		if(PerPush & (KEY_A | KEY_START)) {
			result = 0;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerPush & KEY_B) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(PerRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption--;
			redraw = TRUE;
		} else if(PerRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption++;
			redraw = TRUE;
		} else {
			if(PerRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]--;
				redraw = TRUE;
			}
			if(PerRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]++;
				redraw = TRUE;
			}
		}
	}

	mnWindowClose(helpWin, WINDOW_FLAG_INSTANT);
	mnWindowClose(settingsWin, WINDOW_FLAG_INSTANT);
	/* Keep the edits, auto-sleep also taking effect at once (IdleSleepEnabled);
	   or put the saved window colours back. */
	if(result == 0) {
		MainRoutineStateWork.winHue = state->option[SETTINGS_ROW_WINDOW_COLOR];
		MainRoutineStateWork.winBrightness =
			state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		MainRoutineStateWork.msgSpeed =
			state->option[SETTINGS_ROW_MESSAGE_SPEED];
		MainRoutineStateWork.speech = state->option[SETTINGS_ROW_SPEECH];
		MainRoutineStateWork.idleSleepEnabled =
			state->option[SETTINGS_ROW_AUTO_SLEEP];
		IdleSleepEnabled = MainRoutineStateWork.idleSleepEnabled;
	} else {
		mnWindowColApply(
			MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	}
	mnSettingsMenuRelease();
	sySynch(1);
	return result;
}

#define PAUSE_MENU_ROWS		  3 /* Save your quest, Sleep and Change settings */
#define PAUSE_MENU_DEBUG_ROWS 3 /* the entries added in debug mode */
#define PAUSE_MENU_ROW_HEIGHT 24 /* pixels, 3 tiles */
#define PAUSE_MENU_TEXT_X	  48 /* pixels into the window, after the icons */

/* The Pause menu's per-tick event: moves the cursor and pulses the chosen
   entry's icon. */
void mnPauseMenuAnimateCursor(void)
{
	SETTINGS_MENU_STATE *state = OptionMenuState;
	Uint32				 idx;

	mnCursorAdvanceStub(&state->cursor.cursor);
	idx = state->page;
	mnWindowSprPulse(state->icon[idx]);
}

/* Allocates the Pause menu's state block and starts its per-tick event. */
void mnPauseMenuInit(void)
{
	void *state;
	state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));
	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnPauseMenuAnimateCursor, EVENT_PASS_TICK);
}

/* Stops the Pause menu's per-tick event and frees its state block. The event
   goes first, since it reads the state block. */
void mnPauseMenuRelease(void)
{
	syCloseEvent(&mnPauseMenuAnimateCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* Opens the Pause menu's window with its entries (Save your quest, Sleep,
   Change settings, and the debug entries in debug mode; only Change settings
   while FLAG_IN_BATTLE_ARENA is set), their icons and the cursor. */
WINDOW *mnPauseMenuOpen(void)
{
	SETTINGS_MENU_STATE *state;
	Sint32				 rows;
	Sint32				 first;
	Sint32				 settingsOnly;
	Sint32				 top;
	Sint32				 height;
	WINDOW				*win;
	Sint32				 y;
	Sint32				 cacheSlot;
	Sint32				 labelMsg;
	Sint32				 i;
	Sint32				 x;

	rows = PAUSE_MENU_ROWS;
	state = OptionMenuState;
	settingsOnly = gmFlagGetStub(FLAG_IN_BATTLE_ARENA);
	first = 0;
	if(settingsOnly != 0) {
		rows = 1;
		first = PAUSE_MENU_CHANGE_SETTINGS;
	}
	if(DebugMode != 0)
		rows += PAUSE_MENU_DEBUG_ROWS;
	/* Three tiles a row and a tile more. */
	top = 8 - rows;
	height = rows * 3 + 1;
	if(top + height > DISPLAY_TILE_HEIGHT - 1) {
		top = 1;
		height = DISPLAY_TILE_HEIGHT - 1;
	}
	win = mnWindowCreate(5, top, 20, height, WINDOW_FLAG_INSTANT);
	for(i = 1; i < rows; i++)
		mnWindowDrawDividerLine(win, 0, i * 3, 19, i * 3);
	y = 4;
	if(settingsOnly == 0) {
		labelMsg = MsgWorkspaceSaveQuest;
		mnTextDrawMessage(labelMsg, win, PAUSE_MENU_TEXT_X, 4);
		labelMsg++;
		mnTextDrawMessage(
			labelMsg, win, PAUSE_MENU_TEXT_X, 4 + PAUSE_MENU_ROW_HEIGHT);
		y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
	}
	mnTextDrawMessage(MsgWorkspaceChangeSettings, win, PAUSE_MENU_TEXT_X, y);
	y += PAUSE_MENU_ROW_HEIGHT;
	if(DebugMode != 0) {
		labelMsg = MsgWorkspaceRoughMenu;
		mnTextDrawMessage(labelMsg, win, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		mnTextDrawMessage(labelMsg + 1, win, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		labelMsg += 2;
		mnTextDrawMessage(labelMsg, win, PAUSE_MENU_TEXT_X, y);
	}
	cacheSlot = syVRAMCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(cacheSlot, 128, PointingHandTiles);
		state->cursor.spr =
			mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		x = win->x * 8;
		y = win->y * 8 + 16;
		mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
	}
	y = -4;
	for(i = 0; i < rows; i++) {
		state->icon[i] =
			mnCommandIconCreate(PauseMenuIcons[first + i], 0, win, 12, y);
		y += PAUSE_MENU_ROW_HEIGHT;
	}
	return win;
}

/* Runs the Pause menu until the player picks an entry (PAUSE_MENU_ENTRY) or
   cancels with B or START (-1). */
Sint32 mnPauseMenuChoose(void)
{
	WINDOW				*win;
	Bool				 redraw;
	WINDOW_SPR			*pair;
	Sint32				 first;
	Sint32				 i;
	Sint32				 j;
	Sint32				 rows;
	Sint32				 settingsOnly;
	Bool				 dimmed;
	Sint32				 y;
	Sint32				 result;
	Sint32				 selectedEntry;
	Sint32				 resourceIdx;
	Sint32				 x;
	Sint32				 top;
	const Sint8			*pauseIcons;
	WINDOW_SPR			*icon;
	SETTINGS_MENU_STATE *state;

	redraw = TRUE;
	rows = PAUSE_MENU_ROWS;
	settingsOnly = gmFlagGetStub(FLAG_IN_BATTLE_ARENA);
	first = 0;
	mnPauseMenuInit();
	state = OptionMenuState;
	win = mnPauseMenuOpen();
	/* The heading sits above the window, in the localized releases a little
	   lower in debug mode. */
#if PAUSE_HEADING_LOWER_IN_DEBUG
	y = -24;
	if(DebugMode != 0) {
		y = -16;
	}
	pair = mnTitleCreate(MENU_TITLE_PAUSE, win, 40, y);
#else
	pair = mnTitleCreate(MENU_TITLE_PAUSE, win, 40, -24);
#endif
	sySynch(1);
	/* The cursor starts where the block's page is, 0 after the fill. */
	selectedEntry = state->page;
	if(settingsOnly != 0) {
		rows = 1;
		first = PAUSE_MENU_CHANGE_SETTINGS;
	}
	if(DebugMode != 0) {
		rows += PAUSE_MENU_DEBUG_ROWS;
	}
	for(;;) {
		if(redraw != 0) {
			redraw = FALSE;
			selectedEntry = (selectedEntry + rows) % rows;
			state->page = selectedEntry;
			i = 0;
			if(i < rows) {
				j = first;
				pauseIcons = PauseMenuIcons;
				do {
					icon = state->icon[i];
					icon->depth = OAM_DEPTH_FRONT - 4;
					mnIconShowStub(icon);
					resourceIdx = icon->idx;
					dimmed = FALSE;
					if(i != state->page) {
						dimmed = TRUE;
					}
					mnCommandIconLoad(*(Sint8 *)(j + (Sint32)pauseIcons),
						resourceIdx, dimmed);
					i++;
					j++;
				} while(i < rows);
			}
			top = win->y;
			x = win->x * 8;
			y = (((selectedEntry * 3) + top) * 8) + 16;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
		}
		mnTitleBob(pair);
		sySynch(1);
		if(PerPush & KEY_A) {
			result = selectedEntry;
			sySndPlayStub(SE_MENU_CONFIRM);
		} else if(PerPush & (KEY_B | KEY_START)) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
		} else {
			if(PerRepeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedEntry -= 1;
				redraw = TRUE;
			} else if(PerRepeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedEntry += 1;
				redraw = TRUE;
			}
			continue;
		}
		break;
	}
	mnWindowClose(win, WINDOW_FLAG_INSTANT);
	mnPauseMenuRelease();
	sySynch(1);
	/* In the Battle Arena only Change settings shows: count from Save your
	   quest. */
	if(result >= 0) {
		result += first;
	}
	return result;
}

#undef OPTION_SLIDER_WIDTH
#undef WINDOW_SHADE
#undef WINDOW_COLOR_SLIDER_PALETTE
#undef WINDOW_BRIGHTNESS_SLIDER_PALETTE
#undef PAUSE_MENU_ROWS
#undef PAUSE_MENU_DEBUG_ROWS
#undef PAUSE_MENU_ROW_HEIGHT
#undef PAUSE_MENU_TEXT_X
