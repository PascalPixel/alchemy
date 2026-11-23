/// @file
/// Window colours, the settings menu and the Pause menu.
// Part of window.c.

#include "window_internal.h"

/// The pixels a settings slider's marker runs across, from OPTION_SLIDER_X.
#define OPTION_SLIDER_WIDTH 60

/// The settings menu's rows, in the order of their help messages
/// (MsgOptionHelp on) and of OptionMenuWork.option.
enum SettingsMenuRow {
  SETTINGS_ROW_WINDOW_COLOR = 0, ///< "Window color"
  SETTINGS_ROW_WINDOW_BRIGHTNESS = 1,
  SETTINGS_ROW_MESSAGE_SPEED = 2,
  SETTINGS_ROW_SPEECH = 3, ///< "Turn voice sounds on or off."
  SETTINGS_ROW_AUTO_SLEEP = 4,
  SETTINGS_ROW_COUNT = 5
};

/// The window colours the window colour settings set: WINDOW_PALETTE's colours
/// from its background (shade 0, WINDOW_BACKGROUND_COLOR) to shade 6.
#define WINDOW_SHADE(n)                                                        \
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + (n))

/// Unused: three that do nothing.
UNUSED void SettingsMenu_ReservedNoOpA(void) {}
UNUSED void SettingsMenu_ReservedNoOpB(void) {}
UNUSED void SettingsMenu_ReservedNoOpC(void) {}

/// One of the seven entries of the settings menu's work
/// (OptionMenuWork.entries); only its state is known, and only the unused
/// SettingsMenu_ResetEntries writes it.
struct OptionMenuEntry {
  u8 unused[10];
  u16 state; ///< cleared to 0 by SettingsMenu_ResetEntries; nothing reads it
  u8 unusedAfterState[40];
};

/// Clears the settings menu's seven entry states, its row, its tint and its
/// cursor animation.
UNUSED void SettingsMenu_ResetEntries(struct OptionMenuWork *work) {
  struct OptionMenuEntry *entry = (struct OptionMenuEntry *)work->entries;

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
  work->page = 0;
  work->tint[0] = 0;
  work->tint[1] = 0;
  work->tint[2] = 0;
  work->cursorFrame = 0;
}

/// Works out the window tint (red, green and blue, 0 to 31) from the window
/// colour settings: the window color picks a point on the colour wheel and the
/// brightness shifts all three channels.
UNUSED void WindowColor_CalculateTint(struct OptionMenuWork *work) {
  s16 phase;
  s32 bias;
  s32 blue;
  s32 red;
  s32 green;

  phase = (gMainRoutineWork.windowHue + WINDOW_COLOR_COUNT / 2) %
          WINDOW_COLOR_COUNT * 4;
  bias = gMainRoutineWork.windowBrightness - WINDOW_BRIGHTNESS_NEUTRAL;
  red = gWindowColor_WaveTable[(phase % WINDOW_COLOR_WHEEL_STEPS)];
  green = gWindowColor_WaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS / 3) %
                                 WINDOW_COLOR_WHEEL_STEPS];
  blue = gWindowColor_WaveTable[(phase + WINDOW_COLOR_WHEEL_STEPS * 2 / 3) %
                                WINDOW_COLOR_WHEEL_STEPS];
  red += bias;
  green += bias;
  blue += bias;
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  if (red > RGB_LEVEL_MAX)
    red = RGB_LEVEL_MAX;
  if (green > RGB_LEVEL_MAX)
    green = RGB_LEVEL_MAX;
  if (blue > RGB_LEVEL_MAX)
    blue = RGB_LEVEL_MAX;
  work->tint[0] = red;
  work->tint[1] = green;
  work->tint[2] = blue;
}

/// Writes the window colours (shades 0 to 6 of WINDOW_SHADE), each a
/// fixed scale of the window tint.
UNUSED void WindowColor_WritePalette(struct OptionMenuWork *work) {
  *(s16 *)&WINDOW_SHADE(0) = WindowColor_ScaleTint(
      work, FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 5, toFIXED(1.06666));
  *(s16 *)&WINDOW_SHADE(1) = WindowColor_ScaleTint(
      work, FIXED_ONE * 5 / 6, FIXED_ONE * 11 / 15, FIXED_ONE * 14 / 15);
  *(s16 *)&WINDOW_SHADE(2) = WindowColor_ScaleTint(
      work, FIXED_ONE * 11 / 15, FIXED_ONE * 2 / 3, FIXED_ONE * 4 / 5);
  *(s16 *)&WINDOW_SHADE(3) = WindowColor_ScaleTint(
      work, toFIXED(0.63333), FIXED_ONE * 3 / 5, FIXED_ONE * 2 / 3);
  *(s16 *)&WINDOW_SHADE(4) = WindowColor_ScaleTint(
      work, FIXED_ONE * 31 / 30, FIXED_ONE * 13 / 15, FIXED_ONE * 6 / 5);
  *(s16 *)&WINDOW_SHADE(5) = WindowColor_ScaleTint(
      work, toFIXED(1.13333), FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 3);
  *(s16 *)&WINDOW_SHADE(6) = WindowColor_ScaleTint(
      work, FIXED_ONE * 37 / 30, FIXED_ONE, FIXED_ONE * 22 / 15);
}

/// Returns the window tint with each channel scaled (16.16 fixed point) and
/// clamped to 0 to 31, as a BGR555 colour.
u16 WindowColor_ScaleTint(struct OptionMenuWork *work, FIXED redScale,
                          FIXED greenScale, FIXED blueScale) {
  s32 red;
  s32 green;
  s32 blue;

  red = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(work->tint[0]), redScale));
  green = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(work->tint[1]), greenScale));
  blue = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(work->tint[2]), blueScale));
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  if (red > RGB_LEVEL_MAX)
    red = RGB_LEVEL_MAX;
  if (green > RGB_LEVEL_MAX)
    green = RGB_LEVEL_MAX;
  if (blue > RGB_LEVEL_MAX)
    blue = RGB_LEVEL_MAX;
  return red + ((blue << 10) + (green << 5));
}

/// Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
/// 31, as a BGR555 colour.
/// rgb holds three components, not an already packed palette word.
u16 Color_ScaleComponents(s16 *rgb, FIXED redScale, FIXED greenScale,
                          FIXED blueScale) {
  s32 red;
  s32 green;
  s32 blue;

  red = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(rgb[0]), redScale));
  green = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(rgb[1]), greenScale));
  blue = FIXEDtoINT(Iwram_MulFX(INTtoFIXED(rgb[2]), blueScale));
  CLAMP(red, 0, RGB_LEVEL_MAX);
  CLAMP(green, 0, RGB_LEVEL_MAX);
  CLAMP(blue, 0, RGB_LEVEL_MAX);
  return red + ((blue << 10) + (green << 5));
}

/// Sets the window colours (WINDOW_SHADE) for a window color (0 to 23) and
/// brightness (0 to 14, 7 neutral).
void WindowColor_Apply(s32 phase, s32 brightness) {
  s16 rgb[3];
  s16 step;
  s16 red;
  s16 green;
  s16 blue;
  s16 offset;

  // The 24 window colors are every fourth step of the 96-step wheel, color 0 at
  // step 48.
  step = ((phase + WINDOW_COLOR_COUNT / 2) % WINDOW_COLOR_COUNT) * 4;
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
  WINDOW_SHADE(0) = Color_ScaleComponents(rgb, FIXED_ONE * 14 / 15,
                                          FIXED_ONE * 4 / 5, toFIXED(1.06666));
  WINDOW_SHADE(1) = Color_ScaleComponents(
      rgb, FIXED_ONE * 5 / 6, FIXED_ONE * 11 / 15, FIXED_ONE * 14 / 15);
  WINDOW_SHADE(2) = Color_ScaleComponents(rgb, FIXED_ONE * 11 / 15,
                                          FIXED_ONE * 2 / 3, FIXED_ONE * 4 / 5);
  WINDOW_SHADE(3) = Color_ScaleComponents(rgb, toFIXED(0.63333),
                                          FIXED_ONE * 3 / 5, FIXED_ONE * 2 / 3);
  WINDOW_SHADE(4) = Color_ScaleComponents(
      rgb, FIXED_ONE * 31 / 30, FIXED_ONE * 13 / 15, FIXED_ONE * 6 / 5);
  WINDOW_SHADE(5) = Color_ScaleComponents(
      rgb, toFIXED(1.13333), FIXED_ONE * 14 / 15, FIXED_ONE * 4 / 3);
  WINDOW_SHADE(6) = Color_ScaleComponents(rgb, FIXED_ONE * 37 / 30, FIXED_ONE,
                                          FIXED_ONE * 22 / 15);
}

/// The rows of the unused three-row settings cursor (OptionMenuWork.page).
enum OptionMenuRow {
  OPTION_ROW_MESSAGE_SPEED = 0,
  OPTION_ROW_WINDOW_COLOR = 1,
  OPTION_ROW_WINDOW_BRIGHTNESS = 2
};

// The five-row settings menu SettingsMenu_Run runs uses none of these four
// functions.
/// Moves the three-row settings cursor up, wrapping to the bottom. The three
/// rows are those of SettingsMenu_LowerSetting.
UNUSED void SettingsMenu_PreviousRow(struct OptionMenuWork *work) {
  if (work->page == OPTION_ROW_MESSAGE_SPEED)
    work->page = OPTION_ROW_WINDOW_BRIGHTNESS;
  else
    work->page--;
}

/// Moves the three-row settings cursor down, wrapping to the top.
UNUSED void SettingsMenu_NextRow(struct OptionMenuWork *work) {
  if (++work->page > OPTION_ROW_WINDOW_BRIGHTNESS)
    work->page = OPTION_ROW_MESSAGE_SPEED;
}

/// Lowers the setting on the three-row cursor's row: message speed, window
/// color or window brightness.
UNUSED void SettingsMenu_LowerSetting(struct OptionMenuWork *work) {
  switch (work->page) {
  case OPTION_ROW_MESSAGE_SPEED:
    if (gMainRoutineWork.messageSpeed != 0)
      gMainRoutineWork.messageSpeed--;
    break;
  case OPTION_ROW_WINDOW_COLOR:
    if (gMainRoutineWork.windowHue != 0)
      gMainRoutineWork.windowHue--;
    break;
  case OPTION_ROW_WINDOW_BRIGHTNESS:
    if (gMainRoutineWork.windowBrightness != 0)
      gMainRoutineWork.windowBrightness--;
    break;
  }
}

/// Raises the setting on the three-row cursor's row: message speed, window
/// color or window brightness.
UNUSED void SettingsMenu_RaiseSetting(struct OptionMenuWork *work) {
  switch (work->page) {
  case OPTION_ROW_MESSAGE_SPEED:
    if (gMainRoutineWork.messageSpeed < MESSAGE_SPEED_FAST)
      gMainRoutineWork.messageSpeed++;
    break;
  case OPTION_ROW_WINDOW_COLOR:
    if (gMainRoutineWork.windowHue <= WINDOW_COLOR_COUNT - 1)
      gMainRoutineWork.windowHue++;
    break;
  case OPTION_ROW_WINDOW_BRIGHTNESS:
    if (gMainRoutineWork.windowBrightness <= WINDOW_BRIGHTNESS_COUNT - 1)
      gMainRoutineWork.windowBrightness++;
    break;
  }
}

/// An empty function, reached only through a bare entry of the window
/// far-call table.
void SettingsMenu_SelectionNoOp(void) {}

/// The settings menu's per-frame task: moves the cursor and the two slider
/// markers, animates the marker of the slider in use, and pulses the chosen
/// setting's icon.
void SettingsMenu_AnimateCursors(void) {
  s32 cursorResource;
  s32 markerResource;
  s32 selectedOption;
  struct OptionMenuWork *work;

  work = gOptionMenuWork;
  selectedOption = work->page;
  MenuCursor_AdvanceFar(&work->cursor.cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[0].cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[1].cursor);

  if (selectedOption == SETTINGS_ROW_WINDOW_COLOR) {
    cursorResource = (work->cursorFrame & 7) + RESOURCE_MENU_CURSOR_PICTURES;
  } else {
    cursorResource = RESOURCE_MENU_CURSOR_PICTURES;
  }
  VramCache_Load((u8)work->marker[0].sprite->index, 256,
                 Resource_GetTableEntry(cursorResource));

  if (selectedOption == SETTINGS_ROW_WINDOW_BRIGHTNESS) {
    markerResource = (work->cursorFrame & 7) + RESOURCE_MENU_CURSOR_PICTURES;
  } else {
    markerResource = RESOURCE_MENU_CURSOR_PICTURES;
  }
  VramCache_Load((u8)work->marker[1].sprite->index, 256,
                 Resource_GetTableEntry(markerResource));

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

  work = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                             sizeof(struct OptionMenuWork));
  Dma_Fill32(0, work, sizeof(struct OptionMenuWork));
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
/// The task goes first, since it reads the work block.
void SettingsMenu_Release(void) {
  Task_Remove(SettingsMenu_AnimateCursors);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

/// The sprite palettes of the sliders' ends, which gSettingsMenu_SliderPalette
/// fills.
#define WINDOW_COLOR_SLIDER_PALETTE 14
#define WINDOW_BRIGHTNESS_SLIDER_PALETTE (WINDOW_COLOR_SLIDER_PALETTE + 1)

/// Opens the settings menu's window: the labels, the window color and
/// brightness sliders with their markers, the cursor and the setting icons.
/// Returns the window.
struct Window *SettingsMenu_Open(void) {
  struct OptionMenuWork *work;
  struct Window *window;
  struct WindowSprite *marker;
  s32 x;
  s32 y;
  s32 labelMessage;

  work = gOptionMenuWork;
  window = Window_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14,
                         WINDOW_FLAG_INSTANT);
  Window_DrawDividerLine(window, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
  Window_DrawDividerLine(window, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
  Window_DrawDividerLine(window, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
  Window_DrawDividerLine(window, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
  labelMessage = MsgWindowColorLabel;
  Text_DrawMessageAsTiles(labelMessage, window, 8, 0);
  labelMessage++;
  Text_DrawMessageAsTiles(labelMessage, window, 8, 16);
#if LANGUAGE != LANGUAGE_JAPANESE
  labelMessage = MsgMessageSpeedLabel;
  Text_DrawMessageAsTiles(labelMessage, window, 8, 32);
  labelMessage++;
  Text_DrawMessageAsTiles(labelMessage, window, 32, 40);
#else
  Text_DrawMessageAsTiles(MsgMessageSpeedLabel, window, 8, 40);
#endif
  Text_DrawMessageAsTiles(MsgSpeechLabel, window, 8, 64);
  Text_DrawMessageAsTiles(MsgAutoSleepLabel, window, 8, 88);

  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(x, 128, gPointingHandTiles);
    marker = WindowSprite_Create(x, OUTPUT_SIZE_16X16, window, 0, 0);
    work->cursor.sprite = marker;
    x = window->x * 8;
    y = window->y * 8 + 12;
    MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }

  // The ends of both sliders: OUTPUT_SIZE_32X8 sprites, the right end 4 tiles
  // into the picture, each slider in its own palette.
  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    Dma_Set(gSettingsMenu_SliderPalette,
            (void *)&OBJ_PLTT_COLOR(WINDOW_COLOR_SLIDER_PALETTE, 0),
            DMA_COPY_HALFWORDS(2 * PALETTE_COLORS), REG_DMA3);
    VramCache_Load(x, 256, gSettingsMenu_SliderTiles);
    y = 0;
    marker = WindowSprite_Create(x, OUTPUT_SIZE_32X8, window,
                                 OPTION_SLIDER_X - 6, y);
    marker->oam.palette = WINDOW_COLOR_SLIDER_PALETTE;
    marker = WindowSprite_Create(x, OUTPUT_SIZE_32X8, window,
                                 OPTION_SLIDER_X + 26, y);
    marker->oam.index += 4;
    marker->oam.palette = WINDOW_COLOR_SLIDER_PALETTE;
    y = 16;
    marker = WindowSprite_Create(x, OUTPUT_SIZE_32X8, window,
                                 OPTION_SLIDER_X - 6, y);
    marker->oam.palette = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
    marker = WindowSprite_Create(x, OUTPUT_SIZE_32X8, window,
                                 OPTION_SLIDER_X + 26, y);
    marker->oam.index += 4;
    marker->oam.palette = WINDOW_BRIGHTNESS_SLIDER_PALETTE;
  }

  // The two markers: 16x16 sprites of 256 colours, whose 256 bytes
  // SettingsMenu_AnimateCursors fills each frame.
  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(x, 256, 0);
    marker = WindowSprite_Create(x, OUTPUT_SIZE_16X16, window, 0, 0);
    marker->oam.fullColor = 1;
    work->marker[0].sprite = marker;
    x = window->x * 8 + OPTION_SLIDER_X;
    x += work->option[SETTINGS_ROW_WINDOW_COLOR] * OPTION_SLIDER_WIDTH /
         work->optionCount[SETTINGS_ROW_WINDOW_COLOR];
    y = window->y * 8 + 4;
    MenuCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
  }

  x = VramCache_FindFreeEntry();
  if (x < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(x, 256, 0);
    marker = WindowSprite_Create(x, OUTPUT_SIZE_16X16, window, 0, 0);
    marker->oam.fullColor = 1;
    work->marker[1].sprite = marker;
    x = window->x * 8 + OPTION_SLIDER_X;
    x += work->option[SETTINGS_ROW_WINDOW_BRIGHTNESS] * OPTION_SLIDER_WIDTH /
         work->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
    y = window->y * 8 + 20;
    MenuCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
  }

  y = 28;
  work->choiceIcons[0][0] = CommandIcon_Create(gSettingsMenu_SpeedIcons[0], 0,
                                               window, OPTION_CHOICE_X, y);
  work->choiceIcons[0][1] = CommandIcon_Create(gSettingsMenu_SpeedIcons[1], 0,
                                               window, OPTION_CHOICE_X + 24, y);
  work->choiceIcons[0][2] = CommandIcon_Create(gSettingsMenu_SpeedIcons[2], 0,
                                               window, OPTION_CHOICE_X + 48, y);
  y = 52;
  work->choiceIcons[1][0] =
      CommandIcon_Create(gSettingsMenu_SpeechIcons[0], 0, window, 100, y);
  work->choiceIcons[1][1] =
      CommandIcon_Create(gSettingsMenu_SpeechIcons[1], 0, window, 124, y);
  y = 76;
  work->choiceIcons[2][0] =
      CommandIcon_Create(gSettingsMenu_SleepIcons[0], 0, window, 100, y);
  work->choiceIcons[2][1] =
      CommandIcon_Create(gSettingsMenu_SleepIcons[1], 0, window, 124, y);
  return window;
}

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
  struct WindowSprite *frame;

  redraw = 1;
  selectedOption = 0;
  SettingsMenu_InitializeWork();
  work = gOptionMenuWork;
  helpWindow = Window_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3,
                             WINDOW_FLAG_INSTANT);
  settingsWindow = SettingsMenu_Open();
  // Picture 7 of gMenuTitlePictures, the Setup heading.
  pair = MenuTitle_Create(7, settingsWindow, 64, -48);
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
        frame = work->choiceIcons[0][i];
        frame->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(frame);
        resourceIndex = (u8)work->choiceIcons[0][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_MESSAGE_SPEED])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SpeedIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->choiceIcons[1][i];
        frame->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(frame);
        resourceIndex = (u8)work->choiceIcons[1][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_SPEECH])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SpeechIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->choiceIcons[2][i];
        frame->depth = OAM_DEPTH_FRONT - 4;
        Icon_PrepareActorFar(frame);
        resourceIndex = (u8)work->choiceIcons[2][i]->index;
        dimmed = 0;
        if (i != work->option[SETTINGS_ROW_AUTO_SLEEP])
          dimmed = 1;
        CommandIcon_Load(gSettingsMenu_SleepIcons[i], resourceIndex, dimmed);
      }

      x = settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*windowColor * OPTION_SLIDER_WIDTH) /
           work->optionCount[SETTINGS_ROW_WINDOW_COLOR];
      y = settingsWindow->y * 8 + 4;
      MenuCursor_SetFar(&work->marker[0].cursor, x, y, 1);
      x = settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*brightness * OPTION_SLIDER_WIDTH) /
           work->optionCount[SETTINGS_ROW_WINDOW_BRIGHTNESS];
      y = settingsWindow->y * 8 + 20;
      MenuCursor_SetFar(&work->marker[1].cursor, x, y, 1);

      x = work->option[SETTINGS_ROW_MESSAGE_SPEED] + MsgMessageSpeedSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 40,
                                OPTION_SPEED_END, 48);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 40);
      x = work->option[SETTINGS_ROW_SPEECH] + MsgSpeechSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 64,
                                OPTION_SETTING_END, 72);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 64);
      x = work->option[SETTINGS_ROW_AUTO_SLEEP] + MsgAutoSleepSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 88,
                                OPTION_SETTING_END, 96);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 88);
      // The windows show the edited colours at once.
      WindowColor_Apply(*windowColor, *brightness);

      // The cursor goes 3 tiles a row down the window's left edge, the first
      // row's a tile lower; the help window says what the row sets
      // (`Change the window's color.` for the first).
      x = settingsWindow->x * 8;
      y = (selectedOption * 3 + settingsWindow->y) * 8 + 4;
      if (selectedOption == SETTINGS_ROW_WINDOW_COLOR)
        y += 8;
      MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
      WindowSprite_ClearList(helpWindow);
      Text_DrawMessage(selectedOption + MsgOptionHelp, helpWindow, 0, 0);
    }
    MenuTitle_Bob(pair);
    wait(1);

    // SELECT steps through the colour presets (window/scales.c).
    if (gKeysPressed & KEY_SELECT) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      work->preset++;
      redraw = 1;
      if (work->preset > ARRAY_COUNT(gSettingsMenu_HuePresets) - 1)
        work->preset = 0;
      *windowColor = gSettingsMenu_HuePresets[work->preset];
      *brightness = gSettingsMenu_BrightnessPresets[work->preset];
      continue;
    }
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = 0;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (gKeysPressed & KEY_B) {
      result = -1;
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedOption--;
      redraw = 1;
    } else if (gKeysRepeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedOption++;
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        work->option[selectedOption]--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        work->option[selectedOption]++;
        redraw = 1;
      }
    }
  }

  Window_Close(helpWindow, 2);
  Window_Close(settingsWindow, 2);
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

#define PAUSE_MENU_ROWS 3        ///< Save your quest, Sleep and Change settings
#define PAUSE_MENU_DEBUG_ROWS 3  ///< the entries added in debug mode
#define PAUSE_MENU_ROW_HEIGHT 24 ///< pixels, 3 tiles
#define PAUSE_MENU_TEXT_X 48     ///< pixels into the window, after the icons

/// The Pause menu's per-frame task: moves the cursor and pulses the
/// chosen entry's icon.
void PauseMenu_AnimateCursor(void) {
  struct OptionMenuWork *work = gOptionMenuWork;
  u32 index;

  MenuCursor_AdvanceFar(&work->cursor.cursor);
  index = work->page;
  WindowSprite_Pulse(work->icon[index]);
}

/// Allocates the Pause menu's work block and starts its per-frame task.
void PauseMenu_Initialize(void) {
  void *work;
  work = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                             sizeof(struct OptionMenuWork));
  Dma_Fill32(0, work, sizeof(struct OptionMenuWork));
  Task_Add(PauseMenu_AnimateCursor, TASK_PASS_FRAME);
}

/// Stops the Pause menu's per-frame task and frees its work block.
/// The task goes first, since it reads the work block.
void PauseMenu_Release(void) {
  Task_Remove(&PauseMenu_AnimateCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

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
  struct Window *window;
  s32 y;
  s32 cacheSlot;
  s32 labelMessage;
  s32 i;
  s32 x;

  rows = PAUSE_MENU_ROWS;
  work = gOptionMenuWork;
  settingsOnly = GameFlag_GetFar(FLAG_IN_BATTLE_ARENA);
  first = 0;
  if (settingsOnly != 0) {
    rows = 1;
    first = PAUSE_MENU_CHANGE_SETTINGS;
  }
  if (gDebugMode != 0)
    rows += PAUSE_MENU_DEBUG_ROWS;
  // Three tiles a row and a tile more.
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > DISPLAY_TILE_HEIGHT - 1) {
    top = 1;
    height = DISPLAY_TILE_HEIGHT - 1;
  }
  window = Window_Create(5, top, 20, height, WINDOW_FLAG_INSTANT);
  for (i = 1; i < rows; i++)
    Window_DrawDividerLine(window, 0, i * 3, 19, i * 3);
  y = 4;
  if (settingsOnly == 0) {
    labelMessage = MsgWorkspaceSaveQuest;
    Text_DrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, 4);
    labelMessage++;
    Text_DrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X,
                     4 + PAUSE_MENU_ROW_HEIGHT);
    y = 4 + 2 * PAUSE_MENU_ROW_HEIGHT;
  }
  Text_DrawMessage(MsgWorkspaceChangeSettings, window, PAUSE_MENU_TEXT_X, y);
  y += PAUSE_MENU_ROW_HEIGHT;
  if (gDebugMode != 0) {
    labelMessage = MsgWorkspaceRoughMenu;
    Text_DrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, y);
    y += PAUSE_MENU_ROW_HEIGHT;
    Text_DrawMessage(labelMessage + 1, window, PAUSE_MENU_TEXT_X, y);
    y += PAUSE_MENU_ROW_HEIGHT;
    labelMessage += 2;
    Text_DrawMessage(labelMessage, window, PAUSE_MENU_TEXT_X, y);
  }
  cacheSlot = VramCache_FindFreeEntry();
  if (cacheSlot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(cacheSlot, 128, gPointingHandTiles);
    work->cursor.sprite =
        WindowSprite_Create(cacheSlot, OUTPUT_SIZE_16X16, window, 0, 0);
    x = window->x * 8;
    y = window->y * 8 + 16;
    MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icon[i] =
        CommandIcon_Create(gPauseMenu_Icons[first + i], 0, window, 12, y);
    y += PAUSE_MENU_ROW_HEIGHT;
  }
  return window;
}

/// Runs the Pause menu until the player picks an entry (enum PauseMenuEntry) or
/// cancels with B or START (-1).
s32 PauseMenu_Choose(void) {
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
  s32 selectedEntry;
  s32 resourceIndex;
  s32 x;
  s32 top;
  const s8 *pauseIcons;
  struct WindowSprite *icon;
  struct OptionMenuWork *work;

  redraw = 1;
  rows = PAUSE_MENU_ROWS;
  settingsOnly = GameFlag_GetFar(FLAG_IN_BATTLE_ARENA);
  first = 0;
  PauseMenu_Initialize();
  work = gOptionMenuWork;
  window = PauseMenu_Open();
  // Picture 6 of gMenuTitlePictures, the Pause heading, above the window.
#if TBS_PATCH_1
  y = -24;
  if (gDebugMode != 0) {
    y = -16;
  }
  pair = MenuTitle_Create(6, window, 40, y);
#else
  pair = MenuTitle_Create(6, window, 40, -24);
#endif
  wait(1);
  // The cursor starts where the block's page is, 0 after the fill.
  selectedEntry = work->page;
  if (settingsOnly != 0) {
    rows = 1;
    first = PAUSE_MENU_CHANGE_SETTINGS;
  }
  if (gDebugMode != 0) {
    rows += PAUSE_MENU_DEBUG_ROWS;
  }
  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      selectedEntry = (selectedEntry + rows) % rows;
      work->page = selectedEntry;
      i = 0;
      if (i < rows) {
        j = first;
        pauseIcons = gPauseMenu_Icons;
        do {
          icon = work->icon[i];
          icon->depth = OAM_DEPTH_FRONT - 4;
          Icon_PrepareActorFar(icon);
          resourceIndex = (u8)icon->index;
          dimmed = 0;
          if (i != work->page) {
            dimmed = 1;
          }
          CommandIcon_Load(*(s8 *)(j + (s32)pauseIcons), resourceIndex, dimmed);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = window->x * 8;
      y = (((selectedEntry * 3) + top) * 8) + 16;
      MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
    }
    MenuTitle_Bob(pair);
    wait(1);
    if (gKeysPressed & KEY_A) {
      result = selectedEntry;
      Sound_PlayFar(SE_MENU_CONFIRM);
    } else if (gKeysPressed & (KEY_B | KEY_START)) {
      result = -1;
      Sound_PlayFar(SE_MENU_CANCEL);
    } else {
      if (gKeysRepeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedEntry -= 1;
        redraw = 1;
      } else if (gKeysRepeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedEntry += 1;
        redraw = 1;
      }
      continue;
    }
    break;
  }
  Window_Close(window, 2);
  PauseMenu_Release();
  wait(1);
  // In the Battle Arena only Change settings shows: count from Save your quest.
  if (result >= 0) {
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
