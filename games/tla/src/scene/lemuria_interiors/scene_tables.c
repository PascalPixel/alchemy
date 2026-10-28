#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames();
void Func_0200031c();
void Func_0200282c();
void Func_02002834();
void Func_020028ac();
void Func_020028c4();
void Func_020028fc();
void Func_0200291c();
void GameFlag_Test();
void Event_AskYesNoWithActorAnimation();
void ObjectMotion_ArmCallback();
void Object_LinkObjectAndSetCallback();
void UiText_OpenMessageAtObject();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return gSceneRegions; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gScenePlacements;
}

void Func_02000058(s32 arg0) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;

  result0 = Value1((s32 (*)())GameFlag_Test, 0x966);
  if (result0 == 0)
    goto L_02008082;
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors11);
  Call2((void (*)())Func_020028c4, arg0, 0);
  Call2((void (*)())Object_LinkObjectAndSetCallback, 10, 4);
  goto L_020080f6;
L_02008082:
  result1 = Value1((s32 (*)())GameFlag_Test, 0x965);
  if (result1 == 0)
    goto L_020080ba;
  ((void (*)())Func_0200282c)();
  Call1((void (*)())Func_020028fc, 0);
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors08);
  Call2((void (*)())Func_020028c4, arg0, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, arg0, 0x3000, 0);
  ((void (*)())Func_02002834)();
  goto L_020080f6;
L_020080ba:
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors05);
  Value2((s32 (*)())UiText_OpenMessageAtObject, arg0, 0);
  result2 = Value0((s32 (*)())Func_0200291c);
  result3 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result2, 0);
  if (result3 != 0)
    goto L_020080dc;
  ((void (*)())Func_0200031c)();
  goto L_020080f6;
L_020080dc:
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Func_020028c4, arg0, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, arg0, 0x8000, 0);
L_020080f6:
  return;
}
