#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "uiwindow.h"
#include "uiflow.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84];
};

void Func_08040c78(void);
void Func_08040cb0(void);
struct Window *Func_08040cc4(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);

s32 Func_08040d70(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;

  Func_08040c78();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  moved = -1;
  window = Func_08040cc4();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 5) % 5;
      x = (s16)window->x * 8 - 4;
      // The row is read first: as one expression, 4 of 105 instructions differ.
      y = (s16)window->y;
      y = (index * 3 + y) * 8 + 16;
      if (moved == -1)
        Func_08108048(work->panel, x, y);
      else
        Func_08108040(work->panel, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      index = -1;
      goto done;
    }
    if (gInput.pressed & 1) {
      Sound_PlayFar(112);
      goto done;
    }
    if (gInput.repeat & 64) {
      index--;
      moved = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 128) {
      index++;
      moved = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  Window_Close(window, 2);
  Func_08040cb0();
  wait(1);
  return index;
}
