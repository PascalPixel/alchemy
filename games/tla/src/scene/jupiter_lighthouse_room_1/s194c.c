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

extern struct CellTarget gData_020023a0;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001abc(void);
void Func_02001704(void);

void Func_0200194c(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(gData_020023a0.flagByte);
  s32 dz = FieldGameFlag_GetByte(gData_020023a0.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_020023a0.flagByte, 0);
    GameFlag_SetByte(gData_020023a0.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02001abc();
    Func_02001704();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
        effect;
  }
}
