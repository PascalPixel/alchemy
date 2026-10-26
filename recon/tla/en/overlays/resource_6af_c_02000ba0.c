// Draft, differ 96 of 152: rewritten from the listing. Treasure Isle's palette
// cycle, scheduled by Func_02000cf4: while no event runs, it saves the two
// four-colour runs at palette entries 0x30 and 0xa0 the first time, then every
// 8 calls rotates them by one and queues both for DMA. Left: the native keeps
// the event work's byte (which is 0 on this path, and is stored as the reset
// counter) in r8, loading it with ldrb and shifts, where this keeps it in r7
// and loads it with ldrsb; that moves the counter's address and every register
// after it, and the copy loop's index and offset trade registers.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"

struct TreasureIsleEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

extern s32 gData_02001aa4;
extern u16 Data_02001aa8[];
extern u16 gData_02001ac8[];
extern u16 Data_02001ae8[];
extern u16 Data_02001b08[];

void Func_02000ba0(void) {
  struct TreasureIsleEventWork *work = Ram_HeapSlots->eventWork;
  s32 phase;
  s32 i;
  s32 busy;

  if (work->unk166 != 0)
    return;
  busy = (s8)work->unkca4;
  if (busy != 0)
    return;
  if (gData_02001aa4 == -1) {
    Iwram_CopyWords(Data_02001aa8, (void *)0x05000060, 32);
    Iwram_CopyWords(gData_02001ac8, Data_02001aa8, 32);
    Iwram_CopyWords(Data_02001ae8, (void *)0x05000140, 32);
    Iwram_CopyWords(Data_02001b08, Data_02001ae8, 32);
    gData_02001aa4 = 0;
  }
  phase = gData_02001aa4 / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gData_02001ac8[(phase + i) % 4 + 2] = Data_02001aa8[i + 2];
    Data_02001b08[(phase + i) % 4 + 2] = Data_02001ae8[i + 2];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gData_02001ac8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000140, (u32)Data_02001b08, 0x84000008);
  gData_02001aa4++;
}
