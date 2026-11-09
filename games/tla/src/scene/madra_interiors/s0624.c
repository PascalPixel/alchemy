#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void Event_Wait();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Actor_SetHorizontalPositionWithTerrain();
void Actor_ConfigureSpriteColor();
void Event_SetMessageId();
void Event_ShowMessageThenWait();
void Actor_ArmCallbackAndRefresh();
void Func_080d47b4();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Event_ResolvePendingActions();
void Func_080d3070();
void Func_080d3928();
void Sound_Play();
void GameFlag_SetBit();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_SetSingleChildFlags();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Stop();
void Actor_SetMovementTypeById();
void Actor_Jump();
void Actor_OffsetPositionAndResetMotion();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_WaitForAnimationChange();
void Camera_FollowActor();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();
void Actor_TurnToFaceEachOther();
void Actor_RefreshSelectorById();
void Actor_SetModeById();
void FieldMessage_ShowAtActor();

void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02001e80(s32 value);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0802b450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000534(void);
void Func_02000570(void);

/// The rooms' setup: the leader's sprite priority, the two rooms' actors and
/// the sprite of actor 10, by the story flags.
s32 Func_02000624(void) {
  struct FieldSprite *sprite;
  void *event;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001e80(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room1) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!FlagCheck(0x821)) {
      ActorPlace(29, 0, 0);
      ActorPlace(30, 0, 0);
    } else {
      Actor_SetPartAttribute(Actor_GetById(29), 3);
      Actor_SetPartAttribute(Actor_GetById(30), 5);
    }
    if (FlagCheck(0x8ab)) {
      Func_0802b450(2, 110, 16, 10, 16, 64);
      Map_CopyCellAttributeRect(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineWork.entrance == 20)
    Func_02000570();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2) {
    if (FlagCheck(0x8ab)) {
      ActorPlace(10, 0, 0);
    } else {
      Func_080d3928(10, 0xc000);
      Actor_SetSingleChildFlags(Actor_GetById(10), 0);
      ((struct FieldSprite *)Actor_GetById(10)->animation)->nextFrame |= 8;
      sprite = Actor_GetById(10)->animation;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Actor_GetById(10)->animation)->unk19 = 1;
    }
  }
  Func_02000534();
  if (FlagCheck(0x8ff))
    Actor_SetPartAttribute(Actor_GetById(33), 2);
  else
    Actor_SetPartAttribute(Actor_GetById(28), 2);
  return 0;
}
