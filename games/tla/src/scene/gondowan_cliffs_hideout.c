/// @file
/// The scene code of overlay 0x66c, in ROM order.

#include "field/field.h"

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_02000318[];
extern const u8 gData_02000270[];
void Func_020001b4(void);
void Func_020001bc(void);
void Func_0200020c(void);

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(2219))
    return gData_02000318;
  return gData_02000270;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

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

      /* FAKEMATCH(barrier): an empty asm on half and id, so the actor is set
       * between the halves of the value; without it, 2 of 84 instructions
       * differ in tla-en. */
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

