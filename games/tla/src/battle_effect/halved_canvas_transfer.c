#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "irq_lock.h"

struct BattleFxFadeWork {
  s32 unk00;
  s32 halvePending;
  s32 framesLeft;
  s32 step;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);

/// Frame callback: when a transfer is pending, writes the canvas to BG
/// character block 1 at half brightness.
void BattleEffect_TransferHalvedCanvas(void) {
  struct BattleFxFadeWork *work =
      (struct BattleFxFadeWork *)Ram_HeapSlots->battleFxWork[0];
  void *buffer = Ram_HeapSlots->battleFxWork[1];
  u32 ime;

  IRQ_LOCK(ime);
  if (work->halvePending == 1) {
    work->halvePending = 0;
    IRQ_UNLOCK(ime);
    {
      void *destination = (void *)0x06004000;

      /* FAKEMATCH: the native loads the destination before it starts the size
       * (differ 2 without the barrier). */
      __asm__ volatile("" : "+r"(destination));
      ColorBuffer_Halve(buffer, destination, 0x4000);
    }
  } else {
    IRQ_UNLOCK(ime);
  }
}
