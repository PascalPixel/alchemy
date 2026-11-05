// Under -Os, 5 November: differ 6 of 377 (tla-en); older figures below predate it.
// Draft, differ 69 of 377, skeleton gap 25 (tla-en, 2026-11-01)
// The leader rides the object it stands on (its watchedObject) in Air's Rock:
// while that object's byte 0x63 is set the leader is carried up on it, rising
// faster each frame and spinning and flickering by the frame count (cue 137 at
// the start, 0x120 at the end). Facing up (0xc000), the leader then waits up to
// 60 frames for up, right or left toward a cell Func_02000348 doesn't block,
// else takes the first free one of up, right and left, and jumps there (cue
// 152); facing any other way it drops off in front while a dust object
// (Func_02003c40) falls to the ground. Flag 0x250 is set throughout.
// Left: GCSE's PRE and then loop.c move leader + 35 out of the rising loop into
// r9, where the native builds it on each pass in r1 and again after the cue;
// that one step causes the 8-byte frame (a spill of r1 around wait), the extra
// branch after the loop and most of the gap. No spelling of the xor, no bit
// temporary and no move of the priority pointer changes it, and a goto loop
// does not stop PRE and lets combine turn the 0xbffff compare into a shifted
// 0xc0000, so the rising loop stays a while. The other differences are two pool
// loads scheduled one slot off (0xbffff, and 0xfff00000 in both fallbacks).
// Forms that matter: the waiting loop is built from a label because loop.c
// hoists gInput out of a do loop; facing is a u16 local so the else arm rereads
// the angle and compares in full width; still is the zero the native keeps in
// r6 across the calls; leader->angle = dz keeps the order of the two offsets;
// Call4 keeps the three -1s in their own registers.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "input.h"
#include "call.h"

extern s32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void Func_02003cd8(s32 cue);
void Func_02003c30(void);
struct Actor *Func_02003c40(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
s32 Func_02000348(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b58(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003b60(struct Actor *object);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Camera_FollowActor(s32 id, s32 a);

void Func_02001560(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  s32 angle;
  u16 facing;
  s32 still;

  Func_02003bc8();
  Func_02003c88(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(leader, 0);
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
    wait(1);
  }
  Func_02003cd8(0x120);
  facing = ride->angle;
  priority = &leader->priorityFlags;
  if (facing == 0xc000) {
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    *priority = 4;
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
    Actor_SetSingleChildFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->angle;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    still = 0;
    Actor_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    *priority = 1;
    *motion = 3;
    Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    dust = Func_02003c40();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = still;
    Func_02003b58(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Func_02003b60(dust);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  GameFlag_ClearBit(0x250);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  wait(10);
  Func_02003bd0();
}
