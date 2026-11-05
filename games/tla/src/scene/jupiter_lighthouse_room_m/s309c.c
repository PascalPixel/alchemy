#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"

struct Actor *Actor_GetById(s32 id);
void Func_020047d4(void);
void Func_0200494c(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_020048fc(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004724(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020049c4(s32 sound);
void Func_0200472c(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_020047dc(void);

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

struct EventWork {
  u8 unk000[0x154];
  u16 unk154;
  u8 unk156[0xd44 - 0x156];
  u16 unkD44;
  u16 unkD46;
};

void Func_0200309c(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 8 of 216 instructions differ in
   * tla-en. */
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Func_020047d4();
  Func_0200494c(0);
  i = 0;
  work->unkD46 = i;

  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
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
  work->unkD44 = 1;
  work->unkD46 = 1;
  angle = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = Func_020048fc();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02004724(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
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
  Func_020049c4(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Func_0200472c(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkD46 = 1;
  work->unk154 = 0;
  Func_020047dc();
}
