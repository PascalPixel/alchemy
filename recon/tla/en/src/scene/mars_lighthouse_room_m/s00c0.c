#include "types.h"
#include "call.h"
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
  s32 base;
  s32 i;
  s8 busy;

  if (work->unk166 != 0)
    return;
  busy = work->unkca4;
  if (busy != 0)
    return;
  if (gData_020036c4 == -1) {
    Iwram_CopyWords(Data_020039bc, (void *)0x050001a0, 32);
    Iwram_CopyWords(gData_0200393c, (void *)0x05000100, 64);
    Iwram_CopyWords(gData_0200397c, gData_0200393c, 64);
    gData_020036c4 = 0;
  }
  base = gData_020036c4 / 8 % 6;
  Iwram_CopyWords(gData_0200397c, Data_020039bc, 32);
  if (entrance == 1 && Flag_IsSet(SPLIT_CONSTANT(0xa00, 50))) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 10];
  }
  if (entrance == 2 && Flag_IsSet(SPLIT_CONSTANT(0xa00, 51))) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 20];
  }
  if (entrance == 3 && Flag_IsSet(SPLIT_CONSTANT(0xa00, 52))) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 26];
  }
  if (entrance == 4 && Flag_IsSet(SPLIT_CONSTANT(0xa00, 53))) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 4];
  }
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)gData_0200397c, 0x84000008);
  gData_020036c4++;
}
