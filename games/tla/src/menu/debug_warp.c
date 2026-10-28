/// @file
/// The debug warp menu's selection loop.

#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "io_reg.h"
#include "uiwindow.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84]; ///< the cursor's panel
};

/// One row of the debug warp menu: its name and what choosing it does.
struct DebugWarpPreset {
  const u8 *name;
  /// The handlers return nothing; see the FAKEMATCH tag in DebugWarp_Run.
  s32 (*warp)(void);
};

extern const struct DebugWarpPreset gDebugWarp_Presets[];

void Func_0804101c(void);
void Func_08041054(void);
struct Window *Func_08041068(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);
void WaitFrames(s32 frames);
void Sound_PlayFar(s32 cue);
void Window_Close(struct Window *window, s32 release);

/// Shows the nine debug warp presets with the cursor on row index, lets Up
/// and Down move it (wrapping), and returns the row A chooses after running
/// its handler, or -1 for B.
s32 DebugWarp_Run(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;
  u32 down;

  Func_0804101c();
  work = Ram_HeapSlots->unkd0[0];
  WaitFrames(1);
  // -1 places the cursor at once; 1 slides it to a new row.
  moved = -1;
  window = Func_08041068();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 9) % 9;
      x = (s16)window->x * 8 - 4;
      y = ((s16)window->y << 3) + (index << 4) + 12;
      if (moved == -1)
        Func_08108048(work->panel, x, y);
      else
        Func_08108040(work->panel, x, y, 3);
      moved = 0;
    }
    WaitFrames(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      index = -1;
      goto done;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      goto done;
    }
    if (gInput.repeat & KEY_UP) {
      index--;
      moved = 1;
      Sound_PlayFar(111);
    }
    /* FAKEMATCH(temporary): the Down mask kept in a variable; as
     * gInput.repeat & KEY_DOWN, 32 of 115 instructions differ in tla-en. */
    down = KEY_DOWN;
    if (gInput.repeat & down) {
      index++;
      moved = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  Window_Close(window, 2);
  Func_08041054();
  WaitFrames(1);
  /* FAKEMATCH(signature): the handlers are typed as returning s32, which
   * puts the handler in r0 for the call; as void (*)(void), 4 of 115
   * instructions differ in tla-en. */
  if (index != -1)
    gDebugWarp_Presets[index].warp();
  return index;
}
