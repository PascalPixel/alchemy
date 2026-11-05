// Draft, differ 134 (tla-en, 2026-11-05). Func_020003c4 of overlay 6ac.
// Direct array reads under a plain loop give the listing's shape (strh
// through base and index, offsets chained). A rise from 97 with the barriers
// gone. Left: rank: the table holds r5 where natively busy does, i r4 saved
// around calls, at r7, the table r6.
#include "types.h"
#include "call.h"
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
  for (i = 0; i <= 5; i++) {
    s32 j = (base + i) % 6;

    Data_02003a1c[i + 4] = Data_020039dc[j + 4];
    Data_02003a1c[i + 10] = Data_020039dc[j + 10];
    Data_02003a1c[i + 20] = Data_020039dc[j + 20];
    Data_02003a1c[i + 26] = Data_020039dc[j + 26];
  }
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
