#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_system.h"
#include "battle_unit.h"
#include "event_work.h"

void BattleEffect_CleanupSceneObjects(void);

s32 Event_ExecutePackedAction(u32 packed) {
  s32 action, mode, kind, selector, target;
  struct PendingEvent *first, *second;

  action = packed & 0x3ff;
  mode = (packed >> 10) & 15;
  kind = BattleAction_Get(action)->unk04[2];
  selector = Func_080ce31c(kind);
  first = Func_080ce458(0x30000005, kind, selector);
  second = Func_080ce458(0x20000005, kind, selector);
  target = -1;
  if (first != NULL || second != NULL) {
    if (selector != -1 && (selector & 0x100))
      target = selector & 255;
  }
  BattleEffect_LoadActionEffectResources(action, 0);
  BattleEffect_SetupObjectPair(Party_GetLeaderActor(), target);
  Field_RunPsynergyEvent(first, mode, target);
  FieldEvent_RunTypeHandler();
  BattleEffect_StopMainActor();
  Field_RunPsynergyEvent(second, mode, target);
  BattleEffect_CleanupSceneObjects();
  return 0;
}
