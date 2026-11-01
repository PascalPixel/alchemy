// Draft, differ 6 of 222, skeleton gap 3/3 (tla-en, 2026-11-01)
// Twin of ☀️'s PauseMenu_Choose (games/tbs/src/window/window/settings_menu.c),
// the Pause menu's selection loop, here starting from `initial`; in debug
// mode L and R pick entries 20 and 21. Taken from the twin: declaration order,
// rows before first. The reload registers rotate r1, r2, r3, r0, so redraw's
// -1 is set while r0 still holds the parameter: `selected` is copied from it
// afterwards, which closed the reload registers (differ 24 to 6).
// Left: two scheduling pairs, no register differs. The native issues the
// title's -16 ahead of storing the window, and stores first = 2 ahead of
// rows = 1, though rows must be reloaded first for r0 and r1 to come out so.
// Tried without gain: y or top holding -16, y set before the window call
// (differ 90), the assignment inside the argument, first before rows (10),
// separate tests of settingsOnly, an inverted goto, first from rows, a
// ternary (37), wait before the work read (10), twelve declaration orders
// (none below 6). Earlier permuter minutes found nothing lower either.
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
  struct WindowSprite *icons[6];
};

extern const s8 gMenu_WorkspaceIcons[];

s32 GameFlag_Test(s32 flag);
void Func_080405dc(void);
void Func_08040614(void);
struct RenderInput *Func_08040628(void);
struct WindowSprite *RenderResource_CreatePair(s32 group,
                                               struct RenderInput *input,
                                               s32 x, s32 y);
void Func_08045018(struct WindowSprite *pair);
void Icon_PrepareActorFar(struct WindowSprite *icon);
void RenderResource_LoadCommandIcon(s32 index, s32 value, s32 flag);
void Func_08108048(void *cursor, s32 x, s32 y);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void Window_Close(struct RenderInput *window, s32 release);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

s32 Func_08040798(s32 initial) {
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
  struct WorkspaceScreen *work;

  redraw = -1;
  selected = initial;
  rows = 3;
  settingsOnly = GameFlag_Test(0x17e);
  first = 0;
  Func_080405dc();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  window = Func_08040628();
  pair = RenderResource_CreatePair(7, window, 40, -16);
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
          RenderResource_LoadCommandIcon(*(s8 *)(j + (s32)pauseIcons), resourceIndex,
                                   dimmed);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = (s16)window->x * 8;
      y = (s16)top;
      y = (selected * 3 + y) * 8 + 16;
      if (-1 == redraw)
        Func_08108048(work->cursor, x, y);
      else
        Func_08108040(work->cursor, x, y, 3);
      redraw = 0;
    }
    Func_08045018(pair);
    wait(1);
    if (PRESSED & 1) {
      result = selected;
      Sound_PlayFar(112);
      break;
    }
    if (PRESSED & 10) {
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
      if (PRESSED & 0x100) {
        result = 20;
        Sound_PlayFar(112);
        break;
      }
      if (PRESSED & 0x200) {
        result = 21;
        Sound_PlayFar(112);
        break;
      }
    }
  }
  Window_Close(window, 2);
  Func_08040614();
  wait(1);
  if (result >= 0)
    result += first;
  return result;
}
