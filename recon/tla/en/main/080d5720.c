// Near miss, differ 2 of 87 (tla-en, 2026-10-30): instruction order only, a
// sched2 priority no source form reaches. No empty asm before any one
// statement lowers it.
// The leader grabs the nearest workspace item
// (Func_080d5670): with none it returns 0; otherwise cue 152, the leader is
// moved onto the item and linked to it, its motion flags, sprite picture and
// priority are saved, it is drawn at priority 1, and the render mode becomes 3.
// Returns 1. No ☀️ twin found. Left: instruction order only: the native starts
// the priority flags' address (adds r0, r5; adds r0, #35) after the saved
// priority's store, this just before it. Reading the flags through a pointer
// taken before the frame copy brought it from 4 to 2 (permute, 2026-10-18).
// Why (sched2 dump, 2026-10-30): the address copy depends only on r0 and r5,
// so it is ready long before the store, and the copy, add, flags load chain
// ranks it one above the store (store 16, copy 17); the store could win only
// if its cost to the flags load were 2 (a tie), so the remaining order looks
// like the compiler's cost model. Reordering the tail's statements (differ 8 to
// 28), taking the flags pointer at each point (2 to 27), and moving the
// watched object's store or folding it into Actor_SetMode's argument (2 to
// 4) never beat 2; permute (4 minutes, 10295 candidates) found nothing.
// An empty asm barrier after the priority's store puts the address in place
// but moves the watched object's store (str r6, [r5, #104]) a slot earlier,
// wherever the barrier stands after it (differ 2): the barrier depends on
// every earlier use of r5 and r6, which gives that store a second dependent,
// as many as the mode's movs r1, #2 (tied in priority), and the earlier
// statement wins the tie. Empty asms taking the leader or the flags' address,
// with or without the stored value, measured 2 to 27; a one-pass block there
// changes the allocation (differ 29); volatile byte accesses 13 to 27. The
// event work halfword and the saved priority byte are struct fields: through
// cast pointers the halfword's zero comes from the pool and the byte store
// forces sprite byte 9 to be read twice (differ 44).
#include "types.h"
#include "field_sprite.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct Work9cItemPosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct Work9cItemPosition *Func_080d5670(s32 *position);
void Sound_PlayFar(s32 cue);

struct GrabSave {
  u8 unk00[98];
  u8 priority;
};

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

s32 Func_080d5720(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->animation;
  struct Work9cItemPosition *item = Func_080d5670(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, item->x, item->y, item->z);
  Actor_CommitPosition(leader);
  leader->action = leader->motionFlags;
  flags = &leader->priorityFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  gMainRoutineWork.renderMode = 3;
  return 1;
}
