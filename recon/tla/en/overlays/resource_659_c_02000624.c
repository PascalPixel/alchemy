// Draft, differ 2 of 161 under -mthumb-slow-loads: the compiler issues
// gAdventure's load (ldr r5) before lsls r2, r2, #1 and the add into the event
// work; the native issues it after them, as in its twin, overlay 65e's
// Func_02000340. Adopted in 3c40e77e72, when it matched without the option;
// back on the listing since.
#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "../../../../games/tla/src/scene/madra_interiors/madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void FieldEvent_WaitFrames();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Func_02001db8();
void Func_02001e08();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Func_02001eb8();
void Func_02001ec8();
void Func_02001ef0();
void Func_02001f10();
void GameFlag_SetBit();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectDispatch_SetSingleChildFlags();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_CommitPositionAndActivate();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_Launch();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_WaitForAnimationChange();
void Camera_FollowActor();
struct ObjectRuntime *Object_GetById();
void Object_LinkObjectAndSetCallback();
void Object_TurnToFaceEachOther();
void Object_RefreshSelectorById();
void Object_SetModeById();
void UiText_OpenMessageAtObject();

void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02001e80(s32 value);
void Func_02001d20(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001d28(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000534(void);
void Func_02000570(void);

/// The rooms' setup: the leader's sprite priority, the two rooms' actors and
/// the sprite of actor 10, by the story flags.
s32 Func_02000624(void) {
  struct FieldSprite *sprite;
  void *event;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Object_GetById(gAdventure.currentOwner)->priorityFlags |= 32;
  Func_02001e80(0);
  if (gAdventure.scene == (s32)&SceneId_Ov659Room1) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!FlagCheck(0x821)) {
      ActorPlace(29, 0, 0);
      ActorPlace(30, 0, 0);
    } else {
      Object_SetPartAttribute(Object_GetById(29), 3);
      Object_SetPartAttribute(Object_GetById(30), 5);
    }
    if (FlagCheck(0x8ab)) {
      Func_02001d28(2, 110, 16, 10, 16, 64);
      Func_02001d20(2, 46, 16, 10, 16, 0);
    }
  }
  if (gAdventure.entrance == 20)
    Func_02000570();
  if (gAdventure.scene == (s32)&SceneId_Ov659Room2) {
    if (FlagCheck(0x8ab)) {
      ActorPlace(10, 0, 0);
    } else {
      Func_02001ef0(10, 0xc000);
      ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
      ((struct FieldSprite *)Object_GetById(10)->animation)->nextFrame |= 8;
      sprite = Object_GetById(10)->animation;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Object_GetById(10)->animation)->unk19 = 1;
    }
  }
  Func_02000534();
  if (FlagCheck(0x8ff))
    Object_SetPartAttribute(Object_GetById(33), 2);
  else
    Object_SetPartAttribute(Object_GetById(28), 2);
  return 0;
}
