// Draft, differ 52 of 535 (145 before the saved settings were written as
// gPartyState's fields rather than through a byte pointer, 2026-10-25): the option screen, as ☀️'s
// Menu_RunWorkspaceOptions (games/tbs/src/window/window.c). The five messages
// (0x113b, 0x1141, 0x1144, 0x1146 and 0x1161 in English) are pool symbols the
// catalogs must name before adoption (MsgOptionA to MsgOptionE here).
// Remaining: mostly the native's loads issued ahead of constants (each frame
// loop's reload of the frame before the option offset, the slider rows), the
// cursor row's registers and the offsets of the cursor and the preset,
// which the native builds in the other register.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "party_state.h"
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
  struct RenderOutput *frame0[3];
  struct RenderOutput *frame1[2];
  struct RenderOutput *frame2[2];
};

extern u8 MsgOptionA[], MsgOptionB[], MsgOptionC[], MsgOptionD[], MsgOptionE[];
extern const s8 gOptionMenu_SpeedFrames[], gOptionMenu_SpeechFrames[],
    gOptionMenu_SleepFrames[];
extern const u8 gOptionMenu_FirstColorPresets[],
    gOptionMenu_SecondColorPresets[];

void Func_0803fc38(void);
void Func_0803fd14(void);
struct RenderInput *Func_0803fd28(void);
void Func_0803f9c0(s32 hue, s32 brightness);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
struct RenderOutput *RenderResource_CreatePair(s32 group,
                                               struct RenderInput *input,
                                               s32 x, s32 y);
void Func_08045018(struct RenderOutput *pair);
void UiIcon_PrepareObjectFar(struct RenderOutput *icon);
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void UiWork_Finalize(struct RenderInput *window, s32 release);
void UiWindow_ClearInteriorTiles(struct RenderInput *window, s32 x0, s32 y0,
                                 s32 x1, s32 y1);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window,
                                  u32 offsetX, u32 offsetY);
void RenderOutput_ClearList(struct RenderInput *window);
void UiText_DrawResource(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

s32 Func_080400e8(void) {
  struct OptionScreen *work;
  struct RenderInput *settingsWindow;
  struct RenderInput *helpWindow;
  struct RenderOutput *pair;
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
  struct RenderOutput *frame;

  redraw = 1;
  selectedOption = 0;
  Func_0803fc38();
  work = Ram_HeapSlots->unkd0[0];
  helpWindow = UiWindow_Create(1, 2, 28, 3, 2);
  settingsWindow = Func_0803fd28();
  pair = RenderResource_CreatePair(7, settingsWindow, 64, -48);
  WaitFrames(1);
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
        frame = work->frame0[i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame0[i]->index;
        dimmed = 0;
        if (i != work->option[2])
          dimmed = 1;
        RenderResource_LoadFrame(gOptionMenu_SpeedFrames[i], resourceIndex,
                                 dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame1[i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame1[i]->index;
        dimmed = 0;
        if (i != work->option[3])
          dimmed = 1;
        RenderResource_LoadFrame(gOptionMenu_SpeechFrames[i], resourceIndex,
                                 dimmed);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame2[i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        resourceIndex = (u8)work->frame2[i]->index;
        dimmed = 0;
        if (i != work->option[4])
          dimmed = 1;
        RenderResource_LoadFrame(gOptionMenu_SleepFrames[i], resourceIndex,
                                 dimmed);
      }

      x = (s16)settingsWindow->x * 8 + 140;
      x += (*hue * 60) / work->optionCount[0];
      y = (s16)settingsWindow->y * 8 + 4;
      Func_08108040(work->marker[0], x, y, 1);
      x = (s16)settingsWindow->x * 8 + 140;
      x += (*brightness * 60) / work->optionCount[1];
      y = (s16)settingsWindow->y * 8 + 20;
      Func_08108040(work->marker[1], x, y, 1);

      x = work->option[2] + (s32)MsgOptionA;
      UiWindow_ClearInteriorTiles(settingsWindow, 160, 40, 200, 48);
      UiText_DrawCharacterAtOffset(x, settingsWindow, 160, 40);
      x = work->option[3] + (s32)MsgOptionB;
      UiWindow_ClearInteriorTiles(settingsWindow, 160, 64, 184, 72);
      UiText_DrawCharacterAtOffset(x, settingsWindow, 160, 64);
      x = work->option[4] + (s32)MsgOptionC;
      UiWindow_ClearInteriorTiles(settingsWindow, 160, 88, 184, 96);
      UiText_DrawCharacterAtOffset(x, settingsWindow, 160, 88);
      Func_0803f9c0(*hue, *brightness);

      x = (s16)settingsWindow->x * 8;
      y = (selectedOption * 3 + (s16)settingsWindow->y) * 8 + 4;
      if (selectedOption == 0)
        y += 8;
      Func_08108040(work->cursor, x, y, 3);
      RenderOutput_ClearList(helpWindow);
      UiText_DrawResource(selectedOption + (s32)MsgOptionD, helpWindow, 0, 0);
    }
    Func_08045018(pair);
    WaitFrames(1);

    if (PRESSED & 4) {
      Audio_PlayCue(112);
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
      Audio_PlayCue(112);
      break;
    }
    if (PRESSED & 2) {
      result = -1;
      Audio_PlayCue(113);
      break;
    }
    if (gInput.repeat & 64) {
      Audio_PlayCue(111);
      selectedOption--;
      redraw = 1;
    } else if (gInput.repeat & 128) {
      Audio_PlayCue(111);
      selectedOption++;
      redraw = 1;
    } else {
      if (gInput.repeat & 32) {
        Audio_PlayCue(111);
        work->option[selectedOption]--;
        redraw = 1;
      }
      if (gInput.repeat & 16) {
        Audio_PlayCue(111);
        work->option[selectedOption]++;
        redraw = 1;
      }
    }
  }

  UiWork_Finalize(helpWindow, 2);
  UiWork_Finalize(settingsWindow, 2);
  if (result == 0) {
    if (gPartyState.unk244[6] == 0 && work->option[4] == 1)
      UiText_ShowPositionedMessageAndWait((s32)MsgOptionE, 1);
  }
  if (result == 0) {
    gPartyState.paletteGlow[0] = work->option[0];
    gPartyState.paletteGlow[1] = work->option[1];
    gPartyState.eventMode = work->option[2];
    gPartyState.battleFxEnabled = work->option[3];
    gPartyState.unk244[6] = work->option[4];
    gAutoSleepEnabled = gPartyState.unk244[6];
  } else {
    Func_0803f9c0(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  }
  Func_0803fd14();
  WaitFrames(1);
  return result;
}
