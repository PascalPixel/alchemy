#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"
#include "event_inline.h"

extern s32 gData_020024c4;
extern u16 Data_02002958[], Data_02002978[], gData_02002998[], gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  if (*(s8 *)(work + 0xca4) != 0)
    return;
  if (gData_020024c4 == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02002958, (void *)0x05000060, 32);
    Iwram_CopyWords(Data_02002978, Data_02002958, 32);
    Iwram_CopyWords(gData_02002998, (void *)0x05000080, 32);
    Iwram_CopyWords(gData_020029b8, gData_02002998, 32);
    gData_020024c4 = 0;
  }
  phase = gData_020024c4 / 8 % 4;
  k = 0;
  firstCopy = Data_02002978;
  secondCopy = gData_020029b8;
  for (; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = Data_02002958[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000080, (u32)secondCopy, 0x84000008);
  gData_020024c4++;
}
