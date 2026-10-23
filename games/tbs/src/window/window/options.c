/// @file
/// Window colours, the option menu and the pause menu.
// Part of window.c.

#include "../window_internal.h"

/// Nothing calls them.
UNUSED void OptionMenu_ReservedNoOpC9BC(void) {}
UNUSED void OptionMenu_ReservedNoOpC9C0(void) {}
UNUSED void OptionMenu_ReservedNoOpC9C4(void) {}

/// One of the seven entries of the option menu's work from offset 0x400;
/// only its state is known, and only the unused OptionMenu_ResetEntries
/// writes it.
struct OptionMenuEntry {
  u8 unused[10]; ///< no code names it
  u16 state;     ///< cleared to 0 by OptionMenu_ResetEntries; nothing reads it
  u8 unusedAfterState[40]; ///< no code names it
};

/// Clears the option menu's seven entry states, its row, its tint and its
/// cursor animation.
/// Nothing calls it.
UNUSED void OptionMenu_ResetEntries(struct OptionMenuWork *work) {
  struct OptionMenuEntry *entry =
      (struct OptionMenuEntry *)((u8 *)work + 0x400);
  u16 zero = 0;

  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  work->page = zero;
  work->tint[0] = zero;
  work->tint[1] = zero;
  work->tint[2] = zero;
  work->cursorFrame = zero;
}

/// Works out the window tint (red, green and blue, 0 to 31) from the window
/// colour settings: the hue picks a point on the colour wheel and the
/// brightness shifts all three channels.
/// The three wave samples are one third of the 96-step colour cycle apart.
/// The arithmetic is WindowColor_Apply's.
/// Nothing calls it.
UNUSED void WindowColor_CalculateTint(struct OptionMenuWork *work) {
  s16 phase;
  s32 bias;
  s32 blue;
  s32 red;
  s32 green;

  phase = (gGameState.windowColor[0] + 12) % 24 * 4;
  bias = gGameState.windowColor[1] - 7;
  red = gWindowColor_WaveTable[(phase % 96)];
  green = gWindowColor_WaveTable[(phase + 32) % 96];
  blue = gWindowColor_WaveTable[(phase + 64) % 96];
  red += bias;
  green += bias;
  blue += bias;
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  if (red > 31)
    red = 31;
  if (green > 31)
    green = 31;
  if (blue > 31)
    blue = 31;
  work->tint[0] = red;
  work->tint[1] = green;
  work->tint[2] = blue;
}

/// Writes window colours 4 to 10 of BG palette 15, each a fixed scale of the
/// window tint.
/// Nothing calls it.
UNUSED void WindowColor_WritePalette(struct OptionMenuWork *work) {
  *(s16 *)&BG_PLTT_COLOR(15, 4) =
      WindowColor_ScaleTint(work, 0xEEEE, 0xCCCC, 0x11110);
  *(s16 *)&BG_PLTT_COLOR(15, 5) =
      WindowColor_ScaleTint(work, 0xD555, 0xBBBB, 0xEEEE);
  *(s16 *)&BG_PLTT_COLOR(15, 6) =
      WindowColor_ScaleTint(work, 0xBBBB, 0xAAAA, 0xCCCC);
  *(s16 *)&BG_PLTT_COLOR(15, 7) =
      WindowColor_ScaleTint(work, 0xA221, 0x9999, 0xAAAA);
  *(s16 *)&BG_PLTT_COLOR(15, 8) =
      WindowColor_ScaleTint(work, 0x10888, 0xDDDD, 0x13333);
  *(s16 *)&BG_PLTT_COLOR(15, 9) =
      WindowColor_ScaleTint(work, 0x12221, 0xEEEE, 0x15555);
  *(s16 *)&BG_PLTT_COLOR(15, 10) =
      WindowColor_ScaleTint(work, 0x13BBB, 0x10000, 0x17777);
}

/// Returns the window tint with each channel scaled (16.16 fixed point) and
/// clamped to 0 to 31, as a BGR555 colour.
u16 WindowColor_ScaleTint(struct OptionMenuWork *work, s32 redScale,
                          s32 greenScale, s32 blueScale) {
  s32 red;
  s32 green;
  s32 blue;

  red = Iwram_MulQ16(work->tint[0] << 16, redScale) >> 16;
  green = Iwram_MulQ16(work->tint[1] << 16, greenScale) >> 16;
  blue = Iwram_MulQ16(work->tint[2] << 16, blueScale) >> 16;
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  if (red > 31)
    red = 31;
  if (green > 31)
    green = 31;
  if (blue > 31)
    blue = 31;
  return red + ((blue << 10) + (green << 5));
}

/// Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
/// 31, as a BGR555 colour.
/// rgb holds three components, not an already packed palette word.
u16 Color_ScaleComponents(s16 *rgb, s32 redScale, s32 greenScale,
                          s32 blueScale) {
  s32 red;
  s32 green;
  s32 blue;

  red = Iwram_MulQ16(rgb[0] << 16, redScale) >> 16;
  green = Iwram_MulQ16(rgb[1] << 16, greenScale) >> 16;
  blue = Iwram_MulQ16(rgb[2] << 16, blueScale) >> 16;
  CLAMP(red, 0, 31);
  CLAMP(green, 0, 31);
  CLAMP(blue, 0, 31);
  return red + ((blue << 10) + (green << 5));
}

/// Sets the window colours (BG palette 15, colours 4 to 10) for a window hue
/// (0 to 23) and brightness (0 to 14, 7 neutral).
/// The live palette path leaves the saved colour settings untouched. The
/// option screen calls it as the player changes the settings, and with the
/// saved ones when the screen is cancelled. The file menu
/// (FileMenu_SelectFile) shows the colours of the file under the cursor, and
/// the saved ones for an empty file and on leaving. The main menu
/// (menu/main_menu.c), the main loop's start (Game_Run), the end of the
/// Mind Read wave (Ui_SetBank15PaletteAndClearRenderMode) and Vale and the
/// world map call it with the saved settings.
void WindowColor_Apply(s32 phase, s32 brightness) {
  s16 rgb[3];
  s16 step;
  s16 red;
  s16 green;
  s16 blue;
  s16 offset;

  // The 24 hues are every fourth step of the 96-step wheel, hue 0 at step 48.
  // Brightness 7 adds nothing; each step from it adds or takes one level from
  // every channel.
  step = ((phase + 12) % 24) * 4;
  offset = brightness - 7;
  red = gWindowColor_WaveTable[(step % 96)] + offset;
  green = gWindowColor_WaveTable[(step + 32) % 96] + offset;
  blue = gWindowColor_WaveTable[(step + 64) % 96] + offset;
  CLAMP(red, 0, 31);
  CLAMP(green, 0, 31);
  CLAMP(blue, 0, 31);
  rgb[0] = red;
  rgb[1] = green;
  rgb[2] = blue;
  // Each colour scales the tint by channel, in 16.16 (0x10000 is 1): colours
  // 4 to 7 grow darker, from about 0.93, 0.8 and 1.07 to 0.63, 0.6 and 0.67,
  // and 8 to 10 lighter, up to about 1.23, 1 and 1.47. Colour 4 is the
  // windows' background.
  BG_PLTT_COLOR(15, 4) = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
  BG_PLTT_COLOR(15, 5) = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
  BG_PLTT_COLOR(15, 6) = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
  BG_PLTT_COLOR(15, 7) = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
  BG_PLTT_COLOR(15, 8) = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
  BG_PLTT_COLOR(15, 9) = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
  BG_PLTT_COLOR(15, 10) = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

/// The rows of the unused three-row settings cursor (OptionMenuWork.page).
enum OptionMenuRow {
  OPTION_ROW_MESSAGE_SPEED = 0,
  OPTION_ROW_WINDOW_HUE = 1,
  OPTION_ROW_WINDOW_BRIGHTNESS = 2
};

/// Moves the three-row settings cursor up, wrapping to the bottom. The three
/// rows are those of OptionMenu_LowerSetting. XXX: likely left from an
/// earlier option screen, because the five-row one OptionMenu_Run
/// runs uses none of these four functions.
/// Nothing calls it.
UNUSED void OptionMenu_PreviousRow(struct OptionMenuWork *work) {
  if (work->page == OPTION_ROW_MESSAGE_SPEED)
    work->page = OPTION_ROW_WINDOW_BRIGHTNESS;
  else
    work->page--;
}

/// Moves the three-row settings cursor down, wrapping to the top.
/// Nothing calls it.
UNUSED void OptionMenu_NextRow(struct OptionMenuWork *work) {
  if (++work->page > OPTION_ROW_WINDOW_BRIGHTNESS)
    work->page = OPTION_ROW_MESSAGE_SPEED;
}

/// Lowers the setting on the three-row cursor's row: message speed, window hue
/// or window brightness.
/// Nothing calls it.
UNUSED void OptionMenu_LowerSetting(struct OptionMenuWork *work) {
  switch (work->page) {
  case OPTION_ROW_MESSAGE_SPEED:
    if (gGameState.messageSpeed != 0)
      gGameState.messageSpeed--;
    break;
  case OPTION_ROW_WINDOW_HUE:
    if (gGameState.windowColor[0] != 0)
      gGameState.windowColor[0]--;
    break;
  case OPTION_ROW_WINDOW_BRIGHTNESS:
    if (gGameState.windowColor[1] != 0)
      gGameState.windowColor[1]--;
    break;
  }
}

/// Raises the setting on the three-row cursor's row: message speed, window hue
/// or window brightness.
/// Nothing calls it.
UNUSED void OptionMenu_RaiseSetting(struct OptionMenuWork *work) {
  switch (work->page) {
  case OPTION_ROW_MESSAGE_SPEED:
    if (gGameState.messageSpeed <= 1)
      gGameState.messageSpeed++;
    break;
  case OPTION_ROW_WINDOW_HUE:
    if (gGameState.windowColor[0] <= 23)
      gGameState.windowColor[0]++;
    break;
  case OPTION_ROW_WINDOW_BRIGHTNESS:
    if (gGameState.windowColor[1] <= 14)
      gGameState.windowColor[1]++;
    break;
  }
}

/// An empty function, reached only through a bare entry of the window
/// far-call table.
void OptionMenu_SelectionNoOp(void) {}

/// The option menu's per-frame callback: moves the cursor and the two slider
/// markers, animates the marker of the slider in use, and pulses the chosen
/// setting's icon. The markers' pictures, 256 bytes each, are loaded every
/// frame: the marker of the row in use steps through 8 frames of
/// ResourceId_MenuCursorFrames, the other shows the first. Rows 2 to 4
/// (speed, speech, sleep) pulse their chosen icon.
void OptionMenu_AnimateCursors(void) {
  s32 cursorResource;
  s32 markerResource;
  s32 selectedOption;
  struct OptionMenuWork *work;

  work = gOptionMenuWork;
  selectedOption = work->page;
  MenuCursor_AdvanceFar(&work->cursor.cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[0].cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[1].cursor);

  if (selectedOption == 0) {
    cursorResource =
        (work->cursorFrame & 7) + (s32)&ResourceId_MenuCursorFrames;
  } else {
    cursorResource = (s32)&ResourceId_MenuCursorFrames;
  }
  VramCache_Load((u8)work->marker[0].sprite->index, 0x100,
                 Resource_GetTableEntry(cursorResource));

  if (selectedOption == 1) {
    markerResource =
        (work->cursorFrame & 7) + (s32)&ResourceId_MenuCursorFrames;
  } else {
    markerResource = (s32)&ResourceId_MenuCursorFrames;
  }
  VramCache_Load((u8)work->marker[1].sprite->index, 0x100,
                 Resource_GetTableEntry(markerResource));

  if (selectedOption > 1)
    WindowSprite_Pulse(
        work->frame[selectedOption - 2][work->option[selectedOption]]);
  work->cursorFrame++;
}

/// Allocates the option menu's work block, copies the settings into it with
/// how many values each takes, and starts its per-frame callback.
/// The copy lets cancelling discard the edits. The block is
/// HEAP_SLOT_OPTION_MENU (gOptionMenuWork), which OptionMenu_Release frees. The
/// rows: 0 the window hue (24 values), 1 its brightness (15), 2 the message
/// speed (3), 3 speech (2) and 4 auto-sleep (2).
void OptionMenu_InitializeWork(void) {
  struct OptionMenuWork *work;

  work = AllocHeapSlotEwram(HEAP_SLOT_OPTION_MENU, 0x628);
  Dma_Fill32(0, work, 0x628);
  work->option[0] = gGameState.windowColor[0];
  work->optionCount[0] = 24;
  work->option[1] = gGameState.windowColor[1];
  work->optionCount[1] = 15;
  work->option[2] = gGameState.messageSpeed;
  work->optionCount[2] = 3;
  work->option[3] = gGameState.speech;
  work->optionCount[3] = 2;
  work->option[4] = gGameState.idleSleepEnabled;
  work->optionCount[4] = 2;
  Task_Add(OptionMenu_AnimateCursors, 0xc80);
}

/// Stops the option menu's per-frame callback and frees its work block.
/// The callback goes first, since it reads the work block.
void OptionMenu_Release(void) {
  Task_Remove(OptionMenu_AnimateCursors);
  FreeHeapSlot(HEAP_SLOT_OPTION_MENU);
}

#define SLIDER_PALETTE 14 ///< the sprite palette of the hue slider's ends
/// VRAM_CACHE_ENTRY_COUNT, which VramCache_FindFreeEntry returns when no entry
/// is free.
#define RESOURCE_SLOTS 96

/// Opens the option menu's window: the labels, the hue and brightness
/// sliders with their markers, the cursor and the setting icons. Returns the
/// window. Its sprites belong to the window and go when it closes. A
/// slider's marker stands option * 60 / count pixels right of
/// OPTION_SLIDER_X.
struct Window *OptionMenu_Open(void) {
  struct OptionMenuWork *work;
  struct Window *window;
  struct WindowSprite *marker;
  s32 x;
  s32 y;
  s32 labelMessage;

  work = gOptionMenuWork;
  window = Window_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
  Window_DrawDividerLine(window, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
  Window_DrawDividerLine(window, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
  Window_DrawDividerLine(window, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
  Window_DrawDividerLine(window, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
  labelMessage = MsgWindowColorLabel;
  Text_DrawMessageAsTiles(labelMessage, window, 8, 0);
  labelMessage++;
  Text_DrawMessageAsTiles(labelMessage, window, 8, 16);
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 128, gResource_FixedBlockBTiles);
    marker = WindowSprite_Create(x, 0x40000000, window, 0, 0);
    work->cursor.sprite = marker;
    x = window->x * 8;
    y = window->y * 8 + 12;
    MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }

  // The ends of both sliders: 32x16 sprites (0x40004000, the wide shape at
  // size 1), the right end 4 tiles into the picture; the hue slider's in
  // palette 14, the brightness slider's in 15.
  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    Dma_Set(gOptionMenu_SliderPalette,
            (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), DMA_COPY16(32),
            REG_DMA3);
    VramCache_Load(x, 256, gOptionMenu_SliderTiles);
    y = 0;
    marker = WindowSprite_Create(x, 0x40004000, window, OPTION_SLIDER_X - 6, y);
    marker->oam.palette = SLIDER_PALETTE;
    marker =
        WindowSprite_Create(x, 0x40004000, window, OPTION_SLIDER_X + 26, y);
    marker->oam.index += 4;
    marker->oam.palette = SLIDER_PALETTE;
    y = 16;
    marker = WindowSprite_Create(x, 0x40004000, window, OPTION_SLIDER_X - 6, y);
    marker->oam.palette = 15;
    marker =
        WindowSprite_Create(x, 0x40004000, window, OPTION_SLIDER_X + 26, y);
    marker->oam.index += 4;
    marker->oam.palette = 15;
  }

  // The two markers: 16x16 sprites of 256 colours, whose 256 bytes
  // OptionMenu_AnimateCursors fills each frame.
  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    marker = WindowSprite_Create(x, 0x40000000, window, 0, 0);
    marker->oam.palette256 = 1;
    work->marker[0].sprite = marker;
    x = window->x * 8 + OPTION_SLIDER_X;
    x += work->option[0] * 60 / work->optionCount[0];
    y = window->y * 8 + 4;
    MenuCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
  }

  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    marker = WindowSprite_Create(x, 0x40000000, window, 0, 0);
    marker->oam.palette256 = 1;
    work->marker[1].sprite = marker;
    x = window->x * 8 + OPTION_SLIDER_X;
    x += work->option[1] * 60 / work->optionCount[1];
    y = window->y * 8 + 20;
    MenuCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
  }

  y = 28;
  work->frame[0][0] = CommandIcon_Create(gOptionMenu_SpeedFrames[0], 0, window,
                                         OPTION_FRAME_X, y);
  work->frame[0][1] = CommandIcon_Create(gOptionMenu_SpeedFrames[1], 0, window,
                                         OPTION_FRAME_X + 24, y);
  work->frame[0][2] = CommandIcon_Create(gOptionMenu_SpeedFrames[2], 0, window,
                                         OPTION_FRAME_X + 48, y);
  y = 52;
  work->frame[1][0] =
      CommandIcon_Create(gOptionMenu_SpeechFrames[0], 0, window, 100, y);
  work->frame[1][1] =
      CommandIcon_Create(gOptionMenu_SpeechFrames[1], 0, window, 124, y);
  y = 76;
  work->frame[2][0] =
      CommandIcon_Create(gOptionMenu_SleepFrames[0], 0, window, 100, y);
  work->frame[2][1] =
      CommandIcon_Create(gOptionMenu_SleepFrames[1], 0, window, 124, y);
  return window;
}

/// Runs the option menu (window colour and brightness, message speed,
/// speech, auto-sleep) until A or START keeps the settings (returns 0) or B
/// drops them (returns -1). SELECT cycles through the colour presets.
/// Preview edits affect the live palette; only confirmation saves the settings.
/// Every setting wraps at its ends. While sleep is held off (gSleepDisabled,
/// as in the Battle Arena) auto-sleep shows as off and is saved so. The pause
/// menu's Change settings runs it (PauseMenu_Run,
/// screens/debug.c), and reopens the menu when it is cancelled.
s32 OptionMenu_Run(void) {
  struct OptionMenuWork *work;
  struct Window *settingsWindow;
  struct Window *helpWindow;
  struct WindowSprite *pair;
  s8 *hue;
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
  OptionMenu_InitializeWork();
  work = gOptionMenuWork;
  helpWindow = Window_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
  settingsWindow = OptionMenu_Open();
  // Picture 7 of gHeadingPictures, the Setup heading.
  pair = Heading_Create(7, settingsWindow, 64, -48);
  WaitFrames(1);
  hue = &work->option[0];
  brightness = &work->option[1];

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      // Rows 0 to 4 are hue, brightness, text speed, voice sounds and
      // auto-sleep, in the order of their help messages.
      selectedOption = (selectedOption + 5) % 5;
      work->option[selectedOption] =
          (work->option[selectedOption] + work->optionCount[selectedOption]) %
          work->optionCount[selectedOption];
      work->page = selectedOption;
      if (gSleepDisabled != 0)
        work->option[4] = 0;

      // Redraw each choice's icon at depth slot 0xfb, dimmed unless it is
      // the row's setting.
      for (i = 0; i <= 2; i++) {
        frame = work->frame[0][i];
        frame->sentinel = 0xfb;
        Icon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame[0][i]->index;
        dimmed = 0;
        if (i != work->option[2])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SpeedFrames[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame[1][i];
        frame->sentinel = 0xfb;
        Icon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame[1][i]->index;
        dimmed = 0;
        if (i != work->option[3])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SpeechFrames[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame[2][i];
        frame->sentinel = 0xfb;
        Icon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame[2][i]->index;
        dimmed = 0;
        if (i != work->option[4])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SleepFrames[i], resourceIndex, dimmed);
      }

      x = settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*hue * 60) / work->optionCount[0];
      y = settingsWindow->y * 8 + 4;
      MenuCursor_SetFar(&work->marker[0].cursor, x, y, 1);
      x = settingsWindow->x * 8 + OPTION_SLIDER_X;
      x += (*brightness * 60) / work->optionCount[1];
      y = settingsWindow->y * 8 + 20;
      MenuCursor_SetFar(&work->marker[1].cursor, x, y, 1);

      x = work->option[2] + MsgMessageSpeedSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 40,
                                OPTION_SPEED_END, 48);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 40);
      x = work->option[3] + MsgSpeechSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 64,
                                OPTION_SETTING_END, 72);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 64);
      x = work->option[4] + MsgAutoSleepSetting;
      Window_ClearInteriorTiles(settingsWindow, OPTION_SETTING_X, 88,
                                OPTION_SETTING_END, 96);
      Text_DrawMessageAsTiles(x, settingsWindow, OPTION_SETTING_X, 88);
      // The windows show the edited colours at once.
      WindowColor_Apply(*hue, *brightness);

      // The cursor goes 3 tiles a row down the window's left edge, the first
      // row's a tile lower; the help window says what the row sets
      // (`Change the window's color.` for the first).
      x = settingsWindow->x * 8;
      y = (selectedOption * 3 + settingsWindow->y) * 8 + 4;
      if (selectedOption == 0)
        y += 8;
      MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
      WindowSprite_ClearList(helpWindow);
      Text_DrawMessage(selectedOption + MsgOptionHelp, helpWindow, 0, 0);
    }
    Heading_Bob(pair);
    WaitFrames(1);

    // SELECT steps through the six colour presets (window/scales.c).
    if (gKeysPressed & KEY_SELECT) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      work->preset++;
      redraw = 1;
      if (work->preset > 5)
        work->preset = 0;
      *hue = gOptionMenu_HuePresets[work->preset];
      *brightness = gOptionMenu_BrightnessPresets[work->preset];
      continue;
    }
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = 0;
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      break;
    }
    if (gKeysPressed & KEY_B) {
      result = -1;
      Sound_PlaySongFar(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selectedOption--;
      redraw = 1;
    } else if (gKeysRepeat & KEY_DOWN) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selectedOption++;
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        work->option[selectedOption]--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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
    gGameState.windowColor[0] = work->option[0];
    gGameState.windowColor[1] = work->option[1];
    gGameState.messageSpeed = work->option[2];
    gGameState.speech = work->option[3];
    gGameState.idleSleepEnabled = work->option[4];
    gIdleSleepEnabled = gGameState.idleSleepEnabled;
  } else {
    WindowColor_Apply(gGameState.windowColor[0], gGameState.windowColor[1]);
  }
  OptionMenu_Release();
  WaitFrames(1);
  return result;
}

#define PAUSE_MENU_ROWS 3        ///< Save your quest, Sleep and Change settings
#define PAUSE_MENU_DEBUG_ROWS 3  ///< the entries added in debug mode
#define PAUSE_MENU_ROW_HEIGHT 24 ///< pixels, 3 tiles
#define PAUSE_MENU_TEXT_X 48     ///< pixels into the window, after the icons

/// The pause menu's per-frame callback: moves the cursor and pulses the
/// chosen entry's icon.
void PauseMenu_AnimateCursor(void) {
  struct OptionMenuWork *work = gOptionMenuWork;
  u32 index;

  MenuCursor_AdvanceFar(&work->cursor.cursor);
  index = work->page;
  WindowSprite_Pulse(work->icon[index]);
}

/// Allocates the pause menu's work block and starts its per-frame callback.
/// The pause menu shares the option menu's heap slot and cannot run
/// concurrently: PauseMenu_Choose frees it before
/// PauseMenu_Run opens the option menu. 3200 is 0xc80, the
/// main loop's pass.
void PauseMenu_Initialize(void) {
  void *work;
  work = AllocHeapSlotEwram(HEAP_SLOT_OPTION_MENU, 0x628);
  Dma_Fill32(0, work, 0x628);
  Task_Add(PauseMenu_AnimateCursor, 3200);
}

/// Stops the pause menu's per-frame callback and frees its work block.
/// The callback goes first, since it reads the work block.
void PauseMenu_Release(void) {
  Task_Remove(&PauseMenu_AnimateCursor);
  FreeHeapSlot(HEAP_SLOT_OPTION_MENU);
}

/// Opens the pause menu's window with its entries (Save your quest, Sleep,
/// Change settings, and the debug entries in debug mode; only Change settings
/// while FLAG_IN_BATTLE_ARENA is set), their icons and the cursor. Returns the
/// window.
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
  // Three tiles a row and a tile more. The window starts higher by a row for
  // each entry, and at row 1, 19 tall, when it would reach row 19.
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > 19) {
    top = 1;
    height = 19;
  }
  window = Window_Create(5, top, 20, height, 2);
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
  if (cacheSlot < RESOURCE_SLOTS) {
    VramCache_Load(cacheSlot, 128, gResource_FixedBlockBTiles);
    work->cursor.sprite =
        WindowSprite_Create(cacheSlot, 0x40000000, window, 0, 0);
    x = window->x * 8;
    y = window->y * 8 + 16;
    MenuCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icon[i] =
        CommandIcon_Create(gPauseMenu_IconFrames[first + i], 0, window, 12, y);
    y += PAUSE_MENU_ROW_HEIGHT;
  }
  return window;
}

/// Runs the pause menu until the player picks an entry (enum
/// PauseMenuEntry) or cancels with B or START (-1).
/// The result counts in the full entry table even when only Change settings is
/// shown. The menu's window and work block are freed before it returns;
/// PauseMenu_Run (screens/debug.c) acts on the result.
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
  const s8 *iconFrames;
  struct WindowSprite *icon;
  struct OptionMenuWork *work;

  redraw = 1;
  rows = PAUSE_MENU_ROWS;
  settingsOnly = GameFlag_GetFar(FLAG_IN_BATTLE_ARENA);
  first = 0;
  PauseMenu_Initialize();
  work = gOptionMenuWork;
  window = PauseMenu_Open();
  // Picture 6 of gHeadingPictures, the Pause heading, above
  // the window. Outside Japanese it sits 8 pixels lower in debug mode, over
  // the debug menu's taller window.
#if TBS_PATCH_1
  y = -0x18;
  if (gDebugMode != 0) {
    y = -0x10;
  }
  pair = Heading_Create(6, window, 0x28, y);
#else
  pair = Heading_Create(6, window, 0x28, -0x18);
#endif
  WaitFrames(1);
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
        iconFrames = gPauseMenu_IconFrames;
        do {
          icon = work->icon[i];
          icon->sentinel = 0xFB;
          Icon_PrepareObjectFar(icon);
          resourceIndex = (u8)icon->index;
          dimmed = 0;
          if (i != work->page) {
            dimmed = 1;
          }
          CommandIcon_Load(*(s8 *)(j + (s32)iconFrames), resourceIndex, dimmed);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = window->x * 8;
      y = (((selectedEntry * 3) + top) * 8) + 0x10;
      MenuCursor_SetFar(&work->cursor.cursor, x, y, 3);
    }
    Heading_Bob(pair);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      result = selectedEntry;
      Sound_PlaySongFar(SE_MENU_CONFIRM);
    } else if (gKeysPressed & (KEY_B | KEY_START)) {
      result = -1;
      Sound_PlaySongFar(SE_MENU_CANCEL);
    } else {
      if (gKeysRepeat & KEY_UP) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        selectedEntry -= 1;
        redraw = 1;
      } else if (gKeysRepeat & KEY_DOWN) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        selectedEntry += 1;
        redraw = 1;
      }
      continue;
    }
    break;
  }
  Window_Close(window, 2);
  PauseMenu_Release();
  WaitFrames(1);
  // In the Battle Arena only Change settings shows: count from Save your quest.
  if (result >= 0) {
    result += first;
  }
  return result;
}
