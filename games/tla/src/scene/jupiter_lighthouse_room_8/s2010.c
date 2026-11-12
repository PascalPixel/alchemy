#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "cell_target.h"

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

extern struct CellTarget gData_02002bf8;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002188(void);
void Func_02001dc8(void);

void Func_02002010(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = ((s32)FieldGameFlag_GetByte(gData_02002bf8.flagByte));
  s32 dz = ((s32)FieldGameFlag_GetByte(gData_02002bf8.flagByte + 8));

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02002bf8.flagByte, 0);
    GameFlag_SetByte(gData_02002bf8.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02002188();
    Func_02001dc8();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}
