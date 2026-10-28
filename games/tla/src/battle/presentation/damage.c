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
#include "motobj.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "field_event.h"
#include "system.h"
#include "animspr.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "battle_system.h"
#include "battle_target.h"
#include "text/msg_ids.h"

void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context) {
  u8 localContext[4];
  struct BattleUnit *character;
  struct BattleObj *slot;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  character = Unit_Get(unitId);
  character->hp -= damage;
  if (character->hp < 0)
    character->hp = 0;

  slot = BattleObject_Get(unitId);
  Actor_SetMode(slot->object, 5);
  PartyStatus_DrawFar(0);
  MessageArgument_ClearAllFar();

  if (unitId <= 7) {
    if (showMessage != 0)
      Message_ShowAndWaitCoreFar((s32)MsgBitterBlow);
    MessageArgument_PushFar(damage, 5);
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDmgP);
  } else {
    if (showMessage != 0)
      Message_ShowAndWaitCoreFar((s32)MsgCritical);
    MessageArgument_PushFar(damage, 5);
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDmgE);
    MessageArgument_PushFar(unitId, 1);
  }

  BattleMotion_SetMode5AndActivateSlot(unitId);
  if (unitId <= 7) {
    if (character->hp <= 0) {
      MessageArgument_PushFar(unitId, 1);
      Message_ShowAndWaitCoreFar((s32)MsgGoesDown);
    }
  } else if (character->hp <= 0) {
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDowned);
  }

  slot = BattleObject_Get(unitId);
  Actor_SetMode(slot->object, 1);
}

struct BattleObjectAnimation {
  u8 unk00[0x28];
  struct AnimationEntry *entry;
};

s32 Animation_ApplyChildArgumentFar(void *group, s32 flags);

void BattleMotion_SetMode5AndActivateSlot(s32 unitId) {
  struct BattleObjectAnimation *animation;
  struct AnimationEntry *entry;

  if (((struct BattleUnit *)Unit_Get(unitId))->hp <= 0) {
    animation = BattleObject_Get(unitId)->object->records;
    Animation_ApplyChildArgumentFar(animation, 5);
    entry = animation->entry;
    entry->param = 6;
    entry->picture = 0xff;
    WaitFrames(4);
    Map_RenderAnimatedTilePicturesForObjectFar(animation);
    BattleObject_MarkForRelease(unitId);
  }
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action) {
  s16 livingUnits[14];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (((struct BattleUnit *)Unit_Get(targetId))->hp != 0) {
    return targetId;
  }

  if (targetId > 0x7F) {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
  } else {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return 0x100;
  }

  return livingUnits[(u32)(Random16() * livingCount) >> 0x10];
}
