#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Func_02002378(void);
void Func_02002420(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_020023f0(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002310(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002470(s32 sound);
void Func_02002318(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02002380(void);

struct EventWork {
  u8 unk000[0x154];
  s16 value154;
  u8 unk156[0xd44 - 0x156];
  u16 valueD44;
  u16 valueD46;
};

void Func_02000598(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_02002378();
  Func_02002420(0);
  i = 0;
  work->valueD46 = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += SPLIT_CONSTANT(0x1900, 153);
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
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  shadow = Func_020023f0();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02002310(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    wait(1);
  }
  Func_02002470(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    wait(1);
  }
  Func_02002318(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->valueD46 = 1;
  work->value154 = 0;
  Func_02002380();
}
