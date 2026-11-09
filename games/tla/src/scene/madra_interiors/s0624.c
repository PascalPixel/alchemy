#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Actor_SetSingleChildFlags();
void Actor_WaitForAnimationChange();

void Func_02001e80(s32 value);
void Func_02000534(void);
void Func_02000570(void);

/// The rooms' setup: the leader's sprite priority, the two rooms' actors and
/// the sprite of actor 10, by the story flags.
s32 Func_02000624(void) {
  struct FieldSprite *sprite;
  void *event;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001e80(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room1) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!FlagCheck(0x821)) {
      ActorPlace(29, 0, 0);
      ActorPlace(30, 0, 0);
    } else {
      Actor_SetSpriteColor(Actor_Get(29), 3);
      Actor_SetSpriteColor(Actor_Get(30), 5);
    }
    if (FlagCheck(0x8ab)) {
      MapGrid_CopyCellsLayered(2, 110, 16, 10, 16, 64);
      MapGrid_CopyCellAttributes(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineWork.entrance == 20)
    Func_02000570();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2) {
    if (FlagCheck(0x8ab)) {
      ActorPlace(10, 0, 0);
    } else {
      Actor_SetSpriteRotationById(10, 0xc000);
      Actor_SetSingleChildFlags(Actor_Get(10), 0);
      ((struct FieldSprite *)Actor_Get(10)->animation)->nextFrame |= 8;
      sprite = Actor_Get(10)->animation;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Actor_Get(10)->animation)->unk19 = 1;
    }
  }
  Func_02000534();
  if (FlagCheck(0x8ff))
    Actor_SetSpriteColor(Actor_Get(33), 2);
  else
    Actor_SetSpriteColor(Actor_Get(28), 2);
  return 0;
}
