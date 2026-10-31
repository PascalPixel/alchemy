// Near miss, differ 2 of 87 (tla-en, 2026-10-30): scheduler order only. The
// native issues the saved priority's strb before the flags' address (adds r0,
// r5; adds r0, #35). sched2 ranks that address 17 and the store 16: the
// address is two insns from the flags' ldrb, the store one (a cost-1
// store-to-load link). At equal rank the store would win, with two dependents
// to the address's one. So the native's cost model charged that link more;
// no source form reaches it. Measured 2026-10-30: plain flags reads 4, the
// pointer taken after the priority store, a local for the priority or 0xfe
// for ~1 still 2, the frame cleared before the flags 6, the frame and priority
// stored through one save struct 12 to 20; permute (4 minutes) found nothing
// better. Under -mthumb-slow-loads (2026-10-26) no empty asm before any one
// statement lowers it.
// Func_080d5720 for the heap slot 0xa0
// workspace's nodes (Func_080d5930), the render mode becoming 4: the leader
// grabs the nearest node, saving its motion flags, frame and priority.
// Returns 1, or 0 with none near. Left: as in 080d5720, instruction order
// only: the native starts the priority flags' address after the saved
// priority's store; the flags read through a pointer brought it from 4 to 2,
// and the barriers measured there do the same here.
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

struct Work9cItemPosition *Func_080d5930(s32 *position);
void Sound_PlayFar(s32 cue);

struct GrabSave {
  u8 unk00[98];
  u8 priority;
};

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

s32 Func_080d59dc(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->animation;
  struct Work9cItemPosition *item = Func_080d5930(&leader->x);
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
  gMainRoutineWork.renderMode = 4;
  return 1;
}
