// Draft, differ 14 of 237 (was 79): Jupiter Lighthouse room M's pushed object
// placed on cell (63, 15): sets flag 2572, falls into place (at most 30
// frames), and the leader steps and spins while the room changes
// (Func_020000c4); off the cell it only marks the cell it stands on
// (Func_02004774). The fall wait is an if around a do-while, as the native's
// loop is entered at its test; calls go through inline wrappers
// (FAKEMATCH(wrapper) on adoption). Left: the native loads the object's x into
// r3 for the cell test, where this keeps it in r0 for the final call
// (jump-following CSE); and it builds the final call's -6 before loading the
// coordinates. Tried: x in a local (21), the final call without its wrapper
// (16), permute 6 minutes.
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
void Sound_WaitForJingle(void);
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

static __inline__ void Func_02004774_(s32 x, s32 z, s32 a, s32 b) {
  Func_02004774(x, z, a, b);
}

static __inline__ void Func_020048b4_(s32 id, s32 angle) {
  Func_020048b4(id, angle);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020003b8(s32 unused, s32 id) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *object = Object_GetById(id);
  s32 x;
  s32 i;

  if (object->x >> 20 == 63 && object->z >> 20 == 15) {
    GameFlag_SetBit(2572);
    object->motionFlags = 3;
    WaitFrames(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        WaitFrames(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    object->motionFlags = 0;
    Func_020049c4(134);
    Func_020047d4();
    Func_0200494c(0);
    Camera_MoveWithinMapBounds_(254 << 18, -1, 232 << 16, 1);
    Func_020048f4();
    ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x10000, 0x8000);
    x = leader->x >> 20;
    if (x == 64) {
      ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 1032, 232);
      ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 1000, 232);
    } else if (x == 63) {
      ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 1000, 232);
    }
    ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 1000, 248);
    Func_020048b4_(gAdventure.currentOwner, 0);
    Func_02004924(0x10000, 0);
    Func_0200491c(0x10003, 0);
    Func_0200492c(60);
    FieldEvent_WaitFrames(50);
    Func_020048b4_(gAdventure.currentOwner, 0x6000);
    FieldEvent_WaitFrames(20);
    Func_020048b4_(gAdventure.currentOwner, 0x2000);
    FieldEvent_WaitFrames(20);
    Func_020048b4_(gAdventure.currentOwner, 0x6000);
    FieldEvent_WaitFrames(20);
    Func_020048b4_(gAdventure.currentOwner, 0x2000);
    FieldEvent_WaitFrames(20);
    Func_020048b4_(gAdventure.currentOwner, 0x4000);
    FieldEvent_WaitFrames(20);
    Func_020048cc(gAdventure.currentOwner, 257, 50);
    Func_020048b4_(gAdventure.currentOwner, 0);
    FieldEvent_WaitFrames(20);
    Object_SetModeById(gAdventure.currentOwner, 22);
    FieldEvent_WaitFrames(40);
    Func_02002efc(0);
    Func_020000c4();
    Object_SetModeById(gAdventure.currentOwner, 1);
    Object_GetById(gAdventure.currentOwner)->actionFlags |= 1;
    ObjectMotion_SetVariantCallback(4, 3);
    Func_020048d4(4, 258);
    FieldEvent_WaitFrames(40);
    FieldEvent_WaitFrames(60);
    Func_020048e4(0x30000, 0x6000);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    Func_020048f4();
    FieldEvent_WaitFrames(10);
    object->update = 0;
    Func_020049c4(80);
    Sound_WaitForJingle();
    FieldEvent_WaitFrames(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    Func_020048f4();
    FieldEvent_WaitFrames(10);
    Func_02002efc(1);
    Func_020047dc();
  }
  Func_02004774_(object->x, object->z, 0, -6);
}
