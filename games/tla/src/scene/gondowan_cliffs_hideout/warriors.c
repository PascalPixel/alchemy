#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_02000318[];
extern const u8 gData_02000270[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020001b4(void);
void Func_020001bc(void);
void Func_0200020c(void);

void Func_02000074(void) {
  if (!GameFlag_Test(2219)) {
    Func_020001b4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboWarriorsFound);
    Camera_SetSpeedParameters(0x10000, 0x2000);
    Camera_FollowActor(8, 1);
    Func_0200020c();
    Actor_SetAngleToward(10, 4, 2);
    {
      s32 half = 129;
      s32 id = 10;

      /* FAKEMATCH: ⚓️ sets the actor between the halves of the value */
      asm("" : "+l"(half), "+l"(id));
      Func_080d489c(id, half << 1);
    }
    Actor_Jump(10, 4, 15);
    Event_ShowMessageAndWait(10, 0);
    Actor_SetAngleToward(11, 4, 0);
    Actor_SetAngleToward(12, 4, 0);
    Actor_SetAngleToward(13, 4, 0);
    Actor_Jump(11, 2, 15);
    Event_ShowMessageAndWait(11, 0);
    Actor_SetAngleToward(12, 11, 0);
    Motion_SetModeAndWaitAnimation(12, 4);
    Event_ShowMessageAndWait(12, 0);
    Actor_SetAngleToward(12, 4, 0);
    Event_ShowMessageAndWait(12, 0);
    Motion_SetModeAndWaitAnimation(13, 3);
    Event_ShowMessageAndWait(13, 0);
    Func_020001bc();
  }
  GameFlag_SetBit(2447);
}

s32 Func_02000154(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 513;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  return 0;
}

s32 Func_02000184(void) {
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  return 0;
}
