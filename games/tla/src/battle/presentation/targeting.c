/// @file
/// Applying an ability to each of its targets, the battle events queued on the
/// way, and the wait for the player to advance a battle message.
// Part of presentation.c.

#include "../presentation_internal.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "item_icon.h"
#include "menu_list.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"
#include "mul_q16.h"
#include "resource.h"
#include "game_flags.h"

#define TARGET_MODE_NONE 0
#define TARGET_MODE_FOES 1
#define TARGET_MODE_ALLIES 2
#define TARGET_MODE_SELF 4

#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

/* VIEW(f11f66c): routine Unit_Get as void *Unit_Get(s32 owner); strongest_unit
 * has struct BattleUnit *Unit_Get(s32 unitId); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares,
 * BattleAI_ChooseTarget moves: 5 of 776 instructions, BattleUnit_ClearStatuses
 * moves: 1 of 97 instructions, BattleObject_DestroyTemporaryObject moves: 1 of
 * 24 instructions, 32 more in tla-ja. */
extern void *Unit_Get__f11f66c(s32 owner) __asm__("Unit_Get");

#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    if ((u8)(unit)->confusion != 0)                                            \
      (count)++;                                                               \
    if ((unit)->charm != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->elementLevels[0] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[1] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[2] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[3] > 0)                                          \
      (count)++;                                                               \
  }

#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case ABILITY_EFFECT_ATTACK_UP_2:                                           \
    case ABILITY_EFFECT_ATTACK_UP:                                             \
      if ((unit)->attackStage + 1 <= 4)                                        \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_ATTACK_DOWN_2:                                         \
    case ABILITY_EFFECT_ATTACK_DOWN:                                           \
      if ((unit)->attackStage - 1 >= -4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_UP_2:                                          \
    case ABILITY_EFFECT_DEFENSE_UP:                                            \
      if ((unit)->defenseStage + 1 <= 4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_DOWN_2:                                        \
    case ABILITY_EFFECT_DEFENSE_DOWN:                                          \
      if ((unit)->defenseStage - 1 >= -4)                                      \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_UP_2:                                       \
    case ABILITY_EFFECT_RESISTANCE_UP:                                         \
      if ((unit)->resistanceStage + 1 <= 4)                                    \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_DOWN_2:                                     \
    case ABILITY_EFFECT_RESISTANCE_DOWN:                                       \
      if ((unit)->resistanceStage - 1 >= -4)                                   \
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
    case ABILITY_EFFECT_HEAL_HP_60:                                            \
    case ABILITY_EFFECT_HEAL_HP_30:                                            \
    case 70:                                                                   \
    case 71:                                                                   \
      if ((unit)->hp < (unit)->maxHp)                                          \
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
    case ABILITY_EFFECT_DEADLY_POISON:                                         \
      if ((unit)->poison <= 1)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_STUN:                                                  \
    case 85:                                                                   \
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
    case ABILITY_EFFECT_REVIVE_80:                                             \
    case 73:                                                                   \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case ABILITY_EFFECT_TOTAL_DEFENSE:                                         \
    case 88:                                                                   \
      if ((unit)->guardLevel <= 1)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case 0:                                                                    \
    case 1:                                                                    \
    case 2:                                                                    \
      break;                                                                   \
    default:                                                                   \
      (applies) = 1;                                                           \
      break;                                                                   \
    }                                                                          \
    if ((unit)->hp == 0 && Battle_IsReviveEffectFar((action)->effect) == 0)    \
      (applies) = 0;                                                           \
    if ((applies) == 0) {                                                      \
      (kindIndex) = ((action)->flags & ABILITY_KIND_MASK) - 1;                 \
      switch (kindIndex) {                                                     \
      case ABILITY_KIND_ATTACK - 1:                                            \
      case ABILITY_KIND_ATTACK_MULTIPLIED - 1:                                 \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_DAMAGE - 1:                                            \
      case ABILITY_KIND_UNLEASHED_DAMAGE - 1:                                  \
      case ABILITY_KIND_SUMMON - 1:                                            \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_PP_DAMAGE - 1:                                         \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_HP - 1:                                           \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->maxHp)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_PP - 1:                                           \
        if ((unit)->pp < (unit)->maxPp)                                        \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_EFFECT - 1:                                            \
      case 9 - 1:                                                              \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case 7 - 1:                                                              \
        (applies)++;                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

#define STRATEGY_MOST_HP 0
#define STRATEGY_MOST_MAX_HP 1
#define STRATEGY_RANDOM 2

s32 BattleAI_ChooseTarget(s32 actorId, struct BattleAct *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct BattleUnit *unit;
  struct BattleUnit *nextUnit;
  struct BattleUnit *thisUnit;
  s32 candidateCount;
  s32 targetIndex;
  s32 targetCount;
  s32 innerIndex;
  s32 selected;
  s32 unitId;
  s32 value;
  s32 nextValue;
  s32 temp;
  s32 ownSide;
  s32 kindIndex;
  s32 applies;
  u32 roll;
  s16 *slot;

  candidateCount = 0;
  turnOrder = Ram_HeapSlots->battleWork;
  targetCount = 0;

  if (action->targetMode != TARGET_MODE_NONE) {
    ownSide = 0;
    if (action->targetMode == TARGET_MODE_ALLIES ||
        action->targetMode == TARGET_MODE_SELF)
      ownSide = 1;

    if (((actorId < 0 || actorId > 7) && ownSide == 0) ||
        (actorId >= 0 && actorId <= 7 && ownSide != 0)) {
      targetIndex = 0;
      while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_PARTY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    } else {
      targetIndex = 0;
      while (turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END) {
        unitId = turnOrder->enemyIds[targetIndex];
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_ENEMY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    }
  }

  if (candidateCount == 0)
    return -2;

  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = Unit_Get__f11f66c(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return -1;

  if (action->targetMode == TARGET_MODE_FOES && action->range == 1 &&
      Enemy_GetDefinitionFar(
          ((struct BattleUnit *)Unit_Get__f11f66c(actorId))->enemyId)
              ->targetStrategy != STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_KIND_MASK) - ABILITY_KIND_ATTACK) <= 2) {
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_Get__f11f66c(unitIds[innerIndex]);
        nextUnit = Unit_Get__f11f66c(unitIds[innerIndex + 1]);
        if (Enemy_GetDefinitionFar(
                ((struct BattleUnit *)Unit_Get__f11f66c(actorId))->enemyId)
                ->targetStrategy == STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->maxHp;
          nextValue = nextUnit->maxHp;
        }
        if (value < nextValue) {
          temp = unitIds[innerIndex];
          unitIds[innerIndex] = unitIds[innerIndex + 1];
          unitIds[innerIndex + 1] = temp;
          temp = targetPositions[innerIndex];
          targetPositions[innerIndex] = targetPositions[innerIndex + 1];
          targetPositions[innerIndex + 1] = temp;
        }
      }
    }

    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = (11 * Random16()) >> 16;
      selected = 0;
      if (roll > 5)
        selected = 1;
      break;
    case 3:
      selected = (15 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = (18 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else if (selected <= 14)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[(Random16() * targetCount) >> 16];
}

void BattleUnit_ClearStatuses(s32 actor) {
  u8 *state;
  u8 *cursor;
  s32 count;
  u8 zero;

  state = Unit_Get__f11f66c(actor);
  zero = 0;
  count = 3;
  cursor = state + 0x12f;
  do {
    count--;
    *cursor-- = zero;
  } while (count >= 0);

  state[0x131] = 0;
  state[0x132] = 0;
  state[0x133] = 0;
  state[0x134] = 0;
  state[0x135] = 0;
  state[0x136] = 0;
  state[0x137] = 0;
  state[0x138] = 0;
  state[0x139] = 0;
  state[0x13a] = 0;
  state[0x13b] = 0;
  state[0x13c] = 0;
  state[0x13d] = 0;
  state[0x13e] = 0;
  state[0x13f] = 0;
  state[0x140] = 0;
  state[0x141] = 0;
  state[0x142] = 0;
  state[0x143] = 0;
  state[0x144] = 0;
  state[0x145] = 0;
  state[0x146] = 0;
  state[0x147] = 0;
  state[0x148] = 0;

  Unit_CalculateFar(actor);
  BattleUnit_BuildStatusFlags(actor, BattleObject_Get(actor));
}

/// Waits for the player to advance a battle message: once the window has
/// finished printing, shows the bobbing arrow at its corner until B is held,
/// or A, B, L or R is pressed (or held, after 16 frames), then plays the
/// cursor sound. The Broken Seal's BattleMessage_WaitForAdvance, with the
/// message window read from its heap slot.
void BattleMessage_WaitForAdvance(void) {
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 pictureTiles;
  s32 sheet;
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;

  while (!Message_AllTypedFar())
    wait(1);
  prompt = &promptStorage;
  // The arrow sheet's address held in a local is added after the window's
  // loads; at its use, 36 of 146 instructions differ in tla-en.
  sheet = (s32)gBattleMessage_AdvanceArrowTiles;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight pictures, one every four frames.
    pictureTiles = ((gFrameCount >> 2) & 7) * 128 + sheet;
    window = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->window;
    offset = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->offset;
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, 4);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    ((u32 *)prompt)[1] = 0xa400;
    ((u32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pictureTiles);
    prompt->x = window->x * 8 + (offset->x >> 8) + 4;
    // Bob up and down by up to 2 pixels, once every 16 frames.
    prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                (offset->y >> 8) + 6;
    Oam_AddSpriteAtDepth(prompt, 240);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one.
    // The held keys reread through a volatile pointer for the last test, a
    // load apart from the B test's; as gInput.held, 14 of 146 instructions
    // differ in tla-en.
    if ((gInput.held & 2) || (gInput.pressed & 0x303) ||
        (frame > 15 && (*(volatile u32 *)&gInput.held & 0x303)))
      break;
    wait(1);
  }
  Sound_PlayFar(111);
  VramCache_FreeEntry(tileSlot);
  wait(1);
}

/// The Broken Seal's routine: shows the cursor
/// bobbing at (x, y) and waits for A, B, L or R; returns 1.
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
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, 4);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    ((s32 *)prompt)[1] = 0x40000000;
    ((s32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pointerTiles);
    prompt->x = position.x + ((gFrameCount & 4) >> 1) - 4;
    prompt->y = position.y - ((gFrameCount & 4) >> 2) - 8;
    Oam_AddSpriteAtDepth(prompt, 240);
    if (gInput.pressed & 0x303)
      break;
    wait(1);
  }
  VramCache_FreeEntry(tileSlot);
  wait(1);
  return 1;
}

void BattleMessage_WaitForTyping(void) {
  while (!Message_AllTypedFar())
    wait(1);
  ((s32 *)Ram_HeapSlots->unk84[4])[2] = 1;
}

s32 BattleObject_DestroyTemporaryObject(s32 arg0) {
  // UB: declared s32 but returns no value; no caller reads it.
  struct BattleUnit *creature;
  struct BattleObj *runtime;

  creature = Unit_Get__f11f66c(arg0);
  if (creature->side == 1) {
    Unit_SetHpFar(creature, 0);
    BattleObject_RemoveFromLists(arg0);
    BattleObject_MarkForRelease(arg0);
    runtime = BattleObject_Get(arg0);
    Actor_ReleaseFar((struct Actor *)runtime->object);
    runtime->object = 0;
    runtime->active = 0;
  }
}

void BattlePlayback_GrantDjinnBonus(struct BattleEventFlagWork *runtime,
                                    u32 operand) {
  runtime->flags |= 1;
}

/* VIEW(f11f66c): routine BattleMotion_FlashHit as void
 * BattleMotion_FlashHit(u32 unitId, s32 a, s32 b); the definition has void
 * BattleMotion_FlashHit(s32 id); tried as the definition's void
 * BattleMotion_FlashHit(s32 id), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/battle/battle.c:6851: too many arguments to
 * function `BattleMotion_FlashHit'. */
extern void
BattleMotion_FlashHit__f11f66c(u32 unitId, s32 a,
                               s32 b) __asm__("BattleMotion_FlashHit");

/// Runs the queued battle events in order, then resets the queue.
u32 BattlePlayback_RunAll(void) {
  struct BattleWork *work;
  struct BattleEventState *runtime;
  struct BattleEventQueue *queue;
  s32 i;

  work = Ram_HeapSlots->battleWork;
  runtime = &work->events;
  queue = &runtime->queue;

  for (i = 0; i < queue->count; i++) {
    u8 opcode = queue->opcodes[i];
    if (opcode <= 15)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        BattlePlayback_GrantDjinnBonus(runtime, queue->operands[i]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleObject_DestroyTemporaryObject(queue->operands[i]);
        break;
      case BATTLE_EVENT_UNIT_NAME:
        MessageArgument_PushFar(queue->operands[i], 1);
        break;
      case BATTLE_EVENT_NUMBER:
        MessageArgument_PushFar(queue->operands[i], 5);
        break;
      case BATTLE_EVENT_ITEM_NAME:
        MessageArgument_PushFar(queue->operands[i] & 0x1ff, 2);
        break;
      case BATTLE_EVENT_ABILITY_NAME:
        MessageArgument_PushFar(queue->operands[i] & 0x3fff, 4);
        break;
      case BATTLE_EVENT_MARK:
        ((struct BattleDisplayFlags *)Ram_HeapSlots->unk84[4])->marked = 1;
        break;
      case BATTLE_EVENT_CLEAR_ARGUMENTS:
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_MESSAGE:
        if ((s32)queue->operands[i] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[i]);
        BattleMessage_WaitForAdvance();
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[i] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[i]);
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_HIT:
        if (runtime->pendingCue > 0)
          Sound_PlayFar(runtime->pendingCue);
        BattleMotion_FlashHit__f11f66c(queue->operands[i], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
        BattleUnit_ClearStatuses(queue->operands[i]);
        BattleMotion_ShowDowned(queue->operands[i]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[i],
                                    BattleObject_Get(queue->operands[i]));
        BattleObject_UpdatePose(queue->operands[i]);
        break;
      case 15:
        BattleObject_RemoveFromLists(queue->operands[i]);
        break;
      }
  }
  return BattlePlayback_Reset();
}

void BattlePlayback_Push(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &Ram_HeapSlots->battleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
}

// Battle: identify effects that apply additional effects to defeated targets.

/// Returns 1 for an effect that still applies when the ability's damage has
/// just knocked its target out: the drains, which give the damage they dealt
/// back to the user.
/// effect: effect ID.
s32 Ability_EffectAppliesToDowned(s32 effect) {
  switch (effect) {
  case 31: // Drain HP.
  case 32: // Drain PP.
  case 60: // Drain HP (half).
  case 69: // Drain PP (one tenth).
    return 1;
  default:
    return 0;
  }
}

s32 BattleUnit_KeepsOneHp(s32 unitId) {
  struct BattleUnit *unit = Unit_Get__f11f66c(unitId);

  if (unit->enemyId == 110 || unit->enemyId == 113)
    return 1;
  return 0;
}

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

    if (value == 254) {
      if (divider >= 0 && slot >= 0) {
        s32 distance = i - divider;

        if (distance < 0)
          distance = divider - i;
        /* FAKEMATCH(barrier): an empty asm that, with the one below, keeps
         * side's 1 and counts' address inside the loop instead of hoisted
         * into r8 and ip; without it, 32 of 67 instructions differ in
         * tla-en. */
        __asm__("");
        if (slot - divider >= 0) {
          if (distance >= slot - divider)
            goto next;
        } else if (distance >= divider - slot) {
          goto next;
        }
      }
      slot = i;
      /* FAKEMATCH(barrier): the second empty asm keeping the loop's 1 and
       * counts' address inside it; without it, 32 of 67 instructions differ
       * in tla-en. */
      __asm__("");
    next:;
    } else {
      if (value == 255)
        break;
      if (value == 128) {
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
    list[i + 1] = 255;
  }
}

// An ability applied to one target: damage, healing and its added effect.

#define ALLY 2 /* Ally. */

#define STEP_LIM(v)                                                            \
  {                                                                            \
    if ((v) < -4)                                                              \
      (v) = -4;                                                                \
    if ((v) > 4)                                                               \
      (v) = 4;                                                                 \
  }

#define ALLY_CHECK(id) ((id) >= 0 && (id) < 8)

#define ELEMENT_BONUS()                                                        \
  {                                                                            \
    if (element != 4)                                                          \
      bonus = power - target->element[element].resistance;                     \
  }

#define MESSAGE_PUSH(m) BattlePlayback_Push(BATTLE_EVENT_MESSAGE, (u32)(m))

#define MESSAGE_SIDE(m, e)                                                     \
  {                                                                            \
    if (ALLY_CHECK(tid))                                                       \
      MESSAGE_PUSH(m);                                                         \
    else                                                                       \
      MESSAGE_PUSH(e);                                                         \
  }

#define GUARD_CUT()                                                            \
  {                                                                            \
    guard = target->guardLevel;                                                \
    if (guard != 0) {                                                          \
      if (guard == 1)                                                          \
        dmg /= 2;                                                              \
      else if (guard == 2)                                                     \
        dmg = dmg * 2 / 5;                                                     \
      else                                                                     \
        dmg /= 10;                                                             \
    }                                                                          \
  }

#define RES_STEP(d, v, message)                                                \
  {                                                                            \
    target->resistanceStage += (d);                                            \
    STEP_LIM(target->resistanceStage);                                         \
    BattlePlayback_Push(BATTLE_EVENT_NUMBER, (v));                             \
    MESSAGE_PUSH(message);                                                     \
    target->resistanceTurns = 7;                                               \
  }

#define STAT_SET(f, message)                                                   \
  {                                                                            \
    MESSAGE_PUSH(message);                                                     \
    (f) = 7;                                                                   \
  }

/// Applies the acting unit's ability to one of its targets: damage or
/// healing by the ability kind, then its added effect, as The Broken Seal's
/// BattleAbility_ApplyToTarget does, with this game's differences.
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot) {
  struct BattleAct *action;
  s32 abilityId;
  struct BattleUnit *actor;
  s32 aid;
  s32 element;
  s32 bonus;
  struct BattleWork *work;
  s32 dist;
  s32 half;
  s32 adj;
  s32 dealt;
  s32 crush;
  s32 hit;
  s32 crit;
  s32 skip;
  s32 type;
  s32 aff;
  struct BattleUnit *bak;
  s32 power;
  s32 hp;
  s32 kind;
  s32 scale;
  s32 pass;
  s16 *cmd;
  u8 tid;
  s8 *rm;
  struct BattleUnit *target;
  u32 n;
  s32 value;
  s32 dmg;
  s32 ap;
  s8 *am;
  s32 guard;
  s16 list[8];
  s32 cnt;
  s32 tmp;
  s32 size;
  bonus = 0;
  work = Ram_HeapSlots->battleWork;
  half = 0;
  dealt = 0;
  crush = 0;
  skip = 0;
  aff = 0;
  size = sizeof(struct BattleUnit);
  bak = Heap_Alloc(size);

  if (abilityId == 312 || abilityId == 316)
    aid = plan->actorId2;
  else
    aid = plan->actorId;
  tid = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  adj = plan->targetMultiplier[slot];
  crit = plan->targetCritical[slot];

  action = Ability_Get(abilityId);
  actor = Unit_Get__f11f66c(aid);
  target = Unit_Get__f11f66c(tid);
  Iwram_CopyWordsEntry(bak, target, size);
  switch (target->enemyId) {
  case 221:
    if (plan->command == 1 && (u32)(aid ^ tid) >> 7 != 0) {
      BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgDissipated);
      goto done;
    }
    break;
  case 101:
  case 102:
  case 103:
    if (abilityId != 573 && work->unk868 != 0 && (u32)(aid ^ tid) >> 7 != 0) {
      BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgFieldProtects);
      goto done;
    }
    break;
  }
  if (target->guardLevel == 4 && (u32)(aid ^ tid) >> 7 != 0) {
    BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
    BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgNoEffect);
    goto done;
  }

  if (action->range != 255) {
    dist = plan->targetOffset[slot];
    if (dist < 0)
      dist = -dist;
  } else {
    dist = 0;
  }

  if (element != 4) {
    s32 i;

    value = target->baseElement[element].resistance;
    for (i = 0; i < 4 && value >= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = -1;

    for (i = 0; i < 4 && value <= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = 1;
  }

  if (plan->element >= 0 && plan->element <= 3) {
    cmd = &plan->command;
    if (*cmd != 2) {
      power = actor->element[plan->element].power;
      goto power_end;
    }
  } else
    cmd = &plan->command;
  power = 100;
power_end:

  if (plan->command == 5 && (plan->element >= 0 && plan->element <= 3) &&
      aff > 0) {
    s32 chance;

    chance = power - target->element[plan->element].resistance;
    chance += 30;
    chance *= 65536 / 100;
    if (chance > (Battle_RandomFar() & 0xffff))
      BattlePlayback_Push(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  if (abilityId == 686 || abilityId == 357)
    Battle_ApplyActionExtras(tid, 0, 0);
  if (abilityId == 720 || abilityId == 721)
    Battle_ApplyActionExtras(tid, 1, 0);
  if (abilityId == 728)
    Battle_ApplyActionExtras(tid, 1, 1);
  type = action->flags & 15;
  {
    s32 res;

    res = plan->targetHit[slot];
    if (res == -1)
      hit = Battle_RollEffectSuccessFar(aid, tid, element, action->effect,
                                        gHitTable[dist]);
    else
      hit = res;
  }
  if (action->effect == 50 || action->effect == 51 || action->effect == 86 ||
      action->effect == 87) {
    s32 cls;
    s32 ent;
    s32 chg;

    cls = actor->enemyId;
    chg = -1;
    ent = Enemy_GetFreeUnitId();
    if (action->effect == 51) {
      cls = BattleFormation_GetRandomMember(work->enemyGroup);
    } else if (action->effect == 86) {
      if (actor->enemyId == 164)
        cls = (Battle_RandomFar() & 3) + 378;
      else
        cls = 81;
    } else if (action->effect == 87) {
      if (work->queueCount != 0) {
        cls = work->queuedSummons[work->queueHead];
        chg = work->queuedCharges[work->queueHead];
      } else
        hit = 0;
    }
    if (hit != 0 && Enemy_HasSpriteRoom(cls) != 0 && ent >= 0) {
      if (chg == -1) {
        chg = BattleEnemy_ClaimNameLetter(cls, 1);
        if (chg & 0x8000)
          BattleEnemy_LetterFirstOfKind(cls);
      }
      BattleUnit_SetFar(ent, cls, chg & 0x7fff);
      if (action->effect == 87) {
        work->queueCount--;
        work->queueHead = (work->queueHead + 1) % 2;
      }
      if (actor->enemyId == 164)
        BattleParty_InsertUnitCentered(work->enemyIds, ent);
      else {
        s32 i;

        for (i = 0; i < 6; i++) {
          if (work->enemyIds[i] == 254) {
            work->enemyIds[i] = ent;
            break;
          }
          if (work->enemyIds[i] == 255) {
            work->enemyIds[i] = ent;
            work->enemyIds[i + 1] = 255;
            break;
          }
        }
      }
      Enemy_Update();
      {
        struct BattleObj *obj;

        obj = BattleObject_Get(ent);
        BattleObject_Make((struct BattleObj *)obj, ent, obj->anchorX / 0x10000,
                          obj->anchorZ / 0x10000);
      }
      BattleObject_Put();
      {
        s32 num;

        num = BattleEnemy_List(list);
        for (cnt = 0; cnt < num; cnt++)
          BattleMotion_ReturnToSpot((u16)list[cnt]);
      }
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, ent);
      if (action->effect == 87)
        MESSAGE_PUSH(MsgUpToFight);
      else if (abilityId != 503)
        MESSAGE_PUSH(MsgAppears);
      else
        MESSAGE_PUSH(MsgSplitOff);
    } else if (abilityId == 503) {
      MESSAGE_PUSH(MsgSplitFailed);
    } else {
      MESSAGE_PUSH(MsgNoOneCame);
    }
  }

  if (hit != 0) {
    if (action->effect == 53 || action->effect == 83) {
      hit = 0;
      for (n = 0; n < 20; n++) {
        if (work->actions[n].unitId == tid) {
          hit = 1;
          break;
        }
      }
    } else if (action->effect == 35) {
      half = 1;
    } else if (action->effect == 34) {
      crush = 1;
    } else if (action->effect == 27) {
      skip = 1;
    } else if (action->effect == 55) {
      if (actor->hp != 0)
        BattlePlayback_Push(BATTLE_EVENT_REMOVE_UNIT, aid);
    } else if (action->effect == 32) {
      if (target->pp != 0)
        type = 10;
      else
        hit = 0;
    } else if (action->effect == 90) {
      half = 2;
    } else if (action->effect == 91) {
      half = 2;
    }
  }

  if ((action->effect != 75 || slot == 0) && skip == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(action->effect) != 0)) {
    s32 pp;
    s32 cur;

    switch (type) {
    case 3:
    case 4: {
      s32 def;

      if (target->guardLevel == 4)
        break;
      hp = target->hp;
      def = target->def;
      scale = def;
      if (half != 0) {
        if (half == 2)
          scale = 0;
        else
          scale = def / 2;
      }
      pass = 1;
      do {
        ELEMENT_BONUS();
        if (pass == 0)
          bonus = 0;
        ap = action->power;
        if (type == 4)
          dmg = Battle_CalculateAttackDamageFar(actor->atk, scale, 0, bonus) *
                ap / 10;
        else {
          s32 atk;

          atk = actor->atk;
          if (abilityId == 312 || abilityId == 316) {
            atk = ((struct BattleUnit *)Unit_Get__f11f66c(plan->actorId))->atk;
            if (abilityId == 312)
              atk += ((struct BattleUnit *)Unit_Get__f11f66c(plan->actorId2))
                         ->atk /
                     4;
          }
          dmg = Battle_CalculateAttackDamageFar(atk, scale, ap, bonus);
        }
        dmg *= adj;
        if (crit != 0) {
          if (crit == 1)
            dmg = dmg * 5 / 4;
          else
            dmg = dmg * 3 / 2;
          dmg += (target->level / 5) + 6;
          if (pass == 0) {
            BattlePlayback_Push(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              message = (s32)MsgCritical;
              if (ALLY_CHECK(tid))
                message += 1;
              BattlePlayback_Push(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if (dmg <= 0)
          dmg = 1;
        if (crush != 0) {
          if (dmg < hp - 1) {
            dmg = hp - 1;
            if (dmg <= 0)
              dmg = 1;
          }
        }
        if (GameFlag_Get(366) != 0 && plan->command == 5 && hp <= dmg) {
          dmg = hp - 1;
        }
        pass++;
      } while (pass <= 1);
      hp -= dmg;
      BattlePlayback_Push(BATTLE_EVENT_HIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
      }

      if (hp <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        hp = 1;
      if (hp <= 0) {
        hp = 0;
        BattlePlayback_Push(BATTLE_EVENT_DEFEAT, tid);
        if (target->enemyId != 369 && target->enemyId != 373) {
          BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
          MESSAGE_SIDE(MsgGoesDown, MsgFelled);
        }
      } else
        BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - hp;
      target->hp = hp;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 10: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpLossTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      if (action->effect == 32 && dmg > pp)
        dmg = pp;
      if (dmg > pp)
        dmg = pp;

      BattlePlayback_Push(BATTLE_EVENT_HIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgPpLossP;
        else
          message = (s32)MsgPpLossE;
        BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->pp - pp;
      target->pp = pp;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 1: {
      if (action->power == 0)
        break;
      cur = target->hp;
      if (cur == 0)
        break;

      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gHpRecoveryTable[dist] / 100;
      dmg *= adj;
      dmg += Battle_RandomFar() & 3;
      cur += dmg;
      if (cur > target->maxHp) {
        cur = target->maxHp;
        dmg = cur - target->hp;
      }
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (cur == target->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgHpRecover);
      }
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case -1: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpDmgTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      BattlePlayback_Push(BATTLE_EVENT_HIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgP;
        else
          message = (s32)MsgDmgE;
        BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      target->pp = pp;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 5:
    case 6:
    case 8: {
      s32 round;

      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      cur = target->hp;
      round = 1;
      do {
        ELEMENT_BONUS();
        if (round == 0)
          bonus = 0;
        ap = action->power;
        if (abilityId == 683 || abilityId == 673 || abilityId == 724 ||
            plan->command == 6 || plan->command == 10) {
          s32 rate;

          rate = 0;
          switch (abilityId) {
          default:
          case 393:
            break;
          case 406:
          case 388:
          case 396:
          case 380:
            rate = 3;
            break;
          case 409:
          case 399:
          case 391:
          case 383:
            rate = 12;
            break;
          case 401:
            rate = 12;
            break;
          case 683:
            rate = 35;
            break;
          case 407:
          case 400:
          case 397:
          case 389:
          case 384:
          case 381:
            rate = 6;
            break;
          case 398:
          case 390:
          case 408:
          case 382:
            rate = 9;
            break;
          case 392:
          case 410:
            rate = 9;
            break;
          case 402:
            rate = 7;
            break;
          case 411:
          case 403:
          case 385:
            rate = 15;
            break;
          case 394:
            rate = 21;
            break;
          case 412:
            rate = 24;
            break;
          case 673:
          case 386:
            rate = 30;
            break;
          case 724:
          case 404:
            rate = 40;
            break;
          }
          value = target->maxHp;
          if (value > 10000)
            value = 10000;
          ap += value * rate / 100;
        }
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        switch (type & 15) {
        case 5:
          dmg = gHpDmgTable5[dist] * dmg / 100;
          break;
        case 8:
          dmg = dmg * gHpDmgTable8[dist] / 100;
          break;
        case 6:
          dmg = dmg * gHpDmgTable6[dist] / 100;
          break;
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if ((gMainRoutineWork.battleRule24b == 6 ||
             (GameFlag_Get(366) != 0 && plan->command == 6)) &&
            cur > dmg) {
          dmg = cur;
        }
        round++;
      } while (round <= 1);
      BattlePlayback_Push(BATTLE_EVENT_HIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
        cur -= dmg;
      }

      if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        cur = 1;
      if (gMainRoutineWork.battleRule24b == 6)
        cur = 0;
      if (cur <= 0) {
        BattlePlayback_Push(BATTLE_EVENT_DEFEAT, tid);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          cur = 0;
          if (ALLY_CHECK(tid))
            message = (s32)MsgGoesDown;
          else
            message = (s32)MsgFelled;
          BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
        }
      } else
        BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 11: {
      if (action->power == 0)
        break;
      pp = target->pp;
      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gPpRecoveryTable[dist] / 100;
      dmg *= adj;
      pp += dmg;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        dmg = pp - target->pp;
      }
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (pp == target->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = pp;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 0:
    case 7:
    case 9:
      break;

    case 2:
      if (target->guardLevel == 4)
        break;
      if (hit != 0) {
        if (action->power == 0)
          break;
        cur = target->hp;
        ELEMENT_BONUS();
        ap = action->power;
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        dmg = dmg * gHpDmgTable[dist] / 100;
        GUARD_CUT();
        BattlePlayback_Push(BATTLE_EVENT_HIT, tid);
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          if (ALLY_CHECK(tid))
            message = (s32)MsgDmgP;
          else
            message = (s32)MsgDmgE;
          BattlePlayback_Push(BATTLE_EVENT_MESSAGE, message);
          cur -= dmg;
        }

        if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
          cur = 1;
        if (cur <= 0) {
          BattlePlayback_Push(BATTLE_EVENT_DEFEAT, tid);
          cur = 0;
          if (target->enemyId != 369 && target->enemyId != 373) {
            BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
            MESSAGE_SIDE(MsgGoesDown, MsgFelled);
          }
        } else
          BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
        dealt = target->hp - cur;
        target->hp = cur;
        Unit_RecalculateGaugesFar(tid);
        break;
      }
      BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgNoEffect);
      break;
    }
  }

  if (work->unk868 != 0 && abilityId == 573) {
    BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgFieldBroken);
    work->unk868 = 0;
  }
  BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
  if ((action->effect != 75 || slot != 0) &&
      (Battle_IsReviveEffectFar(action->effect) != 0 || target->hp != 0 ||
       Ability_EffectAppliesToDowned(action->effect) != 0) &&
      hit != 0) {
    switch (action->effect) {

    case 64:
      if (target->delusion != 0) {
        target->delusion = 0;
        MESSAGE_PUSH(MsgCureDelusion);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        MESSAGE_PUSH(MsgWakes);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        MESSAGE_PUSH(MsgCureSeal);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        MESSAGE_PUSH(MsgCureCurse);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        MESSAGE_PUSH(MsgCureSpirit);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->poison != 0) {
        MESSAGE_PUSH(MsgCurePoison);
        target->poison = 0;
      }
      BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    case 4:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgCureCurse);
      break;

    case 61:
    case 62:
    case 70:
    case 71:
    case 76: {
      s32 old;
      s32 maxu;
      s32 maxv;
      s32 heal;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from hp to maxHp reads each
       * stat twice, unsigned and signed; as reads of target->hp and
       * target->maxHp, 94 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->hp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 76) {
        s32 m;

        pt--;
        maxu = *pt--;
        m = *(s16 *)(pt + 1);
        heal += m * 2 / 5;
      } else if (action->effect == 71) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 10;
      } else if (action->effect == 70) {
        maxu = (u16)target->maxHp;
        heal += (s16)maxu / 2;
        /* FAKEMATCH(barrier): an empty asm on maxu, so the clamp shifts maxu
         * left by 16 again instead of sharing this halving's shift; without
         * it, 45 of 3451 instructions differ in tla-en. */
        asm("" : "+r"(maxu));
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        if (action->effect == 61) {
          s32 p = maxv * 60;
          heal += p / 100;
        } else {
          s32 p = maxv * 30;
          heal += p / 100;
        }
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 1)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgHpRecover);
      }
      target->hp = heal;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 63:
    case 77:
    case 78: {
      s32 old;
      s32 maxv;
      s32 heal;
      s32 maxu;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from pp to maxPp reads each
       * stat twice, unsigned and signed; as reads of target->pp and
       * target->maxPp, 255 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->pp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 77) {
        maxv = target->maxPp;
        maxu = (u16)target->maxPp;
        heal += (s16)maxv / 10;
      } else if (action->effect == 78) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 3 / 10;
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 100;
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 11)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = heal;
      Unit_RecalculateGaugesFar(tid);
      break;
    }

    case 59:
      target->agilityStage = 8;
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->agi - bak->agi);
      MESSAGE_PUSH(MsgAgiUp);
      break;

    case 58:
      am = &target->agilityStage;
      {
        u8 v;

        v = -4;
        *am = v;
      }
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, bak->agi - target->agi);
      MESSAGE_PUSH(MsgAgiDown);
      break;

    case 9:
      target->attackStage -= 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 8:
      target->attackStage -= 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 7:
      target->attackStage += 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 6:
      target->attackStage += 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 13:
      target->defenseStage -= 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 12:
      target->defenseStage -= 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 11:
      target->defenseStage += 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 10:
      target->defenseStage += 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 5:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp;
      Unit_RecalculateGaugesFar(tid);
      break;

    case 56:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp / 2;
      Unit_RecalculateGaugesFar(tid);
      break;

    case 57:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      Unit_RecalculateGaugesFar(tid);
      break;

    case 73:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = (s16)(target->maxHp * 6 / 10);
      Unit_RecalculateGaugesFar(tid);
      break;

    case 3:
      if (target->poison != 0)
        MESSAGE_PUSH(MsgCurePoison);
      target->poison = 0;
      break;

    case 17:
      RES_STEP(-1, (bak->resistanceStage - target->resistanceStage) * 20,
               MsgResDown);
      break;

    case 16: {
      target->resistanceStage -= 2;
      if (-4 > target->resistanceStage)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattlePlayback_Push(BATTLE_EVENT_NUMBER,
                          (bak->resistanceStage - target->resistanceStage) *
                              20);
      MESSAGE_PUSH(MsgResDown);
      target->resistanceTurns = 7;
    } break;

    case 15:
      RES_STEP(1, (target->resistanceStage - bak->resistanceStage) * 20,
               MsgResUp);
      break;

    case 14: {
      target->resistanceStage += 2;
      if (*(rm = &target->resistanceStage) < -4)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattlePlayback_Push(BATTLE_EVENT_NUMBER,
                          (target->resistanceStage - bak->resistanceStage) *
                              20);
      MESSAGE_PUSH(MsgResUp);
      target->resistanceTurns = 7;
    } break;

    case 18:
      if (target->poison != 0)
        break;
      MESSAGE_PUSH(MsgPoisoned);
      target->poison = 1;
      break;

    case 19:
      if (target->poison > 1)
        break;
      MESSAGE_PUSH(MsgVenom);
      target->poison = 2;
      break;

    case 20:
    case 75:
      STAT_SET(target->delusion, MsgDelusion);
      break;

    case 21:
      STAT_SET(target->confusion, MsgConfused);
      break;

    case 22:
      STAT_SET(target->charm, MsgCharmed);
      break;

    case 23:
    case 85:
      STAT_SET(target->stun, MsgStunned);
      break;

    case 24:
      STAT_SET(target->sleep, MsgAsleep);
      break;

    case 25:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 7;
      break;

    case 67:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 16;
      break;

    case 27:
      if (BattleUnit_KeepsOneHp(tid) != 0)
        break;
      BattlePlayback_Push(BATTLE_EVENT_DEFEAT, tid);
      if (target->side == ALLY)
        MESSAGE_PUSH(MsgKoDown);
      else if (abilityId == 219)
        MESSAGE_PUSH(MsgSuffocate);
      else
        MESSAGE_PUSH(MsgSpiritDrain);
      target->hp = 0;
      Unit_RecalculateGaugesFar(tid);
      break;

    case 29:
      MESSAGE_PUSH(MsgHpRegen);
      target->refrain = 5;
      break;

    case 30:
      STAT_SET(target->reflect, MsgReflect);
      break;

    case 31:
    case 60:
      ap = dealt;
      dmg = actor->hp;
      /* FAKEMATCH(barrier): an empty asm on dmg, so its copy comes before
       * dealt is reloaded from the stack; without it, 2 of 3451 instructions
       * differ in tla-en. */
      __asm__("" : "=r"(dmg) : "0"(dmg));
      if (action->effect == 60)
        ap /= 2;
      kind = ap;
      dmg += kind;
      if (dmg > actor->maxHp) {
        dmg = actor->maxHp;
        kind = dmg - actor->hp;
      }
      BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (dmg == actor->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, kind);
        MESSAGE_PUSH(MsgHpRecover);
      }
      actor->hp = dmg;
      Unit_RecalculateGaugesFar(aid);
      break;

    case 32: {
      s32 heal;
      s32 amt;

      heal = actor->pp;
      amt = dealt;
      heal += amt;
      if (heal > actor->maxPp) {
        heal = actor->maxPp;
        amt = heal - actor->pp;
      }
      BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (heal == actor->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattlePlayback_Push(BATTLE_EVENT_NUMBER, amt);
        MESSAGE_PUSH(MsgPpRecover);
      }
      actor->pp = heal;
      Unit_RecalculateGaugesFar(aid);
      break;
    }

    case 69:
      dmg = dealt / 10;
      if (target->pp < dmg)
        dmg = target->pp;
      if (actor->pp + dmg > actor->maxPp)
        dmg = actor->maxPp - actor->pp;
      if (dmg == 0)
        break;
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(aid, dmg);
      break;

    case 84:
      dmg = (s16)(target->maxPp / 10);
      if (target->pp < dmg)
        dmg = target->pp;
      if (dmg == 0)
        break;
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgMorePpLossP, MsgMorePpLossE);
      Unit_AddPpFar(tid, -dmg);
      break;

    case 33:
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
      target->elementLevels[0] = 0;
      target->elementLevels[1] = 0;
      target->elementLevels[2] = 0;
      target->elementLevels[3] = 0;
      MESSAGE_PUSH(MsgBuffsReset);
      break;

    case 26:
      MESSAGE_PUSH(MsgEvilSpirit);
      target->haunt = 1;
      break;

    case 28:
    case 80:
      if (target->grimReaper == 0) {
        MESSAGE_PUSH(MsgDeathCurse);
        target->grimReaper = 7;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattlePlayback_Push(BATTLE_EVENT_NUMBER, target->grimReaper);
      MESSAGE_PUSH(MsgDeathCount);
      break;

    case 66:
      MESSAGE_PUSH(MsgReadies);
      target->ready = 2;
      break;

    case 54:
      MESSAGE_PUSH(MsgChallenge);
      target->challenge = 1;
      if (ALLY_CHECK(tid))
        work->challengeFlag |= 2;
      break;

    case 53:
    case 83:

      MESSAGE_PUSH(MsgImmobile);
      target->immobile = 1;
      break;

    case 46:

      MESSAGE_PUSH(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = 1;
      break;

    case 72:
      MESSAGE_PUSH(MsgAura);
      if ((u32)target->guardLevel > 1)
        break;
      target->guardLevel = 2;
      break;

    case 47:
    case 88:
      MESSAGE_PUSH(MsgAura2);
      if (target->guardLevel > 2)
        break;
      target->guardLevel = 3;
      break;

    case 79:
      MESSAGE_PUSH(MsgVanished);
      if ((u32)target->guardLevel > 3)
        break;
      target->guardLevel = 4;
      break;

    case 45:
      BattlePlayback_Push(BATTLE_EVENT_MESSAGE, -1);
      break;

    case 81:
      MESSAGE_PUSH(MsgAllResting);
      work->unk046[1] = 1;
      break;

    case 82:
      MESSAGE_PUSH(MsgBlownAway);
      BattlePlayback_Push(15, tid);
      break;

    case 74:
      MESSAGE_PUSH(MsgCounterReady);
      target->equipBonus[1] = 1;
      break;

    default:
      break;
    }
  }

done:
  BattlePlayback_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
  if (plan->command != 9 && (s8)target->equipBonus[1] != 0 &&
      action->targetMode == 1 && target->hp != 0) {
    work->unk858 = tid;
    work->unk85c = aid;
  }

  if (target->hp != 0) {
    if (target->sleep != 0)
      if (target->sleep <= 6 && dealt > 0 && (3 & Battle_RandomFar()) == 0) {
        target->sleep = 0;
        BattlePlayback_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
  }
  Heap_Release(bak);
  Unit_CalculateFar(tid);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  if (target->hp != 0)
    BattlePlayback_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
  if (abilityId != 403 && actor->haunt != 0 && (Battle_RandomFar() & 3) == 0 &&
      dealt > 0) {
    s32 share;

    share = dealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}

// Battle: test whether an ability uses PP.

/// Returns 1 for abilities that cost PP, and for Psy Drain whatever its cost.
/// abilityId: ability ID.
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == 126) // Psy Drain.
    return 1;
  pp = Ability_Get(abilityId)->ppCost;
  return (u32)((0 - pp) | pp) >> 31; // 1 if pp is nonzero.
}

#define AI_BATTLE ((struct AIBattleWork *)Ram_HeapSlots->battleWork)

/// Chooses an enemy's action, as The Broken Seal's BattleAI_ChooseAction does:
/// one of the eight in its definition, picked by its action strategy, with an
/// item instead where its item mask says so, and a target worth the ability.
/// Called with retry 0 for each enemy as the turn is built and with retry 1
/// when it acts (BattleStaging_DispatchAction).
s32 BattleAI_ChooseAction(struct BattleActionRecord *request, s32 retry) {
  struct BattleUnit *unit;
  u32 selection;
  struct AIState *selectionMemory;
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
  struct AIBattleWork *battle;
  struct ItemDefinition *item;
  s32 useItem;
  s8 strategyKind;
  struct BattleAct *ability;
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

  unit = Unit_Get__f11f66c(request->unitId);
  itemsAllowed = 1;
  failedChoices = 0;
  needsChoice = 1;
  selection = -1U;
  if (unit->classIndex != 0)
    return -1;

  selectionMemory = (struct AIState *)&unit->rewards;
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (unit->enemyId == 101) {
    selectionMemory =
        (struct AIState *)&((struct AIBattleWork *)Ram_HeapSlots->battleWork)
            ->aiStates[request->unk0e];
    if (request->unk0e == 1)
      enemy = Enemy_GetDefinitionFar(102);
  } else if (unit->enemyId == 369) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(371);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(370);
      break;
    case 3:
      AI_BATTLE->unk869 = 7;
      enemy = Enemy_GetDefinitionFar(372);
      break;
    }
  } else if (unit->enemyId == 373) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(375);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(374);
      break;
    }
  } else if (unit->enemyId == 376) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    if (request->unk0e == 1) {
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(377);
    }
  } else if (unit->enemyId == 218) {
    if (selectionMemory->initialized == 0)
      selectionMemory->form = (Random16() * 3) >> 16;
    enemy = Enemy_GetDefinitionFar(Data_08128960[selectionMemory->form]);
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
    if (!(0x1ff & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == 0) {
        request->command = COMMAND_ITEM;
        {
          s32 cantUse = 509;

          request->parameter = cantUse;
        }
        goto done;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      if (item->useType != 1) {
        useItem = 0;
      } else {
        Ability_Get(item->actionId);
        abilityId = item->actionId;
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
      request->command = COMMAND_DEFEND2;
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = COMMAND_FLEE;
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    case 74:
    case 88:
      if (retry == 0)
        request->priority += 10000;
      break;
    }

    if (retry == 0) {
      switch (request->command) {
      case COMMAND_DEFEND:
      case COMMAND_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    if (request->command != COMMAND_ITEM &&
        Ability_IsPsynergy(abilityId) != 0) {
      request->command = COMMAND_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp && enemy->targetStrategy != 0)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == 2)
        goto next_candidate;
    }
    command = request->command;
    if (command == COMMAND_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 && (command == COMMAND_DEFEND || command == COMMAND_DEFEND2))
      goto next_candidate;

    if (command == COMMAND_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == 1)
        request->command = COMMAND_ATTACK;
      if (abilityId == 729) {
        request->command = COMMAND_SUMMON;
        BattleDjinn_CountStandby(0, counts);
        summonIndex = Summon_ListLearnedFar(summons);
        for (summonIndex--; summonIndex >= 0; summonIndex--) {
          summon = SummonDefinition_Get(summons[summonIndex]);
          for (element = 0; element <= 3 &&
                            summon->djinnRequired[element] <= counts[element];
               element++)
            ;
          if (element == 4)
            break;
        }
        if (summonIndex < 0)
          goto next_candidate;
        request->parameter = summons[summonIndex];
        abilityId = SummonDefinition_Get(summons[summonIndex])->nameMessageId;
        ability = Ability_Get(abilityId);
      }
    }

    request->range = ability->range;
    switch (ability->targetMode) {
    case 2:
    case 4:
      if (abilityId == 723 || abilityId == 731)
        allyTarget = BattleAI_ChooseTarget(128, ability);
      else
        allyTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (allyTarget == -2) {
        allyIsParty = 0;
        if ((u16)request->unitId <= 7)
          allyIsParty = 1;
        allyTarget = BattleAI_ChooseRandomTarget(allyIsParty);
      }
      if (allyTarget != -1) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case 1:
      opponentTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (opponentTarget == -2) {
        opponentIsParty = 0;
        if ((u16)request->unitId <= 7)
          opponentIsParty = 1;
        opponentTarget = BattleAI_ChooseRandomTarget(opponentIsParty);
      }
      if (opponentTarget != -1) {
        request->target = opponentTarget;
        needsChoice = 0;
      }
      break;
    case 3:
      request->target = Battle_FindUnitPlace(request->unitId);
      break;
    default:
      if (ability->effect == 86 &&
          !Enemy_HasSpriteRoom(unit->enemyId == 164 ? 378 : 81))
        break;
      battle = (struct AIBattleWork *)Ram_HeapSlots->battleWork;
      if (ability->effect == 87) {
        if (battle->queueCount == 0)
          break;
        if (!Enemy_HasSpriteRoom(battle->queuedSummons[battle->queueHead]))
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
  if (needsChoice != 0 && failedChoices > 16) {
    request->command = COMMAND_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  if (unit->enemyId == 218 && retry == 1 && selectionMemory->selection == 0)
    selectionMemory->form = (selectionMemory->form + 1) % 3;
  return 0;
}

void Dma_StopAllThenRunHook(void) {
  Math_UnsignedMulHigh(5, 0);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
}
