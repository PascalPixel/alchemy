#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "cell_target.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_0200015c(struct Actor *object);
void Func_020004dc(s32 mode);
void Func_02001d3c(s32 id);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_020015cc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x;
  s32 z = leader->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);

  x >>= 20;
  z >>= 20;
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, (x << 4) + 8,
                             (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(leader, (x << 20) + PIXELS(8), leader->y,
                                  (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  FieldCamera_GetActor()->y -= 0x80000;
}

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_020019f4 calls it the first time the leader steps on
/// one of the table's cells.
void Func_02001654(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s16 sourceX;
  s32 leaderX = leader->x;
  s16 sourceZBit;
  s32 x;
  s32 z;

  /* FAKEMATCH(barrier): passes the source column through an asm reading the
   * leader's x, so the x load ties the 64 mask's copy and wins on order; as
   * (leader->x >> 20) | (s16)(table->x & 64), 2 of 49 instructions differ in
   * tla-en. */
  asm("" : "=r"(sourceX) : "0"(table->x), "r"(leaderX));
  x = (leaderX >> 20) | (s16)(sourceX & 64);
  /* FAKEMATCH(temporary): the row's 64 bit in a local of its own; as
   * (s16)(table->z & 64) in the expression, 16 of 49 instructions differ in
   * tla-en. */
  sourceZBit = table->z & 64;
  z = (leader->z >> 20) | sourceZBit;
  Func_020015cc();
  Event_PlaySound(161);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}
