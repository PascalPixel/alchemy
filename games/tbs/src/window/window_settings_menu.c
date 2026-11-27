/* @file */
/* Window colours, the settings menu and the Pause menu. */
/* Part of window.c. */

/* The pixels a settings slider's marker runs across, from OPTION_SLIDER_X. */
#define OPTION_SLIDER_WIDTH 60

/* The headings of MenuTitlePictures (graphics/ui/menu_titles.s) these menus */
/* show above their windows. */
enum {
	MENU_TITLE_PAUSE = 6,	 /* "Pause" */
	MENU_TITLE_SETTINGS = 7, /* "Settings" */
};

/* The settings menu's rows, in the order of their help messages */
/* (MsgOptionHelp on) and of SETTINGS_MENU_STATE.option. */
typedef enum SETTINGS_MENU_ROW {
	SETTINGS_ROW_WINDOW_COLOR = 0, /* "Window color" */
	SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
	SETTINGS_ROW_MESSAGE_SPEED = 2,
	SETTINGS_ROW_SPEECH = 3, /* "Turn voice sounds on or off." */
	SETTINGS_ROW_AUTO_SLEEP = 4,
	SETTINGS_ROW_COUNT = 5
} SETTINGS_MENU_ROW;

/* The window colours the window colour settings set: WINDOW_PALETTE's colours
 */
/* from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6. */
#define WINDOW_SHADE(n)                                                        \
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))

/* Unused: three empty functions. */
UNUSED void mnSettingsMenuReservedNoOpA(void) {}
UNUSED void mnSettingsMenuReservedNoOpB(void) {}
UNUSED void mnSettingsMenuReservedNoOpC(void) {}

/* One of the seven entries of the settings menu's state */
/* (SETTINGS_MENU_STATE.entries); only its state is known, and only the unused
 */
/* mnSettingsMenuResetEntries writes it. */
typedef struct OPTION_MENU_ENTRY {
	u8 unused[10];
	u16 state; /* cleared to 0 by mnSettingsMenuResetEntries; nothing reads it
				*/
	u8 unusedAfterState[40];
} OPTION_MENU_ENTRY;

/* Clears the settings menu's seven entry states, its row, its tint and its */
/* cursor animation. */
UNUSED void mnSettingsMenuResetEntries(SETTINGS_MENU_STATE *optionMenuState) {
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

/* Works out the window tint (red, green and blue, 0 to 31) from the window */
/* colour settings: the window color picks a point on the colour wheel and the
 */
/* brightness shifts all three channels. */
UNUSED void mnWindowColCalcTint(SETTINGS_MENU_STATE *state) {
	s16 phase;
	s32 bias;
	s32 blue;
	s32 red;
	s32 green;

	phase = (MainRoutineStateWork.winHue + WINDOW_COLOR_COUNT / 2) %
			WINDOW_COLOR_COUNT * 4;
	bias = MainRoutineStateWork.winBrightness - WINDOW_BRIGHTNESS_NEUTRAL;
	red = WindowColor_WaveTable[(phase % WINDOW_COLOR_WHEEL_STEPS)];
	green = WindowColor_WaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS / 3) %
								  WINDOW_COLOR_WHEEL_STEPS];
	blue = WindowColor_WaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
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

/* Writes the window colours (shades 0 to 6 of WINDOW_SHADE), each a */
/* fixed scale of the window tint. */
UNUSED void mnWindowColWritePalette(SETTINGS_MENU_STATE *state) {
	*(s16 *)&WINDOW_SHADE(0) = mnWindowColScaleTint(
		state, FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 5, toFIXED(1.06666));
	*(s16 *)&WINDOW_SHADE(1) = mnWindowColScaleTint(
		state, FIXED_ONE * 5 / 6, FIXED_ONE * 11 / 15, FIXED_ONE * 14 / 15);
	*(s16 *)&WINDOW_SHADE(2) = mnWindowColScaleTint(
		state, FIXED_ONE * 11 / 15, FIXED_ONE * 2 / 3, FIXED_ONE * 4 / 5);
	*(s16 *)&WINDOW_SHADE(3) = mnWindowColScaleTint(
		state, toFIXED(0.63333), FIXED_ONE * 3 / 5, FIXED_ONE * 2 / 3);
	*(s16 *)&WINDOW_SHADE(4) = mnWindowColScaleTint(
		state, FIXED_ONE * 31 / 30, FIXED_ONE * 13 / 15, FIXED_ONE * 6 / 5);
	*(s16 *)&WINDOW_SHADE(5) = mnWindowColScaleTint(
		state, toFIXED(1.13333), FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 3);
	*(s16 *)&WINDOW_SHADE(6) = mnWindowColScaleTint(
		state, FIXED_ONE * 37 / 30, FIXED_ONE, FIXED_ONE * 22 / 15);
}

/* Returns the window tint with each channel scaled (16.16 fixed point) and */
/* clamped to 0 to 31, as a BGR555 colour. */
u16 mnWindowColScaleTint(SETTINGS_MENU_STATE *state, FIXED redScale,
						 FIXED greenScale, FIXED blueScale) {
	s32 red;
	s32 green;
	s32 blue;

	red = FIXEDtoINT(syMulFX(INTtoFIXED(state->tint[0]), redScale));
	green = FIXEDtoINT(syMulFX(INTtoFIXED(state->tint[1]), greenScale));
	blue = FIXEDtoINT(syMulFX(INTtoFIXED(state->tint[2]), blueScale));
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
 */
/* 31, as a BGR555 colour. */
/* rgb holds three components, not an already packed palette word. */
u16 mnColScaleComponents(s16 *rgb, FIXED redScale, FIXED greenScale,
						 FIXED blueScale) {
	s32 red;
	s32 green;
	s32 blue;

	red = FIXEDtoINT(syMulFX(INTtoFIXED(rgb[0]), redScale));
	green = FIXEDtoINT(syMulFX(INTtoFIXED(rgb[1]), greenScale));
	blue = FIXEDtoINT(syMulFX(INTtoFIXED(rgb[2]), blueScale));
	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	return red + ((blue << 10) + (green << 5));
}

/* Sets the window colours (WINDOW_SHADE) for a window color (0 to 23) and */
/* brightness (0 to 14, 7 neutral). */
void mnWindowColApply(s32 phase, s32 brightness) {
	s16 rgb[3];
	s16 step;
	s16 red;
	s16 green;
	s16 blue;
	s16 offset;

	/* The 24 window colors are every fourth step of the 96-step wheel, color 0
	 * at */
	/* step 48. */
	step = ((phase + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
	offset = brightness - WINDOW_BRIGHTNESS_NEUTRAL;
	red = WindowColor_WaveTable[(step % WINDOW_COLOR_WHEEL_STEPS)] + offset;
	green = WindowColor_WaveTable[(step + WINDOW_COLOR_WHEEL_STEPS / 3) %
								  WINDOW_COLOR_WHEEL_STEPS] +
			offset;
	blue = WindowColor_WaveTable[(step + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
								 WINDOW_COLOR_WHEEL_STEPS] +
		   offset;
	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	/* Each shade scales the tint by channel, in 16.16 (FIXED_ONE is 1): shades
	 */
	/* 0 to 3 grow darker, from about 0.93, 0.8 and 1.07 to 0.63, 0.6 and 0.67,
	 */
	/* and 4 to 6 lighter, up to about 1.23, 1 and 1.47. */
	WINDOW_SHADE(0) = mnColScaleComponents(rgb, FIXED_ONE * 14 / 15,
										   FIXED_ONE * 4 / 5, toFIXED(1.06666));
	WINDOW_SHADE(1) = mnColScaleComponents(
		rgb, FIXED_ONE * 5 / 6, FIXED_ONE * 11 / 15, FIXED_ONE * 14 / 15);
	WINDOW_SHADE(2) = mnColScaleComponents(
		rgb, FIXED_ONE * 11 / 15, FIXED_ONE * 2 / 3, FIXED_ONE * 4 / 5);
	WINDOW_SHADE(3) = mnColScaleComponents(
		rgb, toFIXED(0.63333), FIXED_ONE * 3 / 5, FIXED_ONE * 2 / 3);
	WINDOW_SHADE(4) = mnColScaleComponents(
		rgb, FIXED_ONE * 31 / 30, FIXED_ONE * 13 / 15, FIXED_ONE * 6 / 5);
	WINDOW_SHADE(5) = mnColScaleComponents(
		rgb, toFIXED(1.13333), FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 3);
	WINDOW_SHADE(6) = mnColScaleComponents(rgb, FIXED_ONE * 37 / 30, FIXED_ONE,
										   FIXED_ONE * 22 / 15);
}

/* The rows of the unused three-row settings cursor (SETTINGS_MENU_STATE.page).
 */
typedef enum OPTION_MENU_ROW {
	OPTION_ROW_MESSAGE_SPEED = 0,
	OPTION_ROW_WINDOW_COLOR = 1,
	OPTION_ROW_WINDOW_BRIGHTNESS = 2
} OPTION_MENU_ROW;

/* The five-row settings menu mnSettingsMenuRun runs uses none of these four. */

/* Moves the three-row settings cursor up, wrapping to the bottom. The three */
/* rows are those of mnSettingsMenuLowerSetting. */
UNUSED void mnSettingsMenuPreviousRow(SETTINGS_MENU_STATE *state) {
	if(state->page == OPTION_ROW_MESSAGE_SPEED)
		state->page = OPTION_ROW_WINDOW_BRIGHTNESS;
	else
		state->page--;
}

/* Moves the three-row settings cursor down, wrapping to the top. */
UNUSED void mnSettingsMenuNextRow(SETTINGS_MENU_STATE *state) {
	if(++state->page > OPTION_ROW_WINDOW_BRIGHTNESS)
		state->page = OPTION_ROW_MESSAGE_SPEED;
}

/* Lowers the setting on the three-row cursor's row: message speed, window */
/* color or window brightness. */
UNUSED void mnSettingsMenuLowerSetting(SETTINGS_MENU_STATE *state) {
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

/* Raises the setting on the three-row cursor's row: message speed, window */
/* color or window brightness. */
UNUSED void mnSettingsMenuRaiseSetting(SETTINGS_MENU_STATE *state) {
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

/* An empty function, reached only through a bare entry of the window */
/* stub table. */
void mnSettingsMenuSelectionNoOp(void) {}

/* The settings menu's per-frame task: moves the cursor and the two slider */
/* markers, animates the marker of the slider in use, and pulses the chosen */
/* setting's icon. */
void mnSettingsMenuAnimateCursors(void) {
	s32 cursorResource;
	s32 markerResource;
	s32 selectedOption;
	SETTINGS_MENU_STATE *state;

	state = OptionMenuState;
	selectedOption = state->page;
	mnCursorAdvanceStub(&state->cursor.cursor);
	mnCursorMoveTowardTargetStub(&state->marker[0].cursor);
	mnCursorMoveTowardTargetStub(&state->marker[1].cursor);

	if(selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
		cursorResource = (state->cursorFrm & 7) + RESOURCE_MENU_CURSOR_PICTURES;
	}
	else {
		cursorResource = RESOURCE_MENU_CURSOR_PICTURES;
	}
	syVramCacheLoad((u8)state->marker[0].spr->idx, 256,
					syResourceGetTableEntry(cursorResource));

	if(selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
		markerResource = (state->cursorFrm & 7) + RESOURCE_MENU_CURSOR_PICTURES;
	}
	else {
		markerResource = RESOURCE_MENU_CURSOR_PICTURES;
	}
	syVramCacheLoad((u8)state->marker[1].spr->idx, 256,
					syResourceGetTableEntry(markerResource));

	if(selectedOption > SETTINGS_ROW_WINDOW_BRIGHTNESS)
		mnWindowSprPulse(
			state->choiceIcons[selectedOption - SETTINGS_ROW_MESSAGE_SPEED]
							  [state->option[selectedOption]]);
	state->cursorFrm++;
}

/* Allocates the settings menu's state block, copies the settings into it with
 */
/* how many values each takes, and starts its per-frame task. */
void mnSettingsMenuInitState(void) {
	SETTINGS_MENU_STATE *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_SETTINGS_MENU,
								 sizeof(SETTINGS_MENU_STATE));
	btDmaFill32(0, state, sizeof(SETTINGS_MENU_STATE));
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
	sySetEvent(mnSettingsMenuAnimateCursors, TASK_PASS_FRAME);
}

/* Stops the settings menu's per-frame task and frees its state block. */
/* The task goes first, since it reads the state block. */
void mnSettingsMenuRelease(void) {
	syCloseEvent(mnSettingsMenuAnimateCursors);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* The sprite palettes of the sliders' ends, which SettingsMenu_SliderPalette */
/* fills. */
#define WINDOW_COLOR_SLIDER_PALETTE		 14
#define WINDOW_BRIGHTNESS_SLIDER_PALETTE (WINDOW_COLOR_SLIDER_PALETTE + 1)

/* Opens the settings menu's window: the labels, the window color and */
/* brightness sliders with their markers, the cursor and the setting icons. */
/* Returns the window. */
WINDOW *mnSettingsMenuOpen(void) {
	SETTINGS_MENU_STATE *state;
	WINDOW *window;
	WINDOW_SPR *marker;
	s32 cacheSlot;
	s32 x;
	s32 y;
	s32 labelMessage;

	state = OptionMenuState;
	window = mnWindowCreate(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14,
							WINDOW_FLAG_INSTANT);
	mnWindowDrawDividerLine(window, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
	mnWindowDrawDividerLine(window, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
	mnWindowDrawDividerLine(window, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
	mnWindowDrawDividerLine(window, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
	labelMessage = MsgWindowColorLabel;
	mnTextDrawMessageAsTiles(labelMessage, window, 8, 0);
	labelMessage++;
	mnTextDrawMessageAsTiles(labelMessage, window, 8, 16);
#if LANGUAGE != LANGUAGE_JAPANESE
	labelMessage = MsgMessageSpeedLabel;
	mnTextDrawMessageAsTiles(labelMessage, window, 8, 32);
	labelMessage++;
	mnTextDrawMessageAsTiles(labelMessage, window, 32, 40);
#else
	mnTextDrawMessageAsTiles(MsgMessageSpeedLabel, window, 8, 40);
#endif
	mnTextDrawMessageAsTiles(MsgSpeechLabel, window, 8, 64);
	mnTextDrawMessageAsTiles(MsgAutoSleepLabel, window, 8, 88);

	cacheSlot = syVramCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(cacheSlot, 128, PointingHandTiles);
		marker = mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, window,
								   0, 0);
		state->cursor.spr = marker;
		x = window->x * 8;
		y = window->y * 8 + 12;
		mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
	}

	/* The ends of both sliders: OUTPUT_SIZE_32X8 sprites, the right end 4 tiles
	 */
	/* into the picture, each slider in its own palette. */
	cacheSlot = syVramCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syDMAXCopy(SettingsMenu_SliderPalette,
				   (void *)&OBJ_PLTT_COLOR(WINDOW_COLOR_SLIDER_PALETTE, 0),
				   DMA_COPY_HALFWORDS(2 * PALETTE_COLORS), REG_DMA3);
		syVramCacheLoad(cacheSlot, 256, SettingsMenu_SliderTiles);
		y = 0;
		marker = mnWindowSprCreate(cacheSlot, OUTPUT_SIZE_32X8, window,
								   OPTION_SLIDER_X - 6, y);
		marker->spr.oam.pal = WINDOW_COLOR_SLIDER_PALETTE;
		marker = mnWindowSprCreate(cacheSlot, OUTPUT_SIZE_32X8, window,
								   OPTION_SLIDER_X + 26, y);
		marker->spr.oam.idx += 4;
		marker->spr.oam.pal = WINDOW_COLOR_SLIDER_PALETTE;
		y = 16;
		marker = mnWindowSprCreate(cacheSlot, OUTPUT_SIZE_32X8, window,
								   OPTION_SLIDER_X - 6, y);
		marker->spr.oam.pal = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
		marker = mnWindowSprCreate(cacheSlot, OUTPUT_SIZE_32X8, window,
								   OPTION_SLIDER_X + 26, y);
		marker->spr.oam.idx += 4;
		marker->spr.oam.pal = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
	}

	/* The two markers: 16x16 sprites of 256 colours, whose 256 bytes */
	/* mnSettingsMenuAnimateCursors fills each frame. */
	cacheSlot = syVramCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(cacheSlot, 256, 0);
		marker = mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, window,
								   0, 0);
		marker->spr.oam.fullCol = 1;
		state->marker[0].spr = marker;
		x = window->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
			 state->optionCnt[SETTINGS_ROW_WINDOW_COLOR];
		y = window->y * 8 + 4;
		mnCursorSetPosImmediateStub(&state->marker[0].cursor, x, y);
	}

	cacheSlot = syVramCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(cacheSlot, 256, 0);
		marker = mnWindowSprCreate(cacheSlot, WINDOW_SPRITE_SIZE_16X16, window,
								   0, 0);
		marker->spr.oam.fullCol = 1;
		state->marker[1].spr = marker;
		x = window->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] *
			 OPTION_SLIDER_WIDTH /
			 state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		y = window->y * 8 + 20;
		mnCursorSetPosImmediateStub(&state->marker[1].cursor, x, y);
	}

	y = 28;
	state->choiceIcons[0][0] = mnCommandIconCreate(
		SettingsMenu_SpeedIcons[0], 0, window, OPTION_CHOICE_X, y);
	state->choiceIcons[0][1] = mnCommandIconCreate(
		SettingsMenu_SpeedIcons[1], 0, window, OPTION_CHOICE_X + 24, y);
	state->choiceIcons[0][2] = mnCommandIconCreate(
		SettingsMenu_SpeedIcons[2], 0, window, OPTION_CHOICE_X + 48, y);
	y = 52;
	state->choiceIcons[1][0] =
		mnCommandIconCreate(SettingsMenu_SpeechIcons[0], 0, window, 100, y);
	state->choiceIcons[1][1] =
		mnCommandIconCreate(SettingsMenu_SpeechIcons[1], 0, window, 124, y);
	y = 76;
	state->choiceIcons[2][0] =
		mnCommandIconCreate(SettingsMenu_SleepIcons[0], 0, window, 100, y);
	state->choiceIcons[2][1] =
		mnCommandIconCreate(SettingsMenu_SleepIcons[1], 0, window, 124, y);
	return window;
}

/* Runs the settings menu (window colour and brightness, message speed, speech,
 */
/* auto-sleep) until A or START keeps the settings (returns 0) or B drops them
 */
/* (returns -1). */
s32 mnSettingsMenuRun(void) {
	SETTINGS_MENU_STATE *state;
	WINDOW *settingsWindow;
	WINDOW *helpWindow;
	WINDOW_SPR *pair;
	s8 *windowColor;
	s8 *brightness;
	s32 redraw;
	s32 selectedOption;
	s32 i;
	s32 dimmed;
	s32 resourceIndex;
	s32 x;
	s32 y;
	s32 result;
	WINDOW_SPR *frame;

	redraw = 1;
	selectedOption = 0;
	mnSettingsMenuInitState();
	state = OptionMenuState;
	helpWindow = mnWindowCreate(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3,
								WINDOW_FLAG_INSTANT);
	settingsWindow = mnSettingsMenuOpen();
	pair = mnTitleCreate(MENU_TITLE_SETTINGS, settingsWindow, 64, -48);
	sySynch(1);
	windowColor = &state->option[SETTINGS_ROW_WINDOW_COLOR];
	brightness = &state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];

	for(;;) {
		if(redraw != 0) {
			redraw = 0;
			selectedOption =
				(selectedOption + SETTINGS_ROW_COUNT) % SETTINGS_ROW_COUNT;
			state->option[selectedOption] = (state->option[selectedOption] +
											 state->optionCnt[selectedOption]) %
											state->optionCnt[selectedOption];
			state->page = selectedOption;
			if(SleepDisabled != 0)
				state->option[SETTINGS_ROW_AUTO_SLEEP] = 0;

			/* Redraw each choice's icon at OAM_DEPTH_FRONT - 4, dimmed unless
			 * it is */
			/* the row's setting. */
			for(i = 0; i <= 2; i++) {
				frame = state->choiceIcons[0][i];
				frame->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frame);
				resourceIndex = (u8)state->choiceIcons[0][i]->idx;
				dimmed = 0;
				if(i != state->option[SETTINGS_ROW_MESSAGE_SPEED])
					dimmed = 1;
				mnCommandIconLoad(SettingsMenu_SpeedIcons[i], resourceIndex,
								  dimmed);
			}
			for(i = 0; i <= 1; i++) {
				frame = state->choiceIcons[1][i];
				frame->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frame);
				resourceIndex = (u8)state->choiceIcons[1][i]->idx;
				dimmed = 0;
				if(i != state->option[SETTINGS_ROW_SPEECH])
					dimmed = 1;
				mnCommandIconLoad(SettingsMenu_SpeechIcons[i], resourceIndex,
								  dimmed);
			}
			for(i = 0; i <= 1; i++) {
				frame = state->choiceIcons[2][i];
				frame->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(frame);
				resourceIndex = (u8)state->choiceIcons[2][i]->idx;
				dimmed = 0;
				if(i != state->option[SETTINGS_ROW_AUTO_SLEEP])
					dimmed = 1;
				mnCommandIconLoad(SettingsMenu_SleepIcons[i], resourceIndex,
								  dimmed);
			}

			x = settingsWindow->x * 8 + OPTION_SLIDER_X;
			x += (*windowColor * OPTION_SLIDER_WIDTH) /
				 state->optionCnt[SETTINGS_ROW_WINDOW_COLOR];
			y = settingsWindow->y * 8 + 4;
			mnCursorSetStub(&state->marker[0].cursor, x, y, 1);
			x = settingsWindow->x * 8 + OPTION_SLIDER_X;
			x += (*brightness * OPTION_SLIDER_WIDTH) /
				 state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS];
			y = settingsWindow->y * 8 + 20;
			mnCursorSetStub(&state->marker[1].cursor, x, y, 1);

			x = state->option[SETTINGS_ROW_MESSAGE_SPEED] +
				MsgMessageSpeedSetting;
			mnWindowClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 40,
									   OPTION_SPEED_END, 48);
			mnTextDrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 40);
			x = state->option[SETTINGS_ROW_SPEECH] + MsgSpeechSetting;
			mnWindowClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 64,
									   OPTION_SETTING_END, 72);
			mnTextDrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 64);
			x = state->option[SETTINGS_ROW_AUTO_SLEEP] + MsgAutoSleepSetting;
			mnWindowClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 88,
									   OPTION_SETTING_END, 96);
			mnTextDrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 88);
			/* The windows show the edited colours at once. */
			mnWindowColApply(*windowColor, *brightness);

			/* The cursor goes 3 tiles a row down the window's left edge, the
			 * first */
			/* row's a tile lower; the help window says what the row sets */
			/* (`Change the window's color.` for the first). */
			x = settingsWindow->x * 8;
			y = (selectedOption * 3 + settingsWindow->y) * 8 + 4;
			if(selectedOption == SETTINGS_ROW_WINDOW_COLOR)
				y += 8;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			mnWindowSprClearList(helpWindow);
			mnTextDrawMessage(selectedOption + MsgOptionHelp, helpWindow, 0, 0);
		}
		mnTitleBob(pair);
		sySynch(1);

		/* SELECT steps through the colour presets (window/scales.c). */
		if(KeysPressed & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CONFIRM);
			state->preset++;
			redraw = 1;
			if(state->preset > ARRAY_COUNT(SettingsMenu_HuePresets) - 1)
				state->preset = 0;
			*windowColor = SettingsMenu_HuePresets[state->preset];
			*brightness = SettingsMenu_BrightnessPresets[state->preset];
			continue;
		}
		if(KeysPressed & (KEY_A | KEY_START)) {
			result = 0;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(KeysPressed & KEY_B) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(KeysRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption--;
			redraw = 1;
		}
		else if(KeysRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption++;
			redraw = 1;
		}
		else {
			if(KeysRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]--;
				redraw = 1;
			}
			if(KeysRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]++;
				redraw = 1;
			}
		}
	}

	mnWindowClose(helpWindow, WINDOW_FLAG_INSTANT);
	mnWindowClose(settingsWindow, WINDOW_FLAG_INSTANT);
	/* Keep the edits, auto-sleep also taking effect at once */
	/* (IdleSleepEnabled); or put the saved window colours back. */
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
	}
	else {
		mnWindowColApply(MainRoutineStateWork.winHue,
						 MainRoutineStateWork.winBrightness);
	}
	mnSettingsMenuRelease();
	sySynch(1);
	return result;
}

#define PAUSE_MENU_ROWS		  3 /* Save your quest, Sleep and Change settings */
#define PAUSE_MENU_DEBUG_ROWS 3 /* the entries added in debug mode */
#define PAUSE_MENU_ROW_HEIGHT 24 /* pixels, 3 tiles */
#define PAUSE_MENU_TEXT_X	  48 /* pixels into the window, after the icons */

/* The Pause menu's per-frame task: moves the cursor and pulses the */
/* chosen entry's icon. */
void mnPauseMenuAnimateCursor(void) {
	SETTINGS_MENU_STATE *state = OptionMenuState;
	u32 index;

	mnCursorAdvanceStub(&state->cursor.cursor);
	index = state->page;
	mnWindowSprPulse(state->icon[index]);
}

/* Allocates the Pause menu's state block and starts its per-frame task. */
void mnPauseMenuInit(void) {
	void *state;
	state = syHeapSlotAllocEwram(HEAP_SLOT_SETTINGS_MENU,
								 sizeof(SETTINGS_MENU_STATE));
	btDmaFill32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnPauseMenuAnimateCursor, TASK_PASS_FRAME);
}

/* Stops the Pause menu's per-frame task and frees its state block. */
/* The task goes first, since it reads the state block. */
void mnPauseMenuRelease(void) {
	syCloseEvent(&mnPauseMenuAnimateCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* Opens the Pause menu's window with its entries (Save your quest, Sleep, */
/* Change settings, and the debug entries in debug mode; only Change settings */
/* while FLAG_IN_BATTLE_ARENA is set), their icons and the cursor. */
WINDOW *mnPauseMenuOpen(void) {
	SETTINGS_MENU_STATE *state;
	s32 rows;
	s32 first;
	s32 settingsOnly;
	s32 top;
	s32 height;
	WINDOW *window;
	s32 y;
	s32 cacheSlot;
	s32 labelMessage;
	s32 i;
	s32 x;

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
	window = mnWindowCreate(5, top, 20, height, WINDOW_FLAG_INSTANT);
	for(i = 1; i < rows; i++)
		mnWindowDrawDividerLine(window, 0, i * 3, 19, i * 3);
	y = 4;
	if(settingsOnly == 0) {
		labelMessage = MsgWorkspaceSaveQuest;
		mnTextDrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, 4);
		labelMessage++;
		mnTextDrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X,
						  4 + PAUSE_MENU_ROW_HEIGHT);
		y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
	}
	mnTextDrawMessage(MsgWorkspaceChangeSettings, window, PAUSE_MENU_TEXT_X, y);
	y += PAUSE_MENU_ROW_HEIGHT;
	if(DebugMode != 0) {
		labelMessage = MsgWorkspaceRoughMenu;
		mnTextDrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		mnTextDrawMessage(labelMessage + 1, window, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		labelMessage += 2;
		mnTextDrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, y);
	}
	cacheSlot = syVramCacheFindFreeEntry();
	if(cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(cacheSlot, 128, PointingHandTiles);
		state->cursor.spr = mnWindowSprCreate(
			cacheSlot, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
		x = window->x * 8;
		y = window->y * 8 + 16;
		mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
	}
	y = -4;
	for(i = 0; i < rows; i++) {
		state->icon[i] =
			mnCommandIconCreate(PauseMenu_Icons[first + i], 0, window, 12, y);
		y += PAUSE_MENU_ROW_HEIGHT;
	}
	return window;
}

/* Runs the Pause menu until the player picks an entry (PAUSE_MENU_ENTRY) or
 */
/* cancels with B or START (-1). */
s32 mnPauseMenuChoose(void) {
	WINDOW *window;
	s32 redraw;
	WINDOW_SPR *pair;
	s32 first;
	s32 i;
	s32 j;
	s32 rows;
	s32 settingsOnly;
	s32 dimmed;
	s32 y;
	s32 result;
	s32 selectedEntry;
	s32 resourceIndex;
	s32 x;
	s32 top;
	const s8 *pauseIcons;
	WINDOW_SPR *icon;
	SETTINGS_MENU_STATE *state;

	redraw = 1;
	rows = PAUSE_MENU_ROWS;
	settingsOnly = gmFlagGetStub(FLAG_IN_BATTLE_ARENA);
	first = 0;
	mnPauseMenuInit();
	state = OptionMenuState;
	window = mnPauseMenuOpen();
	/* The heading sits above the window, in the localized releases a little */
	/* lower in debug mode. */
#if TBS_PATCH_1
	y = -24;
	if(DebugMode != 0) {
		y = -16;
	}
	pair = mnTitleCreate(MENU_TITLE_PAUSE, window, 40, y);
#else
	pair = mnTitleCreate(MENU_TITLE_PAUSE, window, 40, -24);
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
			redraw = 0;
			selectedEntry = (selectedEntry + rows) % rows;
			state->page = selectedEntry;
			i = 0;
			if(i < rows) {
				j = first;
				pauseIcons = PauseMenu_Icons;
				do {
					icon = state->icon[i];
					icon->depth = OAM_DEPTH_FRONT - 4;
					mnIconShowStub(icon);
					resourceIndex = icon->idx;
					dimmed = 0;
					if(i != state->page) {
						dimmed = 1;
					}
					mnCommandIconLoad(*(s8 *)(j + (s32)pauseIcons),
									  resourceIndex, dimmed);
					i++;
					j++;
				} while(i < rows);
			}
			top = window->y;
			x = window->x * 8;
			y = (((selectedEntry * 3) + top) * 8) + 16;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
		}
		mnTitleBob(pair);
		sySynch(1);
		if(KeysPressed & KEY_A) {
			result = selectedEntry;
			sySndPlayStub(SE_MENU_CONFIRM);
		}
		else if(KeysPressed & (KEY_B | KEY_START)) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
		}
		else {
			if(KeysRepeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedEntry -= 1;
				redraw = 1;
			}
			else if(KeysRepeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedEntry += 1;
				redraw = 1;
			}
			continue;
		}
		break;
	}
	mnWindowClose(window, WINDOW_FLAG_INSTANT);
	mnPauseMenuRelease();
	sySynch(1);
	/* In the Battle Arena only Change settings shows: count from Save your
	 * quest. */
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
