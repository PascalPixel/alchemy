/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s4ec8's offset up to the next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"
#include "cell_target.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_aerie_beacon.h"
#include "module/scene/scene.h"

void Func_02004d9c(void);

DEFINE_FUNC_02001250(Func_02004ec8, Data_02006b74, Func_02004d9c)

DEFINE_FUNC_020013E4(Func_0200510c)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02005534 calls it the first time the leader steps on
/// one of the table's cells.
void Func_02005194(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s16 sourceX;
  s32 leaderX = leader->x;
  s32 x;
  s32 z;

  /* FAKEMATCH(barrier): passes the source column through an asm reading the
   * leader's x, so the x load ties the 64 mask's copy and wins on order; as
   * (leader->x >> 20) | (s16)(table->x & 64), 2 of 49 instructions differ in
   * tla-en. */
  asm("" : "=r"(sourceX) : "0"(table->x), "r"(leaderX));
  x = (leaderX >> 20) | (s16)(sourceX & 64);
  z = CellTarget_CellWithBit(leader->z, table->z & 64);
  Func_0200510c();
  Event_PlaySound(161);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}

void Func_02005204(struct PushEvent *event) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = object->x >> 20;
  do {
  } while (0);
  x |= (s16)(event->x & mask);
  z = object->z >> 20;
  z |= (s16)(event->z & mask);
  Func_0200510c();
  Event_PlaySound(229);
  MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {
    u32 zero = 0;

    object->facing = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  {
    /* FAKEMATCH(temporary): a word local holding 0xf1f, built with movs,
     * lsls, adds as the native does; as a constant stored to the halfword
     * register, loaded from the pool, 20 of 131 instructions differ in tla-en.
     */
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(204);
    object->scaleX += -0x400;
    object->scaleY += -0x300;
    object->y += -0x9999;
    System_Wait(1);
  }
  do {
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
