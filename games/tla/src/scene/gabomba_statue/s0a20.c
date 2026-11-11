#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020056b4(struct Actor *object);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_FinishScreenTransition(void);

void Func_02000a20(struct Actor *object) {
  u32 i;
  struct MainRoutineWork *party;

  Func_020056b4(object);
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = PIXELS(6);
  object->z = 0xf741cf;
  object->angle = FACING_SOUTH;
  party = &gMainRoutineWork;
  Actor_SetAttachedEffect(party->currentOwner, 258);
  Actor_SetAnimation(party->currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetAnimation(party->currentOwner, 28);
  FieldEvent_Wait(10);
  for (i = 0; i <= 19; i++) {
    object->scaleX -= 0x300;
    object->scaleY -= 0x300;
    object->y -= 0x18000;
    System_Wait(1);
    if (i == 10)
      Event_PlaySound(204);
    if (i == 20) {
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
      Event_FinishScreenTransition();
    }
  }
}
