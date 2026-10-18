// Draft, differ 103 of 222, 2026-10-18: written from the listing; the
// remaining differences are register choices (the redraw flag, the row count
// and the window spilled differently) and the order of the input tests'
// constants.
// The system menu's selection loop, as ☀️'s
// Menu_RunWorkspaceSelectionLoop (games/tbs/src/window/window.c), starting
// from `selected`; in debug mode L and R pick entries 20 and 21.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "render_input.h"

struct WorkspaceScreen {
  u8 unk000[0x574];
  u16 page;
  u8 unk576[0x2e];
  u8 cursor[0x6c];
  struct RenderOutput *icons[6];
};

extern const s8 gMenu_WorkspaceIconFrames[];

s32 GameFlag_Test(s32 flag);
void Func_080405dc(void);
void Func_08040614(void);
struct RenderInput *Func_08040628(void);
struct RenderOutput *RenderResource_CreatePair(s32 group,
                                               struct RenderInput *input,
                                               s32 x, s32 y);
void Func_08045018(struct RenderOutput *pair);
void UiIcon_PrepareObjectFar(struct RenderOutput *icon);
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
void Func_08108048(void *cursor, s32 x, s32 y);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void UiWork_Finalize(struct RenderInput *window, s32 release);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

s32 Func_08040798(s32 selected) {
  struct RenderInput *window;
  s32 redraw;
  struct RenderOutput *pair;
  s32 first;
  s32 i;
  s32 j;
  s32 rows;
  s32 settingsOnly;
  s32 dimmed;
  s32 result;
  struct RenderOutput *icon;
  struct WorkspaceScreen *work;
  const s8 *iconFrames;
  s32 resourceIndex;
  s32 top;
  s32 x;
  s32 y;

  redraw = -1;
  rows = 3;
  settingsOnly = GameFlag_Test(0x17e);
  first = 0;
  Func_080405dc();
  work = Ram_HeapSlots->unkd0[0];
  WaitFrames(1);
  window = Func_08040628();
  pair = RenderResource_CreatePair(7, window, 40, -16);
  if (settingsOnly != 0) {
    first = 2;
    rows = 1;
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
        iconFrames = gMenu_WorkspaceIconFrames;
        do {
          icon = work->icons[i];
          icon->sentinel = 251;
          UiIcon_PrepareObjectFar(icon);
          resourceIndex = (u8)icon->index;
          dimmed = 0;
          if (i != work->page)
            dimmed = 1;
          RenderResource_LoadFrame(*(s8 *)(j + (s32)iconFrames), resourceIndex,
                                   dimmed);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = (s16)window->x * 8;
      y = ((selected * 3) + (s16)top) * 8 + 16;
      if (-1 == redraw)
        Func_08108048(work->cursor, x, y);
      else
        Func_08108040(work->cursor, x, y, 3);
      redraw = 0;
    }
    Func_08045018(pair);
    WaitFrames(1);
    if (PRESSED & 1) {
      result = selected;
      Audio_PlayCue(112);
      break;
    }
    if (PRESSED & 10) {
      result = -1;
      Audio_PlayCue(113);
      break;
    }
    if (gInput.repeat & 64) {
      Audio_PlayCue(111);
      selected--;
      redraw = 1;
      continue;
    }
    if (gInput.repeat & 128) {
      Audio_PlayCue(111);
      selected++;
      redraw = 1;
      continue;
    }
    if (gDebugMode != 0) {
      if (PRESSED & 0x100) {
        result = 20;
        Audio_PlayCue(112);
        break;
      }
      if (PRESSED & 0x200) {
        result = 21;
        Audio_PlayCue(112);
        break;
      }
    }
  }
  UiWork_Finalize(window, 2);
  Func_08040614();
  WaitFrames(1);
  if (result >= 0)
    result += first;
  return result;
}
