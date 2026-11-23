/// @file
/// The window colours the player picks, the settings menu that picks them with
/// the message speed, speech and auto-sleep, and the Pause menu.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "window/settings_menu.h"
#include "window/screens.h"

/// The settings menu's rows, in the order of their help messages
/// (MsgWindowColorHelp on) and of OptionMenuWork.option.
enum SettingsMenuRow {
  SETTINGS_ROW_WINDOW_COLOR = 0,
  SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
  SETTINGS_ROW_MESSAGE_SPEED = 2,
  SETTINGS_ROW_SPEECH = 3,
  SETTINGS_ROW_AUTO_SLEEP = 4,
  SETTINGS_ROW_COUNT = 5
};

/// Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
/// 31, as a BGR555 colour. rgb holds three components, not an already packed
/// palette word.
u16 Color_ScaleComponents(s16 *rgb, FIXED redScale, FIXED greenScale,
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

extern const u8 gWindowColor_WaveTable[];

/// The window colours the window colour settings set: WINDOW_PALETTE's colours
/// from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6.
#define WINDOW_SHADE(n)                                                        \
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))
#define WINDOW_SHADE_SET(n, color)                                             \
  do {                                                                         \
    u32 color_ = (color);                                                      \
    WINDOW_SHADE(n) = color_;                                                  \
  } while (0)

/// Sets the window colours (WINDOW_SHADE) for a window colour (0 to
/// WINDOW_COLOR_COUNT - 1, a hue around the colour wheel) and brightness (0 to
/// WINDOW_BRIGHTNESS_COUNT - 1, WINDOW_BRIGHTNESS_NEUTRAL the plain one).
void WindowColor_Apply(s32 hue, s32 brightness) {
  s16 rgb[3];
  s16 step;
  s16 red;
  s16 green;
  s16 blue;
  s16 offset;

  step = ((hue + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
  offset = brightness - WINDOW_BRIGHTNESS_NEUTRAL;
  red = gWindowColor_WaveTable[(step % WINDOW_COLOR_WHEEL_STEPS)] + offset;
  green = gWindowColor_WaveTable[(step + WINDOW_COLOR_WHEEL_STEPS / 3) %
                                 WINDOW_COLOR_WHEEL_STEPS] +
          offset;
  blue = gWindowColor_WaveTable[(step + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
                                WINDOW_COLOR_WHEEL_STEPS] +
         offset;
  CLAMP(red, 0, RGB_LEVEL_MAX);
  CLAMP(green, 0, RGB_LEVEL_MAX);
  CLAMP(blue, 0, RGB_LEVEL_MAX);
  rgb[0] = red;
  rgb[1] = green;
  rgb[2] = blue;
  // Each shade scales the tint by channel, in 16.16 (FIXED_ONE is 1): shades
  // 0 to 3 grow darker, from about 0.93, 0.8 and 1.07 to 0.63, 0.6 and 0.67,
  // and 4 to 6 lighter, up to about 1.23, 1 and 1.47.
  WINDOW_SHADE_SET(0,
                   Color_ScaleComponents(rgb, FIXED_ONE * 14 / 15,
                                         FIXED_ONE * 4 / 5, toFIXED(1.06666)));
  WINDOW_SHADE_SET(1, Color_ScaleComponents(rgb, FIXED_ONE * 5 / 6,
                                            FIXED_ONE * 11 / 15,
                                            FIXED_ONE * 14 / 15));
  WINDOW_SHADE_SET(2,
                   Color_ScaleComponents(rgb, FIXED_ONE * 11 / 15,
                                         FIXED_ONE * 2 / 3, FIXED_ONE * 4 / 5));
  WINDOW_SHADE_SET(3,
                   Color_ScaleComponents(rgb, toFIXED(0.63333),
                                         FIXED_ONE * 3 / 5, FIXED_ONE * 2 / 3));
  WINDOW_SHADE_SET(4, Color_ScaleComponents(rgb, FIXED_ONE * 31 / 30,
                                            FIXED_ONE * 13 / 15,
                                            FIXED_ONE * 6 / 5));
  WINDOW_SHADE_SET(5, Color_ScaleComponents(rgb, toFIXED(1.13333),
                                            FIXED_ONE * 14 / 15,
                                            FIXED_ONE * 4 / 3));
  WINDOW_SHADE_SET(6, Color_ScaleComponents(rgb, FIXED_ONE * 37 / 30, FIXED_ONE,
                                            FIXED_ONE * 22 / 15));
}

/// The settings menu's per-frame task: moves the cursor and the two slider
/// markers, animates the marker of the slider in use, and pulses the chosen
/// setting's icon.
void SettingsMenu_AnimateCursors(void) {
  s32 firstMarkerPicture;
  s32 secondMarkerPicture;
  s32 selectedOption;
  struct OptionMenuWork *work;

  work = Ram_HeapSlots->settingsMenu;
  selectedOption = work->page;
  MenuCursor_AdvanceFar(&work->cursor.cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[0].cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[1].cursor);

  if (selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
    firstMarkerPicture =
        (work->cursorFrame & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
  } else {
    firstMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
  }
  VramCache_Load((u8)work->marker[0].sprite->index, 256,
                 Resource_GetTableEntry(firstMarkerPicture));

  if (selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
    secondMarkerPicture =
        (work->cursorFrame & 7) + RESOURCE_SHOP_CURSOR_PICTURES;
  } else {
    secondMarkerPicture = RESOURCE_SHOP_CURSOR_PICTURES;
  }
  VramCache_Load((u8)work->marker[1].sprite->index, 256,
                 Resource_GetTableEntry(secondMarkerPicture));

  if (selectedOption > SETTINGS_ROW_WINDOW_BRIGHTNESS)
    WindowSprite_Pulse(
        work->choiceIcons[selectedOption - SETTINGS_ROW_MESSAGE_SPEED]
                         [work->option[selectedOption]]);
  work->cursorFrame++;
}

/// Allocates the settings menu's work block, copies the settings into it with
/// how many values each takes, and starts its per-frame task.
void SettingsMenu_InitializeWork(void) {
  struct OptionMenuWork *work;
  volatile u32 zero;

  work = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                             sizeof(struct OptionMenuWork));
  zero = 0;
  Dma_Set((const void *)&zero, work,
          DMA_FILL_WORDS(sizeof(struct OptionMenuWork) / 4), REG_DMA3);
  work->option[SETTINGS_ROW_WINDOW_COLOR] = gMainRoutineWork.windowHue;
  work->optionCount[SETTINGS_ROW_WINDOW_COLOR] = WINDOW_COLOR_COUNT;
  work->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] =
      gMainRoutineWork.windowBrightness;
  work->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS] = WINDOW_BRIGHTNESS_COUNT;
  work->option[SETTINGS_ROW_MESSAGE_SPEED] = gMainRoutineWork.messageSpeed;
  work->optionCount[SETTINGS_ROW_MESSAGE_SPEED] = MESSAGE_SPEED_FAST + 1;
  work->option[SETTINGS_ROW_SPEECH] = gMainRoutineWork.speech;
  work->optionCount[SETTINGS_ROW_SPEECH] = 2;
  work->option[SETTINGS_ROW_AUTO_SLEEP] = gMainRoutineWork.idleSleepEnabled;
  work->optionCount[SETTINGS_ROW_AUTO_SLEEP] = 2;
  Task_Add(SettingsMenu_AnimateCursors, TASK_PASS_FRAME);
}

/// Stops the settings menu's per-frame task and frees its work block.
void SettingsMenu_Release(void) {
  Task_Remove(SettingsMenu_AnimateCursors);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

/// The sprite palettes of the hue slider's ends and the brightness slider's.
#define SLIDER_PALETTE 14
#define BRIGHTNESS_SLIDER_PALETTE (SLIDER_PALETTE + 1)

/// The settings window's layout, sized to its labels: the window, the last
/// column of its dividers, the hue and brightness sliders and the message
/// speed icons. Japanese writes "Message speed" on one line.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define OPTION_WINDOW_X 2
#define OPTION_WINDOW_WIDTH 26
#define OPTION_DIVIDER_END 25
#define OPTION_SLIDER_X 100
#define OPTION_CHOICE_X 92
#else
#define OPTION_WINDOW_X 1
#if (LANGUAGE == LANGUAGE_ENGLISH)
#define OPTION_WINDOW_WIDTH 27
#else
#define OPTION_WINDOW_WIDTH 28
#endif
#define OPTION_DIVIDER_END 26
#define OPTION_SLIDER_X 140
#define OPTION_CHOICE_X 84
#endif

/// The pixels a settings slider's marker runs across, from OPTION_SLIDER_X.
#define OPTION_SLIDER_WIDTH 60

extern const u8 gPointingHandTiles[];
/// The hue and brightness sliders' picture and colours.
extern const u8 gSettingsMenu_SliderTiles[];
extern const u16 gSettingsMenu_SliderPalette[];

extern s8 gSettingsMenu_SpeedIcons[];
extern s8 gSettingsMenu_SpeechIcons[];
extern s8 gSettingsMenu_SleepIcons[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];

struct WindowSprite *CommandIcon_Create(s32 icon, s32 remap,
                                        struct Window *window, s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

/// Opens the settings menu's window: the labels, the hue and brightness
/// sliders with their markers, the cursor and the setting icons. Returns the
/// window. Its sprites belong to the window and go when it closes. A
/// slider's marker stands option * OPTION_SLIDER_WIDTH / count pixels right of
/// OPTION_SLIDER_X.
struct Window *SettingsMenu_Open(void) {
  struct OptionMenuWork *work;
  struct Window *win;
  struct WindowSprite *out;
  s32 x;
  s32 y;
  s32 msg;

  work = Ram_HeapSlots->settingsMenu;
  win = Window_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14,
                      WINDOW_FLAG_INSTANT);
  Window_DrawDividerLine(win, 0, 2, OPTION_DIVIDER_END, 2);
  Window_DrawDividerLine(win, 0, 4, OPTION_DIVIDER_END, 4);
  Window_DrawDividerLine(win, 0, 7, OPTION_DIVIDER_END, 7);
  Window_DrawDividerLine(win, 0, 10, OPTION_DIVIDER_END, 10);
  msg = (s32)&MsgWindowColorLabel;
  Text_DrawMessageAsTiles(msg, win, 8, 0);
  msg++;
  Text_DrawMessageAsTiles(msg, win, 8, 16);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  msg = (s32)&MsgMessageSpeedLabel;
  Text_DrawMessageAsTiles(msg, win, 8, 32);
  msg++;
  Text_DrawMessageAsTiles(msg, win, 32, 40);
#else
  Text_DrawMessageAsTiles((s32)&MsgMessageSpeedLabel, win, 8, 40);
#endif
  Text_DrawMessageAsTiles((s32)MsgSpeechLabel, win, 8, 64);
  Text_DrawMessageAsTiles((s32)MsgAutoSleepLabel, win, 8, 88);

  out = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                            OUTPUT_SIZE_16X16, win, 0, 0);
  work->cursor.sprite = out;
  x = (s16)win->x * 8;
  y = (s16)win->y * 8 + 12;
  MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);

  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    Dma_Set(gSettingsMenu_SliderPalette,
            (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), DMA_COPY_HALFWORDS(32),
            REG_DMA3);
    VramCache_Load(x, 256, gSettingsMenu_SliderTiles);
    y = 0;
    out = WindowSprite_Create(x, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X - 6, y);
    out->sprite.oam.palette = SLIDER_PALETTE;
    out =
        WindowSprite_Create(x, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X + 26, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = SLIDER_PALETTE;
    y = 16;
    out = WindowSprite_Create(x, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X - 6, y);
    out->sprite.oam.palette = BRIGHTNESS_SLIDER_PALETTE;
    out =
        WindowSprite_Create(x, OUTPUT_SIZE_32X8, win, OPTION_SLIDER_X + 26, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = BRIGHTNESS_SLIDER_PALETTE;
  }

  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, OUTPUT_SIZE_16X16, win, 0, 0);
    out->sprite.oam.fullColor = 1;
    work->marker[0].sprite = out;
    x = (s16)win->x * 8 + OPTION_SLIDER_X;
    x += work->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
         work->optionCount[SETTINGS_ROW_WINDOW_COLOR];
    y = (s16)win->y * 8 + 4;
    MenuCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
  }

  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, OUTPUT_SIZE_16X16, win, 0, 0);
    out->sprite.oam.fullColor = 1;
    work->marker[1].sprite = out;
    x = (s16)win->x * 8 + OPTION_SLIDER_X;
    x += work->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] * OPTION_SLIDER_WIDTH /
         work->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
    y = (s16)win->y * 8 + 20;
    MenuCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
  }

  y = 28;
  work->choiceIcons[0][0] = CommandIcon_Create(gSettingsMenu_SpeedIcons[0], 0,
                                               win, OPTION_CHOICE_X, y);
  work->choiceIcons[0][1] = CommandIcon_Create(gSettingsMenu_SpeedIcons[1], 0,
                                               win, OPTION_CHOICE_X + 24, y);
  work->choiceIcons[0][2] = CommandIcon_Create(gSettingsMenu_SpeedIcons[2], 0,
                                               win, OPTION_CHOICE_X + 48, y);
  y = 52;
  work->choiceIcons[1][0] =
      CommandIcon_Create(gSettingsMenu_SpeechIcons[0], 0, win, 100, y);
  work->choiceIcons[1][1] =
      CommandIcon_Create(gSettingsMenu_SpeechIcons[1], 0, win, 124, y);
  y = 76;
  work->choiceIcons[2][0] =
      CommandIcon_Create(gSettingsMenu_SleepIcons[0], 0, win, 100, y);
  work->choiceIcons[2][1] =
      CommandIcon_Create(gSettingsMenu_SleepIcons[1], 0, win, 124, y);
  return win;
}

/// The layout that fits each language's text: the help window's column and
/// width in tiles, then, in pixels from the settings window's left, where the
/// choices' fields start and end.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SETTINGS_HELP_WINDOW_X 2
#define SETTINGS_HELP_WINDOW_WIDTH 26
#define CHOICE_X 168
#define CHOICE_RIGHT 192
#define FIRST_CHOICE_RIGHT 192
#else
#define SETTINGS_HELP_WINDOW_X 1
#define SETTINGS_HELP_WINDOW_WIDTH 28
#define CHOICE_X 160
#define CHOICE_RIGHT 184
// The first choice's field is wider in German, French and Italian.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define FIRST_CHOICE_RIGHT 208
#else
#define FIRST_CHOICE_RIGHT 200
#endif
#endif

/// The heading above the settings window: Japanese shows the Settings heading;
/// from patch 1 the screen shows the Pause heading the Pause menu also shows.
#if TLA_PATCH_1
#define SETTINGS_HEADING MENU_TITLE_PAUSE
#else
#define SETTINGS_HEADING MENU_TITLE_SETTINGS
#endif

extern u8 MsgMessageSpeedSlow[], MsgSpeechOff[], MsgAutoSleepOff[],
    MsgWindowColorHelp[], MsgSleepModeHelp[];
extern const u8 gSettingsMenu_HuePresets[], gSettingsMenu_BrightnessPresets[];

/// The colour presets SELECT steps through (gSettingsMenu_HuePresets).
#define SETTINGS_PRESET_COUNT 6

void Icon_PrepareActorFar(struct WindowSprite *icon);
void CommandIcon_Load(s32 index, s32 value, s32 flag);
void FieldMessage_Show(s32 message, s32 mode);

/// Runs the settings menu (window colour and brightness, message speed, speech,
/// auto-sleep) until A or START keeps the settings (returns 0) or B drops them
/// (returns -1).
s32 SettingsMenu_Run(void) {
  struct OptionMenuWork *work;
  struct Window *settingsWindow;
  struct Window *helpWindow;
  struct WindowSprite *pair;
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
  struct WindowSprite *icon;

  redraw = 1;
  selectedOption = 0;
  SettingsMenu_InitializeWork();
  work = Ram_HeapSlots->settingsMenu;
  helpWindow =
      Window_Create(SETTINGS_HELP_WINDOW_X, 2, SETTINGS_HELP_WINDOW_WIDTH, 3,
                    WINDOW_FLAG_INSTANT);
  settingsWindow = SettingsMenu_Open();
  pair = MenuTitle_Create(SETTINGS_HEADING, settingsWindow, 64, -48);
  wait(1);
  windowColor = &work->option[SETTINGS_ROW_WINDOW_COLOR];
  brightness = &work->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      selectedOption =
          (selectedOption + SETTINGS_ROW_COUNT) % SETTINGS_ROW_COUNT;
      work->option[selectedOption] =
          (work->option[selectedOption] + work->optionCount[selectedOption]) %
          work->optionCount[selectedOption];
      work->page = selectedOption;
      if (gSleepDisabled != 0)
        work->option[SETTINGS_ROW_AUTO_SLEEP] = 0;

      // Redraw each choice's icon at OAM_DEPTH_FRONT - 4, dimmed unless it is
      // the row's setting.

      for (i = 0; i <= 2; i++) {
        icon = work->choiceIcons[0][i];
        icon->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[0][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_MESSAGE_SPEED])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SpeedIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[1][i];
        icon->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[1][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_SPEECH])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SpeechIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[2][i];
        icon->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[2][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_AUTO_SLEEP])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SleepIcons[i], resourceIndex, dimmed);
      }

      x = (s16)settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*windowColor * OPTION_SLIDER_WIDTH) /
           work->optionCount[SETTINGS_ROW_WINDOW_COLOR];
      y = (s16)settingsWindow->y * 8 + 4;
      MenuCursor_SetFar(&work->marker[0].cursor, x, y, 1);
      x = (s16)settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*brightness * OPTION_SLIDER_WIDTH) /
           work->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
      y = (s16)settingsWindow->y * 8 + 20;
      MenuCursor_SetFar(&work->marker[1].cursor, x, y, 1);

      x = work->option[SETTINGS_ROW_MESSAGE_SPEED] + (s32)MsgMessageSpeedSlow;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 40,
                                FIRST_CHOICE_RIGHT, 48);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 40);
      x = work->option[SETTINGS_ROW_SPEECH] + (s32)MsgSpeechOff;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 64, CHOICE_RIGHT, 72);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 64);
      x = work->option[SETTINGS_ROW_AUTO_SLEEP] + (s32)MsgAutoSleepOff;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 88, CHOICE_RIGHT, 96);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 88);
      // The windows show the edited colours at once.
      WindowColor_Apply(*windowColor, *brightness);

      // The cursor goes 3 tiles a row down the window's left edge, the first
      // row's a tile lower; the help window says what the row sets.
      x = (s16)settingsWindow->x * 8;
      {
        s32 top = (s16)settingsWindow->y;

        y = (selectedOption * 3 + top) * 8 + 4;
      }
      if (selectedOption == SETTINGS_ROW_WINDOW_COLOR)
        y += 8;
      MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
      WindowSprite_ClearList(helpWindow);
      Text_DrawMessage(selectedOption + (s32)MsgWindowColorHelp,
                       (s32)helpWindow, 0, 0);
    }
    MenuTitle_Bob(pair);
    wait(1);

    // SELECT steps through the colour presets (window/scales.c).
    if (gInput.pressed & KEY_SELECT) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      work->preset++;
      redraw = 1;
      if (work->preset > SETTINGS_PRESET_COUNT - 1)
        work->preset = 0;
      *windowColor = gSettingsMenu_HuePresets[work->preset];
      *brightness = gSettingsMenu_BrightnessPresets[work->preset];
      continue;
    }
    if (gInput.pressed & (KEY_A | KEY_START)) {
      result = 0;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (gInput.pressed & KEY_B) {
      result = -1;
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if (gInput.repeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedOption--;
      redraw = 1;
    } else if (gInput.repeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedOption++;
      redraw = 1;
    } else {
      if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        work->option[selectedOption]--;
        redraw = 1;
      }
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        work->option[selectedOption]++;
        redraw = 1;
      }
    }
  }

  Window_Close(helpWindow, WINDOW_FLAG_INSTANT);
  Window_Close(settingsWindow, WINDOW_FLAG_INSTANT);
  if (result == 0) {
    // "In Sleep Mode, press Select and the L and R Buttons to return to the
    // game."
    if (gMainRoutineWork.idleSleepEnabled == 0 &&
        work->option[SETTINGS_ROW_AUTO_SLEEP] == 1)
      FieldMessage_Show((s32)MsgSleepModeHelp, 1);
  }
  // Keep the edits, auto-sleep also taking effect at once
  // (gIdleSleepEnabled); or put the saved window colours back.
  if (result == 0) {
    gMainRoutineWork.windowHue = work->option[SETTINGS_ROW_WINDOW_COLOR];
    gMainRoutineWork.windowBrightness =
        work->option[SETTINGS_ROW_WINDOW_BRIGHTNESS];
    gMainRoutineWork.messageSpeed = work->option[SETTINGS_ROW_MESSAGE_SPEED];
    gMainRoutineWork.speech = work->option[SETTINGS_ROW_SPEECH];
    gMainRoutineWork.idleSleepEnabled = work->option[SETTINGS_ROW_AUTO_SLEEP];
    gIdleSleepEnabled = gMainRoutineWork.idleSleepEnabled;
  } else {
    WindowColor_Apply(gMainRoutineWork.windowHue,
                      gMainRoutineWork.windowBrightness);
  }
  SettingsMenu_Release();
  wait(1);
  return result;
}

/// The Pause menu's per-frame task: moves the cursor and pulses the
/// chosen entry's icon.
void PauseMenu_AnimateCursor(void) {
  struct OptionMenuWork *work = Ram_HeapSlots->settingsMenu;

  MenuCursor_AdvanceFar(&work->cursor.cursor);
  WindowSprite_Pulse(work->icon[work->page]);
}

/// Allocates the Pause menu's work block and starts its per-frame task.
/// The Pause menu shares the settings menu's heap slot: PauseMenu_Choose frees
/// it before PauseMenu_Run opens the settings menu.
void PauseMenu_Initialize(void) {
  void *work = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                                   sizeof(struct OptionMenuWork));

  Dma_Fill32(0, work, sizeof(struct OptionMenuWork));
  Task_Add(PauseMenu_AnimateCursor, TASK_PASS_FRAME);
}

/// Stops the Pause menu's per-frame task and frees its work block.
void PauseMenu_Release(void) {
  Task_Remove(PauseMenu_AnimateCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

extern u8 MsgSaveYourQuest[];
extern u8 MsgChangeSettings[];
extern u8 MsgRoughMenu[];
extern const s8 gMenu_WorkspaceIcons[];

#define PAUSE_MENU_ROWS 3        ///< Save your quest, Sleep and Change settings
#define PAUSE_MENU_DEBUG_ROWS 3  ///< the entries added in debug mode
#define PAUSE_MENU_ROW_HEIGHT 24 ///< pixels, 3 tiles
#define PAUSE_MENU_TEXT_X 48     ///< pixels into the window, after the icons

/// Opens the Pause menu's window with its entries (Save your quest, Sleep,
/// Change settings, and the debug entries in debug mode; only Change settings
/// while FLAG_IN_BATTLE_ARENA is set), their icons and the cursor.
struct Window *PauseMenu_Open(void) {
  struct OptionMenuWork *work;
  s32 rows;
  s32 first;
  s32 settingsOnly;
  s32 top;
  s32 height;
  struct Window *win;
  s32 y;
  s32 msg;
  s32 i;

  rows = PAUSE_MENU_ROWS;
  work = Ram_HeapSlots->settingsMenu;
  settingsOnly = GameFlag_Get(FLAG_IN_BATTLE_ARENA);
  first = 0;
  if (settingsOnly != 0) {
    rows = 1;
    first = PAUSE_MENU_CHANGE_SETTINGS;
  }
  if (gDebugMode != 0)
    rows += PAUSE_MENU_DEBUG_ROWS;
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > (DISPLAY_TILE_HEIGHT - 1)) {
    top = 1;
    height = DISPLAY_TILE_HEIGHT - 1;
  }
  win = Window_Create(5, top, 20, height, WINDOW_FLAG_INSTANT);
  for (i = 1; i < rows; i++)
    Window_DrawDividerLine(win, 0, i * 3, 19, i * 3);
  y = 4;
  if (settingsOnly == 0) {
    msg = (s32)MsgSaveYourQuest;
    Text_DrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, 4);
    msg++;
    Text_DrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X,
                     4 + PAUSE_MENU_ROW_HEIGHT);
    y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
  }
  Text_DrawMessage((s32)MsgChangeSettings, (s32)win, PAUSE_MENU_TEXT_X, y);
  y += PAUSE_MENU_ROW_HEIGHT;
  if (gDebugMode != 0) {
    msg = (s32)MsgRoughMenu;
    Text_DrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, y);
    y += PAUSE_MENU_ROW_HEIGHT;
    Text_DrawMessage(msg + 1, (s32)win, PAUSE_MENU_TEXT_X, y);
    y += PAUSE_MENU_ROW_HEIGHT;
    msg += 2;
    Text_DrawMessage(msg, (s32)win, PAUSE_MENU_TEXT_X, y);
  }
  work->cursor.sprite = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16, win, 0, 0);
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icon[i] =
        CommandIcon_Create(gMenu_WorkspaceIcons[first + i], 0, win, 12, y);
    y += PAUSE_MENU_ROW_HEIGHT;
  }
  return win;
}

/// The heading's top, in pixels from the window's; from patch 1 it sits 8
/// pixels lower.
#if TLA_PATCH_1
#define HEADING_Y (-16)
#else
#define HEADING_Y (-24)
#endif

/// Runs the Pause menu until the player picks an entry (enum
/// PauseMenuEntry) or cancels with B or START (-1).
/// The result counts in the full entry table even when only Change settings is
/// shown. The menu's window and work block are freed before it returns;
/// PauseMenu_Run (screens_pause_menu.c) acts on the result.
s32 PauseMenu_Choose(s32 initial) {
  struct Window *window;
  s32 redraw;
  struct WindowSprite *pair;
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
  struct WindowSprite *icon;
  struct OptionMenuWork *work;

  redraw = -1;
  selected = initial;
  rows = PAUSE_MENU_ROWS;
  settingsOnly = GameFlag_Get(FLAG_IN_BATTLE_ARENA);
  first = 0;
  PauseMenu_Initialize();
  work = Ram_HeapSlots->settingsMenu;
  wait(1);
  window = PauseMenu_Open();
  pair = MenuTitle_Create(MENU_TITLE_PAUSE, window, 40, HEADING_Y);
  if (settingsOnly != 0) {
    rows = 1;
    first = PAUSE_MENU_CHANGE_SETTINGS;
  }
  if (gDebugMode != 0)
    rows += PAUSE_MENU_DEBUG_ROWS;
  for (;;) {
    if (redraw != 0) {
      selected = (selected + rows) % rows;
      work->page = selected;
      i = 0;
      if (i < rows) {
        j = first;
        pauseIcons = gMenu_WorkspaceIcons;
        do {
          icon = work->icon[i];
          icon->depth = OAM_DEPTH_FRONT - 4;
          Icon_PrepareActorFar(icon);
          resourceIndex = (u8)icon->index;
          dimmed = 0;
          if (i != work->page)
            dimmed = 1;
          CommandIcon_Load(*(s8 *)(j + (s32)pauseIcons), resourceIndex, dimmed);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = (s16)window->x * 8;
      y = (s16)top;
      y = (selected * 3 + y) * 8 + 16;
      if (-1 == redraw)
        MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
      else
        MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
      redraw = 0;
    }
    MenuTitle_Bob(pair);
    wait(1);
    if (gInput.pressed & KEY_A) {
      result = selected;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (gInput.pressed & (KEY_B | KEY_START)) {
      result = -1;
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if (gInput.repeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selected--;
      redraw = 1;
      continue;
    }
    if (gInput.repeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selected++;
      redraw = 1;
      continue;
    }
    if (gDebugMode != 0) {
      if (gInput.pressed & KEY_R) {
        result = PAUSE_MENU_DEBUG_R;
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
      if (gInput.pressed & KEY_L) {
        result = PAUSE_MENU_DEBUG_L;
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
  }
  Window_Close(window, WINDOW_FLAG_INSTANT);
  PauseMenu_Release();
  wait(1);
  if (result >= 0)
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
