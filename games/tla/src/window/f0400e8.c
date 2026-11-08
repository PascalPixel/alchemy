#include "types.h"
#include "edition.h"
#include "version.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "main_routine_work.h"
#include "render_input.h"

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
extern const s8 gOptionMenu_SpeedIcons[], gOptionMenu_SpeechIcons[],
    gOptionMenu_SleepIcons[];
extern const u8 gOptionMenu_FirstColorPresets[],
    gOptionMenu_SecondColorPresets[];

void Func_0803fc38(void);
void Func_0803fd14(void);
struct RenderInput *Menu_OpenWorkspaceOptions(void);
void Func_0803f9c0(s32 hue, s32 brightness);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                  s32 style);
struct WindowSprite *
RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y);
void Func_08045018(struct WindowSprite *pair);
void Icon_PrepareActorFar(struct WindowSprite *icon);
void RenderResource_LoadCommandIcon(s32 index, s32 value, s32 flag);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void Window_Close(struct RenderInput *window, s32 release);
void Window_ClearInteriorTiles(struct RenderInput *window, s32 x0, s32 y0,
                               s32 x1, s32 y1);
void Text_DrawCharacterAtOffset(s32 message, struct RenderInput *window,
                                u32 offsetX, u32 offsetY);
void WindowSprite_ClearList(struct RenderInput *window);
void Text_DrawMessage(s32 message, struct RenderInput *window, s32 x, s32 y);
void FieldMessage_Show(s32 message, s32 mode);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

s32 Func_080400e8(void) {
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
  Func_0803fc38();
  work = Ram_HeapSlots->unkd0[0];
  helpWindow = Window_Create(HELP_WINDOW_X, 2, HELP_WINDOW_WIDTH, 3, 2);
  settingsWindow = Menu_OpenWorkspaceOptions();
  pair = RenderResource_CreatePair(SETTINGS_HEADING, settingsWindow, 64, -48);
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
        RenderResource_LoadCommandIcon(gOptionMenu_SpeedIcons[i], resourceIndex,
                                       dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[1][i];
        icon->sentinel = 0xfb;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[1][i]->index;
        dimmed = 0;
        if (i != work->option[3])
          dimmed = 1;
        RenderResource_LoadCommandIcon(gOptionMenu_SpeechIcons[i],
                                       resourceIndex, dimmed);
      }
      for (i = 0; i <= 1; i++) {
        icon = work->choiceIcons[2][i];
        icon->sentinel = 0xfb;
        Icon_PrepareActorFar(icon);
        resourceIndex = (u8)work->choiceIcons[2][i]->index;
        dimmed = 0;
        if (i != work->option[4])
          dimmed = 1;
        RenderResource_LoadCommandIcon(gOptionMenu_SleepIcons[i], resourceIndex,
                                       dimmed);
      }

      x = (s16)settingsWindow->x * 8 + SLIDER_X;
      x += (*hue * 60) / work->optionCount[0];
      y = (s16)settingsWindow->y * 8 + 4;
      Func_08108040(work->marker[0], x, y, 1);
      x = (s16)settingsWindow->x * 8 + SLIDER_X;
      x += (*brightness * 60) / work->optionCount[1];
      y = (s16)settingsWindow->y * 8 + 20;
      Func_08108040(work->marker[1], x, y, 1);

      x = work->option[2] + (s32)MsgKey113b;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 40,
                                FIRST_CHOICE_RIGHT, 48);
      Text_DrawCharacterAtOffset(x, settingsWindow, CHOICE_X, 40);
      x = work->option[3] + (s32)MsgKey1141;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 64, CHOICE_RIGHT, 72);
      Text_DrawCharacterAtOffset(x, settingsWindow, CHOICE_X, 64);
      x = work->option[4] + (s32)MsgKey1144;
      Window_ClearInteriorTiles(settingsWindow, CHOICE_X, 88, CHOICE_RIGHT, 96);
      Text_DrawCharacterAtOffset(x, settingsWindow, CHOICE_X, 88);
      Func_0803f9c0(*hue, *brightness);

      x = (s16)settingsWindow->x * 8;
      {
        s32 top = (s16)settingsWindow->y;

        y = (selectedOption * 3 + top) * 8 + 4;
      }
      if (selectedOption == 0)
        y += 8;
      Func_08108040(work->cursor, x, y, 3);
      WindowSprite_ClearList(helpWindow);
      Text_DrawMessage(selectedOption + (s32)MsgKey1146, helpWindow, 0, 0);
    }
    Func_08045018(pair);
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

  Window_Close(helpWindow, 2);
  Window_Close(settingsWindow, 2);
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
    Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                  gMainRoutineWork.paletteGlow[1]);
  }
  Func_0803fd14();
  wait(1);
  return result;
}
