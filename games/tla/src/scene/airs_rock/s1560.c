#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "input.h"
#include "event_inline.h"

extern s32 gFrameCount;
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c30(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
s32 Func_02000348(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b28(struct Actor *object, s32 mode);
void OverlayObject_WaitUntilIdle(struct Actor *object);

void Func_02001560(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion;
  s32 rise, dx, dz, moved, i, height;
  s32 angle;
  u16 facing;
  s32 still;

  Func_02003bc8();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(leader, 0);
  GameFlag_SetBit(0x250);
  Sound_Play(137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->angle = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(0x120);
  facing = ride->angle;
  if (facing == 0xc000) {
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    leader->priorityFlags = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
  waiting:
      moved = 0;
      if (gInput.held & 0x40) {
        dx = 0;
        dz = 0xfff00000;
        leader->angle = 0xc000;
        moved = 1;
      }
      if (gInput.held & 0x10) {
        dx = 0x100000;
        dz = 0;
        leader->angle = dz;
        moved = 1;
      }
      if (gInput.held & 0x20) {
        dx = 0xfff00000;
        leader->angle = 0x8000;
        dz = 0;
        moved = 1;
      }
      if (moved && Func_02000348(2, (ride->x + dx) >> 20,
                                 (ride->z - ride->y + dz) >> 20) >> 8 != 255)
        goto found;
      wait(1);
      if (++i != 60)
        goto waiting;
    dx = 0;
    leader->angle = 0xc000;
    dz = 0xfff00000;
    if ((u32)(Func_02000348(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >> 16 != 255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->angle = dz;
    if ((u32)(Func_02000348(2, (ride->x + dx) >> 20,
                            (ride->z - ride->y) >> 20)
              << 8) >> 16 != 255)
      goto found;
    dx = 0xfff00000;
    leader->angle = 0x8000;
  found:
    height = Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx,
                                  ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Func_02003b28(leader, 6);
    wait(6);
    Sound_Play(152);
    Func_02003b28(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    leader->priorityFlags = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Actor_SetMoveTarget(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Script_WaitForEventTimeout(leader);
    Actor_SetSingleChildFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->angle;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    still = 0;
    Actor_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    leader->priorityFlags = 1;
    *motion = 3;
    FieldCamera_MoveTo(-1, -1, -1, 0);
    dust = Func_080d4568();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = still;
    Actor_SetMoveTarget(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Script_WaitForEventTimeout(dust);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  GameFlag_ClearBit(0x250);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  wait(10);
  Func_02003bd0();
}
