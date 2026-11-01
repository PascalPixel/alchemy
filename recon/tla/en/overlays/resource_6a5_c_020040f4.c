// Draft, differ 143 of 216, skeleton gap 10/13 (tla-en, 2026-11-01)
// Overlay 6a5's copy of the function drafted in resource_698_c_020003cc.c: the
// leader rides up with its watched object, spinning and flickering, until that
// object clears its flag; then it is set down on the 8-pixel grid beside it,
// falls to the ground and the camera follows it again. Real loops, not goto:
// only they keep 0x3ffff a pool load (a goto loop's compare becomes 0x40000).
// i++ follows wait(1); sched2 moves it above the call. The word local one gives
// the halfword stores a movs (as constants they load from the pool); its value
// also reaches the priority store, which is the pool's 1. facing is set then
// shifted in two statements, which keeps asrs (one expression gives lsrs).
// Left: gcse's PRE hoists actor + 35 out of the rise loop, where the native
// rebuilds it each pass. That costs a fourth high register (the extra push),
// the three instructions before the loop and the rebuild after it, and swaps
// r5 and r6 from there on. The shadow's zero loads from the pool in the native
// (ldr r5); here it is movs, or the low byte of 0x10000 when stored through a
// pointer. The branches round the mid-function pools land elsewhere because of
// the length. Tried: an alias of actor after the loop (stops PRE, loop.c then
// hoists it, life 3 of 33 insns); the address through a local, inside and after
// the loop; u8, s8, u16 and s16 zero locals; declaration orders.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Func_02005814(void);
void Func_02005984(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_0200592c(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005784(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020059f4(s32 sound);
void Func_0200578c(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_0200581c(void);

void Func_020040f4(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *motion;
  u8 *busy;
  u8 *p;
  s32 facing;
  s32 one;
  s32 i;

  Func_02005814();
  Func_02005984(0);
  i = 0;
  *(u16 *)(work + 0xd46) = i;
  motion = &actor->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(actor, 0);
  busy = (u8 *)linked + 0x63;
  while (*busy != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  one = 1;
  *(u16 *)(work + 0xd44) = one;
  *(u16 *)(work + 0xd46) = one;
  facing = linked->angle;
  facing >>= 13;
  actor->x = (((linked->x >> 19) & ~1) + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  p = &actor->priorityFlags;
  *p = 1;
  *motion = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  shadow = Func_0200592c();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02005784(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
    do {
      wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + 0x100000);
  }
  Func_020059f4(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Func_0200578c(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  one = 1;
  *(u16 *)(work + 0xd46) = one;
  *(u16 *)(work + 0x154) = 0;
  Func_0200581c();
}
