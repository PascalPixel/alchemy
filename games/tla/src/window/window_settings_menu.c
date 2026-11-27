/* The window colours the player picks, the settings menu that picks them with
   the message speed, speech and auto-sleep, and the Pause menu. Part of
   window.c. */

#include "window/settings_menu.h"

/* The settings menu's rows, in the order of their help messages
   (MsgWindowColorHelp on) and of SETTINGS_MENU_STATE.option. */
typedef enum SETTINGS_MENU_ROW {
	SETTINGS_ROW_WINDOW_COLOR = 0,
	SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
	SETTINGS_ROW_MESSAGE_SPEED = 2,
	SETTINGS_ROW_SPEECH = 3,
	SETTINGS_ROW_AUTO_SLEEP = 4,
	SETTINGS_ROW_COUNT = 5
} SETTINGS_MENU_ROW;

/* Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
   31, as a BGR555 colour. rgb holds three components, not an already packed
   palette word. */
Uint16 mnColorScaleComponents(
	Sint16 *rgb, FIXED redScale, FIXED greenScale, FIXED blueScale)
{
	Sint32 red = FIXEDtoINT(rgb[0] * redScale);
	Sint32 green = FIXEDtoINT(rgb[1] * greenScale);
	Sint32 blue = FIXEDtoINT(rgb[2] * blueScale);

	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	red |= RGB(0, green, blue);
	return red;
}

extern const Uint8 WindowColorWaveTable[];

/* The window colours the window colour settings set: WINDOW_PALETTE's colours
   from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6. */
#define WINDOW_SHADE(n) \
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))
#define WINDOW_SHADE_SET(n, color) \
	do { \
		Uint32 color_ = (color); \
		WINDOW_SHADE(n) = color_; \
	} while(0)

/* Sets the window colours (WINDOW_SHADE) for a window colour (0 to
   WINDOW_COLOR_COUNT - 1, a hue around the colour wheel) and brightness (0 to
   WINDOW_BRIGHTNESS_COUNT - 1, WINDOW_BRIGHTNESS_NEUTRAL the plain one). */
void mnWindowColorApply(Sint32 color, Sint32 brightness)
{
	Sint16 rgb[3];
	Sint16 step;
	Sint16 red;
	Sint16 green;
	Sint16 blue;
	Sint16 offset;

	step = ((color + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
	offset = brightness - WINDOW_BRIGHTNESS_NEUTRAL;
	red = WindowColorWaveTable[(step % WINDOW_COLOR_WHEEL_STEPS)] + offset;
	green = WindowColorWaveTable[(step + WINDOW_COLOR_WHEEL_STEPS / 3) %
				WINDOW_COLOR_WHEEL_STEPS] +
		offset;
	blue = WindowColorWaveTable[(step + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
			   WINDOW_COLOR_WHEEL_STEPS] +
		offset;
	CLAMP(red, 0, RGB_LEVEL_MAX);
	CLAMP(green, 0, RGB_LEVEL_MAX);
	CLAMP(blue, 0, RGB_LEVEL_MAX);
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	/* Each shade scales the tint by channel, in 16.16 (toFIXED(1) is 1): shades
	   0 to 3 grow darker, from about 0.93, 0.8 and 1.07 to 0.63, 0.6 and 0.67,
	   and 4 to 6 lighter, up to about 1.23, 1 and 1.47. */
	WINDOW_SHADE_SET(0,
		mnColorScaleComponents(
			rgb, toFIXED(14) / 15, toFIXED(4) / 5, toFIXED(1.06666)));
	WINDOW_SHADE_SET(1,
		mnColorScaleComponents(
			rgb, toFIXED(5) / 6, toFIXED(11) / 15, toFIXED(14) / 15));
	WINDOW_SHADE_SET(2,
		mnColorScaleComponents(
			rgb, toFIXED(11) / 15, toFIXED(2) / 3, toFIXED(4) / 5));
	WINDOW_SHADE_SET(3,
		mnColorScaleComponents(
			rgb, toFIXED(0.63333), toFIXED(3) / 5, toFIXED(2) / 3));
	WINDOW_SHADE_SET(4,
		mnColorScaleComponents(
			rgb, toFIXED(31) / 30, toFIXED(13) / 15, toFIXED(6) / 5));
	WINDOW_SHADE_SET(5,
		mnColorScaleComponents(
			rgb, toFIXED(1.13333), toFIXED(14) / 15, toFIXED(4) / 3));
	WINDOW_SHADE_SET(6,
		mnColorScaleComponents(
			rgb, toFIXED(37) / 30, toFIXED(1), toFIXED(22) / 15));
}

/* The settings menu's per-tick event: moves the cursor and the two slider
   markers, animates the marker of the slider in use, and pulses the chosen
   setting's icon. */
void mnSettingsMenuAnimateCursors(void)
{
	Sint32				 firstMarkerPicture;
	Sint32				 secondMarkerPicture;
	Sint32				 selectedOption;
	SETTINGS_MENU_STATE *state;

	state = RamHeapSlots->settingsMenu;
	selectedOption = state->page;
	mnCursorAdvanceStub(&state->cursor.cursor);
	mnCursorMoveTowardTargetStub(&state->marker[0].cursor);
	mnCursorMoveTowardTargetStub(&state->marker[1].cursor);

	if(selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
		firstMarkerPicture =
			(state->cursorFrame & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
	} else {
		firstMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
	}
	syVRAMCacheLoad((Uint8)state->marker[0].sprite->index, 256,
		syResourceGetTableEntry(firstMarkerPicture));

	if(selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
		secondMarkerPicture =
			(state->cursorFrame & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
	} else {
		secondMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
	}
	syVRAMCacheLoad((Uint8)state->marker[1].sprite->index, 256,
		syResourceGetTableEntry(secondMarkerPicture));

	if(selectedOption > SETTINGS_ROW_WINDOW_BRIGHTNESS)
		mnWindowSpritePulse(state->choiceIcons[selectedOption -
			SETTINGS_ROW_MESSAGE_SPEED][state->option[selectedOption]]);
	state->cursorFrame++;
}

/* Allocates the settings menu's state, copies the settings into it with how
   many values each takes, and starts its per-tick event. */
void mnSettingsMenuInitState(void)
{
	SETTINGS_MENU_STATE *state;
	volatile Uint32		 zero;

	state = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));
	zero = 0;
	syDMAXCopy((const void *)&zero, state,
		DMA_FILL_WORDS(sizeof(SETTINGS_MENU_STATE) / 4), REG_DMA3);
	state->option[SETTINGS_ROW_WINDOW_COLOR] = MainRoutineStateWork.windowHue;
	state->optionCount[SETTINGS_ROW_WINDOW_COLOR] = WINDOW_COLOR_COUNT;
	state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] =
		MainRoutineStateWork.windowBrightness;
	state->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS] =
		WINDOW_BRIGHTNESS_COUNT;
	state->option[SETTINGS_ROW_MESSAGE_SPEED] = MainRoutineStateWork.msgSpeed;
	state->optionCount[SETTINGS_ROW_MESSAGE_SPEED] = MESSAGE_SPEED_FAST + 1;
	state->option[SETTINGS_ROW_SPEECH] = MainRoutineStateWork.speech;
	state->optionCount[SETTINGS_ROW_SPEECH] = 2;
	state->option[SETTINGS_ROW_AUTO_SLEEP] =
		MainRoutineStateWork.idleSleepEnabled;
	state->optionCount[SETTINGS_ROW_AUTO_SLEEP] = 2;
	sySetEvent(mnSettingsMenuAnimateCursors, EVENT_PASS_TICK);
}

/* Stops the settings menu's per-tick event and frees its state. */
void mnSettingsMenuRelease(void)
{
	syCloseEvent(mnSettingsMenuAnimateCursors);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* The sprite palettes of the hue slider's ends and the brightness slider's. */
#define SLIDER_PALETTE			  14
#define BRIGHTNESS_SLIDER_PALETTE (SLIDER_PALETTE + 1)

/* The settings window's layout, sized to its labels: the window, the last
   column of its dividers, the hue and brightness sliders and the message speed
   icons. Japanese writes "Message speed" on one line. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define OPTION_WINDOW_X		2
#define OPTION_WINDOW_WIDTH 26
#define OPTION_DIVIDER_END	25
#define OPTION_SLIDER_X		100
#define OPTION_CHOICE_X		92
#else
#define OPTION_WINDOW_X		1
#define OPTION_WINDOW_WIDTH ENGLISH_OR(27, 28)
#define OPTION_DIVIDER_END	26
#define OPTION_SLIDER_X		140
#define OPTION_CHOICE_X		84
#endif

/* The pixels a settings slider's marker runs across, from OPTION_SLIDER_X. */
#define OPTION_SLIDER_WIDTH 60

extern const Uint8 PointingHandTiles[];
/* The hue and brightness sliders' picture and colours. */
extern const Uint8	SettingsMenuSliderTiles[];
extern const Uint16 SettingsMenuSliderPalette[];

extern Sint8 SettingsMenuSpeedIcons[];
extern Sint8 SettingsMenuSpeechIcons[];
extern Sint8 SettingsMenuSleepIcons[];
extern Uint8 MsgSpeechLabel[];
extern Uint8 MsgAutoSleepLabel[];

WINDOW_SPR *mnCommandIconCreate(
	Sint32 icon, Sint32 remap, WINDOW *window, Sint32 x, Sint32 y);

/* Opens the settings menu's window: the labels, the hue and brightness sliders
   with their markers, the cursor and the setting icons. Returns the window. Its
   sprites belong to the window and go when it closes. A slider's marker stands
   option * OPTION_SLIDER_WIDTH / count pixels right of OPTION_SLIDER_X. */
WINDOW *mnSettingsMenuOpen(void)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*window;
	WINDOW_SPR			*out;
	Sint32				 x;
	Sint32				 y;
	Sint32				 msg;

	state = RamHeapSlots->settingsMenu;
	window = mnWindowCreate(
		OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, WINDOW_FLAG_INSTANT);
	mnWindowDrawDividerLine(window, 0, 2, OPTION_DIVIDER_END, 2);
	mnWindowDrawDividerLine(window, 0, 4, OPTION_DIVIDER_END, 4);
	mnWindowDrawDividerLine(window, 0, 7, OPTION_DIVIDER_END, 7);
	mnWindowDrawDividerLine(window, 0, 10, OPTION_DIVIDER_END, 10);
	msg = (Sint32)&MsgWindowColorLabel;
	mnTextDrawMessageAsTiles(msg, window, 8, 0);
	msg++;
	mnTextDrawMessageAsTiles(msg, window, 8, 16);
#if (LANGUAGE != LANGUAGE_JAPANESE)
	msg = (Sint32)&MsgMessageSpeedLabel;
	mnTextDrawMessageAsTiles(msg, window, 8, 32);
	msg++;
	mnTextDrawMessageAsTiles(msg, window, 32, 40);
#else
	mnTextDrawMessageAsTiles((Sint32)&MsgMessageSpeedLabel, window, 8, 40);
#endif
	mnTextDrawMessageAsTiles((Sint32)MsgSpeechLabel, window, 8, 64);
	mnTextDrawMessageAsTiles((Sint32)MsgAutoSleepLabel, window, 8, 88);

	out = mnWindowSpriteCreate(mnPointingHandLoadIntoFreeSlot(),
		WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
	state->cursor.sprite = out;
	x = window->x * 8;
	y = window->y * 8 + 12;
	mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);

	x = syVRAMCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syDMAXCopy(SettingsMenuSliderPalette,
			(void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), DMA_COPY_HALFWORDS(32),
			REG_DMA3);
		syVRAMCacheLoad(x, 256, SettingsMenuSliderTiles);
		y = 0;
		out = mnWindowSpriteCreate(
			x, WINDOW_SPRITE_SIZE_32X8, window, OPTION_SLIDER_X - 6, y);
		out->sprite.oam.palette = SLIDER_PALETTE;
		out = mnWindowSpriteCreate(
			x, WINDOW_SPRITE_SIZE_32X8, window, OPTION_SLIDER_X + 26, y);
		out->sprite.oam.index += 4;
		out->sprite.oam.palette = SLIDER_PALETTE;
		y = 16;
		out = mnWindowSpriteCreate(
			x, WINDOW_SPRITE_SIZE_32X8, window, OPTION_SLIDER_X - 6, y);
		out->sprite.oam.palette = BRIGHTNESS_SLIDER_PALETTE;
		out = mnWindowSpriteCreate(
			x, WINDOW_SPRITE_SIZE_32X8, window, OPTION_SLIDER_X + 26, y);
		out->sprite.oam.index += 4;
		out->sprite.oam.palette = BRIGHTNESS_SLIDER_PALETTE;
	}

	x = syVRAMCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(x, 256, 0);
		out = mnWindowSpriteCreate(x, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
		out->sprite.oam.fullColor = 1;
		state->marker[0].sprite = out;
		x = window->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
			state->optionCount[SETTINGS_ROW_WINDOW_COLOR];
		y = window->y * 8 + 4;
		mnCursorSetPosImmediateStub(&state->marker[0].cursor, x, y);
	}

	x = syVRAMCacheFindFreeEntry();
	if(x < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(x, 256, 0);
		out = mnWindowSpriteCreate(x, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
		out->sprite.oam.fullColor = 1;
		state->marker[1].sprite = out;
		x = window->x * 8 + OPTION_SLIDER_X;
		x += state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] *
			OPTION_SLIDER_WIDTH /
			state->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		y = window->y * 8 + 20;
		mnCursorSetPosImmediateStub(&state->marker[1].cursor, x, y);
	}

	y = 28;
	state->choiceIcons[0][0] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[0], 0, window, OPTION_CHOICE_X, y);
	state->choiceIcons[0][1] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[1], 0, window, OPTION_CHOICE_X + 24, y);
	state->choiceIcons[0][2] = mnCommandIconCreate(
		SettingsMenuSpeedIcons[2], 0, window, OPTION_CHOICE_X + 48, y);
	y = 52;
	state->choiceIcons[1][0] =
		mnCommandIconCreate(SettingsMenuSpeechIcons[0], 0, window, 100, y);
	state->choiceIcons[1][1] =
		mnCommandIconCreate(SettingsMenuSpeechIcons[1], 0, window, 124, y);
	y = 76;
	state->choiceIcons[2][0] =
		mnCommandIconCreate(SettingsMenuSleepIcons[0], 0, window, 100, y);
	state->choiceIcons[2][1] =
		mnCommandIconCreate(SettingsMenuSleepIcons[1], 0, window, 124, y);
	return window;
}

/* The layout that fits each language's text: the help window's column and width
   in tiles, then, in pixels from the settings window's left, where the choices'
   fields start and end. */
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
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_ITALIAN)
#define FIRST_CHOICE_RIGHT 208
#else
#define FIRST_CHOICE_RIGHT 200
#endif
#endif

/* The heading above the settings window: Japanese shows the Settings heading;
   in every edition but the Japanese the screen shows the Pause heading the
   Pause menu also shows. */
#define SETTINGS_HEADING \
	WHEN(SETTINGS_SHOW_PAUSE_HEADING, MENU_TITLE_PAUSE, MENU_TITLE_SETTINGS)

extern Uint8 MsgMessageSpeedSlow[], MsgSpeechOff[], MsgAutoSleepOff[],
	MsgWindowColorHelp[], MsgSleepModeHelp[];
extern const Uint8 SettingsMenuHuePresets[], SettingsMenuBrightnessPresets[];

/* The colour presets SELECT steps through (SettingsMenuHuePresets). */
#define SETTINGS_PRESET_COUNT 6

void mnIconShowStub(WINDOW_SPR *icon);
void mnCommandIconLoad(Sint32 index, Sint32 value, Sint32 flag);

/* Runs the settings menu (window colour and brightness, message speed, speech,
   auto-sleep) until A or START keeps the settings (returns 0) or B drops them
   (returns -1). */
Sint32 mnSettingsMenuRun(void)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*settingsWindow;
	WINDOW				*helpWindow;
	WINDOW_SPR			*pair;
	Sint8				*windowColor;
	Sint8				*brightness;
	Bool				 redraw;
	Sint32				 selectedOption;
	Sint32				 i;
	Bool				 dimmed;
	Sint32				 resourceIndex;
	Sint32				 x;
	Sint32				 y;
	Sint32				 result;
	WINDOW_SPR			*icon;

	redraw = TRUE;
	selectedOption = 0;
	mnSettingsMenuInitState();
	state = RamHeapSlots->settingsMenu;
	helpWindow = mnWindowCreate(SETTINGS_HELP_WINDOW_X, 2,
		SETTINGS_HELP_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
	settingsWindow = mnSettingsMenuOpen();
	pair = mnTitleCreate(SETTINGS_HEADING, settingsWindow, 64, -48);
	sySynch(1);
	windowColor = &state->option[SETTINGS_ROW_WINDOW_COLOR];
	brightness = &state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];

	for(;;) {
		if(redraw != 0) {
			redraw = FALSE;
			selectedOption =
				(selectedOption + SETTINGS_ROW_COUNT) % SETTINGS_ROW_COUNT;
			state->option[selectedOption] =
				(state->option[selectedOption] +
					state->optionCount[selectedOption]) %
				state->optionCount[selectedOption];
			state->page = selectedOption;
			if(SleepDisabled != 0)
				state->option[SETTINGS_ROW_AUTO_SLEEP] = 0;

			/* Redraw each choice's icon at OAM_DEPTH_FRONT - 4, dimmed unless
			   it is the row's setting. */

			for(i = 0; i <= 2; i++) {
				icon = state->choiceIcons[0][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
				resourceIndex = (Uint8)state->choiceIcons[0][i]->index;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_MESSAGE_SPEED])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSpeedIcons[i], resourceIndex, dimmed);
			}
			for(i = 0; i <= 1; i++) {
				icon = state->choiceIcons[1][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
				resourceIndex = (Uint8)state->choiceIcons[1][i]->index;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_SPEECH])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSpeechIcons[i], resourceIndex, dimmed);
			}
			for(i = 0; i <= 1; i++) {
				icon = state->choiceIcons[2][i];
				icon->depth = OAM_DEPTH_FRONT - 4;
				mnIconShowStub(icon);
				resourceIndex = (Uint8)state->choiceIcons[2][i]->index;
				dimmed = FALSE;
				if(i != state->option[SETTINGS_ROW_AUTO_SLEEP])
					dimmed = TRUE;
				mnCommandIconLoad(
					SettingsMenuSleepIcons[i], resourceIndex, dimmed);
			}

			x = settingsWindow->x * 8 + OPTION_SLIDER_X;
			x += (*windowColor * OPTION_SLIDER_WIDTH) /
				state->optionCount[SETTINGS_ROW_WINDOW_COLOR];
			y = settingsWindow->y * 8 + 4;
			mnCursorSetStub(&state->marker[0].cursor, x, y, 1);
			x = settingsWindow->x * 8 + OPTION_SLIDER_X;
			x += (*brightness * OPTION_SLIDER_WIDTH) /
				state->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
			y = settingsWindow->y * 8 + 20;
			mnCursorSetStub(&state->marker[1].cursor, x, y, 1);

			x = state->option[SETTINGS_ROW_MESSAGE_SPEED] +
				(Sint32)MsgMessageSpeedSlow;
			mnWindowClearInteriorTiles(
				settingsWindow, CHOICE_X, 40, FIRST_CHOICE_RIGHT, 48);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 40);
			x = state->option[SETTINGS_ROW_SPEECH] + (Sint32)MsgSpeechOff;
			mnWindowClearInteriorTiles(
				settingsWindow, CHOICE_X, 64, CHOICE_RIGHT, 72);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 64);
			x = state->option[SETTINGS_ROW_AUTO_SLEEP] +
				(Sint32)MsgAutoSleepOff;
			mnWindowClearInteriorTiles(
				settingsWindow, CHOICE_X, 88, CHOICE_RIGHT, 96);
			mnTextDrawMessageAsTiles(x, settingsWindow, CHOICE_X, 88);
			/* The windows show the edited colours at once. */
			mnWindowColorApply(*windowColor, *brightness);

			/* The cursor goes 3 tiles a row down the window's left edge, the
			   first row's a tile lower; the help window says what the row
			   sets. */
			x = settingsWindow->x * 8;
			{
				Sint32 top = settingsWindow->y;

				y = (selectedOption * 3 + top) * 8 + 4;
			}
			if(selectedOption == SETTINGS_ROW_WINDOW_COLOR)
				y += 8;
			mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			mnWindowSpriteClearList(helpWindow);
			mnTextDrawMessage(selectedOption + (Sint32)MsgWindowColorHelp,
				(Sint32)helpWindow, 0, 0);
		}
		mnTitleBob(pair);
		sySynch(1);

		/* SELECT steps through the colour presets (window/scales.c). */
		if(PerWork.push & KEY_SELECT) {
			sySoundPlayStub(SE_MENU_CONFIRM);
			state->preset++;
			redraw = TRUE;
			if(state->preset > SETTINGS_PRESET_COUNT - 1)
				state->preset = 0;
			*windowColor = SettingsMenuHuePresets[state->preset];
			*brightness = SettingsMenuBrightnessPresets[state->preset];
			continue;
		}
		if(PerWork.push & (KEY_A | KEY_START)) {
			result = 0;
			sySoundPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.push & KEY_B) {
			result = -1;
			sySoundPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(PerWork.repeat & KEY_UP) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption--;
			redraw = TRUE;
		} else if(PerWork.repeat & KEY_DOWN) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectedOption++;
			redraw = TRUE;
		} else {
			if(PerWork.repeat & KEY_LEFT) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]--;
				redraw = TRUE;
			}
			if(PerWork.repeat & KEY_RIGHT) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				state->option[selectedOption]++;
				redraw = TRUE;
			}
		}
	}

	mnWindowClose(helpWindow, WINDOW_FLAG_INSTANT);
	mnWindowClose(settingsWindow, WINDOW_FLAG_INSTANT);
	if(result == 0) {
		/* "In Sleep Mode, press Select and the L and R Buttons to return to the
		   game." */
		if(MainRoutineStateWork.idleSleepEnabled == 0 &&
			state->option[SETTINGS_ROW_AUTO_SLEEP] == 1)
			mnFieldMessageShow((Sint32)MsgSleepModeHelp, 1);
	}
	/* Keep the edits, auto-sleep also taking effect at once (IdleSleepEnabled);
	   or put the saved window colours back. */
	if(result == 0) {
		MainRoutineStateWork.windowHue =
			state->option[SETTINGS_ROW_WINDOW_COLOR];
		MainRoutineStateWork.windowBrightness =
			state->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];
		MainRoutineStateWork.msgSpeed =
			state->option[SETTINGS_ROW_MESSAGE_SPEED];
		MainRoutineStateWork.speech = state->option[SETTINGS_ROW_SPEECH];
		MainRoutineStateWork.idleSleepEnabled =
			state->option[SETTINGS_ROW_AUTO_SLEEP];
		IdleSleepEnabled = MainRoutineStateWork.idleSleepEnabled;
	} else {
		mnWindowColorApply(MainRoutineStateWork.windowHue,
			MainRoutineStateWork.windowBrightness);
	}
	mnSettingsMenuRelease();
	sySynch(1);
	return result;
}

/* The Pause menu's per-tick event: moves the cursor and pulses the chosen
   entry's icon. */
void mnPauseMenuAnimateCursor(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor.cursor);
	mnWindowSpritePulse(state->icon[state->page]);
}

/* Allocates the Pause menu's state and starts its per-tick event. The Pause
   menu shares the settings menu's heap slot: mnPauseMenuChoose frees it before
   mnPauseMenuRun opens the settings menu. */
void mnPauseMenuInit(void)
{
	void *state = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));

	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnPauseMenuAnimateCursor, EVENT_PASS_TICK);
}

/* Stops the Pause menu's per-tick event and frees its state. */
void mnPauseMenuRelease(void)
{
	syCloseEvent(mnPauseMenuAnimateCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

extern Uint8	   MsgSaveYourQuest[];
extern Uint8	   MsgChangeSettings[];
extern Uint8	   MsgRoughMenu[];
extern const Sint8 PauseMenuIcons[];

#define PAUSE_MENU_ROWS		  3 /* Save your quest, Sleep and Change settings */
#define PAUSE_MENU_DEBUG_ROWS 3 /* the entries added in debug mode */
#define PAUSE_MENU_ROW_HEIGHT 24 /* pixels, 3 tiles */
#define PAUSE_MENU_TEXT_X	  48 /* pixels into the window, after the icons */

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
	WINDOW				*window;
	Sint32				 y;
	Sint32				 msg;
	Sint32				 i;

	rows = PAUSE_MENU_ROWS;
	state = RamHeapSlots->settingsMenu;
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
	window = mnWindowCreate(5, top, 20, height, WINDOW_FLAG_INSTANT);
	for(i = 1; i < rows; i++)
		mnWindowDrawDividerLine(window, 0, i * 3, 19, i * 3);
	y = 4;
	if(settingsOnly == 0) {
		msg = (Sint32)MsgSaveYourQuest;
		mnTextDrawMessage(msg, (Sint32)window, PAUSE_MENU_TEXT_X, 4);
		msg++;
		mnTextDrawMessage(
			msg, (Sint32)window, PAUSE_MENU_TEXT_X, 4 + PAUSE_MENU_ROW_HEIGHT);
		y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
	}
	mnTextDrawMessage(
		(Sint32)MsgChangeSettings, (Sint32)window, PAUSE_MENU_TEXT_X, y);
	y += PAUSE_MENU_ROW_HEIGHT;
	if(DebugMode != 0) {
		msg = (Sint32)MsgRoughMenu;
		mnTextDrawMessage(msg, (Sint32)window, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		mnTextDrawMessage(msg + 1, (Sint32)window, PAUSE_MENU_TEXT_X, y);
		y += PAUSE_MENU_ROW_HEIGHT;
		msg += 2;
		mnTextDrawMessage(msg, (Sint32)window, PAUSE_MENU_TEXT_X, y);
	}
	state->cursor.sprite =
		mnWindowSpriteCreate(mnPointingHandLoadIntoFreeSlot(),
			WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
	y = -4;
	for(i = 0; i < rows; i++) {
		state->icon[i] =
			mnCommandIconCreate(PauseMenuIcons[first + i], 0, window, 12, y);
		y += PAUSE_MENU_ROW_HEIGHT;
	}
	return window;
}

/* The heading's top, in pixels from the window's; in every edition but the
   Japanese it sits 8 pixels lower. */
#define HEADING_Y WHEN(PAUSE_HEADING_LOWER, (-16), (-24))

/* Runs the Pause menu until the player picks an entry (enum PAUSE_MENU_ENTRY)
   or cancels with B or START (-1). The result counts in the full entry table
   even when only Change settings is shown. The menu's window and state are
   freed before it returns; mnPauseMenuRun (screens_pause_menu.c) acts on the
   result. */
Sint32 mnPauseMenuChoose(Sint32 init)
{
	WINDOW				*window;
	Sint32				 redraw;
	WINDOW_SPR			*pair;
	Sint32				 first;
	Sint32				 i;
	Sint32				 j;
	Sint32				 rows;
	Sint32				 settingsOnly;
	Bool				 dimmed;
	Sint32				 y;
	Sint32				 result;
	Sint32				 selected;
	Sint32				 resourceIndex;
	Sint32				 x;
	Sint32				 top;
	const Sint8			*pauseIcons;
	WINDOW_SPR			*icon;
	SETTINGS_MENU_STATE *state;

	redraw = -1;
	selected = init;
	rows = PAUSE_MENU_ROWS;
	settingsOnly = syGameFlagGet(FLAG_IN_BATTLE_ARENA);
	first = 0;
	mnPauseMenuInit();
	state = RamHeapSlots->settingsMenu;
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
				pauseIcons = PauseMenuIcons;
				do {
					icon = state->icon[i];
					icon->depth = OAM_DEPTH_FRONT - 4;
					mnIconShowStub(icon);
					resourceIndex = (Uint8)icon->index;
					dimmed = FALSE;
					if(i != state->page)
						dimmed = TRUE;
					mnCommandIconLoad(*(Sint8 *)(j + (Sint32)pauseIcons),
						resourceIndex, dimmed);
					i++;
					j++;
				} while(i < rows);
			}
			top = window->y;
			x = window->x * 8;
			y = (Sint16)top;
			y = (selected * 3 + y) * 8 + 16;
			if(-1 == redraw)
				mnCursorSetPosImmediateStub(&state->cursor.cursor, x, y);
			else
				mnCursorSetStub(&state->cursor.cursor, x, y, 3);
			redraw = 0;
		}
		mnTitleBob(pair);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			result = selected;
			sySoundPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.push & (KEY_B | KEY_START)) {
			result = -1;
			sySoundPlayStub(SE_MENU_CANCEL);
			break;
		}
		if(PerWork.repeat & KEY_UP) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selected--;
			redraw = 1;
			continue;
		}
		if(PerWork.repeat & KEY_DOWN) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selected++;
			redraw = 1;
			continue;
		}
		if(DebugMode != 0) {
			if(PerWork.push & KEY_R) {
				result = PAUSE_MENU_DEBUG_R;
				sySoundPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if(PerWork.push & KEY_L) {
				result = PAUSE_MENU_DEBUG_L;
				sySoundPlayStub(SE_MENU_CONFIRM);
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
