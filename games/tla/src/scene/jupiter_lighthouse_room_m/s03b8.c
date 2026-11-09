#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"
#include "event_inline.h"

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);
void Func_02002efc(s32 value);
void Func_020000c4(void);
void Sound_WaitForJingle(void);
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

static __inline__ void Func_02004774_(s32 x, s32 z, s32 a, s32 b) {
  Func_02004774(x, z, a, b);
}

static __inline__ void Func_020048b4_(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020003b8(s32 unused, s32 id) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(id);
  s32 x;
  s32 i;

  if (object->x >> 20 == 63 && object->z >> 20 == 15) {
    Event_SetFlag(2572);
    object->motionFlags = 3;
    System_Wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    object->motionFlags = 0;
    Event_PlaySound(134);
    Func_020047d4();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(254 << 18, -1, 232 << 16, 1);
    Func_020048f4();
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
    x = leader->x >> 20;
    if (x == 64) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1032, 232);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    } else if (x == 63) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    }
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 248);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(60);
    FieldEvent_Wait(50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x4000);
    FieldEvent_Wait(20);
    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 257, 50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
    FieldEvent_Wait(40);
    Func_02002efc(0);
    Func_020000c4();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    FieldActor_StartRepeatedMotion(4, 3);
    Actor_SetAttachedEffect(4, 258);
    FieldEvent_Wait(40);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x30000, 0x6000);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    Func_020048f4();
    FieldEvent_Wait(10);
    object->update = 0;
    Event_PlaySound(80);
    Sound_WaitForJingle();
    FieldEvent_Wait(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    Func_020048f4();
    FieldEvent_Wait(10);
    Func_02002efc(1);
    Func_020047dc();
  }
  Func_02004774_(object->x, object->z, 0, -6);
}
