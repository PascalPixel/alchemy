#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "object_lookup.h"
#include "actor_view.h"
#include "types.h"
#include "system.h"
#include "sequence_parent_work.h"
#include "animspr.h"

#define Actor_SetAnimationSpeedFar                                 \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_SetRandomTableValueOnObject(struct ActorView *object) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 index = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 id) {
  struct ActorView *object;
  struct AnimationEntry *entry;

  object = ObjectTable_Get(id);
  if (object != NULL) {
    gSequenceParentWork.savedCallback = (u32)object->update;
    gSequenceParentWork.savedByte = 0;
    if (object->kind == 1) {
      entry = object->target.animation->entries[0];
      if (entry != NULL) {
        gSequenceParentWork.savedByte = entry->param;
      }
    }
    object->update = BattleEffect_SetRandomTableValueOnObject;
    object->value5b = 1;
    Actor_SetAnimationSpeedFar(object, 0);
  }
}

void BattleEffect_ResumeObject(s32 id) {
  struct ActorView *object = ObjectTable_Get(id);

  if (object != NULL) {
    if (object->update == BattleEffect_SetRandomTableValueOnObject) {
      object->update = (void *)gSequenceParentWork.savedCallback;
      gSequenceParentWork.savedCallback = 0;
      Animation_ApplyChildValuesFar(object, (s8)gSequenceParentWork.savedByte);
    }
    object->value5b = 0;
    Actor_SetAnimationSpeedFar(object, 16);
  }
}
