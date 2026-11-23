/// @file
/// Applying an ability to each of its targets, the battle events queued on the
/// way, and the wait for the player to advance a battle message.
// Part of battle.c.

#include "battle_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "window/item_icon.h"
#include "window/menu_list.h"
#include "battle/battle.h"
#include "battle/presentation.h"
#include "system/mul_fx.h"
#include "graphics/resource.h"
#include "game/game_flags.h"

// The target's checks, effect and messages
/// A stat stage changes the stat by an eighth of it (Unit_Calculate), from half
/// at the lowest to one and a half at the highest.
#define STAT_STAGE_MIN -4
#define STAT_STAGE_MAX 4
/// Zephyr's Agility stage, beyond the usual highest: twice the Agility.
#define AGILITY_STAGE_DOUBLED 8

/// The turns a stat change or a status lasts; a status or a lowered stat may
/// end sooner.
#define STAT_CHANGE_TURNS 7
#define AGILITY_CHANGE_TURNS 5
#define STATUS_TURNS 7
#define REGENERATE_TURNS 5
/// The Grim Reaper's countdown.
#define CURSE_TURNS 7

/// The rate Battle_CalculatePsynergyPowerFar and Battle_CalculateRecoveryFar
/// scale by: 256 is 1.
#define FULL_RATE 256

/// A message operand that shows nothing: the event only waits for the player.
#define MESSAGE_NONE -1

/// A summon's bonus damage counts the target's max HP up to this much.
#define SUMMON_BONUS_HP_MAX 10000

/// Counts unit's stun among the statuses Restore cures, for
/// COUNT_PARTIAL_CURES.
// BUG: Restore cures stun and never confusion or charm, as its description
// says ("Remove sleep, stun, and delusion.") and its cure does, but the count
// takes confusion and charm and not stun, so Cerberus, the Hydros Statue and
// the Refresh Ball pass over an ally who is only stunned.
#ifdef BUGFIX
#define COUNT_STUN(unit, count)                                                \
  if ((unit)->stun != 0)                                                       \
    (count)++;
#else
#define COUNT_STUN(unit, count)                                                \
  if ((u8)(unit)->confusion != 0)                                              \
    (count)++;                                                                 \
  if ((unit)->charm != 0)                                                      \
    (count)++;
#endif

/// Counts the statuses of unit that Restore cures: delusion, stun (COUNT_STUN),
/// sleep, the Psynergy seal and the Grim Reaper.
#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    COUNT_STUN((unit), (count))                                                \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

/// Counts the statuses Tonic cures: those, the evil spirit and poison.
#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

/// Counts what Break would undo on unit; a raised Agility is not counted.
#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_VENUS] > 0)                                \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_MERCURY] > 0)                              \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_MARS] > 0)                                 \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_JUPITER] > 0)                              \
      (count)++;                                                               \
  }

/// How worthwhile an enemy's ability is against unit, 0 for pointless: a stat
/// change that can still move, a cure or heal it needs, a status it lacks, a
/// downed unit to revive.
#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case ABILITY_EFFECT_ATTACK_UP_SHARPLY:                                     \
    case ABILITY_EFFECT_ATTACK_UP:                                             \
      if ((unit)->attackStage + 1 <= STAT_STAGE_MAX)                           \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:                                   \
    case ABILITY_EFFECT_ATTACK_DOWN:                                           \
      if ((unit)->attackStage - 1 >= STAT_STAGE_MIN)                           \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_UP_SHARPLY:                                    \
    case ABILITY_EFFECT_DEFENSE_UP:                                            \
      if ((unit)->defenseStage + 1 <= STAT_STAGE_MAX)                          \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:                                  \
    case ABILITY_EFFECT_DEFENSE_DOWN:                                          \
      if ((unit)->defenseStage - 1 >= STAT_STAGE_MIN)                          \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY:                                 \
    case ABILITY_EFFECT_RESISTANCE_UP:                                         \
      if ((unit)->resistanceStage + 1 <= STAT_STAGE_MAX)                       \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:                               \
    case ABILITY_EFFECT_RESISTANCE_DOWN:                                       \
      if ((unit)->resistanceStage - 1 >= STAT_STAGE_MIN)                       \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_POISON:                                           \
      if ((unit)->poison != 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_RESTORE:                                               \
      COUNT_PARTIAL_CURES((unit), (applies));                                  \
      break;                                                                   \
    case ABILITY_EFFECT_FIZZ:                                                  \
    case ABILITY_EFFECT_SPRITZ:                                                \
    case ABILITY_EFFECT_CRYSTAL:                                               \
    case ABILITY_EFFECT_SPRING:                                                \
      if ((unit)->hp < (unit)->hpMax)                                          \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_BREAK:                                                 \
      COUNT_POSITIVE_MODIFIERS((unit), (applies));                             \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_ALL:                                              \
      COUNT_ALL_CURES((unit), (applies));                                      \
      break;                                                                   \
    case ABILITY_EFFECT_CURSE:                                                 \
      if ((unit)->grimReaper == 0)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_POISON:                                                \
      if ((unit)->poison == 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_VENOM:                                                 \
      if ((unit)->poison <= 1)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_STUN:                                                  \
    case ABILITY_EFFECT_STUN_THREE_ROLLS:                                      \
      if ((unit)->stun == 0)                                                   \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_SLEEP:                                                 \
      if ((unit)->sleep == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_HAUNT:                                                 \
      if ((unit)->haunt == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_REVIVE:                                                \
    case ABILITY_EFFECT_REVIVE_HALF:                                           \
    case ABILITY_EFFECT_DEW:                                                   \
    case ABILITY_EFFECT_BALM:                                                  \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case ABILITY_EFFECT_TOTAL_DEFENSE:                                         \
    case ABILITY_EFFECT_GUARD_AURA:                                            \
      if ((unit)->defendLevel <= 1)                                            \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_NONE:                                                  \
    case ABILITY_EFFECT_RESTORE_HP:                                            \
    case ABILITY_EFFECT_RESTORE_PP:                                            \
      break;                                                                   \
    default:                                                                   \
      (applies) = 1;                                                           \
      break;                                                                   \
    }                                                                          \
    if ((unit)->hp == 0 && Battle_IsReviveEffectFar((action)->effect) == 0)    \
      (applies) = 0;                                                           \
    if ((applies) == 0) {                                                      \
      (kindIndex) = ((action)->flags & ABILITY_FORMULA_MASK) - 1;              \
      switch (kindIndex) {                                                     \
      case ABILITY_FORMULA_ATTACK - 1:                                         \
      case ABILITY_FORMULA_ATTACK_MULTIPLIED - 1:                              \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_DAMAGE - 1:                                         \
      case ABILITY_FORMULA_ENEMY_DAMAGE - 1:                                   \
      case ABILITY_FORMULA_SUMMON - 1:                                         \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_PP_DAMAGE - 1:                                      \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_HEAL_HP - 1:                                        \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->hpMax)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_HEAL_PP - 1:                                        \
        if ((unit)->pp < (unit)->ppMax)                                        \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_EFFECT - 1:                                         \
      case ABILITY_FORMULA_NO_POWER - 1:                                       \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_EFFECT_ONLY - 1:                                    \
        (applies)++;                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// Picks the target of an enemy's ability, or a confused unit's, at random
/// among those it would affect; a single-target attack by an enemy with a
/// strategy favours the strongest.
s32 BattleAI_ChooseTarget(s32 userId, struct AbilityDefinition *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct Unit *unit;
  struct Unit *nextUnit;
  struct Unit *thisUnit;
  s32 candidateCount;
  s32 targetIndex;
  s32 targetCount;
  s32 innerIndex;
  s32 selected;
  s32 unitId;
  s32 value;
  s32 nextValue;
  s32 swap;
  s32 ownSide;
  s32 kindIndex;
  s32 applies;
  u32 roll;
  s16 *slot;

  candidateCount = 0;
  turnOrder = Ram_HeapSlots->battleWork;
  targetCount = 0;

  // The candidates: every unit on the side the ability may target.
  if (action->targetMode != ABILITY_TARGET_ONE) {
    ownSide = 0;
    if (action->targetMode == ABILITY_TARGET_ALLIES ||
        action->targetMode == ABILITY_TARGET_SELF)
      ownSide = 1;

    if (((userId < 0 || userId > ALLY_MAX) && ownSide == 0) ||
        (userId >= 0 && userId <= ALLY_MAX && ownSide != 0)) {
      for (targetIndex = 0;
           turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END;
           targetIndex++) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == userId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_PARTY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
      }
    } else {
      for (targetIndex = 0;
           turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END;
           targetIndex++) {
        unitId = turnOrder->enemyIds[targetIndex];
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == userId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_ENEMY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
      }
    }
  }

  if (candidateCount == 0)
    return TARGET_PICK_SIDE_EMPTY;

  // Keep the candidates the ability would affect, in place.
  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = Unit_Get(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return TARGET_PICK_NONE;

  if (action->targetMode == ABILITY_TARGET_FOES && action->range == 1 &&
      Enemy_GetDefinitionFar(Unit_Get(userId)->enemyId)->targetStrategy !=
          TARGET_STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_FORMULA_MASK) - ABILITY_FORMULA_ATTACK) <=
          ABILITY_FORMULA_DAMAGE - ABILITY_FORMULA_ATTACK) {
    // Sort the targets by HP, the most first.
    // BUG: each pass starts one place later, so HP 1, 2 and 3 end as 2, 3
    // and 1.
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_Get(unitIds[innerIndex]);
        nextUnit = Unit_Get(unitIds[innerIndex + 1]);
        if (Enemy_GetDefinitionFar(Unit_Get(userId)->enemyId)->targetStrategy ==
            TARGET_STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->hpMax;
          nextValue = nextUnit->hpMax;
        }
        if (value < nextValue) {
          swap = unitIds[innerIndex];
          unitIds[innerIndex] = unitIds[innerIndex + 1];
          unitIds[innerIndex + 1] = swap;
          swap = targetPositions[innerIndex];
          targetPositions[innerIndex] = targetPositions[innerIndex + 1];
          targetPositions[innerIndex + 1] = swap;
        }
      }
    }

    // Weighted toward the front: of a roll's outcomes the first target takes
    // 6, the second 5, the third 4 and the fourth 3; five or more fall
    // through to an even roll.
    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = Random_Below(6 + 5);
      selected = 0;
      if (roll >= 6)
        selected = 1;
      break;
    case 3:
      selected = Random_Below(6 + 5 + 4);
      if (selected < 6)
        selected = 0;
      else if (selected < 6 + 5)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = Random_Below(6 + 5 + 4 + 3);
      if (selected < 6)
        selected = 0;
      else if (selected < 6 + 5)
        selected = 1;
      else if (selected < 6 + 5 + 4)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[Random_Below(targetCount)];
}

/// Clears every status and stat change of a unit that goes down and works out
/// its stats again.
void BattleUnit_ClearStatuses(s32 unitId) {
  struct Unit *unit;
  s32 element;

  unit = Unit_Get(unitId);
  for (element = 0; element < ELEMENT_COUNT; element++)
    unit->summonBoost[element] = 0;

  unit->poison = 0;
  unit->attackTurns = 0;
  unit->attackStage = 0;
  unit->defenseTurns = 0;
  unit->defenseStage = 0;
  unit->resistanceTurns = 0;
  unit->resistanceStage = 0;
  unit->delusion = 0;
  unit->confusion = 0;
  unit->charm = 0;
  unit->stun = 0;
  unit->sleep = 0;
  unit->psySeal = 0;
  unit->regenerate = 0;
  unit->reflect = 0;
  unit->haunt = 0;
  unit->grimReaper = 0;
  unit->equipBonus[0] = 0;
  unit->equipBonus[1] = 0;
  unit->ready = 0;
  unit->immobile = 0;
  unit->agilityTurns = 0;
  unit->agilityStage = 0;
  unit->challenge = 0;

  Unit_CalculateFar(unitId);
  BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
}

/// Waits for the player to advance a battle message: once the window has
/// finished printing, shows the bobbing arrow at its corner until B is held,
/// or A, B, L or R is pressed (or held, after 16 frames), then plays the
/// cursor sound.
void BattleMessage_WaitForAdvance(void) {
  struct AdvanceArrowSprite *prompt;
  struct AdvanceArrowSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 pictureTiles;
  s32 sheet;
  struct Window *window;
  struct BattleMessageOffset *offset;

  while (!Message_AllTypedFar())
    wait(1);
  prompt = &promptStorage;
  sheet = (s32)gBattleMessage_AdvanceArrowTiles;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight pictures, one every four frames.
    pictureTiles = ((gFrameCount >> 2) & 7) * 128 + sheet;
    window = ((struct BattleMessageWork *)Ram_HeapSlots->battleMessage)->window;
    offset = ((struct BattleMessageWork *)Ram_HeapSlots->battleMessage)->offset;
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, WINOUT_WIN01_BG2);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, WINOUT_WIN01_OBJ);
    Io_Write16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
    // Both attribute words are set whole: an 8 by 16 arrow of 256 colours,
    // blended with what is behind it.
    ((u32 *)prompt)[1] =
        OBJ_SIZE_8X16 | OBJ_256_COLORS | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    ((u32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pictureTiles);
    prompt->x = (s16)window->x * 8 + (offset->x >> 8) + 4;
    // Bob up and down by up to 2 pixels, once every 16 frames.
    prompt->y = CURSOR_BOB_Y() + (s16)window->y * 8 + (offset->y >> 8) + 6;
    Oam_AddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one.
    if ((gInput.held & KEY_B) || (gInput.pressed & KEY_ADVANCE) ||
        (frame > 15 && (gInput.held & KEY_ADVANCE)))
      break;
    wait(1);
  }
  Sound_PlayFar(SE_MENU_CURSOR_MOVE);
  VramCache_FreeEntry(tileSlot);
  wait(1);
}

/// Shows the cursor bobbing at (x, y) and waits for the player to go on with A,
/// B, L or R; returns 1.
s32 BattleMessage_WaitForAdvanceAt(s32 x, s32 y) {
  struct MenuPoint position;
  s32 pointerTiles;
  struct CursorSprite promptStorage;
  struct CursorSprite *prompt;
  s32 tileSlot;

  pointerTiles = WindowSprite_GetPointerTilesFar(0);
  position.x = x;
  position.y = y;
  while (!Message_AllTypedFar())
    wait(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  while (1) {
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, WINOUT_WIN01_BG2);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, WINOUT_WIN01_OBJ);
    Io_Write16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
    ((s32 *)prompt)[1] = OBJ_SIZE_16X16;
    ((s32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pointerTiles);
    prompt->x = position.x + ((gFrameCount & 4) >> 1) - 4;
    prompt->y = position.y - ((gFrameCount & 4) >> 2) - 8;
    Oam_AddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
    if (gInput.pressed & KEY_ADVANCE)
      break;
    wait(1);
  }
  VramCache_FreeEntry(tileSlot);
  wait(1);
  return 1;
}

/// Waits until the battle message has typed out, then marks the next message
/// (BattleMessageWork.marked).
void BattleMessage_WaitForTyping(void) {
  while (!Message_AllTypedFar())
    wait(1);
  ((struct BattleMessageWork *)Ram_HeapSlots->battleMessage)->marked = 1;
}

/// Takes an enemy out of the battle at once, one that leaves rather than falls.
s32 BattleObject_DestroyTemporaryObject(s32 unitId) {
  // UB: declared s32 but returns no value; no caller reads it.
  struct Unit *creature;
  struct BattleObject *runtime;

  creature = Unit_Get(unitId);
  if (creature->side == BATTLE_SIDE_ENEMY) {
    Unit_SetHpFar(creature, 0);
    BattleObject_RemoveFromLists(unitId);
    BattleObject_MarkForRelease(unitId);
    runtime = BattleObject_Get(unitId);
    Actor_ReleaseFar((struct Actor *)runtime->object);
    runtime->object = 0;
    runtime->active = 0;
  }
}

/// Grants the Djinn bonus for the rest of the action.
void BattlePlayback_GrantDjinnBonus(struct BattlePlaybackWork *runtime,
                                    u32 operand) {
  runtime->flags |= 1;
}

/// Runs the queued battle events in order, then resets the queue.
u32 BattlePlayback_RunAll(void) {
  struct BattleWork *battle;
  struct BattlePlaybackWork *runtime;
  struct BattlePlaybackQueue *queue;
  s32 i;

  battle = Ram_HeapSlots->battleWork;
  runtime = &battle->events;
  queue = &runtime->queue;

  for (i = 0; i < queue->count; i++) {
    u8 opcode = queue->opcodes[i];
    if (opcode <= PLAYBACK_BLOW_AWAY)
      switch (opcode) {
      case PLAYBACK_DJINN_BONUS:
        BattlePlayback_GrantDjinnBonus(runtime, queue->operands[i]);
        break;
      case PLAYBACK_REMOVE_UNIT:
        BattleObject_DestroyTemporaryObject(queue->operands[i]);
        break;
      case PLAYBACK_UNIT_NAME:
        MessageArgument_PushFar(queue->operands[i], MESSAGE_ARGUMENT_ACTOR);
        break;
      case PLAYBACK_NUMBER:
        MessageArgument_PushFar(queue->operands[i], MESSAGE_ARGUMENT_NUMBER);
        break;
      case PLAYBACK_ITEM_NAME:
        MessageArgument_PushFar(queue->operands[i] & INVENTORY_ITEM_ID_MASK,
                                MESSAGE_ARGUMENT_ITEM);
        break;
      case PLAYBACK_ABILITY_NAME:
        MessageArgument_PushFar(queue->operands[i] & PSYNERGY_ABILITY_MASK,
                                MESSAGE_ARGUMENT_ABILITY);
        break;
      case PLAYBACK_MARK:
        ((struct BattleMessageWork *)Ram_HeapSlots->battleMessage)->marked = 1;
        break;
      case PLAYBACK_CLEAR_ARGUMENTS:
        MessageArgument_ClearAllFar();
        break;
      case PLAYBACK_MESSAGE:
        if ((s32)queue->operands[i] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[i]);
        BattleMessage_WaitForAdvance();
        MessageArgument_ClearAllFar();
        break;
      case PLAYBACK_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[i] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[i]);
        MessageArgument_ClearAllFar();
        break;
      case PLAYBACK_HIT:
        if (runtime->pendingSound > 0)
          Sound_PlayFar(runtime->pendingSound);
        BattleMotion_FlashHit(queue->operands[i], 0, 0);
        break;
      case PLAYBACK_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
        BattleUnit_ClearStatuses(queue->operands[i]);
        BattleMotion_ShowDowned(queue->operands[i]);
        break;
      case PLAYBACK_DRAW_STATUS:
        PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
        break;
      case PLAYBACK_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[i],
                                    BattleObject_Get(queue->operands[i]));
        BattleObject_UpdatePose(queue->operands[i]);
        break;
      case PLAYBACK_BLOW_AWAY:
        BattleObject_RemoveFromLists(queue->operands[i]);
        break;
      }
  }
  return BattlePlayback_Reset();
}

/// Queues one playback command with its operand. Nothing checks the queue's
/// capacity.
void BattlePlayback_Push(u32 opcode, u32 operand) {
  struct BattlePlaybackQueue *queue = &Ram_HeapSlots->battleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
}

/// Returns 1 for an effect that still applies when the ability's damage has
/// just knocked its target out: the drains, which give the damage they dealt
/// back to the user.
s32 Ability_EffectAppliesToDowned(s32 effect) {
  switch (effect) {
  case ABILITY_EFFECT_DRAIN_HP:
  case ABILITY_EFFECT_DRAIN_PP:
  case ABILITY_EFFECT_DRAIN_HALF_HP:
  case ABILITY_EFFECT_PSY_LEECH:
    return 1;
  default:
    return 0;
  }
}

/// Returns 1 for a unit no blow may fell, which is left 1 HP: Agatio and Karst
/// in their first battle.
s32 BattleUnit_KeepsOneHp(s32 unitId) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->enemyId == ENEMY_AGATIO || unit->enemyId == ENEMY_KARST)
    return 1;
  return 0;
}

/// Puts unit into a unit list at the removed place nearest the list's divider,
/// or appends it: where a Star Magician's ball takes its place.
void BattleParty_InsertUnitCentered(u16 *list, s32 unit) {
  s32 counts[2] = {0, 0};
  s32 side;
  s32 slot;
  s32 divider;
  s32 i;
  u16 *entry;

  slot = -1;
  side = 0;
  divider = slot;
  i = 0;
  entry = list;
  do {
    u32 value = *entry++;

    if (value == BATTLE_UNIT_REMOVED) {
      if (divider >= 0 && slot >= 0) {
        if (ABS(i - divider) < ABS(slot - divider))
          slot = i;
      } else {
        slot = i;
      }
    } else {
      if (value == BATTLE_UNIT_LIST_END)
        break;
      if (value == FIRST_ENEMY_UNIT) {
        side ^= 1;
        divider = i;
      }
      counts[side]++;
    }
    i++;
  } while (i <= 5);
  if (slot >= 0) {
    list[slot] = unit;
  } else {
    list[i] = unit;
    list[i + 1] = BATTLE_UNIT_LIST_END;
  }
}

/// Party unit ids run to ALLY_MAX; enemy units start at FIRST_ENEMY_UNIT.
#define IS_PARTY_UNIT(unitId) ((unitId) >= 0 && (unitId) <= ALLY_MAX)

/// Whether two units are on opposite sides: party unit ids are below
/// FIRST_ENEMY_UNIT, enemy ids from it.
#define ARE_OPPONENTS(unitId, otherId) ((u32)((unitId) ^ (otherId)) >> 7 != 0)

/// Holds a stat stage within STAT_STAGE_MIN to STAT_STAGE_MAX.
#define CLAMP_STAT_STAGE(stage)                                                \
  {                                                                            \
    if ((stage) < STAT_STAGE_MIN)                                              \
      (stage) = STAT_STAGE_MIN;                                                \
    if ((stage) > STAT_STAGE_MAX)                                              \
      (stage) = STAT_STAGE_MAX;                                                \
  }

/// Pushes one message for a unit of the party and another for an enemy.
#define PUSH_SIDE_MESSAGE(unitId, partyMessage, enemyMessage)                  \
  {                                                                            \
    if (IS_PARTY_UNIT(unitId))                                                 \
      PUSH_MESSAGE(partyMessage);                                              \
    else                                                                       \
      PUSH_MESSAGE(enemyMessage);                                              \
  }

/// A guarding unit takes half the damage, two fifths behind Shade, or a tenth
/// behind Total Defense.
#define REDUCE_BY_GUARD(damage, unit)                                          \
  {                                                                            \
    guard = (unit)->defendLevel;                                               \
    if (guard != 0) {                                                          \
      if (guard == DEFEND_NORMAL)                                              \
        (damage) /= 2;                                                         \
      else if (guard == DEFEND_SHIELD)                                         \
        (damage) = (damage) * 2 / 5;                                           \
      else                                                                     \
        (damage) /= 10;                                                        \
    }                                                                          \
  }

/// defenseCut: how much of the target's Defense an attack counts.
#define DEFENSE_HALVED 1  ///< Torch: half
#define DEFENSE_IGNORED 2 ///< Raiden's Wrath and Trident: none

/// The element power an item, or an ability with no element, acts with.
#define ITEM_ELEMENT_POWER 100

/// An unleashed Djinni gives the Djinn bonus (more coins and experience, and a
/// likelier item, from the enemies that fall) against a target weakest to its
/// element, with a chance in percent of the user's power less the target's
/// resistance, plus this many.
#define DJINN_BONUS_CHANCE 30

/// Applies the action's ability to one of its targets, slot, in the seven steps
/// of README.md's "How an ability resolves", queueing what playback will show.
/// The Lost Age adds the shields that turn an ability away, the abilities that
/// call reinforcements, and the Djinn and summon effects of its own.
s32 BattleAbility_ApplyToTarget(struct BattlePlan *plan, s32 slot) {
  struct AbilityDefinition *ability;
  s32 abilityId;
  struct Unit *actor;
  s32 unitId;
  s32 element;
  s32 elementBonus; // elemental power less resistance
  struct BattleWork *battle;
  s32 distance;    // steps from the aimed position, 0 to 5
  s32 defenseCut;  // DEFENSE_HALVED or DEFENSE_IGNORED, else 0
  s32 multiplier;  // 0 on a miss, 2 or 3 for a double or triple blow
  s32 damageDealt; // HP or PP the target lost (negative if healed)
  s32 leaveOneHp;  // the blow leaves the target 1 HP at most
  s32 hit;         // the ability's effect lands
  s32 critical;    // enum BattleCritical
  s32 skipDamage;  // no damage step: instant death replaces it
  s32 kind;        // the ability kind: enum AbilityFormula
  s32 affinity;    // -1 a resisted element, 1 a weakness, else 0
  // A copy of the target as it was, to measure each stat's change; a heap
  // block freed at the end.
  struct Unit *savedTarget;
  s32 elementPower; // the user's power in the ability's element
  s32 hp;
  s32 drained;
  s32 defense;
  s32 pass;
  u8 targetId;
  s8 *resistance;
  struct Unit *target;
  u32 actionIndex;
  s32 value;
  s32 amount; // the damage, or the HP or PP restored
  s32 power;
  s8 *agility;
  s32 guard;
  s16 enemyList[8];
  s32 enemyIndex;
  s32 restored;
  s32 unitSize;

  // 1. The target as it was, its distance and the element.
  elementBonus = 0;
  battle = Ram_HeapSlots->battleWork;
  defenseCut = 0;
  damageDealt = 0;
  leaveOneHp = 0;
  skipDamage = 0;
  affinity = 0;
  unitSize = sizeof(struct Unit);
  savedTarget = Heap_Alloc(unitSize);

  // Meld and Mold strike with the stats of the unit that joins the user.
  // BUG: abilityId is tested before it is read from the plan, so this reads a
  // local not yet set.
  if (abilityId == ABILITY_MELD || abilityId == ABILITY_MOLD)
    unitId = plan->partnerId;
  else
    unitId = plan->unitId;
  targetId = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  multiplier = plan->targetMultiplier[slot];
  critical = plan->targetCritical[slot];

  ability = Ability_Get(abilityId);
  actor = Unit_Get(unitId);
  target = Unit_Get(targetId);
  Iwram_CopyWordsEntry(savedTarget, target, unitSize);

  // What turns an opponent's ability away untouched: a Sentinel dissipates
  // Psynergy, Poseidon's force field stands until the Trident breaks it, and
  // Haze leaves nothing to hit.
  switch (target->enemyId) {
  case ENEMY_SENTINEL:
    if (plan->command == COMMAND_PSYNERGY && ARE_OPPONENTS(unitId, targetId)) {
      BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgDissipated);
      goto done;
    }
    break;
  case ENEMY_POSEIDON:
  case ENEMY_POSEIDON_SECOND_ACTION:
  case ENEMY_POSEIDON_3:
    if (abilityId != ABILITY_TRIDENT && battle->forceField != 0 &&
        ARE_OPPONENTS(unitId, targetId)) {
      BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgFieldProtects);
      goto done;
    }
    break;
  }
  if (target->defendLevel == DEFEND_VANISHED &&
      ARE_OPPONENTS(unitId, targetId)) {
    BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
    PUSH_MESSAGE(MsgNoEffect);
    goto done;
  }

  // An ability that reaches the whole side treats every target as aimed at.
  if (ability->range != ABILITY_RANGE_ALL) {
    distance = plan->targetOffset[slot];
    if (distance < 0)
      distance = -distance;
  } else {
    distance = 0;
  }

  // The affinity picks the message: "damage.", "damage!" or "damage!!!".
  if (element != ELEMENT_NONE) {
    s32 i;

    value = target->baseElement[element].resistance;
    for (i = 0; i < ELEMENT_COUNT && value >= target->baseElement[i].resistance;
         i++)
      ;
    if (i == ELEMENT_COUNT)
      affinity = -1;

    for (i = 0; i < ELEMENT_COUNT && value <= target->baseElement[i].resistance;
         i++)
      ;
    if (i == ELEMENT_COUNT)
      affinity = 1;
  }

  // The user's power in the ability's element; items always use 100.
  if (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER &&
      plan->command != COMMAND_ITEM)
    elementPower = actor->element[plan->element].power;
  else
    elementPower = ITEM_ELEMENT_POWER;

  // 2. The Djinn bonus, and whether the effect lands.
  if (plan->command == COMMAND_DJINN &&
      (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER) &&
      affinity > 0) {
    s32 chance;

    chance = elementPower - target->element[plan->element].resistance;
    chance += DJINN_BONUS_CHANCE;
    // Percent to a fraction of the 16-bit random range.
    chance *= FIXED_ONE / 100;
    if (chance > (Battle_RandomFar() & 0xffff))
      BattlePlayback_Push(PLAYBACK_DJINN_BONUS, 5);
  }

  // Abilities that knock the target's Djinn to standby.
  if (abilityId == ABILITY_DJINNFEST || abilityId == ABILITY_FUGUE)
    BattleUnit_DrainDjinn(targetId, 0, 0);
  if (abilityId == ABILITY_DJINN_BLAST || abilityId == ABILITY_DJINN_STORM)
    BattleUnit_DrainDjinn(targetId, 1, 0);
  if (abilityId == ABILITY_DJINN_STUN)
    BattleUnit_DrainDjinn(targetId, 1, 1);

  // Whether the effect lands: a roll made when the action was built, or
  // one made now, less likely the farther the target is.
  kind = ability->flags & ABILITY_FORMULA_MASK;
  {
    s32 plannedHit;

    plannedHit = plan->targetHit[slot];
    if (plannedHit == HIT_NOT_ROLLED)
      hit = Battle_RollEffectSuccessFar(unitId, targetId, element,
                                        ability->effect,
                                        gRangeFalloff_EffectChance[distance]);
    else
      hit = plannedHit;
  }

  // 3. Calling an ally; Speed Surge is an enemy splitting in two. A Star
  // Magician calls one of its four balls, any other caller a Sea Fighter, and
  // Regen Dance the next enemy waiting in the queue.
  if (ability->effect == ABILITY_EFFECT_CALL_ALLY ||
      ability->effect == ABILITY_EFFECT_CALL_HELP ||
      ability->effect == ABILITY_EFFECT_CALL_REINFORCEMENT ||
      ability->effect == ABILITY_EFFECT_CALL_QUEUED) {
    s32 classId;
    s32 newUnitId;
    s32 suffix;

    classId = actor->enemyId;
    suffix = -1;
    newUnitId = Enemy_GetFreeUnitId();
    if (ability->effect == ABILITY_EFFECT_CALL_HELP) {
      classId = BattleFormation_GetRandomMember(battle->formation);
    } else if (ability->effect == ABILITY_EFFECT_CALL_REINFORCEMENT) {
      if (actor->enemyId == ENEMY_STAR_MAGICIAN)
        classId = (Battle_RandomFar() & 3) + ENEMY_REFRESH_BALL;
      else
        classId = ENEMY_SEA_FIGHTER;
    } else if (ability->effect == ABILITY_EFFECT_CALL_QUEUED) {
      if (battle->spoils.queueCount != 0) {
        classId = battle->spoils.queuedSummons[battle->spoils.queueHead];
        suffix = battle->spoils.queuedLetters[battle->spoils.queueHead];
      } else
        hit = 0;
    }
    if (hit != 0 && Enemy_HasSpriteRoom(classId) != 0 && newUnitId >= 0) {
      // The newcomer's name letter; on the first duplicate the enemy
      // already there takes a letter too.
      if (suffix == -1) {
        suffix = BattleEnemy_ClaimNameLetter(classId, 1);
        if (suffix & ENEMY_LETTER_FIRST_TOO)
          BattleEnemy_LetterFirstOfKind(classId);
      }
      BattleUnit_SetFar(newUnitId, classId, suffix & ENEMY_LETTER_MASK);
      if (ability->effect == ABILITY_EFFECT_CALL_QUEUED) {
        battle->spoils.queueCount--;
        battle->spoils.queueHead = (battle->spoils.queueHead + 1) % 2;
      }
      // A Star Magician's ball takes the place nearest the middle; any other
      // newcomer fills a removed enemy's place, or is appended.
      if (actor->enemyId == ENEMY_STAR_MAGICIAN)
        BattleParty_InsertUnitCentered(battle->enemyIds, newUnitId);
      else {
        s32 i;

        for (i = 0; i < ENEMY_BATTLE_SIZE; i++) {
          if (battle->enemyIds[i] == BATTLE_UNIT_REMOVED) {
            battle->enemyIds[i] = newUnitId;
            break;
          }
          if (battle->enemyIds[i] == BATTLE_UNIT_LIST_END) {
            battle->enemyIds[i] = newUnitId;
            battle->enemyIds[i + 1] = BATTLE_UNIT_LIST_END;
            break;
          }
        }
      }
      // Lay the enemies out again and put the newcomer's sprite in place
      // (the anchors are 16.16 fixed point).
      BattlePlacement_RelayoutEnemies();
      {
        struct BattleObject *battleObject;

        battleObject = BattleObject_Get(newUnitId);
        BattleObject_Make((struct BattleObject *)battleObject, newUnitId,
                          battleObject->anchorX / FIXED_ONE,
                          battleObject->anchorZ / FIXED_ONE);
      }
      BattleObject_Put();
      {
        s32 enemyCount;

        enemyCount = BattleEnemy_List(enemyList);
        for (enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++)
          BattleMotion_ReturnToSpot((u16)enemyList[enemyIndex]);
      }
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, newUnitId);
      if (ability->effect == ABILITY_EFFECT_CALL_QUEUED)
        PUSH_MESSAGE(MsgUpToFight);
      else if (abilityId != ABILITY_SPEED_SURGE)
        PUSH_MESSAGE(MsgAppears);
      else
        PUSH_MESSAGE(MsgSplitOff);
    } else if (abilityId == ABILITY_SPEED_SURGE) {
      PUSH_MESSAGE(MsgSplitFailed);
    } else {
      PUSH_MESSAGE(MsgNoOneCame);
    }
  }

  // 4. Effects that change how the damage step works.
  if (hit != 0) {
    if (ability->effect == ABILITY_EFFECT_GROUND ||
        ability->effect == ABILITY_EFFECT_ULYSSES) {
      // Ground holds only a target that has not yet acted this turn.
      hit = 0;
      for (actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY;
           actionIndex++) {
        if (battle->actions[actionIndex].unitId == targetId) {
          hit = 1;
          break;
        }
      }
    } else if (ability->effect == ABILITY_EFFECT_HALVE_DEFENSE) {
      defenseCut = DEFENSE_HALVED;
    } else if (ability->effect == ABILITY_EFFECT_LEAVE_ONE_HP) {
      leaveOneHp = 1;
    } else if (ability->effect == ABILITY_EFFECT_INSTANT_DEATH) {
      skipDamage = 1;
    } else if (ability->effect == ABILITY_EFFECT_SELF_DESTRUCT) {
      // The user leaves the battle (BattleObject_DestroyTemporaryObject).
      if (actor->hp != 0)
        BattlePlayback_Push(PLAYBACK_REMOVE_UNIT, unitId);
    } else if (ability->effect == ABILITY_EFFECT_DRAIN_PP) {
      // Psy Drain does PP damage, and misses a target with no PP.
      if (target->pp != 0)
        kind = ABILITY_FORMULA_PP_DAMAGE;
      else
        hit = 0;
    } else if (ability->effect == ABILITY_EFFECT_SLAY_DRAGON) {
      defenseCut = DEFENSE_IGNORED;
    } else if (ability->effect == ABILITY_EFFECT_SLAY_SEA_CREATURE) {
      defenseCut = DEFENSE_IGNORED;
    }
  }

  // 5. Damage and healing; a downed target is skipped unless the ability
  // revives, and Shine's damage falls on its first target alone.
  if ((ability->effect != ABILITY_EFFECT_DAZZLE || slot == 0) &&
      skipDamage == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(ability->effect) != 0)) {
    s32 pp;
    s32 newHp;

    switch (kind) {
    // An attack: the user's Attack against the target's Defense. Meld and
    // Mold strike with the Attack of the two units that join in them.
    case ABILITY_FORMULA_ATTACK:
    case ABILITY_FORMULA_ATTACK_MULTIPLIED: {
      s32 baseDefense;

      if (target->defendLevel == DEFEND_VANISHED)
        break;
      hp = target->hp;
      baseDefense = target->def;
      defense = baseDefense;
      if (defenseCut != 0) {
        if (defenseCut == DEFENSE_IGNORED)
          defense = 0;
        else
          defense = baseDefense / 2;
      }
      pass = 1;
      do {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        if (kind == ABILITY_FORMULA_ATTACK_MULTIPLIED)
          amount = Battle_CalculateAttackDamageFar(actor->atk, defense, 0,
                                                   elementBonus) *
                   power / 10;
        else {
          s32 attack;

          attack = actor->atk;
          if (abilityId == ABILITY_MELD || abilityId == ABILITY_MOLD) {
            attack = Unit_Get(plan->unitId)->atk;
            if (abilityId == ABILITY_MELD)
              attack += Unit_Get(plan->partnerId)->atk / 4;
          }
          amount = Battle_CalculateAttackDamageFar(attack, defense, power,
                                                   elementBonus);
        }
        amount *= multiplier;
        if (critical != CRITICAL_NONE) {
          if (critical == CRITICAL_HIT)
            amount = amount * 5 / 4;
          else
            amount = amount * 3 / 2;
          amount += (target->level / 5) + 6;
          if (pass == 0) {
            BattlePlayback_Push(PLAYBACK_MARK, 0);
            {
              s32 message;

              message = (s32)MsgCritical;
              if (IS_PARTY_UNIT(targetId))
                message += 1;
              BattlePlayback_Push(PLAYBACK_MESSAGE_NO_WAIT, message);
            }
          }
        }
        amount += Battle_RandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        if (amount <= 0)
          amount = 1;
        if (leaveOneHp != 0) {
          if (amount < hp - 1) {
            amount = hp - 1;
            if (amount <= 0)
              amount = 1;
          }
        }
        if (GameFlag_Get(FLAG_DJINN_LESSON) != 0 &&
            plan->command == COMMAND_DJINN && hp <= amount) {
          amount = hp - 1;
        }
        pass++;
      } while (pass <= 1);
      hp -= amount;
      BattlePlayback_Push(PLAYBACK_HIT, targetId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      {
        s32 message;

        if (IS_PARTY_UNIT(targetId))
          message = (s32)MsgDmgEmphP + affinity;
        else
          message = (s32)MsgDmgEmphE + affinity;
        BattlePlayback_Push(PLAYBACK_MESSAGE, message);
      }

      if (hp <= 0 && BattleUnit_KeepsOneHp(targetId) != 0)
        hp = 1;
      if (hp <= 0) {
        hp = 0;
        BattlePlayback_Push(PLAYBACK_DEFEAT, targetId);
        if (target->enemyId != ENEMY_DOOM_DRAGON_A &&
            target->enemyId != ENEMY_DOOM_DRAGON_E) {
          BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
          PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
        }
      } else
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      damageDealt = target->hp - hp;
      target->hp = hp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    case ABILITY_FORMULA_PP_DAMAGE: {
      if (target->defendLevel == DEFEND_VANISHED)
        break;
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
      amount = amount * gRangeFalloff_PpLoss[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      if (ability->effect == ABILITY_EFFECT_DRAIN_PP && amount > pp)
        amount = pp;
      if (amount > pp)
        amount = pp;

      BattlePlayback_Push(PLAYBACK_HIT, targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      {
        s32 message;

        if (IS_PARTY_UNIT(targetId))
          message = (s32)MsgPpLossP;
        else
          message = (s32)MsgPpLossE;
        BattlePlayback_Push(PLAYBACK_MESSAGE, message);
        pp -= amount;
      }
      if (pp <= 0)
        pp = 0;
      BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      damageDealt = target->pp - pp;
      target->pp = pp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    case ABILITY_FORMULA_HEAL_HP: {
      if (ability->power == 0)
        break;
      newHp = target->hp;
      if (newHp == 0)
        break;

      power = ability->power;
      amount = Battle_CalculateRecoveryFar(
          power, element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower,
          FULL_RATE);
      amount = amount * gRangeFalloff_HealHp[distance] / 100;
      amount *= multiplier;
      amount += Battle_RandomFar() & 3;
      newHp += amount;
      if (newHp > target->hpMax) {
        newHp = target->hpMax;
        amount = newHp - target->hp;
      }
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      if (newHp == target->hpMax)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, amount);
        PUSH_MESSAGE(MsgHpRecover);
      }
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    // Unused: no formula is -1. PP damage again, by the PP-damage falloff and
    // with the plain damage message.
    case -1: {
      if (target->defendLevel == DEFEND_VANISHED)
        break;
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
      amount = amount * gRangeFalloff_PpDamage[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      BattlePlayback_Push(PLAYBACK_HIT, targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      {
        s32 message;

        if (IS_PARTY_UNIT(targetId))
          message = (s32)MsgDmgP;
        else
          message = (s32)MsgDmgE;
        BattlePlayback_Push(PLAYBACK_MESSAGE, message);
        pp -= amount;
      }
      if (pp <= 0)
        pp = 0;
      BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      target->pp = pp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    case ABILITY_FORMULA_DAMAGE:
    case ABILITY_FORMULA_ENEMY_DAMAGE:
    case ABILITY_FORMULA_SUMMON: {
      s32 pass;

      if (target->defendLevel == DEFEND_VANISHED)
        break;
      if (ability->power == 0)
        break;
      newHp = target->hp;
      pass = 1;
      do {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        // A summon, and Meteor Blow, Star Mine and Cruel Ruin, add a share of
        // the target's max HP, counted up to SUMMON_BONUS_HP_MAX.
        if (abilityId == ABILITY_METEOR_BLOW ||
            abilityId == ABILITY_STAR_MINE || abilityId == ABILITY_CRUEL_RUIN ||
            plan->command == COMMAND_SUMMON ||
            plan->command == COMMAND_DAEDALUS) {
          s32 maxHpPercent;

          maxHpPercent = 0;
          switch (abilityId) {
          default:
          case ABILITY_COATLICUE:
            break;
          case ABILITY_JUPITER:
          case ABILITY_MERCURY:
          case ABILITY_MARS:
          case ABILITY_VENUS:
            maxHpPercent = 3;
            break;
          case ABILITY_THOR:
          case ABILITY_METEOR:
          case ABILITY_BOREAS:
          case ABILITY_JUDGMENT:
            maxHpPercent = 12;
            break;
          case ABILITY_ULYSSES:
            maxHpPercent = 12;
            break;
          case ABILITY_METEOR_BLOW:
            maxHpPercent = 35;
            break;
          case ABILITY_ATALANTA:
          case ABILITY_MEGAERA:
          case ABILITY_KIRIN:
          case ABILITY_NEREID:
          case ABILITY_ZAGAN:
          case ABILITY_RAMSES:
            maxHpPercent = 6;
            break;
          case ABILITY_TIAMAT:
          case ABILITY_NEPTUNE:
          case ABILITY_PROCNE:
          case ABILITY_CYBELE:
            maxHpPercent = 9;
            break;
          case ABILITY_MOLOCH:
          case ABILITY_FLORA:
            maxHpPercent = 9;
            break;
          case ABILITY_DAEDALUS:
            maxHpPercent = 7;
            break;
          case ABILITY_ECLIPSE:
          case ABILITY_DAEDALUS_STRIKE:
          case ABILITY_HAURES:
            maxHpPercent = 15;
            break;
          case ABILITY_AZUL:
            maxHpPercent = 21;
            break;
          case ABILITY_CATASTROPHE:
            maxHpPercent = 24;
            break;
          case ABILITY_STAR_MINE:
          case ABILITY_CHARON:
            maxHpPercent = 30;
            break;
          case ABILITY_CRUEL_RUIN:
          case ABILITY_IRIS:
            maxHpPercent = 40;
            break;
          }
          value = target->hpMax;
          if (value > SUMMON_BONUS_HP_MAX)
            value = SUMMON_BONUS_HP_MAX;
          power += value * maxHpPercent / 100;
        }
        amount =
            Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        switch (kind & ABILITY_FORMULA_MASK) {
        case ABILITY_FORMULA_DAMAGE:
          amount = gRangeFalloff_PsynergyDamage[distance] * amount / 100;
          break;
        case ABILITY_FORMULA_SUMMON:
          amount = amount * gRangeFalloff_SummonDamage[distance] / 100;
          break;
        case ABILITY_FORMULA_ENEMY_DAMAGE:
          amount = amount * gRangeFalloff_EnemyDamage[distance] / 100;
          break;
        }
        amount += Battle_RandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        if ((gMainRoutineWork.battleType == BATTLE_TYPE_LOST ||
             (GameFlag_Get(FLAG_DJINN_LESSON) != 0 &&
              plan->command == COMMAND_SUMMON)) &&
            newHp > amount) {
          amount = newHp;
        }
        pass++;
      } while (pass <= 1);
      BattlePlayback_Push(PLAYBACK_HIT, targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      {
        s32 message;

        if (IS_PARTY_UNIT(targetId))
          message = (s32)MsgDmgEmphP + affinity;
        else
          message = (s32)MsgDmgEmphE + affinity;
        BattlePlayback_Push(PLAYBACK_MESSAGE, message);
        newHp -= amount;
      }

      if (newHp <= 0 && BattleUnit_KeepsOneHp(targetId) != 0)
        newHp = 1;
      if (gMainRoutineWork.battleType == BATTLE_TYPE_LOST)
        newHp = 0;
      if (newHp <= 0) {
        BattlePlayback_Push(PLAYBACK_DEFEAT, targetId);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        {
          s32 message;

          newHp = 0;
          if (IS_PARTY_UNIT(targetId))
            message = (s32)MsgGoesDown;
          else
            message = (s32)MsgFelled;
          BattlePlayback_Push(PLAYBACK_MESSAGE, message);
        }
      } else
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    case ABILITY_FORMULA_HEAL_PP: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      power = ability->power;
      amount = Battle_CalculateRecoveryFar(
          power, element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower,
          FULL_RATE);
      amount = amount * gRangeFalloff_HealPp[distance] / 100;
      amount *= multiplier;
      pp += amount;
      if (pp > target->ppMax) {
        pp = target->ppMax;
        amount = pp - target->pp;
      }
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      if (pp == target->ppMax)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, amount);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = pp;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    case ABILITY_FORMULA_BLANK:
    case ABILITY_FORMULA_EFFECT_ONLY:
    case ABILITY_FORMULA_NO_POWER:
      break;

    case ABILITY_FORMULA_EFFECT:
      if (target->defendLevel == DEFEND_VANISHED)
        break;
      if (hit != 0) {
        if (ability->power == 0)
          break;
        newHp = target->hp;
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        power = ability->power;
        amount =
            Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        amount = amount * gRangeFalloff_EffectDamage[distance] / 100;
        REDUCE_BY_GUARD(amount, target);
        BattlePlayback_Push(PLAYBACK_HIT, targetId);
        BattlePlayback_Push(PLAYBACK_NUMBER, amount);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        {
          s32 message;

          if (IS_PARTY_UNIT(targetId))
            message = (s32)MsgDmgP;
          else
            message = (s32)MsgDmgE;
          BattlePlayback_Push(PLAYBACK_MESSAGE, message);
          newHp -= amount;
        }

        if (newHp <= 0 && BattleUnit_KeepsOneHp(targetId) != 0)
          newHp = 1;
        if (newHp <= 0) {
          BattlePlayback_Push(PLAYBACK_DEFEAT, targetId);
          newHp = 0;
          if (target->enemyId != ENEMY_DOOM_DRAGON_A &&
              target->enemyId != ENEMY_DOOM_DRAGON_E) {
            BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
            PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
          }
        } else
          BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
        damageDealt = target->hp - newHp;
        target->hp = newHp;
        Unit_RecalculateGaugesFar(targetId);
        break;
      }
      BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgNoEffect);
      break;
    }
  }

  // The Trident breaks Poseidon's force field.
  if (battle->forceField != 0 && abilityId == ABILITY_TRIDENT) {
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
    PUSH_MESSAGE(MsgFieldBroken);
    battle->forceField = 0;
  }
  // 6. The added effect; each message after the first pushes the name again.
  // Shine's falls on the targets after its first.
  BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
  if ((ability->effect != ABILITY_EFFECT_DAZZLE || slot != 0) &&
      (Battle_IsReviveEffectFar(ability->effect) != 0 || target->hp != 0 ||
       Ability_EffectAppliesToDowned(ability->effect) != 0) &&
      hit != 0) {
    switch (ability->effect) {

    // Tonic cures every ailment.
    case ABILITY_EFFECT_CURE_ALL:
      if (target->delusion != 0) {
        target->delusion = 0;
        PUSH_MESSAGE(MsgCureDelusion);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        PUSH_MESSAGE(MsgWakes);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        PUSH_MESSAGE(MsgCureSeal);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        PUSH_MESSAGE(MsgCureCurse);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        PUSH_MESSAGE(MsgCureSpirit);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      }
      if (target->poison != 0) {
        PUSH_MESSAGE(MsgCurePoison);
        target->poison = 0;
      }
      BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
      break;

    // Restore cures what Tonic does, but not the evil spirit or poison.
    case ABILITY_EFFECT_RESTORE:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgWakes);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgCureCurse);
      break;

    // Healing by a share of the target's max HP: "HP fully restored" at the
    // maximum, else the amount restored. Nothing shows if no HP came back,
    // unless the ability heals.
    case ABILITY_EFFECT_FIZZ:
    case ABILITY_EFFECT_SPRITZ:
    case ABILITY_EFFECT_CRYSTAL:
    case ABILITY_EFFECT_SPRING:
    case ABILITY_EFFECT_BREATH: {
      s32 before;
      s32 limit;
      s32 maximum;
      s32 heal;
#ifndef UBFIX
      u16 *stat;
#endif

      // UB: a pointer walks from hp down past it to hpMax, reading each stat
      // twice, unsigned and signed; the member reads behind UBFIX share one
      // load each.
#ifdef UBFIX
      before = (u16)target->hp;
      heal = target->hp;
#else
      stat = (u16 *)&target->hp;
      before = *stat--;
      heal = *(s16 *)(stat + 1);
#endif
      if (ability->effect == ABILITY_EFFECT_BREATH) {
        s32 fullHp;

#ifdef UBFIX
        limit = (u16)target->hpMax;
        fullHp = target->hpMax;
#else
        stat--;
        limit = *stat--;
        fullHp = *(s16 *)(stat + 1);
#endif
        heal += fullHp * 2 / 5;
      } else if (ability->effect == ABILITY_EFFECT_SPRING) {
#ifdef UBFIX
        limit = (u16)target->hpMax;
        maximum = target->hpMax;
#else
        stat--;
        limit = *stat--;
        maximum = *(s16 *)(stat + 1);
#endif
        heal += maximum * 7 / 10;
      } else if (ability->effect == ABILITY_EFFECT_CRYSTAL) {
        limit = (u16)target->hpMax;
        heal += (s16)limit / 2;
        /* FAKEMATCH(dead): updates of limit that cancel out. */
        limit++;
        limit--;
      } else {
#ifdef UBFIX
        limit = (u16)target->hpMax;
        maximum = target->hpMax;
#else
        stat--;
        limit = *stat--;
        maximum = *(s16 *)(stat + 1);
#endif
        if (ability->effect == ABILITY_EFFECT_FIZZ)
          heal += maximum * 60 / 100;
        else
          heal += maximum * 30 / 100;
      }
      if (heal > (s16)limit)
        heal = (s16)limit;
      restored = heal - (s16)before;
      if (restored == 0 && kind != ABILITY_FORMULA_HEAL_HP)
        break;
      if (heal == (s16)limit)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, restored);
        PUSH_MESSAGE(MsgHpRecover);
      }
      target->hp = heal;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    // The same by a share of max PP.
    case ABILITY_EFFECT_EMBER:
    case ABILITY_EFFECT_AROMA:
    case ABILITY_EFFECT_ETHER: {
      s32 before;
      s32 maximum;
      s32 heal;
      s32 limit;
#ifndef UBFIX
      u16 *stat;
#endif

      // UB: a pointer walks from pp down past it to ppMax, reading each stat
      // twice, unsigned and signed; the member reads behind UBFIX share one
      // load each.
#ifdef UBFIX
      before = (u16)target->pp;
      heal = target->pp;
#else
      stat = (u16 *)&target->pp;
      before = *stat--;
      heal = *(s16 *)(stat + 1);
#endif
      if (ability->effect == ABILITY_EFFECT_AROMA) {
        maximum = target->ppMax;
        limit = (u16)target->ppMax;
        heal += (s16)maximum / 10;
      } else if (ability->effect == ABILITY_EFFECT_ETHER) {
#ifdef UBFIX
        limit = (u16)target->ppMax;
        maximum = target->ppMax;
#else
        stat--;
        limit = *stat--;
        maximum = *(s16 *)(stat + 1);
#endif
        heal += maximum * 3 / 10;
      } else {
#ifdef UBFIX
        limit = (u16)target->ppMax;
        maximum = target->ppMax;
#else
        stat--;
        limit = *stat--;
        maximum = *(s16 *)(stat + 1);
#endif
        heal += maximum * 7 / 100;
      }
      if (heal > (s16)limit)
        heal = (s16)limit;
      restored = heal - (s16)before;
      if (restored == 0 && kind != ABILITY_FORMULA_HEAL_PP)
        break;
      if (heal == (s16)limit)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, restored);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = heal;
      Unit_RecalculateGaugesFar(targetId);
      break;
    }

    // The stat changes show how far the stat itself moved, 0 at its limit;
    // Zephyr and Vine set Agility's stage outright.
    case ABILITY_EFFECT_AGILITY_UP:
      target->agilityStage = AGILITY_STAGE_DOUBLED;
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, target->agi - savedTarget->agi);
      PUSH_MESSAGE(MsgAgiUp);
      break;

    case ABILITY_EFFECT_AGILITY_DOWN:
      agility = &target->agilityStage;
      {
        u8 stage;

        stage = STAT_STAGE_MIN;
        *agility = stage;
      }
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, savedTarget->agi - target->agi);
      PUSH_MESSAGE(MsgAgiDown);
      break;

    case ABILITY_EFFECT_ATTACK_DOWN:
      target->attackStage -= 1;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:
      target->attackStage -= 2;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP:
      target->attackStage += 1;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP_SHARPLY:
      target->attackStage += 2;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN:
      target->defenseStage -= 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
      target->defenseStage -= 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP:
      target->defenseStage += 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP_SHARPLY:
      target->defenseStage += 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattlePlayback_Push(PLAYBACK_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    // The revivals bring back a downed target with all, half, 80% or 60% of
    // its HP.
    case ABILITY_EFFECT_REVIVE:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->hpMax;
      Unit_RecalculateGaugesFar(targetId);
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->hpMax / 2;
      Unit_RecalculateGaugesFar(targetId);
      break;

    case ABILITY_EFFECT_DEW:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->hpMax * 8 / 10;
      Unit_RecalculateGaugesFar(targetId);
      break;

    case ABILITY_EFFECT_BALM:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = (s16)(target->hpMax * 6 / 10);
      Unit_RecalculateGaugesFar(targetId);
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0)
        PUSH_MESSAGE(MsgCurePoison);
      target->poison = 0;
      break;

    // A Resistance change needs no recalculation to show: it is the change
    // in stages times RESISTANCE_PER_STAGE.
    case ABILITY_EFFECT_RESISTANCE_DOWN:
      target->resistanceStage--;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattlePlayback_Push(PLAYBACK_NUMBER, (savedTarget->resistanceStage -
                                            target->resistanceStage) *
                                               RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY: {
      target->resistanceStage -= 2;
      if (STAT_STAGE_MIN > target->resistanceStage)
        target->resistanceStage = STAT_STAGE_MIN;
      if (target->resistanceStage > STAT_STAGE_MAX)
        target->resistanceStage = STAT_STAGE_MAX;
      BattlePlayback_Push(PLAYBACK_NUMBER, (savedTarget->resistanceStage -
                                            target->resistanceStage) *
                                               RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
    } break;

    case ABILITY_EFFECT_RESISTANCE_UP:
      target->resistanceStage++;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattlePlayback_Push(PLAYBACK_NUMBER, (target->resistanceStage -
                                            savedTarget->resistanceStage) *
                                               RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY: {
      target->resistanceStage += 2;
      if (*(resistance = &target->resistanceStage) < STAT_STAGE_MIN)
        target->resistanceStage = STAT_STAGE_MIN;
      if (target->resistanceStage > STAT_STAGE_MAX)
        target->resistanceStage = STAT_STAGE_MAX;
      BattlePlayback_Push(PLAYBACK_NUMBER, (target->resistanceStage -
                                            savedTarget->resistanceStage) *
                                               RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
    } break;

    // Poison does nothing to a poisoned target, deadly poison nothing to one
    // that already has it.
    case ABILITY_EFFECT_POISON:
      if (target->poison != 0)
        break;
      PUSH_MESSAGE(MsgPoisoned);
      target->poison = POISON_NORMAL;
      break;

    case ABILITY_EFFECT_VENOM:
      if (target->poison > POISON_NORMAL)
        break;
      PUSH_MESSAGE(MsgVenom);
      target->poison = POISON_VENOM;
      break;

    case ABILITY_EFFECT_DELUSION:
    case ABILITY_EFFECT_DAZZLE:
      PUSH_MESSAGE(MsgDelusion);
      target->delusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CONFUSION:
      PUSH_MESSAGE(MsgConfused);
      target->confusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CHARM:
      PUSH_MESSAGE(MsgCharmed);
      target->charm = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_STUN:
    case ABILITY_EFFECT_STUN_THREE_ROLLS:
      PUSH_MESSAGE(MsgStunned);
      target->stun = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SLEEP:
      PUSH_MESSAGE(MsgAsleep);
      target->sleep = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SEAL:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_BIND;
      break;

    case ABILITY_EFFECT_SEAL_2_TURNS:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_LUFF;
      break;

    // Instant death: "X goes down!" on the party, Drown's "X suffocates!"
    // or "X's spirit has been drained!" on an enemy.
    case ABILITY_EFFECT_INSTANT_DEATH:
      if (BattleUnit_KeepsOneHp(targetId) != 0)
        break;
      BattlePlayback_Push(PLAYBACK_DEFEAT, targetId);
      if (target->side == BATTLE_SIDE_PARTY)
        PUSH_MESSAGE(MsgKoDown);
      else if (abilityId == ABILITY_DROWN)
        PUSH_MESSAGE(MsgSuffocate);
      else
        PUSH_MESSAGE(MsgSpiritDrain);
      target->hp = 0;
      Unit_RecalculateGaugesFar(targetId);
      break;

    case ABILITY_EFFECT_REGENERATE:
      PUSH_MESSAGE(MsgHpRegen);
      target->regenerate = REGENERATE_TURNS;
      break;

    case ABILITY_EFFECT_REFLECT:
      PUSH_MESSAGE(MsgReflect);
      target->reflect = STATUS_TURNS;
      break;

    // The drains give the damage dealt back to the user: Drain all of it
    // as HP, Sap half.
    case ABILITY_EFFECT_DRAIN_HP:
    case ABILITY_EFFECT_DRAIN_HALF_HP:
      power = damageDealt;
      amount = actor->hp;
      /* FAKEMATCH(barrier): an empty asm on amount, the user's hit points. */
      __asm__("" : "=r"(amount) : "0"(amount));
      if (ability->effect == ABILITY_EFFECT_DRAIN_HALF_HP)
        power /= 2;
      drained = power;
      amount += drained;
      if (amount > actor->hpMax) {
        amount = actor->hpMax;
        drained = amount - actor->hp;
      }
      BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      if (amount == actor->hpMax)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, drained);
        PUSH_MESSAGE(MsgHpRecover);
      }
      actor->hp = amount;
      Unit_RecalculateGaugesFar(unitId);
      break;

    // Psy Drain gives the PP taken.
    case ABILITY_EFFECT_DRAIN_PP: {
      s32 heal;
      s32 drainedPp;

      heal = actor->pp;
      drainedPp = damageDealt;
      heal += drainedPp;
      if (heal > actor->ppMax) {
        heal = actor->ppMax;
        drainedPp = heal - actor->pp;
      }
      BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      if (heal == actor->ppMax)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattlePlayback_Push(PLAYBACK_NUMBER, drainedPp);
        PUSH_MESSAGE(MsgPpRecover);
      }
      actor->pp = heal;
      Unit_RecalculateGaugesFar(unitId);
      break;
    }

    // Psy Leech: a tenth of the damage as PP, as far as the target has it
    // and the user has room.
    case ABILITY_EFFECT_PSY_LEECH:
      amount = damageDealt / 10;
      if (target->pp < amount)
        amount = target->pp;
      if (actor->pp + amount > actor->ppMax)
        amount = actor->ppMax - actor->pp;
      if (amount == 0)
        break;
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      PUSH_SIDE_MESSAGE(targetId, MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(unitId, amount);
      break;

    // Catastrophe also takes a tenth of the target's max PP, as far as it has
    // it.
    case ABILITY_EFFECT_CATASTROPHE:
      amount = (s16)(target->ppMax / 10);
      if (target->pp < amount)
        amount = target->pp;
      if (amount == 0)
        break;
      BattlePlayback_Push(PLAYBACK_NUMBER, amount);
      PUSH_SIDE_MESSAGE(targetId, MsgMorePpLossP, MsgMorePpLossE);
      Unit_AddPpFar(targetId, -amount);
      break;

    // Break returns raised Attack, Defense, Resistance and Agility stages to
    // 0, leaving lowered ones, and takes away a summon's boost.
    case ABILITY_EFFECT_BREAK:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->summonBoost[ELEMENT_VENUS] = 0;
      target->summonBoost[ELEMENT_MERCURY] = 0;
      target->summonBoost[ELEMENT_MARS] = 0;
      target->summonBoost[ELEMENT_JUPITER] = 0;
      PUSH_MESSAGE(MsgBuffsReset);
      break;

    case ABILITY_EFFECT_HAUNT:
      PUSH_MESSAGE(MsgEvilSpirit);
      target->haunt = 1;
      break;

    case ABILITY_EFFECT_CURSE:
    case ABILITY_EFFECT_CURSE_THREE_ROLLS:
      if (target->grimReaper == 0) {
        PUSH_MESSAGE(MsgDeathCurse);
        target->grimReaper = CURSE_TURNS;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattlePlayback_Push(PLAYBACK_NUMBER, target->grimReaper);
      PUSH_MESSAGE(MsgDeathCount);
      break;

    // Kite: the target acts twice in the next turn (the count drops at each
    // turn's end).
    case ABILITY_EFFECT_ACT_TWICE:
      PUSH_MESSAGE(MsgReadies);
      target->ready = 2;
      break;

    // The target rises to the challenge and won't flee; on the party, no one
    // may run ("No time to run!").
    case ABILITY_EFFECT_CHALLENGE:
      PUSH_MESSAGE(MsgChallenge);
      target->challenge = 1;
      if (IS_PARTY_UNIT(targetId))
        battle->challengeFlag |= 2;
      break;

    // The target loses its next action (BattleActionRun_Build).
    case ABILITY_EFFECT_GROUND:
    case ABILITY_EFFECT_ULYSSES:

      PUSH_MESSAGE(MsgImmobile);
      target->immobile = 1;
      break;

    // The guards: the message shows either way, but a guard is never lowered.
    case ABILITY_EFFECT_DEFEND:

      PUSH_MESSAGE(MsgDefendAura);
      if (target->defendLevel > 0)
        break;
      target->defendLevel = DEFEND_NORMAL;
      break;

    case ABILITY_EFFECT_SHIELD:
      PUSH_MESSAGE(MsgDefendAura);
      if ((u32)target->defendLevel > DEFEND_NORMAL)
        break;
      target->defendLevel = DEFEND_SHIELD;
      break;

    case ABILITY_EFFECT_TOTAL_DEFENSE:
    case ABILITY_EFFECT_GUARD_AURA:
      PUSH_MESSAGE(MsgTotalDefenseAura);
      if (target->defendLevel > DEFEND_SHIELD)
        break;
      target->defendLevel = DEFEND_TOTAL;
      break;

    case ABILITY_EFFECT_VANISH:
      PUSH_MESSAGE(MsgVanished);
      if ((u32)target->defendLevel > DEFEND_TOTAL)
        break;
      target->defendLevel = DEFEND_VANISHED;
      break;

    // Contain, Threaten and Tremble do nothing; the player only reads their
    // message.
    case ABILITY_EFFECT_THREATEN:
      BattlePlayback_Push(PLAYBACK_MESSAGE, MESSAGE_NONE);
      break;

    case ABILITY_EFFECT_CEASE_FIRE:
      PUSH_MESSAGE(MsgAllResting);
      battle->ceaseFire = 1;
      break;

    // Gale blows the target out of the battle, as one that flees.
    case ABILITY_EFFECT_BLOW_AWAY:
      PUSH_MESSAGE(MsgBlownAway);
      BattlePlayback_Push(PLAYBACK_BLOW_AWAY, targetId);
      break;

    // The second equipment total doubles as the readiness to counterattack.
    case ABILITY_EFFECT_COUNTER:
      PUSH_MESSAGE(MsgCounterReady);
      target->equipBonus[1] = 1;
      break;

    default:
      break;
    }
  }

  // 7. Waking from damage, and the stats worked out again.
done:
  BattlePlayback_Push(PLAYBACK_CLEAR_ARGUMENTS, 0);
  // A unit ready to counterattack remembers who struck it.
  if (plan->command != COMMAND_DJINN_COUNTER &&
      (s8)target->equipBonus[1] != 0 &&
      ability->targetMode == ABILITY_TARGET_FOES && target->hp != 0) {
    battle->counterUnitId = targetId;
    battle->counterTargetId = unitId;
  }

  // Damage wakes a sleeping target one time in four, unless it fell asleep this
  // turn.
  if (target->hp != 0) {
    if (target->sleep != 0)
      if (target->sleep < STATUS_TURNS && damageDealt > 0 &&
          (3 & Battle_RandomFar()) == 0) {
        target->sleep = 0;
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgWakes);
      }
  }
  Heap_Release(savedTarget);
  Unit_CalculateFar(targetId);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  if (target->hp != 0)
    BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, targetId);
  // A haunted unit pays, one time in four, a quarter of what it took, at the
  // end of its action; Daedalus's second strike does not count.
  if (abilityId != ABILITY_DAEDALUS_STRIKE && actor->haunt != 0 &&
      (Battle_RandomFar() & 3) == 0 && damageDealt > 0) {
    s32 share;

    share = damageDealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}

/// Returns 1 for abilities that cost PP, and for Psy Drain whatever its cost.
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == ABILITY_PSY_DRAIN)
    return 1;
  pp = Ability_Get(abilityId)->ppCost;
  return pp != 0;
}

#define AI_BATTLE ((struct BattleWork *)Ram_HeapSlots->battleWork)

/// The choices an enemy may fail to carry out before it defends instead.
#define AI_MAX_FAILED_CHOICES 16

/// Chooses an enemy's action: one of the eight in its definition, picked by its
/// action strategy, with an item instead where its item mask says so, and a
/// target worth the ability. Called with retry 0 for each enemy as the turn is
/// built and with retry 1 when it acts (BattleStaging_DispatchAction).
s32 BattleAI_ChooseAction(struct BattleActionRecord *request, s32 retry) {
  struct Unit *unit;
  u32 selection;
  struct BattleAIState *selectionMemory;
  const struct EnemyDefinition *enemy;
  s32 itemsAllowed;
  s32 failedChoices;
  s32 needsChoice;
  const struct AIEnemyRest *halfwords;
  const s8 *actionStrategy;
  const u8 *itemActionMask;
  s16 command;
  s32 allyTarget;
  s32 opponentTarget;
  s32 allyIsParty;
  s32 opponentIsParty;
  struct BattleWork *battle;
  struct ItemDefinition *item;
  s32 useItem;
  s8 strategyKind;
  struct AbilityDefinition *ability;
  s32 abilityId;
  u8 counts[4];
  u8 summons[36];
  s32 summonIndex;
  s32 element;
  const struct SummonDefinition *summon;

  /// Picks a place in the action list from a 0-255 roll and eight weights:
  /// the first place whose running total passes the roll, else place 0.
  s32 BattleEnemy_SelectWeightedAction(u8 *weights) {
    s32 roll;
    s32 total;
    s32 selectedAction;
    s32 actionIndex;

    roll = Battle_RandomFar() & 0xff;
    total = weights[0];
    selectedAction = 0;
    actionIndex = 0;
    if (roll >= total) {
      while (++actionIndex <= 7) {
        total += weights[actionIndex];
        if (roll < total) {
          selectedAction = actionIndex;
          break;
        }
      }
    }
    return selectedAction;
  }

  unit = Unit_Get(request->unitId);
  itemsAllowed = 1;
  failedChoices = 0;
  needsChoice = 1;
  selection = -1U;
  if (unit->classId != CLASS_NONE)
    return -1;

  selectionMemory = &unit->aiState;
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (unit->enemyId == ENEMY_POSEIDON) {
    selectionMemory = &((struct BattleWork *)Ram_HeapSlots->battleWork)
                           ->aiStates[request->turnAction];
    if (request->turnAction == 1)
      enemy = Enemy_GetDefinitionFar(ENEMY_POSEIDON_SECOND_ACTION);
  } else if (unit->enemyId == ENEMY_DOOM_DRAGON_A) {
    // Each later action of the turn plays another of the dragon's animations.
    // XXX: which head each of 6, 3 and 7 shows; recording the Doom Dragon's
    // turns would settle it.
    selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
    switch (request->turnAction) {
    case 1:
      AI_BATTLE->actingAnimation = 6;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_C);
      break;
    case 2:
      AI_BATTLE->actingAnimation = 3;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_B);
      break;
    case 3:
      AI_BATTLE->actingAnimation = 7;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_D);
      break;
    }
  } else if (unit->enemyId == ENEMY_DOOM_DRAGON_E) {
    selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
    switch (request->turnAction) {
    case 1:
      AI_BATTLE->actingAnimation = 6;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_G);
      break;
    case 2:
      AI_BATTLE->actingAnimation = 3;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_F);
      break;
    }
  } else if (unit->enemyId == ENEMY_DOOM_DRAGON_H) {
    selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
    if (request->turnAction == 1) {
      AI_BATTLE->actingAnimation = 3;
      enemy = Enemy_GetDefinitionFar(ENEMY_DOOM_DRAGON_I);
    }
  } else if (unit->enemyId == ENEMY_DULLAHAN) {
    if (selectionMemory->initialized == 0)
      selectionMemory->form = Random_Below(3);
    enemy =
        Enemy_GetDefinitionFar(gBattleAI_DullahanForms[selectionMemory->form]);
  }

  if (retry != 0 && request->command != COMMAND_ABILITY) {
    switch (enemy->actionStrategy) {
    case ACTION_STRATEGY_RANDOM_START:
    case ACTION_STRATEGY_IN_ORDER:
      selectionMemory->selection = selectionMemory->selection + 1;
      break;
    }
    goto done;
  }

  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;
  halfwords = (const struct AIEnemyRest *)((const u8 *)enemy + 2);

select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  case ACTION_STRATEGY_EVEN:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_EVEN]);
    break;
  case ACTION_STRATEGY_FALLING:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_FALLING]);
    break;
  case ACTION_STRATEGY_FIRST_FAVOURED:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_FIRST_FAVOURED]);
    break;
  case ACTION_STRATEGY_RANDOM_START:
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = Battle_RandomFar();
      selectionMemory->initialized = 1;
    }
    // fall through
  case ACTION_STRATEGY_IN_ORDER:
    selection = selectionMemory->selection;
    if (retry != 0)
      selectionMemory->selection = selection + 1;
    break;
  case ACTION_STRATEGY_NEXT_PICK:
    selection++;
    break;
  case ACTION_STRATEGY_SAME:
    break;
  }

  useItem = (*itemActionMask >> selection) & 1;
  useItem &= itemsAllowed;
  abilityId = halfwords->abilities[selection];
  request->command = COMMAND_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(INVENTORY_ITEM_ID_MASK & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == TARGET_STRATEGY_MOST_HP) {
        request->command = COMMAND_ITEM;
        {
          s32 cantUse = ABILITY_CANT_USE;

          request->parameter = cantUse;
        }
        goto done;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      if (item->useType != ITEM_USE_CONSUMED) {
        useItem = 0;
      } else {
        Ability_Get(item->abilityId);
        abilityId = item->abilityId;
        request->command = COMMAND_ITEM;
        request->parameter = 0;
      }
    }
    if (useItem == 0)
      itemsAllowed = 0;
  }

  if (needsChoice != 0) {
    ability = Ability_Get(abilityId);
    switch (ability->effect) {
    case ABILITY_EFFECT_DEFEND:
      request->command = COMMAND_DEFEND;
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    case ABILITY_EFFECT_TOTAL_DEFENSE:
      request->command = COMMAND_TOTAL_DEFENSE;
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = COMMAND_FLEE;
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    case ABILITY_EFFECT_COUNTER:
    case ABILITY_EFFECT_GUARD_AURA:
      if (retry == 0)
        request->priority += PRIORITY_FIRST_IN_TURN;
      break;
    }

    if (retry == 0) {
      switch (request->command) {
      case COMMAND_DEFEND:
      case COMMAND_TOTAL_DEFENSE:
        break;
      default:
        goto done;
      }
    }

    if (request->command != COMMAND_ITEM &&
        Ability_IsPsynergy(abilityId) != 0) {
      request->command = COMMAND_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp &&
          enemy->targetStrategy != TARGET_STRATEGY_MOST_HP)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == TARGET_STRATEGY_RANDOM)
        goto next_candidate;
    }
    command = request->command;
    if (command == COMMAND_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 &&
        (command == COMMAND_DEFEND || command == COMMAND_TOTAL_DEFENSE))
      goto next_candidate;

    if (command == COMMAND_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == ABILITY_ATTACK)
        request->command = COMMAND_ATTACK;
      if (abilityId == ABILITY_CRUCIBLE) {
        request->command = COMMAND_SUMMON;
        BattleDjinn_CountStandby(0, counts);
        summonIndex = Summon_ListLearnedFar(summons);
        for (summonIndex--; summonIndex >= 0; summonIndex--) {
          summon = SummonDefinition_Get(summons[summonIndex]);
          for (element = 0; element <= ELEMENT_JUPITER &&
                            summon->djinnRequired[element] <= counts[element];
               element++)
            ;
          if (element == ELEMENT_COUNT)
            break;
        }
        if (summonIndex < 0)
          goto next_candidate;
        request->parameter = summons[summonIndex];
        abilityId = SummonDefinition_Get(summons[summonIndex])->abilityId;
        ability = Ability_Get(abilityId);
      }
    }

    request->range = ability->range;
    switch (ability->targetMode) {
    case ABILITY_TARGET_ALLIES:
    case ABILITY_TARGET_SELF:
      if (abilityId == ABILITY_GUARD_AURA || abilityId == ABILITY_EARNEST_PLY)
        allyTarget = BattleAI_ChooseTarget(FIRST_ENEMY_UNIT, ability);
      else
        allyTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (allyTarget == TARGET_PICK_SIDE_EMPTY) {
        allyIsParty = 0;
        if ((u16)request->unitId <= ALLY_MAX)
          allyIsParty = 1;
        allyTarget = BattleAI_ChooseRandomTarget(allyIsParty);
      }
      if (allyTarget != TARGET_PICK_NONE) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case ABILITY_TARGET_FOES:
      opponentTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (opponentTarget == TARGET_PICK_SIDE_EMPTY) {
        opponentIsParty = 0;
        if ((u16)request->unitId <= ALLY_MAX)
          opponentIsParty = 1;
        opponentTarget = BattleAI_ChooseRandomTarget(opponentIsParty);
      }
      if (opponentTarget != TARGET_PICK_NONE) {
        request->target = opponentTarget;
        needsChoice = 0;
      }
      break;
    case 3: // Unused: no ability's target mode is 3.
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    default:
      if (ability->effect == ABILITY_EFFECT_CALL_REINFORCEMENT &&
          !Enemy_HasSpriteRoom(unit->enemyId == ENEMY_STAR_MAGICIAN
                                   ? ENEMY_REFRESH_BALL
                                   : ENEMY_SEA_FIGHTER))
        break;
      battle = (struct BattleWork *)Ram_HeapSlots->battleWork;
      if (ability->effect == ABILITY_EFFECT_CALL_QUEUED) {
        if (battle->spoils.queueCount == 0)
          break;
        if (!Enemy_HasSpriteRoom(
                battle->spoils.queuedSummons[battle->spoils.queueHead]))
          break;
      }
      request->target = Battle_FindUnitPlace(request->unitId);
      needsChoice = 0;
      break;
    }
  }
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  if (needsChoice != 0 && failedChoices > AI_MAX_FAILED_CHOICES) {
    request->command = COMMAND_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  if (unit->enemyId == ENEMY_DULLAHAN && retry == 1 &&
      selectionMemory->selection == 0)
    selectionMemory->form = (selectionMemory->form + 1) % 3;
  return 0;
}

/// Unused: makes three empty DMA3 transfers after a multiplication whose result
/// it drops.
void Dma_StopAllThenRunHook(void) {
  Math_UnsignedMulHigh(5, 0);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
}
