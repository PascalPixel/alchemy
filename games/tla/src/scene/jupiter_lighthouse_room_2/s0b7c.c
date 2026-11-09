#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "event_inline.h"

void Func_0200224c(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002254(void);

struct EventWork {
  u8 unk000[0x154];
  s16 value154;
  u8 unk156[0xd44 - 0x156];
  u16 valueD44;
  u16 valueD46;
};

void Func_02000b7c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_0200224c();
  Event_ResolvePendingActions(0);
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
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = Func_080d4568();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Actor_SetMoveTarget(shadow, actor->x,
                      Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    wait(1);
  }
  Sound_Play(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    wait(1);
  }
  Script_WaitForEventTimeout(shadow);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->valueD46 = 1;
  work->value154 = 0;
  Func_02002254();
}
