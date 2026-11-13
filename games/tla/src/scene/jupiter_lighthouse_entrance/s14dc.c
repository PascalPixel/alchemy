/// @file
/// Scene code of overlay 0x698, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_entrance.h"

struct CellTargetEntry;

/* VIEW(s14dc): tag struct CellTarget as struct CellTarget__s14dc; cell_target.h has another definition; tried with the definition the rest of the object has, Teleporter_Sink moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s14dc {
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

#include "io_write.h"

void Teleporter_Sink(struct CellTarget__s14dc *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Leader_CenterInCell();
  Event_PlaySound(229);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->facing = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  do {
    /* FAKEMATCH(dead): the flag cleared behind a loop-end note, so the
     * table is copied out of r8 after the actor's byte store, as in the
     * native; as table->shown = 0, 16 of 131 instructions differ in
     * tla-en. */
    table->shown = 0;
  } while (0);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
