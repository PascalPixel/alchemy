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
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_0200031c();
void Func_0200282c();
void Func_02002834();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return gSceneRegions; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000058(s32 arg0) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;

  result0 = Event_GetFlag(0x966);
  if (result0 == 0)
    goto L_02008082;
  Event_SetMessageId((s32)MsgLemuriaInteriors11);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_WatchAndSetScript(10, 4);
  goto L_020080f6;
L_02008082:
  result1 = Event_GetFlag(0x965);
  if (result1 == 0)
    goto L_020080ba;
  Func_0200282c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors08);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_FaceDirection(arg0, FACING_SOUTHEAST + FACING_STEP, 0);
  Func_02002834();
  goto L_020080f6;
L_020080ba:
  Event_SetMessageId((s32)MsgLemuriaInteriors05);
  FieldMessage_ShowAtActor(arg0, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto L_020080dc;
  Func_0200031c();
  goto L_020080f6;
L_020080dc:
  Event_Wait(20);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_FaceDirection(arg0, FACING_WEST, 0);
L_020080f6:
  return;
}
