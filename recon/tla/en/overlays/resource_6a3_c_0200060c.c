// Draft, differ 146 of 292: Jupiter Lighthouse room M's floor opening (until flag
// 2591): sets flag 514, opens the cells under (49, 103) and (49, 106) with
// sounds 124 and 217 from the camera at (776, 208); object 23, if it stands on
// the ground at x cell 49 to 52 and flag 2573 is clear, falls through (setting
// 2573): from cells 50 and 51 it drops with sound 310, lands with the screen
// shaken (sound 134) and slides off sideways; from 49 and 52 it simply drops;
// either way it lands below with sound 144 and the camera follows the leader
// again.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200494c(s32 value);
void Func_020049c4(s32 sound);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
void Func_020048f4(void);
void Func_0200474c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020048e4(s32 a, s32 b);
void Func_0200475c(s32 a, s32 b, s32 c);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Func_02004844(s32 id, s32 a, s32 b);

void Func_0200060c(void) {
  struct ObjectRuntime *object = Object_GetById(23);
  s32 spot = 0;
  s32 x;
  s32 i;

  if (GameFlag_Test(2591))
    return;
  GameFlag_SetBit(514);
  Func_020047d4();
  Func_0200494c(0);
  Func_020049c4(124);
  Camera_MoveWithinMapBounds(194 << 18, -1, 208 << 16, 1);
  Func_020048f4();
  FieldEvent_WaitFrames(6);
  Func_020049c4(217);
  Func_0200474c(49, 103, 4, 3, 49, 76);
  FieldEvent_WaitFrames(6);
  Func_0200474c(49, 106, 4, 3, 49, 76);
  x = object->x >> 20;
  if (x == 49)
    spot = 1;
  if (x == 50)
    spot = 2;
  if (x == 51)
    spot = 3;
  if (x == 52)
    spot = 4;
  if (object->y != 0)
    spot = 0;
  if (GameFlag_Test(2573))
    spot = 0;
  if (spot != 0)
    GameFlag_SetBit(2573);
  if ((u32)(spot - 2) <= 1) {
    Func_020048e4(0x40000, 0x8000);
    Camera_FollowActor(23, 1);
    Func_020048f4();
    Func_020049c4(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = -0x500000;
    for (i = 0; object->y > object->terrainHeight;) {
      i++;
      WaitFrames(1);
      if (i > 29)
        break;
    }
    Func_0200475c(0x30000, 0x30000, 0xf333);
    Func_020049c4(134);
    object->gravity = 0x10000;
    object->velocityY = 0x60000;
    ObjectMotion_SetSpeedParameters(23, 0x18000, 0xc000);
    if (spot == 2)
      ObjectMotion_OffsetPositionAndResetMotion(23, -32, 0);
    else
      ObjectMotion_OffsetPositionAndResetMotion(23, 32, 0);
    FieldEvent_WaitFrames(7);
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    object->gravity = 0x8000;
    object->terrainHeight = -0xf00000;
    FieldEvent_WaitFrames(80);
    Func_0200475c(0x50000, 0x50000, 0x10000);
    Func_020049c4(144);
    FieldEvent_WaitFrames(20);
    Func_0200475c(-1, -1, 0xe666);
    Func_02004844(23, 0, 0);
    FieldEvent_WaitFrames(10);
  } else if (spot == 1 || spot == 4) {
    ObjectMotion_SetActionVariant(23, 1);
    Func_020048e4(0x40000, 0x8000);
    Camera_FollowActor(23, 1);
    Func_020048f4();
    Func_020049c4(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = -0xf00000;
    FieldEvent_WaitFrames(20);
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    FieldEvent_WaitFrames(60);
    Func_0200475c(0x50000, 0x50000, 0x10000);
    Func_020049c4(144);
    FieldEvent_WaitFrames(20);
    Func_0200475c(-1, -1, 0xe666);
    Func_02004844(23, 0, 0);
    FieldEvent_WaitFrames(10);
  }
  FieldEvent_WaitFrames(20);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}
