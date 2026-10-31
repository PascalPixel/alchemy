// Near miss, differ 12 of 131 (tla-en, 2026-10-31): the base of
// gMainRoutineWork.currentOwner's address takes r3 where the native leaves it
// unallocated. Draft of overlay 698's Func_020014dc, in 18 overlays of the 698
// family. The leader's cell copied with sound 229, the party walks off it
// shrinking while REG_WINOUT shows the sprite window, then the screen
// transition. The frame count is the column variable set to 0 by its loop
// and counted after the wait, which gives the leader r6 and the count r7 as
// the native has them (54 with the count set before the stores, 67 with a
// separate counter); as a for loop, a while loop or a goto loop it measures
// the same. Left (8 + 4): global.c ranks the base's pseudo (2 refs, live
// length 2 doubled by its constant REG_EQUIV) below the leader's (17 refs over
// 79 insns), so after the leader takes r6 from local-alloc's base-and-address
// pair the base is allocated again and gets r3; left unallocated (checked by
// skipping its allocation in the compiler under a debugger), reload builds it
// in r1 with the offset in r2 and the table's copy for table->x moves after
// the column shift, exactly as the native. Unknown: what C makes the base
// rank first or the leader rank below it. Tried without change: Actor_GetById
// taking u32 or int (s16 51, u16 30, u8 22), the declarations split or
// reordered in five orders, register on each local, the column expressions
// split or commuted (80 commuted), one or three parameters, the window write
// as a block (18) or plain (36), gMainRoutineWork through a local pointer
// (44), the owner's address in a local (66). The last flag store needs the
// one-pass block below: the native orders the actor's byte store before
// copying the table out of r8, as when the two stores may alias; tried
// without it: shown as u16 or volatile (46), stored through an s16 pointer,
// the chained assignment either way round (16, 20), after the event work
// store (20), the actor in a local (16), a zero temporary (36), the byte
// through a u8 pointer (21, the order right but its zero loaded again).
// Permuter 8 minutes: only the one-pass block (12).
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
void Func_020013e4(void);
void Func_02001b44(s32 sound);
void Func_02001a44(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Actor_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
#include "io_write.h"

void Func_020014dc(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Func_020013e4();
  Func_02001b44(229);
  Func_02001a44(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Func_02001b44(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    wait(1);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  do {
    /* FAKEMATCH(one-pass): the flag cleared behind a loop-end note, so the
     * table is copied out of r8 after the actor's byte store, as in the
     * native; as table->shown = 0, 16 of 131 instructions differ in
     * tla-en. */
    table->shown = 0;
  } while (0);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
