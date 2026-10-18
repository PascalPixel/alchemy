// Draft, differ 21 of 115: the debug warp menu's selection loop, a sibling of
// Func_08040d70 over nine rows 16 pixels apart: Up and Down move the cursor,
// A runs the chosen preset's warp from gDebugWarp_Presets, B cancels (-1).
// Left: the native reads gInput.pressed again for the A test where the draft
// reuses the B test's load (no volatile view added), the window's copy to r8
// placed after building -1 instead of before, and the closing -1 and warp
// pointer in r0 rather than r4/r3 (register choice). Tried: y written with
// multiplies (combine folds it into (y + index * 2) * 8, differ 35).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "uiwindow.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84];
};

struct DebugWarpPreset {
  const u8 *name;
  void (*warp)(void);
};

extern const struct DebugWarpPreset gDebugWarp_Presets[];

void Func_0804101c(void);
void Func_08041054(void);
struct UiWindow *Func_08041068(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void UiWork_Finalize(struct UiWindow *window, s32 release);

s32 Func_080410f8(s32 index) {
  struct ScreenWork *work;
  struct UiWindow *window;
  s32 moved;

  Func_0804101c();
  work = Ram_HeapSlots->unkd0[0];
  WaitFrames(1);
  window = Func_08041068();
  moved = -1;
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
    if (gInput.pressed & 2) {
      Audio_PlayCue(113);
      index = -1;
      goto done;
    }
    if (gInput.pressed & 1) {
      Audio_PlayCue(112);
      goto done;
    }
    if (gInput.repeat & 64) {
      index--;
      moved = 1;
      Audio_PlayCue(111);
    }
    if (gInput.repeat & 128) {
      index++;
      moved = 1;
      Audio_PlayCue(111);
    }
  } while (1);
done:
  UiWork_Finalize(window, 2);
  Func_08041054();
  WaitFrames(1);
  if (index != -1)
    gDebugWarp_Presets[index].warp();
  return index;
}
