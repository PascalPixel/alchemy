#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "system.h"
#include "ram_buffer.h"

void *UnitGet(s32 owner);
void UiWork_ClearValueNameTablesFar(void);
extern u8 gData_03001ae8[];
s32 BattlePres_ShowDefendMessage(s16 *);
s32 Func_0811ca54(s16 *);
s32 Func_0811c710(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

s32 BattlePres_RunAction(s16 *action) {
  struct BattlePresentationTransition *transition;
  s32 actorId;
  s32 battleMode;
  u8 *actor;

  actorId = action[0];
  actor = UnitGet(actorId);
  if (*(s16 *)(actor + 0x38) == 0)
    return -1;

  action[5] =
      BattleTarget_ReplaceDefeated((const struct BattleActionRecord *)action);
  transition = Ram_HeapSlots->transitionWork;
  if (action[0] > 4)
    battleMode = -0x2000;
  else
    battleMode = 0x2000;
  transition->targetYaw = battleMode;
  transition->frames = 60;
  UiWork_ClearValueNameTablesFar();

  switch (action[3]) {
  case 99:
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyFlees);
    if (BattleEscape_PlayRun((struct BattleActionRecord *)action) != 0)
      return 1;
    break;
  case 3:
    WaitFrames(45);
    BattlePres_ShowDefendMessage(action);
    break;
  case 2:
    WaitFrames(45);
    Func_0811ca54(action);
    break;
  case 0:
  default: {
    struct BattlePresentationTransition *tr = Ram_HeapSlots->transitionWork;
    tr->flag = 0;
    Func_0811ca54(action);
    tr->flag = 0;
    break;
  }
  case 1:
    Func_0811c710((struct BattleActionRecord *)action);
    break;
  }

  UiWork_ResetFreeChannelFar();
  return 0;
}

s32 BattleObject_IsValidId(u32 objectId) {
  if (objectId <= 7) {
    return 0;
  }
  if (objectId - 128 <= 5) {
    return 0;
  }
  return -1;
}
