// Draft, differ 74 of 216, skeleton gap 30 (tla-en, 2026-11-01)
// Overlay 6a4's copy of the function shared by thirteen overlays (698 to 6a4).
// Plain while loops, not gotos: a real loop keeps combine from folding 0x3ffff
// into a 0x40000 compare, and `i <= 59 && ...` gives the native's guard. The
// work's halfwords go through a struct because a struct's u16 store takes the
// bit-field path: its SImode value gives the movs, and the leftover HImode 1
// and 0 give the two byte stores their pool loads. facing >> 13 is signed only
// from an s32 local. Left: mainly one cause. GCSE's PRE shares (actor + 35), the
// priority flags' address, between the rise loop and the store after it, and
// loop.c hoists it (life 3 of 34 insns); that spills it to the stack and takes
// a fourth high register, where the native recomputes it in the loop and again
// after. Most of the gap touches that address or the extra register; whether
// all of it does is unproved until it is gone. Tried: a two-step
// flags pointer (cse folds it), xor operand order, u8 * locals for the state
// and motion flags (worse), declaration order. Unsolved: a form that stops PRE
// without a device; nothing in the C seen so far can.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"

struct Actor *Actor_GetById(s32 id);
void Func_02002a88(void);
void Func_02002b18(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_02002af8(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002a28(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002b68(s32 sound);
void Func_02002a30(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02002a90(void);

struct EventWork {
  u8 unk000[0x154];
  s16 value154;
  u8 unk156[0xd44 - 0x156];
  u16 valueD44;
  u16 valueD46;
};

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02000930(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_02002a88();
  Func_02002b18(0);
  i = 0;
  work->valueD46 = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  work->valueD44 = 1;
  work->valueD46 = 1;
  facing = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = Func_02002af8();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02002a28(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    wait(1);
  }
  Func_02002b68(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    wait(1);
  }
  Func_02002a30(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->valueD46 = 1;
  work->value154 = 0;
  Func_02002a90();
}
