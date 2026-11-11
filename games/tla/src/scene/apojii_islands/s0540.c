#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"
#include "event_inline.h"

extern s32 gData_020024c8;
extern u16 Data_020029d8[], Data_020029f8[], Data_02002a18[], Data_02002a38[];

void Func_02000540(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c8 == -1) {
    Call3((void (*)())Iwram_CopyWords, (s32)Data_020029d8, 0x05000060, 32);
    Iwram_CopyWords(Data_020029f8, Data_020029d8, 32);
    Iwram_CopyWords(Data_02002a18, (void *)0x050001a0, 32);
    Iwram_CopyWords(Data_02002a38, Data_02002a18, 32);
    gData_020024c8 = busy;
  }
  phase = gData_020024c8 / 8 % 4;
  for (k = 0; k <= 3; k++) {
    /* FAKEMATCH(dead): the phase and the first colour in a once-only block,
     * whose end holds the order around it; as plain statements, 6 of 152
     * instructions differ in tla-en. */
    do {
      j = (phase + k) % 4;
      Data_020029f8[12 + j] = Data_020029d8[12 + k];
    } while (0);
    Data_02002a38[12 + j] = Data_02002a18[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)Data_020029f8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)Data_02002a38, 0x84000008);
  gData_020024c8++;
}
