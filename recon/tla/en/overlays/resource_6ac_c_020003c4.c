// Draft, differ 115 of 179: rewritten from the listing, its loop as a goto loop
// over byte offsets (the native reloads both tables' addresses on every pass).
// Room M's palette cycle, scheduled by Func_02000e84: while no event runs, it
// saves palette rows 2 and 8 the first time, then every 8 calls rotates four
// six-colour runs by one, restores the runs of the cells whose flags 0xa32 to
// 0xa35 are clear, and queues the row for DMA. Left, as in Treasure Isle's
// Func_02000ba0: the native reloads IwramCopyWords' address for each call and
// keeps the event work's byte (0 here, stored as the reset counter) in r5 and
// the counter's address in r7; here IwramCopyWords stays in r5 and every
// callee-saved register shifts (a forwarding inline for the copies changes
// nothing).
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "flag_call.h"

struct MarsLighthouseRoomMWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  s8 unkca4;
};

extern s32 gData_02003704;
extern u16 gData_02003a5c[];
extern u16 Data_020039dc[];
extern u16 Data_02003a1c[];
extern const u16 Data_0200ba70[];

void Func_020003c4(void) {
  struct MarsLighthouseRoomMWork *work = Ram_HeapSlots->eventWork;
  s32 busy;
  s32 base;
  s32 i;
  s32 at;

  if (work->unk166 != 0)
    return;
  busy = work->unkca4;
  if (busy != 0)
    return;
  if (gData_02003704 == -1) {
    Iwram_CopyWords(gData_02003a5c, (void *)0x05000040, 32);
    Iwram_CopyWords(Data_020039dc, (void *)0x05000100, 64);
    Iwram_CopyWords(Data_02003a1c, Data_020039dc, 64);
    gData_02003704 = busy;
  }
  base = gData_02003704 / 8 % 6;
  at = 8;
  i = 0;
next:
  {
    s32 j = (base + i) % 6 * 2;

    *(u16 *)((u8 *)Data_02003a1c + at) = *(u16 *)((u8 *)Data_020039dc + j + 8);
    *(u16 *)((u8 *)Data_02003a1c + at + 12) =
        *(u16 *)((u8 *)Data_020039dc + j + 20);
    *(u16 *)((u8 *)Data_02003a1c + at + 32) =
        *(u16 *)((u8 *)Data_020039dc + j + 40);
    *(u16 *)((u8 *)Data_02003a1c + at + 44) =
        *(u16 *)((u8 *)Data_020039dc + j + 52);
  }
  i++;
  at += 2;
  if (i <= 5)
    goto next;
  if (!Flag_IsSet(0xa32))
    Iwram_CopyWords(&Data_02003a1c[10], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa33))
    Iwram_CopyWords(&Data_02003a1c[20], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa34))
    Iwram_CopyWords(&Data_02003a1c[26], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa35))
    Iwram_CopyWords(&Data_02003a1c[4], Data_0200ba70, 12);
  IO_WRITE_QUEUE_PUSH(0x05000100, (u32)Data_02003a1c, 0x84000010);
  gData_02003704++;
}
