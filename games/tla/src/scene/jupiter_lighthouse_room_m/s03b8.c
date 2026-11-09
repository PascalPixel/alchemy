#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Sound_Play(s32 sound);
void Func_020047d4(void);
void Func_020047dc(void);
void Event_ResolvePendingActions(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020048f4(void);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallbackAndRefresh(s32 id, s32 angle);
void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Func_080d47b4(s32 id, s32 emote, s32 frames);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02002efc(s32 value);
void Func_020000c4(void);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Func_080d489c(s32 id, s32 emote);
void Camera_SetSpeedParameters(s32 a, s32 b);
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
    GameFlag_SetBit(2572);
    object->motionFlags = 3;
    wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    object->motionFlags = 0;
    Sound_Play(134);
    Func_020047d4();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(254 << 18, -1, 232 << 16, 1);
    Func_020048f4();
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
    x = leader->x >> 20;
    if (x == 64) {
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 1032, 232);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 1000, 232);
    } else if (x == 63) {
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 1000, 232);
    }
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 1000, 248);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    BattleEffect_StartBufferInterpolation(60);
    Event_Wait(50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    Event_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    Event_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    Event_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    Event_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x4000);
    Event_Wait(20);
    Func_080d47b4(gMainRoutineWork.currentOwner, 257, 50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    Event_Wait(20);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
    Event_Wait(40);
    Func_02002efc(0);
    Func_020000c4();
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    Actor_StartRepeatedMotion(4, 3);
    Func_080d489c(4, 258);
    Event_Wait(40);
    Event_Wait(60);
    Camera_SetSpeedParameters(0x30000, 0x6000);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    Func_020048f4();
    Event_Wait(10);
    object->update = 0;
    Sound_Play(80);
    Sound_WaitForJingle();
    Event_Wait(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    Func_020048f4();
    Event_Wait(10);
    Func_02002efc(1);
    Func_020047dc();
  }
  Func_02004774_(object->x, object->z, 0, -6);
}
