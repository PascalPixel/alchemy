// Draft, differ 124 (tla-en, 2026-11-05). Func_020003c4 of overlay 6ac.
// Plain loops under -Os. Natively busy is r5, base r8, i r4 saved around
// __modsi3 in a slot, at r7 stepping 2 from 8 with r2 chained +12, +20, +12,
// and the table's address reloaded per pass, and a table pointer set in the
// loop stays live to the end. Left: that giv shape; here it uses a shift and
// displacements.
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
  s32 k;
  u16 *dst;

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
  for (k = 4, i = 0; i <= 5; i++, k++) {
    s32 j = (base + i) % 6;

    dst = Data_02003a1c;
    dst[k] = Data_020039dc[j + 4];
    dst[k + 6] = Data_020039dc[j + 10];
    dst[k + 16] = Data_020039dc[j + 20];
    dst[k + 22] = Data_020039dc[j + 26];
  }
  if (!Flag_IsSet(0xa32))
    Iwram_CopyWords(&dst[10], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa33))
    Iwram_CopyWords(&dst[20], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa34))
    Iwram_CopyWords(&dst[26], Data_0200ba70, 12);
  if (!Flag_IsSet(0xa35))
    Iwram_CopyWords(&dst[4], Data_0200ba70, 12);
  IO_WRITE_QUEUE_PUSH(0x05000100, (u32)dst, 0x84000010);
  gData_02003704++;
}
