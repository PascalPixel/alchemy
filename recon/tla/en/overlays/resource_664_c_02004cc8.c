// Draft, differ 78 of 200 (was 82; the English listing; the Japanese one
// does not save and restore the leader's priority flags): inside an event,
// the leader is carried up 80 pixels with a lift object (type 280, cue 137),
// spinning and flickering until it stops, then drops (cues 288 and 152) onto
// the ground below the lift and walks off it; the lift and the prepared
// object are removed. The speed and acceleration are stored in that order.
// Left: mostly the choice among r1, r2 and r3 for the field addresses and
// constants, and the pool offsets that follow.
#include "types.h"
#include "actor.h"
#include "adventure.h"

extern u32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_02005dec(struct Actor *object);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Func_02005d94(struct Actor *object);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02005d7c(struct Actor *object);

void Func_02004cc8(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor *prepared, *lift;
  u8 priorityFlags;
  s32 height;

  Func_02005e14();
  Func_02005f2c(0);
  priorityFlags = leader->priorityFlags;
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(leader, 0);
  leader->speedLimit = 0x18000;
  leader->acceleration = 0x18000;
  lift = Import_ActorCreate(280, leader->x, leader->y - 0x80000, leader->z);
  Func_02005d5c(lift, 2);
  Actor_SetSingleChildFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = 0x16666;
  lift->acceleration = 0x16666;
  Func_02005d8c(leader, leader->x, leader->y + 0x500000, leader->z);
  Func_02005d8c(lift, lift->x, lift->y + 0x480000, lift->z);
  Func_02005f94(137);
  Event_StartScreenTransition();
  while (!Func_02005dec(leader)) {
    leader->angle = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    WaitFrames(1);
  }
  Func_02005f94(288);
  FieldEvent_WaitFrames(10);
  Actor_ArmCallback(gAdventure.currentOwner, 0xc000, 0);
  height = Map_GetTerrainHeight(0, lift->x, lift->z - 0x100000);
  leader->y = height;
  leader->terrainHeight = height;
  Func_02005d5c(leader, 6);
  WaitFrames(6);
  Func_02005f94(152);
  Func_02005d5c(leader, 7);
  leader->speedLimit = 0x20000;
  leader->acceleration = 0x8000;
  leader->velocityY = 0x40000;
  leader->motionFlags = 2;
  leader->priorityFlags = 1;
  Func_02005d8c(leader, leader->x, leader->y, leader->z - 0x100000);
  OverlayObject_WaitUntilIdle(leader);
  Func_02005d94(leader);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  Func_02005d5c(lift, 5);
  WaitFrames(12);
  Func_02005d7c(lift);
  leader->motionFlags = 3;
  leader->terrainHeight = leader->y;
  Actor_SetSingleChildFlags(leader, 1);
  leader->priorityFlags = priorityFlags;
  Func_02005d7c(prepared);
  WaitFrames(1);
  Event_WaitTransitionFrames();
  Func_02005e1c();
}
