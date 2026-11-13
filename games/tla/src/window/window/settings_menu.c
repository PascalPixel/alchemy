/// @file
/// Window colours, the settings menu and the Pause menu.
// Part of window.c.

#include "../window_internal.h"

u16 Color_ScaleComponents(s16 *rgb, s32 scale_r, s32 scale_g, s32 scale_b) {
  s32 r = rgb[0] * scale_r >> 16;
  s32 g = rgb[1] * scale_g >> 16;
  s32 b = rgb[2] * scale_b >> 16;

  CLAMP(r, 0, 31);
  CLAMP(g, 0, 31);
  CLAMP(b, 0, 31);
  r |= b << 10 | g << 5;
  return r;
}

extern const u8 gPaletteGlow_WaveTable[];

u16 Color_ScaleComponents(s16 *rgb, s32 scale_r, s32 scale_g, s32 scale_b);

#define GLOW_COLOR_SET(index, value)                                           \
  do {                                                                         \
    u32 color_ = (value);                                                      \
    BG_PLTT_COLOR(15, (index)) = color_;                                       \
  } while (0)

void WindowColor_Apply(s32 phase, s32 brightness) {
  s16 rgb[3];
  s16 step;
  s16 red;
  s16 green;
  s16 blue;
  s16 offset;

  step = ((phase + 12) % 24) * 4;
  offset = brightness - 7;
  red = gPaletteGlow_WaveTable[(step % 96)] + offset;
  green = gPaletteGlow_WaveTable[(step + 32) % 96] + offset;
  blue = gPaletteGlow_WaveTable[(step + 64) % 96] + offset;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  rgb[0] = red;
  rgb[1] = green;
  rgb[2] = blue;
  GLOW_COLOR_SET(4, Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110));
  GLOW_COLOR_SET(5, Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee));
  GLOW_COLOR_SET(6, Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc));
  GLOW_COLOR_SET(7, Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa));
  GLOW_COLOR_SET(8, Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333));
  GLOW_COLOR_SET(9, Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555));
  GLOW_COLOR_SET(10, Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777));
}

struct OptionMarkerOutput {
  u8 unk00[14];
  u8 index;
};

union OptionCursor {
  struct OptionMarkerOutput *output;
  u8 data[0x10];
};

struct OptionWork {
  u8 unk000[0x574];
  u16 page;
  u16 tint[3];
  u16 cursorFrame;
  u16 preset;
  u8 unk580[0x14];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  union OptionCursor cursor;
  union OptionCursor marker[2];
  u8 unk5d4[0x18];
  void *frame[3][3];
};

extern char ResourceId_ShopCursorPictures;

void MenuCursor_AdvanceFar(union OptionCursor *cursor);
void MenuCursor_MoveTowardTargetFar(union OptionCursor *cursor);
void WindowSprite_Pulse(void *object);

void SettingsMenu_AnimateCursors(void) {
  s32 src0;
  s32 src1;
  s32 selection;
  struct OptionWork *work;

  work = Ram_HeapSlots->unkd0[0];
  selection = work->page;
  MenuCursor_AdvanceFar(&work->cursor);
  MenuCursor_MoveTowardTargetFar(&work->marker[0]);
  MenuCursor_MoveTowardTargetFar(&work->marker[1]);

  if (selection == 0) {
    src0 = (work->cursorFrame & 7) + (s32)&ResourceId_ShopCursorPictures;
  } else {
    src0 = (s32)&ResourceId_ShopCursorPictures;
  }
  VramCache_Load((u8)work->marker[0].output->index, 0x100,
                 Resource_GetTableEntry(src0));

  if (selection == 1) {
    src1 = (work->cursorFrame & 7) + (s32)&ResourceId_ShopCursorPictures;
  } else {
    src1 = (s32)&ResourceId_ShopCursorPictures;
  }
  VramCache_Load((u8)work->marker[1].output->index, 0x100,
                 Resource_GetTableEntry(src1));

  if (selection > 1)
    WindowSprite_Pulse(work->frame[selection - 2][work->option[selection]]);
  work->cursorFrame++;
}

void SettingsMenu_AnimateCursors(void);

/// Allocates the settings menu's work block (heap slot 208), copies five
/// settings into it with how many values each takes (24, 15, 3, 2 and 2), and
/// starts its VBlank task. SettingsMenu_Release frees the block.
void SettingsMenu_InitializeWork(void) {
  struct OptionWork *work;
  volatile u32 zero;

  work = (struct OptionWork *)HeapSlot_AllocEwram(208, 0x628);
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (0x628 / 4),
          REG_DMA3);
  work->option[0] = gMainRoutineWork.paletteGlow[0];
  work->optionCount[0] = 24;
  work->option[1] = gMainRoutineWork.paletteGlow[1];
  work->optionCount[1] = 15;
  work->option[2] = gMainRoutineWork.eventMode;
  work->optionCount[2] = 3;
  work->option[3] = gMainRoutineWork.battleFxEnabled;
  work->optionCount[3] = 2;
  work->option[4] = gMainRoutineWork.unk244[6];
  work->optionCount[4] = 2;
  Task_Add(SettingsMenu_AnimateCursors, TASK_PASS_VBLANK);
}

void SettingsMenu_AnimateCursors(void);

/// Stops the settings menu's VBlank task and frees its work block.
/// The task goes first, since it reads the work block.
void SettingsMenu_Release(void) {
  Task_Remove(SettingsMenu_AnimateCursors);
  HeapSlot_Release(208);
}

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

// The settings window's layout, sized to its labels: the window, the last
// column of its dividers, the hue and brightness sliders and the message
// speed icons. Japanese writes "Message speed" on one line.
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

extern const u8 gResource_FixedBlockBTiles[];
extern const u8 Data_0804e584[];
extern const u16 Data_080aa0e8[];

extern s8 gOptionMenu_SpeedIcons[];
extern s8 gOptionMenu_SpeechIcons[];
extern s8 gOptionMenu_SleepIcons[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];

void *CommandIcon_Create(s32 frame, s32 flags, struct RenderInput *input,
                         s32 offsetX, s32 offsetY);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void MenuCursor_SetPositionImmediateFar(void *object, s32 x, s32 y);

// The option screen's work (heap slot 0xd0), as The Broken Seal's
// workspace_options.h lays it out; the listing's offsets agree.
struct OptionCursorMove {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

union OptionCursorMotion {
  struct WindowSprite *output;
  struct OptionCursorMove cursor;
};

struct WorkspaceWork {
  u8 unk000[0x594];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  union OptionCursorMotion cursor;
  union OptionCursorMotion marker[2];
  u8 unk5d4[0x18];
  struct WindowSprite *frame[3][3];
};

/// Opens the settings menu's window: the labels, the hue and brightness
/// sliders with their markers, the cursor and the setting icons. Returns the
/// window. Its sprites belong to the window and go when it closes. A
/// slider's marker stands option * 60 / count pixels right of
/// OPTION_SLIDER_X.
struct RenderInput *SettingsMenu_Open(void) {
  struct WorkspaceWork *work;
  struct RenderInput *win;
  struct WindowSprite *out;
  s32 x;
  s32 y;
  s32 msg;

  work = (struct WorkspaceWork *)Ram_HeapSlots->unkd0[0];
  win = Window_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
  Window_DrawDividerLine((struct Window *)win, 0, 2, OPTION_DIVIDER_END, 2);
  Window_DrawDividerLine((struct Window *)win, 0, 4, OPTION_DIVIDER_END, 4);
  Window_DrawDividerLine((struct Window *)win, 0, 7, OPTION_DIVIDER_END, 7);
  Window_DrawDividerLine((struct Window *)win, 0, 10, OPTION_DIVIDER_END, 10);
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

  out = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000,
                            win, 0, 0);
  work->cursor.output = out;
  x = (s16)win->x * 8;
  y = (s16)win->y * 8 + 12;
  MenuCursor_SetPositionImmediateFar(&work->cursor, x, y);

  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    Dma_Set(Data_080aa0e8, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0),
            0x80000020, REG_DMA3);
    VramCache_Load(x, 256, Data_0804e584);
    y = 0;
    out = WindowSprite_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
    out->sprite.oam.palette = SLIDER_PALETTE;
    out = WindowSprite_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = SLIDER_PALETTE;
    y = 16;
    out = WindowSprite_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
    out->sprite.oam.palette = 15;
    out = WindowSprite_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = 15;
  }

  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, 0x40000000, win, 0, 0);
    out->sprite.oam.palette256 = 1;
    work->marker[0].output = out;
    x = (s16)win->x * 8 + OPTION_SLIDER_X;
    x += __divsi3(work->option[0] * 60, work->optionCount[0]);
    y = (s16)win->y * 8 + 4;
    MenuCursor_SetPositionImmediateFar(&work->marker[0], x, y);
  }

  x = VramCache_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, 0x40000000, win, 0, 0);
    out->sprite.oam.palette256 = 1;
    work->marker[1].output = out;
    x = (s16)win->x * 8 + OPTION_SLIDER_X;
    x += __divsi3(work->option[1] * 60, work->optionCount[1]);
    y = (s16)win->y * 8 + 20;
    MenuCursor_SetPositionImmediateFar(&work->marker[1], x, y);
  }

  y = 28;
  work->frame[0][0] =
      CommandIcon_Create(gOptionMenu_SpeedIcons[0], 0, win, OPTION_CHOICE_X, y);
  work->frame[0][1] = CommandIcon_Create(gOptionMenu_SpeedIcons[1], 0, win,
                                         OPTION_CHOICE_X + 24, y);
  work->frame[0][2] = CommandIcon_Create(gOptionMenu_SpeedIcons[2], 0, win,
                                         OPTION_CHOICE_X + 48, y);
  y = 52;
  work->frame[1][0] =
      CommandIcon_Create(gOptionMenu_SpeechIcons[0], 0, win, 100, y);
  work->frame[1][1] =
      CommandIcon_Create(gOptionMenu_SpeechIcons[1], 0, win, 124, y);
  y = 76;
  work->frame[2][0] =
      CommandIcon_Create(gOptionMenu_SleepIcons[0], 0, win, 100, y);
  work->frame[2][1] =
      CommandIcon_Create(gOptionMenu_SleepIcons[1], 0, win, 124, y);
  return win;
}

struct OptionScreen {
  u8 unk000[0x574];
  u16 page;
  u8 unk576[8];
  u16 preset;
  u8 unk580[0x14];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  u8 cursor[0x10];
  u8 marker[2][0x10];
  u8 unk5d4[0x18];
  struct WindowSprite *choiceIcons[3][3];
};

// The layout that fits each language's text: the help window's column and
// width in tiles, then, in pixels from the settings window's left, where the
// sliders start and where the choices' fields start and end.
#if LANGUAGE == LANGUAGE_JAPANESE
#define HELP_WINDOW_X 2
#define HELP_WINDOW_WIDTH 26
#define SLIDER_X 100
#define CHOICE_X 168
#define CHOICE_RIGHT 192
#define FIRST_CHOICE_RIGHT 192
#else
#define HELP_WINDOW_X 1
#define HELP_WINDOW_WIDTH 28
#define SLIDER_X 140
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

// The heading above the settings window, a picture of
// gRenderResource_PairSourceTable: Japanese takes 8, the Setup heading; from
// patch 1 the screen takes 7, the Pause heading the pause menu also shows.
#if TLA_PATCH_1
#define SETTINGS_HEADING 7
#else
#define SETTINGS_HEADING 8
#endif

extern u8 MsgKey113b[], MsgKey1141[], MsgKey1144[], MsgKey1146[], MsgKey1161[];
extern const u8 gOptionMenu_FirstColorPresets[],
    gOptionMenu_SecondColorPresets[];

void SettingsMenu_InitializeWork(void);
void SettingsMenu_Release(void);
struct RenderInput *SettingsMenu_Open(void);
void WindowColor_Apply(s32 hue, s32 brightness);
struct WindowSprite *MenuTitle_Create(s32 group, struct RenderInput *input,
                                      s32 x, s32 y);
void MenuTitle_Bob(struct WindowSprite *pair);
void Icon_PrepareActorFar(struct WindowSprite *icon);
void CommandIcon_Load(s32 index, s32 value, s32 flag);
void MenuCursor_SetFar(void *cursor, s32 x, s32 y, s32 mode);
void FieldMessage_Show(s32 message, s32 mode);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

s32 SettingsMenu_Run(void) {
  struct OptionScreen *work;
  struct RenderInput *settingsWindow;
  struct RenderInput *helpWindow;
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
  struct WindowSprite *icon;

  redraw = 1;
  selectedOption = 0;
  SettingsMenu_InitializeWork();
  work = Ram_HeapSlots->unkd0[0];
  helpWindow = Window_Create(HELP_WINDOW_X, 2, HELP_WINDOW_WIDTH, 3, 2);
  settingsWindow = SettingsMenu_Open();
  pair = MenuTitle_Create(SETTINGS_HEADING, settingsWindow, 64, -48);
  wait(1);
  hue = &work->option[0];
  brightness = &work->option[1];

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      selectedOption = (selectedOption + 5) % 5;
      work->option[selectedOption] =
          (work->option[selectedOption] + work->optionCount[selectedOption]) %
          work->optionCount[selectedOption];
      work->page = selectedOption;
      if (gSleepDisabled != 0)
        work->option[4] = 0;

      for (i = 0; i <= 2; i++) {
        icon = work->choiceIcons[0][i];
        icon->sentinel = 0xfb;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[0][i]->index;
        dimmed = 0;
        if (i != work->option[2])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SpeedIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[1][i];
        icon->sentinel = 0xfb;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[1][i]->index;
        dimmed = 0;
        if (i != work->option[3])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SpeechIcons[i], resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[2][i];
        icon->sentinel = 0xfb;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[2][i]->index;
        dimmed = 0;
        if (i != work->option[4])
          dimmed = 1;
        CommandIcon_Load(gOptionMenu_SleepIcons[i], resourceIndex, dimmed);
      }

      x = (s16)settingsWindow->x * 8 + SLIDER_X;
      x += (*hue * 60) / work->optionCount[0];
      y = (s16)settingsWindow->y * 8 + 4;
      MenuCursor_SetFar(work->marker[0], x, y, 1);
      x = (s16)settingsWindow->x * 8 + SLIDER_X;
      x += (*brightness * 60) / work->optionCount[1];
      y = (s16)settingsWindow->y * 8 + 20;
      MenuCursor_SetFar(work->marker[1], x, y, 1);

      x = work->option[2] + (s32)MsgKey113b;
      Window_ClearInteriorTiles((const struct Window *)settingsWindow, CHOICE_X,
                                40, FIRST_CHOICE_RIGHT, 48);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 40);
      x = work->option[3] + (s32)MsgKey1141;
      Window_ClearInteriorTiles((const struct Window *)settingsWindow, CHOICE_X,
                                64, CHOICE_RIGHT, 72);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 64);
      x = work->option[4] + (s32)MsgKey1144;
      Window_ClearInteriorTiles((const struct Window *)settingsWindow, CHOICE_X,
                                88, CHOICE_RIGHT, 96);
      Text_DrawMessageAsTiles(x, settingsWindow, CHOICE_X, 88);
      WindowColor_Apply(*hue, *brightness);

      x = (s16)settingsWindow->x * 8;
      {
        s32 top = (s16)settingsWindow->y;

        y = (selectedOption * 3 + top) * 8 + 4;
      }
      if (selectedOption == 0)
        y += 8;
      MenuCursor_SetFar(work->cursor, x, y, 3);
      WindowSprite_ClearList(helpWindow);
      Text_DrawMessage(selectedOption + (s32)MsgKey1146, (s32)helpWindow, 0, 0);
    }
    MenuTitle_Bob(pair);
    wait(1);

    if (PRESSED & 4) {
      Sound_PlayFar(112);
      work->preset++;
      redraw = 1;
      if (work->preset > 5)
        work->preset = 0;
      *hue = gOptionMenu_FirstColorPresets[work->preset];
      *brightness = gOptionMenu_SecondColorPresets[work->preset];
      continue;
    }
    if (PRESSED & 9) {
      result = 0;
      Sound_PlayFar(112);
      break;
    }
    if (PRESSED & 2) {
      result = -1;
      Sound_PlayFar(113);
      break;
    }
    if (gInput.repeat & 64) {
      Sound_PlayFar(111);
      selectedOption--;
      redraw = 1;
    } else if (gInput.repeat & 128) {
      Sound_PlayFar(111);
      selectedOption++;
      redraw = 1;
    } else {
      if (gInput.repeat & 32) {
        Sound_PlayFar(111);
        work->option[selectedOption]--;
        redraw = 1;
      }
      if (gInput.repeat & 16) {
        Sound_PlayFar(111);
        work->option[selectedOption]++;
        redraw = 1;
      }
    }
  }

  Window_Close((struct Window *)helpWindow, 2);
  Window_Close((struct Window *)settingsWindow, 2);
  if (result == 0) {
    if (gMainRoutineWork.unk244[6] == 0 && work->option[4] == 1)
      FieldMessage_Show((s32)MsgKey1161, 1);
  }
  if (result == 0) {
    gMainRoutineWork.paletteGlow[0] = work->option[0];
    gMainRoutineWork.paletteGlow[1] = work->option[1];
    gMainRoutineWork.eventMode = work->option[2];
    gMainRoutineWork.battleFxEnabled = work->option[3];
    gMainRoutineWork.unk244[6] = work->option[4];
    gAutoSleepEnabled = gMainRoutineWork.unk244[6];
  } else {
    WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                      gMainRoutineWork.paletteGlow[1]);
  }
  SettingsMenu_Release();
  wait(1);
  return result;
}

struct ScreenPageWork {
  u8 unk000[0x574];
  u16 page;
  u8 unk576[0x2e];
  u8 panel[0x6c];
  s32 values[1];
};

void PauseMenu_AnimateCursor(void) {
  struct ScreenPageWork *work = Ram_HeapSlots->unkd0[0];

  MenuCursor_AdvanceFar((union OptionCursor *)work->panel);
  WindowSprite_Pulse((void *)work->values[work->page]);
}

/// Allocates the Pause menu's work block and starts its per-frame task.
/// The Pause menu shares the settings menu's heap slot and cannot run
/// concurrently: PauseMenu_Choose frees it before
/// PauseMenu_Run opens the settings menu. The task runs in the
/// frame pass.
void PauseMenu_Initialize(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(PauseMenu_AnimateCursor, TASK_PASS_VBLANK);
}

/// Stops the Pause menu's per-frame task and frees its work block.
/// The task goes first, since it reads the work block.
void PauseMenu_Release(void) {
  Task_Remove(PauseMenu_AnimateCursor);
  HeapSlot_Release(208);
}

struct WorkspaceScreen {
  u8 unk000[0x5a4];
  struct WindowSprite *cursor;
  u8 unk5a8[0x68];
  struct WindowSprite *icons[6];
};

extern u8 MsgSaveYourQuest[];
extern u8 MsgChangeSettings[];
extern u8 MsgRoughMenu[];
extern const s8 gMenu_WorkspaceIcons[];

s32 GameFlag_Test(s32 flag);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

struct RenderInput *PauseMenu_Open(void) {
  struct WorkspaceScreen *work;
  s32 rows;
  s32 first;
  s32 settingsOnly;
  s32 top;
  s32 height;
  struct RenderInput *win;
  s32 y;
  s32 msg;
  s32 i;

  rows = 3;
  work = Ram_HeapSlots->unkd0[0];
  settingsOnly = GameFlag_Test(0x17e);
  first = 0;
  if (settingsOnly != 0) {
    rows = 1;
    first = 2;
  }
  if (gDebugMode != 0)
    rows += 3;
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > 19) {
    top = 1;
    height = 19;
  }
  win = Window_Create(5, top, 20, height, 2);
  for (i = 1; i < rows; i++)
    Window_DrawDividerLine((struct Window *)win, 0, i * 3, 19, i * 3);
  y = 4;
  if (settingsOnly == 0) {
    msg = (s32)MsgSaveYourQuest;
    Text_DrawMessage(msg, (s32)win, 48, 4);
    msg++;
    Text_DrawMessage(msg, (s32)win, 48, 28);
    y = 52;
  }
  Text_DrawMessage((s32)MsgChangeSettings, (s32)win, 48, y);
  y += 24;
  if (gDebugMode != 0) {
    msg = (s32)MsgRoughMenu;
    Text_DrawMessage(msg, (s32)win, 48, y);
    y += 24;
    Text_DrawMessage(msg + 1, (s32)win, 48, y);
    y += 24;
    msg += 2;
    Text_DrawMessage(msg, (s32)win, 48, y);
  }
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     0x40000000, win, 0, 0);
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icons[i] =
        CommandIcon_Create(gMenu_WorkspaceIcons[first + i], 0, win, 12, y);
    y += 24;
  }
  return win;
}

// The heading's top, in pixels from the window's; from patch 1 it sits 8
// pixels lower.
#if TLA_PATCH_1
#define HEADING_Y (-16)
#else
#define HEADING_Y (-24)
#endif

struct WorkspaceChoiceScreen {
  u8 unk000[0x574];
  u16 page;
  u8 unk576[0x2e];
  u8 cursor[0x6c];
  struct WindowSprite *icons[6];
};

extern const s8 gMenu_WorkspaceIcons[];

void PauseMenu_Initialize(void);
void PauseMenu_Release(void);
struct RenderInput *PauseMenu_Open(void);
struct WindowSprite *MenuTitle_Create(s32 group, struct RenderInput *input,
                                      s32 x, s32 y);
void MenuTitle_Bob(struct WindowSprite *pair);
void Icon_PrepareActorFar(struct WindowSprite *icon);
void CommandIcon_Load(s32 index, s32 value, s32 flag);
void MenuCursor_SetPositionImmediateFar(void *cursor, s32 x, s32 y);
void MenuCursor_SetFar(void *cursor, s32 x, s32 y, s32 mode);

#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

/// Runs the Pause menu until the player picks an entry (enum
/// PauseMenuEntry) or cancels with B or START (-1).
/// The result counts in the full entry table even when only Change settings is
/// shown. The menu's window and work block are freed before it returns;
/// PauseMenu_Run (screens/debug.c) acts on the result.
s32 PauseMenu_Choose(s32 initial) {
  struct RenderInput *window;
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
  struct WorkspaceChoiceScreen *work;

  redraw = -1;
  selected = initial;
  rows = 3;
  settingsOnly = GameFlag_Test(0x17e);
  first = 0;
  PauseMenu_Initialize();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  window = PauseMenu_Open();
  pair = MenuTitle_Create(7, window, 40, HEADING_Y);
  if (settingsOnly != 0) {
    rows = 1;
    first = 2;
  }
  if (gDebugMode != 0)
    rows += 3;
  for (;;) {
    if (redraw != 0) {
      selected = (selected + rows) % rows;
      work->page = selected;
      i = 0;
      if (i < rows) {
        j = first;
        pauseIcons = gMenu_WorkspaceIcons;
        do {
          icon = work->icons[i];
          icon->sentinel = 251;
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
        MenuCursor_SetPositionImmediateFar(work->cursor, x, y);
      else
        MenuCursor_SetFar(work->cursor, x, y, 3);
      redraw = 0;
    }
    MenuTitle_Bob(pair);
    wait(1);
    if (KEYS_PRESSED & 1) {
      result = selected;
      Sound_PlayFar(112);
      break;
    }
    if (KEYS_PRESSED & 10) {
      result = -1;
      Sound_PlayFar(113);
      break;
    }
    if (gInput.repeat & 64) {
      Sound_PlayFar(111);
      selected--;
      redraw = 1;
      continue;
    }
    if (gInput.repeat & 128) {
      Sound_PlayFar(111);
      selected++;
      redraw = 1;
      continue;
    }
    if (gDebugMode != 0) {
      if (KEYS_PRESSED & 0x100) {
        result = 20;
        Sound_PlayFar(112);
        break;
      }
      if (KEYS_PRESSED & 0x200) {
        result = 21;
        Sound_PlayFar(112);
        break;
      }
    }
  }
  Window_Close((struct Window *)window, 2);
  PauseMenu_Release();
  wait(1);
  if (result >= 0)
    result += first;
  return result;
}

#undef GLOW_COLOR_SET
#undef SLIDER_PALETTE
#undef RESOURCE_SLOTS
#undef OPTION_WINDOW_X
#undef OPTION_WINDOW_WIDTH
#undef OPTION_DIVIDER_END
#undef OPTION_SLIDER_X
#undef OPTION_CHOICE_X
#undef HELP_WINDOW_X
#undef HELP_WINDOW_WIDTH
#undef SLIDER_X
#undef CHOICE_X
#undef CHOICE_RIGHT
#undef FIRST_CHOICE_RIGHT
#undef SETTINGS_HEADING
#undef PRESSED
#undef KEYS_PRESSED
#undef HEADING_Y
