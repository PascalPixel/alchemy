/// @file
/// Auto battle: the actions it builds and plays without the command menu, the
/// shoulder-key camera and the escape.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/window.h"

/// Auto battle: gives every living unit a plain attack and sorts them into
/// action order. Returns how many actions there are.
s32 AutoBattle_BuildActions(struct BattleActionRecord *entries) {
  struct BattleActionRecord swap;
  u16 unitIds[UNIT_MAX];
  s32 partyCount;
  s32 count = 0;
  s32 enemyCount;
  s32 targetRange;
  s32 index;
  struct BattleActionRecord *entry;
  struct Unit *unit;

  // The party's actions.
  partyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  // DEAD: Unit_Get's results are not used; the ROM keeps the calls.
  for (index = 0; index != PARTY_BATTLE_SIZE; index++)
    Unit_Get(index);
  for (index = 0; index < partyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->priority = unit->agility;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = FIRST_ENEMY_UNIT;
    count++;
  }
  // The enemies attack a random party member, replaced when the action plays if
  // it is down.
  enemyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
  entry = &entries[partyCount];
  targetRange = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, NULL);
  for (index = 0; index < enemyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry->unitId = unitId;
    entry->priority = unit->agility >> 1;
    if (entry->priority != 0)
      entry->priority += (u32)(Random16() * unit->agility) >> 16;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = Random_Below(targetRange);
    count++;
    entry++;
  }
  // Bubble the faster units forward.
  // BUG: one pass short of a full sort; with two actions nothing is sorted.
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if ((s16)entries[pos].priority > (s16)entries[pos - 1].priority) {
        Dma_Set(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&swap, &entries[pos - 1], DMA_COPY_WORDS(4), REG_DMA3);
        swaps++;
      }
    }
    if (swaps == 0)
      break;
  }
  return count;
}

/// Unused: turns the battle camera while L or R is held, a 128th of a turn
/// each frame, and redraws the zoomed background unless a background holds it.
UNUSED void BattleCamera_AdjustByShoulderKeys(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;
  struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;

  if ((gInput.held & KEY_L) != 0) {
    camera->yaw += TRIG_TURN / 128;
  }
  if ((gInput.held & KEY_R) != 0) {
    camera->yaw -= TRIG_TURN / 128;
  }
  if (background->keepRange == 0) {
    BattleCamera_SetRange(toFIXED(120), toFIXED(120), 0, 0, FIXED_ONE);
  }
}

/// Auto battle: plays one action, a defeated target replaced by a living unit
/// of its side. Returns 1 when the party got away, -1 when the unit is down.
s32 AutoBattle_RunAction(struct BattleActionRecord *action) {
  struct BattleBackgroundState *background;
  s32 unitId;
  s32 yaw;
  struct Unit *actor;

  unitId = action->unitId;
  actor = Unit_Get(unitId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleAbility_ReplaceDefeated(action);
  background = Ram_HeapSlots->battleBackground;
  // Here any unit id above Felix's counts as an enemy, Jenna, Sheba and Piers
  // included.
  if (action->unitId > FELIX)
    yaw = YAW_ENEMY_ACTION;
  else
    yaw = YAW_PARTY_ACTION;
  background->targetYaw = yaw;
  background->frames = 60;
  MessageArgument_ClearAllFar();

  switch (action->command) {
  case COMMAND_FLEE:
    BattleMessage_ShowAndWaitFar((s32)&MsgPartyFlees);
    if (BattleFlee_PlayRun(action) != 0)
      return 1;
    break;
  case COMMAND_DEFEND:
    wait(45);
    AutoBattle_ShowDefend((s16 *)action);
    break;
  case COMMAND_ITEM:
    wait(45);
    AutoBattle_RunAttackOrItem(action);
    break;
  case COMMAND_ATTACK:
  default: {
    struct BattleBackgroundState *view = Ram_HeapSlots->battleBackground;
    view->keepRange = 0;
    AutoBattle_RunAttackOrItem(action);
    view->keepRange = 0;
    break;
  }
  case COMMAND_PSYNERGY:
    AutoBattle_RunPsynergy(action);
    break;
  }

  Message_ResetIdleFar();
  return 0;
}

/// Returns 0 for a party member's or an enemy's unit id, -1 for any other.
s32 BattleObject_IsValidId(u32 unitId) {
  if (unitId <= ALLY_MAX) {
    return 0;
  }
  if (unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
    return 0;
  }
  return -1;
}

/// Auto battle: the party tries to run, and gets away 15 times in 16. Returns 1
/// when it got away.
s32 BattleFlee_PlayRun(struct BattleActionRecord *action) {
  s16 partyMembers[UNIT_MAX];
  s32 partySize;
  s32 memberSlot;

  (void)action;
  // Flee unless a roll below 16 comes up 0, 15 in 16.
  if (Random_Below(16) != 0) {
    partySize = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, partyMembers);
    for (memberSlot = 0; memberSlot != partySize; memberSlot++) {
      BattleMotion_SetupEscapeObject(partyMembers[memberSlot]);
      wait(8);
    }
    wait(22);
    return 1;
  }
  BattleMessage_ShowAndWaitFar(MsgCannotEscape);
  return 0;
}

/// Auto battle: shows that a standing unit is defending, "<unit> is
/// defending!". `action` is the action record, read for its unit id. Returns -1
/// for an invalid unit id, 0 otherwise.
s32 AutoBattle_ShowDefend(s16 *action) {
  s32 objectId;
  s32 result;
  struct Unit *unit;

  objectId = *action;
  unit = Unit_Get(objectId);
  if (BattleObject_IsValidId(objectId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(objectId, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar((s32)MsgActorDefends);
  return 0;
}

/// Auto battle: casts Psynergy, the caster's side fading out while its effect
/// plays on the target side. Returns -1 for an invalid unit or target.
s32 AutoBattle_RunPsynergy(struct BattleActionRecord *action) {
  u16 visibleUnits[UNIT_MAX];
  struct BattlePresentationState context;
  s32 actorId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct MotionObject *actorRecord;
  u32 index;

  livingCount = 0;
  actorId = action->unitId;
  if (BattleObject_IsValidId(actorId) < 0)
    return -1;
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0)
    return -1;

  {
    struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;
    // An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
    // YAW_ENEMY_ACTION.
    background->targetYaw =
        action->unitId > FELIX ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
    background->frames = 60;
  }
  wait(10);
  // A roll nothing uses, which still advances the random number generator.
  Random16();
  actorRecord = BattleObject_Get(actorId)->object;

  // The caster's side, taken as the side the target is not on, and the
  // caster's POSE_CAST when its id is among that side's first sideCount ids.
  if (targetId >= 0 && targetId <= ALLY_MAX) {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    sideStart = FIRST_ENEMY_UNIT;
  } else {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId)
      Actor_SelectAnimationFar((struct Actor *)actorRecord, POSE_CAST);
  }

  // Fade the caster's side out over 16 frames, and close the party status
  // window.
  wait(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - index, BLEND_WEIGHT_MAX);
    wait(1);
  }
  PartyStatus_DrawFar(PARTY_STATUS_CLOSED);

  // Ready only the target side's living units, and play the effect.
  if (targetId >= FIRST_ENEMY_UNIT) {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + FIRST_ENEMY_UNIT;
      if (Unit_Get(unit)->hp > 0)
        visibleUnits[livingCount++] = unit;
    }
  } else {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (Unit_Get(index)->hp > 0)
        visibleUnits[livingCount++] = index;
    }
  }
  visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForList((s16 *)visibleUnits, 0);

  context.kind = action->parameter;
  context.actor = actorId;
  for (index = 0; index != livingCount; index++)
    context.actors[index] = visibleUnits[index];
  context.count = livingCount;
  if (targetId >= 0 && targetId <= ALLY_MAX)
    context.side = EFFECT_SIDE_PARTY;
  else
    context.side = EFFECT_SIDE_ENEMIES;
  BattleEffect_RunSummonFar((s32 *)&context);

  // Bring every unit back and fade the caster's side in.
  wait(10);
  BattleObject_Put();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = BLEND_ALPHA(index, BLEND_WEIGHT_MAX);
    wait(1);
  }
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_OPAQUE);
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
  wait(3);
  return 0;
}

/// Unused auto battle attack: "<unit> attacks!", the attacker leaps up to its
/// target, the blow, and both return. Returns -1 when either id is not a
/// battle object.
UNUSED s32 AutoBattle_RunApproachAttack(struct BattleActionRecord *action) {
  struct BattlePresentationState state;

  // A short wait when the camera already faces the party's side.
  if (Ram_HeapSlots->battleBackground->targetYaw == YAW_PARTY_ACTION) {
    Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
    wait(10);
  } else {
    Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
    wait(30);
  }

  state.actor = action->unitId;
  if (BattleObject_IsValidId(state.actor) < 0)
    return -1;

  state.actors[0] = action->target;
  if (BattleObject_IsValidId(state.actors[0]) < 0)
    return -1;

  // DEAD: Unit_Get's results are not used; the ROM keeps the calls. The roll
  // only advances the random number generator.
  Unit_Get(state.actor);
  Unit_Get(state.actors[0]);
  Random16();
  MessageArgument_PushFar(state.actor, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar((s32)MsgActorAttacks);
  BattleMotion_ApproachTarget(state.actor, state.actors[0], 13, 0);
  Actor_SetAnimationSpeedFar(
      (struct Actor *)BattleObject_Get(state.actor)->object, ANIM_STEP_NORMAL);
  BattleObject_Get(state.actors[0]);

  state.count = 1;
  if ((u16)state.actors[0] <= ALLY_MAX)
    state.side = EFFECT_SIDE_PARTY;
  else
    state.side = EFFECT_SIDE_ENEMIES;
  state.prelude = 0;

  wait(4);
  BattleEffect_RunByIdRangeFar(&state);
  BattleMotion_ReturnToSpot(state.actors[0]);
  BattleMotion_ReturnToSpot(state.actor);
  return 0;
}

/// Auto battle: an attack or an item, played on the first living unit of the
/// target's side as each element's pillar in turn. Returns -1 for an invalid
/// unit.
s32 AutoBattle_RunAttackOrItem(struct BattleActionRecord *action) {
  struct BattlePresentationState state;
  struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;
  struct MotionObject *object;
  s32 targetYaw;
  s32 i;

  if (action->unitId > FELIX)
    targetYaw = YAW_ENEMY_ACTION;
  else
    targetYaw = YAW_PARTY_ACTION;
  if (background->targetYaw == targetYaw) {
    background->frames = 40;
    wait(40);
  } else {
    background->targetYaw = targetYaw;
    background->frames = 40;
    wait(40);
  }
  state.kind = action->parameter;
  state.range = action->range;
  state.actor = action->unitId;
  state.firstTarget = action->target;
  if (BattleObject_IsValidId(state.actor) < 0)
    return -1;
  if (state.firstTarget >= FIRST_ENEMY_UNIT)
    state.count =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)state.actors);
  else
    state.count =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)state.actors);
  // The status window without its PP row (see PARTY_STATUS_SUMMONS).
  PartyStatus_DrawFar(Ram_HeapSlots->battleState->partyStatus &
                      ~PARTY_STATUS_SHOW_PP);
  // The unit raises its hand (POSE_CAST); then only the first living unit of
  // the target's side takes the effect.
  object = BattleObject_Get(state.actor)->object;
  Actor_SelectAnimationFar((struct Actor *)object, POSE_CAST);
  Actor_SetAnimationSpeedFar((struct Actor *)object, ANIM_STEP_NORMAL);
  if ((u16)action->target <= ALLY_MAX) {
    state.side = EFFECT_SIDE_PARTY;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)state.actors);
    state.count = 1;
  } else {
    state.side = EFFECT_SIDE_ENEMIES;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)state.actors);
    state.count = 1;
  }
  // Keep each target's sprite entries' parameters for the effect.
  for (i = 0; i != state.count; i++) {
    struct Sprite *animation =
        (struct Sprite *)BattleObject_Get(state.actors[i])->object->records;
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      state.params[i][j] = animation->entries[j]->colorMode;
  }
  state.kind = ELEMENT_VENUS;
  state.variant = 0;
  BattleEffect_RunElementPillarFar(&state);
  state.kind = ELEMENT_MERCURY;
  BattleEffect_RunElementPillarFar(&state);
  state.kind = ELEMENT_MARS;
  BattleEffect_RunElementPillarFar(&state);
  state.kind = ELEMENT_JUPITER;
  BattleEffect_RunElementPillarFar(&state);
  state.kind = 0;
  /* FAKEMATCH(dead): a never-true test, state.kind being 0, and its arm; as
   * BattleEffect_RunFromTableFar(&state) alone. */
  if (state.kind)
    BattleEffect_RunByIdRangeFar(&state);
  else
    BattleEffect_RunFromTableFar(&state);
  Actor_SelectAnimationFar((struct Actor *)object, POSE_STANDING);
  for (i = 0; i != state.count; i++)
    BattleMotion_ReturnToSpot(state.actors[i]);
  BattleMotion_ReturnToSpot(state.actor);
  return 0;
}

/// Unused auto battle damage: takes damage HP from a unit and shows it, with "A
/// bitter blow!" or "A critical hit!" first when showMessage is set.
UNUSED void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                                   u8 *context) {
  u8 localContext[4];
  struct Unit *character;
  struct BattleObject *slot;

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
  Actor_SelectAnimationFar((struct Actor *)slot->object, POSE_DAMAGE);
  PartyStatus_DrawFar(PARTY_STATUS_HP_ONLY);
  MessageArgument_ClearAllFar();

  if (unitId <= ALLY_MAX) {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar((s32)MsgBitterBlow);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDmgP);
  } else {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar((s32)MsgCritical);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDmgE);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  }

  BattleMotion_PlayFall(unitId);
  if (unitId <= ALLY_MAX) {
    if (character->hp <= 0) {
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)MsgGoesDown);
    }
  } else if (character->hp <= 0) {
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDowned);
  }

  slot = BattleObject_Get(unitId);
  Actor_SelectAnimationFar((struct Actor *)slot->object, POSE_STANDING);
}

/// Unused, as its one caller, AutoBattle_ApplyDamage, is: for a unit whose HP
/// has run out, plays its falling effect to the last frame and marks its battle
/// object for release (BattleObject_MarkForRelease).
void BattleMotion_PlayFall(s32 unitId) {
  struct Sprite *animation;
  struct SpriteAnimation *entry;

  if (Unit_Get(unitId)->hp <= 0) {
    animation = BattleObject_Get(unitId)->object->records;
    Sprite_StartAnimationFar(animation, POSE_DAMAGE);
    entry = animation->entries[0];
    entry->colorMode = SPRITE_COLOR_GREY;
    entry->picture = SPRITE_PICTURE_NONE;
    wait(4);
    SpriteDissolve_RunFar(animation);
    BattleObject_MarkForRelease(unitId);
  }
}

/// BattleAbility_ReplaceDefeated's result when the target's side is all down,
/// which the battle's end comes before.
#define TARGET_SIDE_DEFEATED 256

/// Returns the action's target, or a random living unit of its side when the
/// target is down; TARGET_SIDE_DEFEATED when none is left.
s32 BattleAbility_ReplaceDefeated(const struct BattleActionRecord *action) {
  s16 livingUnits[UNIT_MAX];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (Unit_Get(targetId)->hp != 0) {
    return targetId;
  }

  if (targetId >= FIRST_ENEMY_UNIT) {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
  } else {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return TARGET_SIDE_DEFEATED;
  }

  return livingUnits[(u32)(Random16() * livingCount) >> 16];
}
