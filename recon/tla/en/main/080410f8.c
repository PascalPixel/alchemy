// Near miss, differ 2 of 115 (38 before): the debug warp menu's selection
// loop, a sibling of Func_08040d70 over nine rows 16 pixels apart: Up and
// Down move the cursor, A runs the chosen preset's warp from
// gDebugWarp_Presets, B cancels (-1). Two devices to tag on adoption, found
// by alchemy permute and by hand: the Down mask as a temporary (temporary;
// 38 to 6) and the presets' handlers typed as returning s32, which puts the
// handler in r0 for the call (signature; 6 to 2; they return nothing).
// Left: the native builds the -1 fully (movs; negs) before copying the
// window to r8; this copies between them. The window taken through a
// temporary an asm rewrites after the -1 hoists the -1 above the calls
// (differ 23). Tried: y written with multiplies (combine folds it into
// (y + index * 2) * 8, differ 35).
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
  s32 (*warp)(void);
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
  u32 down;

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
    down = 128;
    if (gInput.repeat & down) {
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
