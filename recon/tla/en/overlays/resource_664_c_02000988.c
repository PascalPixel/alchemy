// Draft, differ 334 of 386: overlay 664's copy of overlay 663's Func_02001560 (the
// leader riding the object it stands on; see that draft): in Ov664Room1
// with the leader at x 39 (in 8-pixel units) it ends with Func_02005efc(16)
// instead of stepping off, dropping off in front leaves priority flags 33,
// and only bit 0 is set again at the end, before the camera follows.
// Left: as overlay 663's draft: registers from the start (the native keeps the leader in r7 and the ride in r6, the motion and priority pointers in r11 and r9, the offsets in r8 and r10 and the rider on the stack).
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "input.h"

extern char SceneId_Ov664Room1;
extern s32 gFrameCount;
void Func_02005efc(s32 a);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Func_02005ee4(void);
struct ObjectRuntime *Func_02005ef4(void);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
s32 Func_02000690(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void Func_02005d8c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005d94(struct ObjectRuntime *object);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Camera_FollowActor(s32 id, s32 a);

void Func_02000988(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *ride = leader->linkedObject;
  struct ObjectRuntime *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u32 angle;

  Func_02005e14();
  Func_02005f2c(0);
  motion = &leader->motionFlags;
  *motion = 4;
  ObjectDispatch_SetSingleChildFlags(leader, 0);
  GameFlag_SetBit(0x250);
  Func_02005f94(137);
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
    WaitFrames(1);
  }
  Func_02005f94(0x120);
  if (gAdventure.scene == (s32)&SceneId_Ov664Room1 && leader->x >> 19 == 39) {
    Func_02005efc(16);
    Func_02005e1c();
    return;
  }
  priority = &leader->priorityFlags;
  if (ride->angle == 0xc000) {
    rider = ride->linkedObject;
    dx = 0;
    dz = 0;
    *priority = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
    do {
      moved = 0;
      if (gInput.held & 0x40) {
        dx = 0;
        dz = 0xfff00000;
        leader->angle = 0xc000;
        moved = 1;
      }
      if (gInput.held & 0x10) {
        dz = 0;
        dx = 0x100000;
        leader->angle = 0;
        moved = 1;
      }
      if (gInput.held & 0x20) {
        dx = 0xfff00000;
        leader->angle = 0x8000;
        dz = 0;
        moved = 1;
      }
      if (moved && Func_02000690(2, (ride->x + dx) >> 20,
                                 (ride->z - ride->y + dz) >> 20) >>
                           8 !=
                       255)
        goto found;
      i++;
      WaitFrames(1);
    } while (i != 60);
    dx = 0;
    leader->angle = 0xc000;
    dz = 0xfff00000;
    if ((u32)(Func_02000690(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dz = 0;
    dx = 0x100000;
    leader->angle = 0;
    if ((u32)(Func_02000690(2, (ride->x + dx) >> 20, (ride->z - ride->y) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    leader->angle = 0x8000;
    dx = 0xfff00000;
  found:
    height =
        Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx, ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Func_02005d5c(leader, 6);
    WaitFrames(6);
    Func_02005f94(152);
    Func_02005d5c(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    *priority = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Func_02005d8c(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Func_02005d94(leader);
    ObjectDispatch_SetSingleChildFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->angle >> 13;
    leader->x = (((ride->x >> 19) & ~1) + angle - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    ObjectDispatch_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    *priority = 33;
    *motion = 3;
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    dust = Func_02005ef4();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = 0;
    Func_02005d8c(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Func_02005d94(dust);
  }
  Object_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02005ee4();
  GameFlag_ClearBit(0x250);
  WaitFrames(10);
  Func_02005e1c();
}
