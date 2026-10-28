#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

void Palette_StepFadeTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s32 *framesLeft;
  u8 *battle;

  /* FAKEMATCH: the native loads the work pointer before it starts the 0x77b4
   * offset (differ 2 without the barrier). */
  __asm__ volatile("" : "+r"(work));
  framesLeft = (s32 *)(work + 0x77b4);
  battle = (u8 *)Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = (s32 *)(work + 0x77b8);

    (*step)++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                         0x10000 - *step * 1092, 128);
    (*framesLeft)--;
  }
}
