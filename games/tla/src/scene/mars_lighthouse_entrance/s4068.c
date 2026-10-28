#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
struct Actor *Func_02004d28(void);

void Func_02004068(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads x and z before
   * it copies the object to r10 (one reordered row). */
  do {
  } while (0);
  x >>= 20;
  z >>= 20;
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, (x << 4) + 8,
                                    (z << 4) + 8);
  wait(1);
  Actor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8), object->y,
                                   (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  Func_02004d28()->y -= 0x80000;
}

void Func_02004de0(s32 sound);
/// Map_CopyCellsRect through this overlay's import.
void Func_02004c20(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);

/// The cells Func_02004490 watches, and the cell it copies over the one the
/// leader steps on.
struct CellTarget {
  u8 unk00[6];
  s16 x; ///< column of the copied cell, whose row is z + 1
  s16 z;
};
struct CellTargetEntry;

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02004490 calls it the first time the leader steps on
/// one of the table's cells.
void Func_020040f0(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
  Func_02004068();
  Func_02004de0(161);
  Func_02004c20(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}
