/* @file */
/* The window colours the player picks, the settings menu that picks them with
 */
/* the message speed, speech and auto-sleep, and the Pause menu. */
/* Part of window.c. */

#include "window/settings_menu.h"

/* The settings menu's rows, in the order of their help messages */
/* (MsgWindowColorHelp on) and of SETTINGS_MENU_STATE.option. */
typedef enum SETTINGS_MENU_ROW {
	SETTINGS_ROW_WINDOW_COLOR = 0,
	SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
	SETTINGS_ROW_MESSAGE_SPEED = 2,
	SETTINGS_ROW_SPEECH = 3,
	SETTINGS_ROW_AUTO_SLEEP = 4,
	SETTINGS_ROW_COUNT = 5
} SETTINGS_MENU_ROW;

/* Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
 */
/* 31, as a BGR555 colour. rgb holds three components, not an already packed */
/* palette word. */
u16 mnColScaleComponents(s16 *rgb, FIXED redScale, FIXED greenScale,
						 FIXED blueScale) {
	s32 red = FIXEDtoINT(rgb[0] * redScale);
	s32 green = FIXEDtoINT(rgb[1] * greenScale);
	s32 blue = FIXEDtoINT(rgb[2] * blueScale);

	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	red |= RGB(0, green, blue);
	return red;
}

extern const u8 WindowColor_WaveTable[];

/* The window colours the window colour settings set: WINDOW_PALETTE's colours
 */
/* from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6. */
#define WINDOW_SHADE(n)                                                        \
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))
#define WINDOW_SHADE_SET(n, color)                                             \
	do {                                                                       \
		u32 color_ = (color);                                                  \
		WINDOW_SHADE(n) = color_;                                              \
	} while(0)

/* Sets the window colours (WINDOW_SHADE) for a window colour (0 to */
/* WINDOW_COLOR_COUNT - 1, a hue around the colour wheel) and brightness (0 to
 */
/* WINDOW_BRIGHTNESS_COUNT - 1, WINDOW_BRIGHTNESS_NEUTRAL the plain one). */
void mnWindowColApply(s32 color, s32 brightness) {
	s16 rgb[3];
	s16 step;
	s16 red;
	s16 green;
	s16 blue;
	s16 offset;

	step = ((color + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
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
	WINDOW_SHADE_SET(0,
					 mnColScaleComponents(rgb, FIXED_ONE * 14 / 15,
										  FIXED_ONE * 4 / 5, toFIXED(1.06666)));
	WINDOW_SHADE_SET(1, mnColScaleComponents(rgb, FIXED_ONE * 5 / 6,
											 FIXED_ONE * 11 / 15,
											 FIXED_ONE * 14 / 15));
	WINDOW_SHADE_SET(2, mnColScaleComponents(rgb, FIXED_ONE * 11 / 15,
											 FIXED_ONE * 2 / 3,
											 FIXED_ONE * 4 / 5));
	WINDOW_SHADE_SET(3, mnColScaleComponents(rgb, toFIXED(0.63333),
											 FIXED_ONE * 3 / 5,
											 FIXED_ONE * 2 / 3));
	WINDOW_SHADE_SET(4, mnColScaleComponents(rgb, FIXED_ONE * 31 / 30,
											 FIXED_ONE * 13 / 15,
											 FIXED_ONE * 6 / 5));
	WINDOW_SHADE_SET(5, mnColScaleComponents(rgb, toFIXED(1.13333),
											 FIXED_ONE * 14 / 15,
											 FIXED_ONE * 4 / 3));
	WINDOW_SHADE_SET(6, mnColScaleComponents(rgb, FIXED_ONE * 37 / 30,
											 FIXED_ONE, FIXED_ONE * 22 / 15));
}

/* The settings menu's per-frame task: moves the cursor and the two slider */
/* markers, animates the marker of the slider in use, and pulses the chosen */
/* setting's icon. */
void mnSettingsMenuAnimateCursors(void) {
	s32 firstMarkerPicture;
	s32 secondMarkerPicture;
	s32 selectedOption;
	SETTINGS_MENU_STATE *state;

	state = Ram_HeapSlots->settingsMenu;
	selectedOption = state->page;
	mnCursorAdvanceStub(&state->cursor.cursor);
	mnCursorMoveTowardTargetStub(&state->marker[0].cursor);
	mnCursorMoveTowardTargetStub(&state->marker[1].cursor);

	if(selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
		firstMarkerPicture =
			(state->cursorFrm & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
	}
	else {
		firstMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
	}
	syVramCacheLoad((u8)state->marker[0].spr->idx, 256,
					syResourceGetTableEntry(firstMarkerPicture));

	if(selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
		secondMarkerPicture =
			(state->cursorFrm & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
	}
	else {
		secondMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
	}
	syVramCacheLoad((u8)state->marker[1].spr->idx, 256,
					syResourceGetTableEntry(secondMarkerPicture));

	if(selectedOption > SETTINGS_ROW_WINDOW_BRIGHTNESS)
		mnWindowSprPulse(
			state->choiceIcons[selectedOption - SETTINGS_ROW_MESSAGE_SPEED]
							  [state->option[selectedOption]]);
	state->cursorFrm++;
}

/* Allocates the settings menu's state, copies the settings into it with */
/* how many values each takes, and starts its per-frame task. */
void mnSettingsMenuInitState(void) {
	SETTINGS_MENU_STATE *state;
	volatile u32 zero;

	state = syHeapSlotAllocEwram(HEAP_SLOT_SETTINGS_MENU,
								 sizeof(SETTINGS_MENU_STATE));
	zero = 0;
	syDMAXCopy((const void *)&zero, state,
			   DMA_FILL_WORDS(sizeof(SETTINGS_MENU_STATE) / 4), REG_DMA3);
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

/* Stops the settings menu's per-frame task and frees its state. */
void mnSettingsMenuRelease(void) {
	syCloseEvent(mnSettingsMenuAnimateCursors);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* The sprite palettes of the hue slider's ends and the brightness slider's. */
#define SLIDER_PALETTE			  14
#define BRIGHTNESS_SLIDER_PALETTE (SLIDER_PALETTE + 1)

/* The settings window's layout, sized to its labels: the window, the last */
/* column of its dividers, the hue and brightness sliders and the message */
/* speed icons. Japanese writes "Message speed" on one line. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define OPTION_WINDOW_X		2
#define OPTION_WINDOW_WIDTH 26
#define OPTION_DIVIDER_END	25
#define OPTION_SLIDER_X		100
#define OPTION_CHOICE_X		92
#else
#define OPTION_WINDOW_X 1
#if (LANGUAGE == LANGUAGE_ENGLISH)
#define OPTION_WINDOW_WIDTH 27
#else
#define OPTION_WINDOW_WIDTH 28
#endif
#define OPTION_DIVIDER_END 26
#define OPTION_SLIDER_X	   140
#define OPTION_CHOICE_X	   84
#endif

/* The pixels a settings slider's marker runs across, from OPTION_SLIDER_X. */
#define OPTION_SLIDER_WIDTH 60

extern const u8 PointingHandTiles[];
/* The hue and brightness sliders' picture and colours. */
extern const u8 SettingsMenu_SliderTiles[];
extern const u16 SettingsMenu_SliderPalette[];

extern s8 SettingsMenu_SpeedIcons[];
extern s8 SettingsMenu_SpeechIcons[];
extern s8 SettingsMenu_SleepIcons[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];

WINDOW_SPR *mnCommandIconCreate(s32 icon, s32 remap, WINDOW *window, s32 x,
								s32 y);

/* Opens the settings menu's window: the labels, the hue and brightness */
/* sliders with their markers, the cursor and the setting icons. Returns the */
/* window. Its sprites belong to the window and go when it closes. A */
/* slider's marker stands option * OPTION_SLIDER_WIDTH / count pixels right of
 */
/* OPTION_SLIDER_X. */
WINDOW *mnSettingsMenuOpen(void) {
	SETTINGS_MENU_STATE *state;
	WINDOW *win;
	WINDOW_SPR *out;
	s32 x;
	s32 y;
	s32 msg;

	state = Ram_HeapSlots->settingsMenu;
	win = mnWindowCreate(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14,
						 WINDOW_FLAG_INSTANT);
	mnWindowDrawDividerLine(win, 0, 2, OPTION_DIVIDER_END, 2);
	mnWindowDrawDividerLine(win, 0, 4, OPTION_DIVIDER_END, 4);
	mnWindowDrawDividerLine(win, 0, 7, OPTION_DIVIDER_END, 7);
	mnWindowDrawDividerLine(win, 0, 10, OPTION_DIVIDER_END, 10);
	msg = (s32)&MsgWindowColorLabel;
	mnTextDrawMessageAsTiles(msg, win, 8, 0);
	msg++;
	mnTextDrawMessageAsTiles(msg, win, 8, 16);
#if (LANGUAGE != LANGUAGE_JAPANESE)
	msg = (s32)&MsgMessageSpeedLabel;
	mnTextDrawMessageAsTiles(msg, win, 8, 32);
	msg++;
	mnTextDrawMessageAsTiles(msg, win, 32, 40);
#else
	mnTextDrawMessageAsTiles((s32)&MsgMessageSpeedLabel, win, 8, 40);
#endif
	mnTextDrawMessageAsTiles((s32)MsgSpeechLabel, win, 8, 64);
	mnTextDrawMessageAsTiles((s32)MsgAutoSleepLabel, win, 8, 88);

	out = mnWindowSprCreate(mnPointingHandLoadIntoFreeSlot(),
							WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	state->cursor.spr = out;
	x = win->x * 8;
	y = win->y * 8 + 12;
	mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);

	x = syVramCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syDMAXCopy(SettingsMenu_SliderPalette,
				   (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0),
				   DMA_COPY_HALFWORDS(32), REG_DMA3);
		syVramCacheLoad(x, 256, SettingsMenu_SliderTiles);
		y = 0;
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_32X8, win,
								OPTION_SLIDER_X - 6, y);
		out->spr.oam.pal = SLIDER_PALETTE;
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_32X8, win,
								OPTION_SLIDER_X + 26, y);
		out->spr.oam.idx += 4;
		out->spr.oam.pal = SLIDER_PALETTE;
		y = 16;
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_32X8, win,
								OPTION_SLIDER_X - 6, y);
		out->spr.oam.pal = BRIGHTNESS_SLIDER_PALETTE;
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_32X8, win,
								OPTION_SLIDER_X + 26, y);
		out->spr.oam.idx += 4;
		out->spr.oam.pal = BRIGHTNESS_SLIDER_PALETTE;
	}

	x = syVramCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(x, 256, 0);
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		out->spr.oam.fullCol = 1;
		state->marker[0].spr = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
			 state->optionCnt[SETTINGS_ROW_WINDOW_COLOR];
		y = win->y * 8 + 4;
		mnCursorSetPosImmediateStub(&state->marker[0].cursor, x, y);
	}

	x = syVramCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(x, 256, 0);
		out = mnWindowSprCreate(x, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		out->spr.oam.fullCol = 1;
		state->marker[1].spr = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] *
			 OPTION_SLIDER_WIDTH /
			 state->optionCnt[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		y = win->y * 8 + 20;
		mnCursorSetPosImmediateStub(&state->marker[1].cursor, x, y);
	}

	y = 28;
	state->choiceIcons[0][0] = mnCommandIconCreate(SettingsMenu_SpeedIcons[0],
												   0, win, OPTION_CHOICE_X, y);
	state->choiceIcons[0][1] = mnCommandIconCreate(
		SettingsMenu_SpeedIcons[1], 0, win, OPTION_CHOICE_X + 24, y);
	state->choiceIcons[0][2] = mnCommandIconCreate(
		SettingsMenu_SpeedIcons[2], 0, win, OPTION_CHOICE_X + 48, y);
	y = 52;
	state->choiceIcons[1][0] =
		mnCommandIconCreate(SettingsMenu_SpeechIcons[0], 0, win, 100, y);
	state->choiceIcons[1][1] =
		mnCommandIconCreate(SettingsMenu_SpeechIcons[1], 0, win, 124, y);
	y = 76;
	state->choiceIcons[2][0] =
		mnCommandIconCreate(SettingsMenu_SleepIcons[0], 0, win, 100, y);
	state->choiceIcons[2][1] =
		mnCommandIconCreate(SettingsMenu_SleepIcons[1], 0, win, 124, y);
	return win;
}

/* The layout that fits each language's text: the help window's column and */
/* width in tiles, then, in pixels from the settings window's left, where the */
/* choices' fields start and end. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define SETTINGS_HELP_WINDOW_X	   2
#define SETTINGS_HELP_WINDOW_WIDTH 26
#define CHOICE_X				   168
#define CHOICE_RIGHT			   192
#define FIRST_CHOICE_RIGHT		   192
#else
#define SETTINGS_HELP_WINDOW_X	   1
#define SETTINGS_HELP_WINDOW_WIDTH 28
#define CHOICE_X				   160
#define CHOICE_RIGHT			   184
/* The first choice's field is wider in German, French and Italian. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
	(LANGUAGE == LANGUAGE_ITALIAN)
#define FIRST_CHOICE_RIGHT 208
#else
#define FIRST_CHOICE_RIGHT 200
#endif
#endif

/* The heading above the settings window: Japanese shows the Settings heading;
 */
/* from patch 1 the screen shows the Pause heading the Pause menu also shows. */
#if TLA_PATCH_1
#define SETTINGS_HEADING MENU_TITLE_PAUSE
#else
#define SETTINGS_HEADING MENU_TITLE_SETTINGS
#endif

extern u8 MsgMessageSpeedSlow[], MsgSpeechOff[], MsgAutoSleepOff[],
	MsgWindowColorHelp[], MsgSleepModeHelp[];
extern const u8 SettingsMenu_HuePresets[], SettingsMenu_BrightnessPresets[];

/* The colour presets SELECT steps through (SettingsMenu_HuePresets). */
#define SETTINGS_PRESET_COUNT 6

void mnIconShowStub(WINDOW_SPR *icon);
void mnCommandIconLoad(s32 index, s32 value, s32 flag);

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
	WINDOW_SPR *icon;

	redraw = 1;
	selectedOption = 0;
	mnSettingsMenuInitState();
	state = Ram_HeapSlots->settingsMenu;
	helpWindow =
		mnWindowCreate(SETTINGS_HELP_WINDOW_X, 2, SETTINGS_HELP_WINDOW_WIDTH, 3,
					   WINDOW_FLAG_INSTANT);
	settingsWindow = mnSettingsMenuOpen();
	pair = mnTitleCreate(SETTINGS_HEADING, settingsWindow, 64, -48);
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
				icon = state->choiceIcons[0][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
				resourceIndex = (u8)state->choiceIcons[0][i]->idx;
				dimmed = 0;
				if(i != state->option[SETTINGS_ROW_MESSAGE_SPEED])
					dimmed = 1;
				mnCommandIconLoad(SettingsMenu_SpeedIcons[i], resourceIndex,
								  dimmed);
			}
			for(i = 0; i <= 1; i++) {
				icon = state->choiceIcons[1][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
				resourceIndex = (u8)state->choiceIcons[1][i]->idx;
				dimmed = 0;
				if(i != state->option[SETTINGS_ROW_SPEECH])
					dimmed = 1;
				mnCommandIconLoad(SettingsMenu_SpeechIcons[i], resourceIndex,
								  dimmed);
			}
			for(i = 0; i <= 1; i++) {
				icon = state->choiceIcons[2][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
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
				(s32)MsgMessageSpeedSlow;
			mnWindowClearInteriorTiles(settingsWindow, CHOICE_X, 40,
									   FIRST_CHOICE_RIGHT, 48);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 40);
			x = state->option[SETTINGS_ROW_SPEECH] + (s32)MsgSpeechOff;
			mnWindowClearInteriorTiles(settingsWindow, CHOICE_X, 64,
									   CHOICE_RIGHT, 72);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 64);
			x = state->option[SETTINGS_ROW_AUTO_SLEEP] + (s32)MsgAutoSleepOff;
			mnWindowClearInteriorTiles(settingsWindow, CHOICE_X, 88,
									   CHOICE_RIGHT, 96);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 88);
			/* The windows show the edited colours at once. */
			mnWindowColApply(*windowColor, *brightness);

			/* The cursor goes 3 tiles a row down the window's left edge, the
			 * first */
			/* row's a tile lower; the help window says what the row sets. */
			x = settingsWindow->x * 8;
			{
				s32 top = settingsWindow->y;

				y = (selectedOption * 3 + top) * 8 + 4;
			}
			if(selectedOption == SETTINGS_ROW_WINDOW_COLOR)
				y += 8;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			mnWindowSprClearList(helpWindow);
			mnTextDrawMessage(selectedOption + (s32)MsgWindowColorHelp,
							  (s32)helpWindow, 0, 0);
		}
		mnTitleBob(pair);
		sySynch(1);

		/* SELECT steps through the colour presets (window/scales.c). */
		if(InputWork.pressed & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CONFIRM);
			state->preset++;
			redraw = 1;
			if(state->preset > SETTINGS_PRESET_COUNT - 1)
				state->preset = 0;
			*windowColor = SettingsMenu_HuePresets[state->preset];
			*brightness = SettingsMenu_BrightnessPresets[state->preset];
			continue;
		}
		if(InputWork.pressed & (KEY_A | KEY_START)) {
			result = 0;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(InputWork.pressed & KEY_B) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(InputWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption--;
			redraw = 1;
		}
		else if(InputWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption++;
			redraw = 1;
		}
		else {
			if(InputWork.repeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]--;
				redraw = 1;
			}
			if(InputWork.repeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]++;
				redraw = 1;
			}
		}
	}

	mnWindowClose(helpWindow, WINDOW_FLAG_INSTANT);
	mnWindowClose(settingsWindow, WINDOW_FLAG_INSTANT);
	if(result == 0) {
		/* "In Sleep Mode, press Select and the L and R Buttons to return to the
		 */
		/* game." */
		if(MainRoutineStateWork.idleSleepEnabled == 0 &&
		   state->option[SETTINGS_ROW_AUTO_SLEEP] == 1)
			mnFieldMessageShow((s32)MsgSleepModeHelp, 1);
	}
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

/* The Pause menu's per-frame task: moves the cursor and pulses the */
/* chosen entry's icon. */
void mnPauseMenuAnimateCursor(void) {
	SETTINGS_MENU_STATE *state = Ram_HeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor.cursor);
	mnWindowSprPulse(state->icon[state->page]);
}

/* Allocates the Pause menu's state and starts its per-frame task. */
/* The Pause menu shares the settings menu's heap slot: mnPauseMenuChoose frees
 */
/* it before mnPauseMenuRun opens the settings menu. */
void mnPauseMenuInit(void) {
	void *state = syHeapSlotAllocEwram(HEAP_SLOT_SETTINGS_MENU,
									   sizeof(SETTINGS_MENU_STATE));

	btDmaFill32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnPauseMenuAnimateCursor, TASK_PASS_FRAME);
}

/* Stops the Pause menu's per-frame task and frees its state. */
void mnPauseMenuRelease(void) {
	syCloseEvent(mnPauseMenuAnimateCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

extern u8 MsgSaveYourQuest[];
extern u8 MsgChangeSettings[];
extern u8 MsgRoughMenu[];
extern const s8 PauseMenu_Icons[];

#define PAUSE_MENU_ROWS		  3 /* Save your quest, Sleep and Change settings */
#define PAUSE_MENU_DEBUG_ROWS 3 /* the entries added in debug mode */
#define PAUSE_MENU_ROW_HEIGHT 24 /* pixels, 3 tiles */
#define PAUSE_MENU_TEXT_X	  48 /* pixels into the window, after the icons */

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
	WINDOW *win;
	s32 y;
	s32 msg;
	s32 i;

	rows = PAUSE_MENU_ROWS;
	state = Ram_HeapSlots->settingsMenu;
	settingsOnly = syGameFlagGet(FLAG_IN_BATTLE_ARENA);
	first = 0;
	if(settingsOnly != 0) {
		rows = 1;
		first = PAUSE_MENU_CHANGE_SETTINGS;
	}
	if(DebugMode != 0)
		rows += PAUSE_MENU_DEBUG_ROWS;
	top = 8 - rows;
	height = rows * 3 + 1;
	if(top + height > (DISPLAY_TILE_HEIGHT - 1)) {
		top = 1;
		height = DISPLAY_TILE_HEIGHT - 1;
	}
	win = mnWindowCreate(5, top, 20, height, WINDOW_FLAG_INSTANT);
	for(i = 1; i < rows; i++)
		mnWindowDrawDividerLine(win, 0, i * 3, 19, i * 3);
	y = 4;
	if(settingsOnly == 0) {
		msg = (s32)MsgSaveYourQuest;
		mnTextDrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, 4);
		msg++;
		mnTextDrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X,
						  4 + PAUSE_MENU_ROW_HEIGHT);
		y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
	}
	mnTextDrawMessage((s32)MsgChangeSettings, (s32)win, PAUSE_MENU_TEXT_X, y);
	y += PAUSE_MENU_ROW_HEIGHT;
	if(DebugMode != 0) {
		msg = (s32)MsgRoughMenu;
		mnTextDrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		mnTextDrawMessage(msg + 1, (s32)win, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		msg += 2;
		mnTextDrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, y);
	}
	state->cursor.spr = mnWindowSprCreate(mnPointingHandLoadIntoFreeSlot(),
										  WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	y = -4;
	for(i = 0; i < rows; i++) {
		state->icon[i] =
			mnCommandIconCreate(PauseMenu_Icons[first + i], 0, win, 12, y);
		y += PAUSE_MENU_ROW_HEIGHT;
	}
	return win;
}

/* The heading's top, in pixels from the window's; from patch 1 it sits 8 */
/* pixels lower. */
#if TLA_PATCH_1
#define HEADING_Y (-16)
#else
#define HEADING_Y (-24)
#endif

/* Runs the Pause menu until the player picks an entry (enum */
/* PAUSE_MENU_ENTRY) or cancels with B or START (-1). */
/* The result counts in the full entry table even when only Change settings is
 */
/* shown. The menu's window and state are freed before it returns; */
/* mnPauseMenuRun (screens_pause_menu.c) acts on the result. */
s32 mnPauseMenuChoose(s32 initial) {
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
	s32 selected;
	s32 resourceIndex;
	s32 x;
	s32 top;
	const s8 *pauseIcons;
	WINDOW_SPR *icon;
	SETTINGS_MENU_STATE *state;

	redraw = -1;
	selected = initial;
	rows = PAUSE_MENU_ROWS;
	settingsOnly = syGameFlagGet(FLAG_IN_BATTLE_ARENA);
	first = 0;
	mnPauseMenuInit();
	state = Ram_HeapSlots->settingsMenu;
	sySynch(1);
	window = mnPauseMenuOpen();
	pair = mnTitleCreate(MENU_TITLE_PAUSE, window, 40, HEADING_Y);
	if(settingsOnly != 0) {
		rows = 1;
		first = PAUSE_MENU_CHANGE_SETTINGS;
	}
	if(DebugMode != 0)
		rows += PAUSE_MENU_DEBUG_ROWS;
	for(;;) {
		if(redraw != 0) {
			selected = (selected + rows) % rows;
			state->page = selected;
			i = 0;
			if(i < rows) {
				j = first;
				pauseIcons = PauseMenu_Icons;
				do {
					icon = state->icon[i];
					icon->depth = OAM_DEPTH_FRONT - 4;
					mnIconShowStub(icon);
					resourceIndex = (u8)icon->idx;
					dimmed = 0;
					if(i != state->page)
						dimmed = 1;
					mnCommandIconLoad(*(s8 *)(j + (s32)pauseIcons),
									  resourceIndex, dimmed);
					i++;
					j++;
				} while(i < rows);
			}
			top = window->y;
			x = window->x * 8;
			y = (s16)top;
			y = (selected * 3 + y) * 8 + 16;
			if(-1 == redraw)
				mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
			else
				mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			redraw = 0;
		}
		mnTitleBob(pair);
		sySynch(1);
		if(InputWork.pressed & KEY_A) {
			result = selected;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(InputWork.pressed & (KEY_B | KEY_START)) {
			result = -1;
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(InputWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selected--;
			redraw = 1;
			continue;
		}
		if(InputWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selected++;
			redraw = 1;
			continue;
		}
		if(DebugMode != 0) {
			if(InputWork.pressed & KEY_R) {
				result = PAUSE_MENU_DEBUG_R;
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if(InputWork.pressed & KEY_L) {
				result = PAUSE_MENU_DEBUG_L;
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
		}
	}
	mnWindowClose(window, WINDOW_FLAG_INSTANT);
	mnPauseMenuRelease();
	sySynch(1);
	if(result >= 0)
		result += first;
	return result;
}

#undef WINDOW_SHADE
#undef WINDOW_SHADE_SET
#undef SLIDER_PALETTE
#undef BRIGHTNESS_SLIDER_PALETTE
#undef OPTION_WINDOW_X
#undef OPTION_WINDOW_WIDTH
#undef OPTION_DIVIDER_END
#undef OPTION_SLIDER_X
#undef OPTION_CHOICE_X
#undef SETTINGS_HELP_WINDOW_X
#undef SETTINGS_HELP_WINDOW_WIDTH
#undef CHOICE_X
#undef CHOICE_RIGHT
#undef FIRST_CHOICE_RIGHT
#undef SETTINGS_HEADING
#undef HEADING_Y
