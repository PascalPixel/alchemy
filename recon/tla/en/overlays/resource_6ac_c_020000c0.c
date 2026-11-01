// Draft, differ 206 of 230, skeleton gap 31/15 (tla-en, 2026-11-01)
// Overlay 6ac's palette cycle, the twin of Func_020003c4: while no event runs,
// it saves palette rows 13, 8 and 9 the first time, then every 8 calls restores
// row 13 and, on entrances 1 to 4 with flags 0xa32 to 0xa35 set, rotates six
// colours of the row, and queues it for DMA. The whole gap left is register
// pressure: the native keeps r5 and r6 free until the last two loops, so
// IwramCopyWords' address is reloaded per call, gData_0200397c per pass in the
// first two loops, and entrance, base and the pointers sit in r8 to r11, where
// every use costs a mov. Here IwramCopyWords stays in r5 and base in r7.
// Measured: the goto loops give the shape (real loops hoist both tables or
// reduce the offset to a pointer: do-while differ 200, gap 34/22); src and dst
// are function-level so their second set keeps them in a register over the
// __modsi3 call; the ldrb and shifts come from the byte landing in a high
// register, not from its type; volatile on the counter also gives its reload.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "flag_call.h"
#include "system/main_routine_work.h"

struct Overlay6acEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

extern s32 gData_020036c4;
extern u16 gData_0200393c[];
extern u16 gData_0200397c[];
extern u16 Data_020039bc[];

void Func_020000c0(void) {
  s32 entrance = gMainRoutineWork.entrance;
  struct Overlay6acEventWork *work = Ram_HeapSlots->eventWork;
  s32 busy;
  s32 base;
  u8 *src;
  u8 *dst;

  if (work->unk166 != 0)
    return;
  busy = (s8)work->unkca4;
  if (busy != 0)
    return;
  // Read through a pointer type: as s32, gcse drops the reload below.
  if (*(void **)&gData_020036c4 == (void *)-1) {
    Iwram_CopyWords(Data_020039bc, (void *)0x050001a0, 32);
    Iwram_CopyWords(gData_0200393c, (void *)0x05000100, 64);
    Iwram_CopyWords(gData_0200397c, gData_0200393c, 64);
    gData_020036c4 = busy;
  }
  base = gData_020036c4 / 8 % 6;
  Iwram_CopyWords(gData_0200397c, Data_020039bc, 32);
  if (entrance == 1 && Flag_IsSet(0xa32)) {
    s32 at;
    s32 i;

    at = 20;
    i = 0;
  next1:
    {
      s32 j;

      src = (u8 *)gData_0200393c;
      j = (base + i) % 6 * 2 + 20;
      *(u16 *)((u8 *)gData_0200397c + at) = *(u16 *)(src + j);
    }
    i++;
    at += 2;
    if (i <= 5)
      goto next1;
  }
  if (entrance == 2 && Flag_IsSet(0xa33)) {
    s32 at;
    s32 i;

    at = 20;
    i = 0;
  next2:
    {
      s32 j;

      src = (u8 *)gData_0200393c;
      j = (base + i) % 6 * 2 + 40;
      *(u16 *)((u8 *)gData_0200397c + at) = *(u16 *)(src + j);
    }
    i++;
    at += 2;
    if (i <= 5)
      goto next2;
  }
  if (entrance == 3 && Flag_IsSet(0xa34)) {
    s32 at;
    s32 i;

    i = 0;
    at = 20;
  next3:
    {
      s32 j;

      dst = (u8 *)gData_0200397c;
      src = (u8 *)gData_0200393c;
      j = (base + i) % 6 * 2 + 52;
      *(u16 *)(dst + at) = *(u16 *)(src + j);
    }
    i++;
    at += 2;
    if (i <= 5)
      goto next3;
  }
  if (entrance == 4 && Flag_IsSet(0xa35)) {
    s32 at;
    s32 i;

    i = 0;
    at = 20;
  next4:
    {
      s32 j;

      dst = (u8 *)gData_0200397c;
      src = (u8 *)gData_0200393c;
      j = (base + i) % 6 * 2 + 8;
      *(u16 *)(dst + at) = *(u16 *)(src + j);
    }
    i++;
    at += 2;
    if (i <= 5)
      goto next4;
  }
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)gData_0200397c, 0x84000008);
  gData_020036c4++;
}
