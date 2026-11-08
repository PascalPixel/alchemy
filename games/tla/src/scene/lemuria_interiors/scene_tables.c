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
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_0200031c();
void Func_0200282c();
void Func_02002834();
void Func_020028ac();
void Func_020028c4();
void Func_020028fc();
void Func_0200291c();
void GameFlag_Test();
void Event_AskYesNoWithActorAnimation();
void Actor_ArmCallback();
void Actor_WatchAndSetScript();
void FieldMessage_ShowAtActor();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return gSceneRegions; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000058(s32 arg0) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 14 of 61 instructions
   * differ in tla-en. */
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;

  result0 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x966));
  if (result0 == 0)
    goto L_02008082;
  Func_020028ac((s32)MsgLemuriaInteriors11);
  Func_020028c4(arg0, 0);
  Actor_WatchAndSetScript(10, 4);
  goto L_020080f6;
L_02008082:
  result1 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x965));
  if (result1 == 0)
    goto L_020080ba;
  Func_0200282c();
  Func_020028fc(0);
  Func_020028ac((s32)MsgLemuriaInteriors08);
  Func_020028c4(arg0, 0);
  Call3((void (*)())Actor_ArmCallback, arg0, FACING_SOUTHEAST + FACING_STEP, 0);
  Func_02002834();
  goto L_020080f6;
L_020080ba:
  Func_020028ac((s32)MsgLemuriaInteriors05);
  FieldMessage_ShowAtActor(arg0, 0);
  result2 = Value0((s32 (*)())Func_0200291c);
  result3 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result2, 0);
  if (result3 != 0)
    goto L_020080dc;
  Func_0200031c();
  goto L_020080f6;
L_020080dc:
  FieldEvent_Wait(20);
  Func_020028c4(arg0, 0);
  Call3((void (*)())Actor_ArmCallback, arg0, FACING_WEST, 0);
L_020080f6:
  return;
}
