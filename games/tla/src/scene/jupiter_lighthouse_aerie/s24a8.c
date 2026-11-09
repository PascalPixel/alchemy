#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"

struct CellTargetEntry;

struct CellTarget {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

void Func_020023b0(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
#include "io_write.h"

void Func_020024a8(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Func_020023b0();
  Event_PlaySound(229);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  x = 0;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  do {
    if (x == 5)
      Event_PlaySound(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    System_Wait(1);
    x++;
  } while (x <= 39);
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  table->shown = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
