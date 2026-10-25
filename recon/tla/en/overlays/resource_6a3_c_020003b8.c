// Draft, differ 79 of 237: Jupiter Lighthouse room M's pushed object placed on cell
// (63, 15): sets flag 2572, falls into place (at most 30 frames), and the
// scene plays: sound 134, the camera to (1016, 232), the leader walked
// to (1000, 248) (from 1032 or 1000 first by its column), turning and looking
// about, emote 257, motion 22, the room's change (Func_020000c4 between the
// two Func_02002efc calls), object 4's emote 258, the screen shaken, the
// camera over to (1048, 174) and back to (984, 248) with sound 80. In any case
// the object marks the cell it stands on (Func_02004774).
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_020049c4(s32 sound);
void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200494c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020048f4(void);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_020048b4(s32 id, s32 angle);
void Func_02004924(s32 a, s32 b);
void Func_0200491c(s32 a, s32 b);
void Func_0200492c(s32 frames);
void Func_020048cc(s32 id, s32 emote, s32 frames);
void Object_SetModeById(u32 id, s32 mode);
void Func_02002efc(s32 value);
void Func_020000c4(void);
void ObjectMotion_SetVariantCallback(s32 id, s32 variant);
void Func_020048d4(s32 id, s32 emote);
void Func_020048e4(s32 a, s32 b);
void AudioCommand_WaitForStateByteClear(void);
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

void Func_020003b8(s32 unused, s32 id) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object = Object_GetById(id);
  s32 x;
  s32 i;

  if (object->x >> 20 == 63 && object->z >> 20 == 15) {
    GameFlag_SetBit(2572);
    object->motionFlags = 3;
    WaitFrames(2);
    for (i = 0; object->y > object->terrainHeight || object->velocityY != 0;) {
      i++;
      WaitFrames(1);
      if (i > 29)
        break;
    }
    object->motionFlags = 0;
    Func_020049c4(134);
    Func_020047d4();
    Func_0200494c(0);
    Camera_MoveWithinMapBounds(254 << 18, -1, 232 << 16, 1);
    Func_020048f4();
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x10000, 0x8000);
    x = leader->x >> 20;
    if (x == 64) {
      ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 1032, 232);
      ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 1000, 232);
    } else if (x == 63) {
      ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 1000, 232);
    }
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 1000, 248);
    Func_020048b4(gPartyState.currentOwner, 0);
    Func_02004924(0x10000, 0);
    Func_0200491c(0x10003, 0);
    Func_0200492c(60);
    FieldEvent_WaitFrames(50);
    Func_020048b4(gPartyState.currentOwner, 0x6000);
    FieldEvent_WaitFrames(20);
    Func_020048b4(gPartyState.currentOwner, 0x2000);
    FieldEvent_WaitFrames(20);
    Func_020048b4(gPartyState.currentOwner, 0x6000);
    FieldEvent_WaitFrames(20);
    Func_020048b4(gPartyState.currentOwner, 0x2000);
    FieldEvent_WaitFrames(20);
    Func_020048b4(gPartyState.currentOwner, 0x4000);
    FieldEvent_WaitFrames(20);
    Func_020048cc(gPartyState.currentOwner, 257, 50);
    Func_020048b4(gPartyState.currentOwner, 0);
    FieldEvent_WaitFrames(20);
    Object_SetModeById(gPartyState.currentOwner, 22);
    FieldEvent_WaitFrames(40);
    Func_02002efc(0);
    Func_020000c4();
    Object_SetModeById(gPartyState.currentOwner, 1);
    Object_GetById(gPartyState.currentOwner)->actionFlags |= 1;
    ObjectMotion_SetVariantCallback(4, 3);
    Func_020048d4(4, 258);
    FieldEvent_WaitFrames(40);
    FieldEvent_WaitFrames(60);
    Func_020048e4(0x30000, 0x6000);
    Camera_MoveWithinMapBounds(131 << 19, 0, 174 << 16, 1);
    Func_020048f4();
    FieldEvent_WaitFrames(10);
    object->update = 0;
    Func_020049c4(80);
    AudioCommand_WaitForStateByteClear();
    FieldEvent_WaitFrames(10);
    Camera_MoveWithinMapBounds(246 << 18, -0x400000, 248 << 16, 1);
    Func_020048f4();
    FieldEvent_WaitFrames(10);
    Func_02002efc(1);
    Func_020047dc();
  }
  Func_02004774(object->x, object->z, 0, -6);
}
