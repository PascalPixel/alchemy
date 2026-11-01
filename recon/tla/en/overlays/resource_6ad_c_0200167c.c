// Matches: differ 0 (tla-en, 2026-11-01)
// Overlay 6ad's copy of the function drafted in resource_698_c_020003cc.c,
// shared by thirteen overlays (698 to 6a4): the leader rides up with its watched
// object, spinning and flickering, until that object clears its flag; then it
// is set down on the 8-pixel grid beside it, falls to the ground and the camera
// follows it again.
// How it closed from 117, and what the other twelve copies can take:
// - The rise is a real loop, not a goto: only then does 0x3ffff stay a pooled
//   compare (a goto gives 0x40000) and the if's 0x1999 stay in the body. The
//   fall and settle loops stay goto, with wait(1) before i++.
// - The work's halfwords are struct members (movs), the two byte stores of 1
//   and 0 go through a cast pointer (the native's pooled 1 and 0).
// - The motion flags and the watched object's flag are pointer locals (r9, r8).
// - facing is set, then shifted on its own, which keeps asrs after the ldrh; as
//   one expression the front end shifts it unsigned. x is computed before it.
// - The camera call goes through Call4.
// The barrier in the rise is what is left: PRE moves actor + 0x23 out of the
// loop and into a fourth high register, and no plain spelling stops it. The
// native shows no PRE there, which is the "native keeps things in the loop"
// family, and that lead (classic GCSE) is a compiler question for Pascal, not
// this draft's. Tried in its place, none reaching 0: a shared pointer local (56),
// a copy inside a one-pass block (21 in a goto loop, which loses the pooled
// 0x3ffff; 88 in a real loop), do-while under an if, an inline wrapper, a later
// address, stepping the pointer.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Func_02005560(void);
void Func_02005648(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_020055e8(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020054b8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020056c0(s32 sound);
void Func_020054c0(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02005568(void);

struct EventWork {
  u8 unk_00[0x154];
  u16 unk_154;
  u8 unk_156[0xd44 - 0x156];
  u16 unk_d44;
  u16 unk_d46;
};

void Func_0200167c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *motion;
  u8 *flag;
  s32 x;
  s32 facing;
  s32 i;

  Func_02005560();
  Func_02005648(0);
  i = 0;
  work->unk_d46 = 0;
  motion = &actor->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(actor, 0);
  flag = (u8 *)linked + 0x63;
  while (*flag != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    /* FAKEMATCH(barrier): the actor set in the loop keeps PRE from moving its
     * flags' address out; as no asm, 74 of 216 instructions differ in tla-en. */
    asm("" : "+l"(actor));
    wait(1);
  }
  work->unk_d44 = 1;
  work->unk_d46 = 1;
  x = (linked->x >> 19) & ~1;
  facing = linked->angle;
  facing >>= 13;
  actor->x = (x + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  // Through a pointer: the native loads this 1 from the pool.
  *(u8 *)&actor->priorityFlags = 1;
  *motion = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  shadow = Func_020055e8();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  // Through a pointer: the native loads this 0 from the pool.
  *(u8 *)&shadow->motionFlags = 0;
  Func_020054b8(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    wait(1);
    i++;
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Func_020056c0(127);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    wait(1);
    i++;
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  Func_020054c0(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unk_d46 = 1;
  work->unk_154 = 0;
  Func_02005568();
}
