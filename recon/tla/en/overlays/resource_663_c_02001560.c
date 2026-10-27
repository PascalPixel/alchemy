// Draft, differ 298 of 377: the leader rides the object it stands on (its
// linkedObject) in Air's Rock: while that object's byte 0x63 is set the
// leader is carried up on it, rising faster each frame and spinning and
// flickering by the frame count (cue 137 at the start, 0x120 at the end).
// Facing up (0xc000), the leader then waits up to 60 frames for up,
// right or left toward a cell Func_02000348 doesn't block, else takes the
// first free one of up, right and left, and jumps there (cue 152); facing
// any other way it drops off in front while a dust object (Func_02003c40)
// falls to the ground. Flag 0x250 is set throughout.
// Left: registers from the start (the native keeps the leader in r7 and the ride in r6, the motion and priority pointers in r11 and r9, the offsets in r8 and r10 and the rider on the stack), and the rising loop builds its priority pointer each pass.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "input.h"

extern s32 gFrameCount;
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void Func_02003cd8(s32 cue);
void Func_02003c30(void);
struct ObjectRuntime *Func_02003c40(void);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
s32 Func_02000348(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b28(struct ObjectRuntime *object, s32 mode);
void Func_02003b58(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003b60(struct ObjectRuntime *object);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Camera_FollowActor(s32 id, s32 a);

void Func_02001560(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *ride = leader->linkedObject;
  struct ObjectRuntime *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u32 angle;

  Func_02003bc8();
  Func_02003c88(0);
  motion = &leader->motionFlags;
  *motion = 4;
  ObjectDispatch_SetSingleChildFlags(leader, 0);
  GameFlag_SetBit(0x250);
  Func_02003cd8(137);
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
    WaitTicks(1);
  }
  Func_02003cd8(0x120);
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
      if (moved && Func_02000348(2, (ride->x + dx) >> 20,
                                 (ride->z - ride->y + dz) >> 20) >> 8 != 255)
        goto found;
      i++;
      WaitTicks(1);
    } while (i != 60);
    dx = 0;
    leader->angle = 0xc000;
    dz = 0xfff00000;
    if ((u32)(Func_02000348(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >> 16 != 255)
      goto found;
    dz = 0;
    dx = 0x100000;
    leader->angle = 0;
    if ((u32)(Func_02000348(2, (ride->x + dx) >> 20,
                            (ride->z - ride->y) >> 20)
              << 8) >> 16 != 255)
      goto found;
    leader->angle = 0x8000;
    dx = 0xfff00000;
  found:
    height = Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx,
                                  ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Func_02003b28(leader, 6);
    WaitTicks(6);
    Func_02003cd8(152);
    Func_02003b28(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    *priority = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Func_02003b58(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Func_02003b60(leader);
    ObjectDispatch_SetSingleChildFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->angle >> 13;
    leader->x = (((ride->x >> 19) & ~1) + angle - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    ObjectDispatch_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    *priority = 1;
    *motion = 3;
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    dust = Func_02003c40();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = 0;
    Func_02003b58(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Func_02003b60(dust);
  }
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  GameFlag_ClearBit(0x250);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02003c30();
  WaitTicks(10);
  Func_02003bd0();
}
