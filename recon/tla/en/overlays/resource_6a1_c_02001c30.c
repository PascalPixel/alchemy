// Draft, differ 60 of 131: overlay 6a1's copy of the function drafted in
// resource_698_c_020014dc.c (overlay 698's Func_020014dc), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"

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

struct Actor *Actor_GetById(s32 id);
void Func_02001b38(void);
void Func_020022c8(s32 sound);
void Func_020021a8(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Actor_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
#include "io_write.h"

void Func_02001c30(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Func_02001b38();
  Func_020022c8(229);
  Func_020021a8(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  x = 0;
  table->shown = 1;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  do {
    if (x == 5)
      Func_020022c8(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    x++;
    wait(1);
  } while (x <= 39);
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  table->shown = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
