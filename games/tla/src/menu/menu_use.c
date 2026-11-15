/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
// Part of menu.c.

#include "menu_internal.h"
#include "sounds.h"

/// Uses the item in memberId's inventory slot on targetId, or on every party
/// member when its action's range is ABILITY_RANGE_ALL: applies its action at
/// its plain power (Ability_ApplyToTargets with fixedScale 1).
s32 Item_Use(s32 slot, s32 memberId, s32 targetId) {
  struct ItemMenuWork *menu;
  s32 useResult;
  s32 itemId;
  struct Unit *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(memberId);
  menu = Ram_HeapSlots->menuWork;
  itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
  item = Item_GetDirectFar(itemId);
  useResult = Ability_ApplyToTargets(PSYNERGY_ABILITY_MASK & item->abilityId,
                                     memberId, targetId, 1);
  if (useResult != -1) {
    item = Item_GetDirectFar(unit->inventory[slot]);
    if (item->useType == ITEM_USE_CONSUMED) {
      Inventory_RemoveFar(memberId, slot);
      menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
    }
    if (item->useType == ITEM_USE_EMPTIES) {
      if (itemId == ITEM_HERMES_WATER)
        itemId = ITEM_EMPTY_BOTTLE;
      unit->inventory[slot] = itemId;
    }
    useResult = 0;
  }
  return useResult;
}

s32 Battle_CalculateRecoveryFar(s32 value, s32 selector, s32 multiplier);

/// Applies the healing, curing, revival or lasting stat rise of action
/// effectId, used by party member sourceId, to targetId, or to every party
/// member when the action's range is ABILITY_RANGE_ALL.
s32 Ability_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                           s32 fixedScale) {
  struct BattleAct *effect;
  struct ItemMenuWork *menu;
  struct Unit *target;
  struct Unit *caster;
  s32 partyResultShown;
  s32 changed;
  u8 targetIndex;
  s16 elementPower;
  s32 randomAdjust;
  s32 randomDifference;
  s32 amount;
  s32 messageOffset;

  menu = Ram_HeapSlots->menuWork;
  effect = Ability_Get(effectId);
  changed = 0;
  messageOffset = 0;
  partyResultShown = 0;

  if (targetId == 9)
    target = Unit_Get(gMainRoutineWork.leaderActorId);
  else
    target = Unit_Get(targetId);

  /* FAKEMATCH(dead): the dead test loads effect->range ahead of the count
   * guard. */
  if (effect->range == ABILITY_RANGE_ALL && partyResultShown != 0)
    messageOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == ABILITY_RANGE_ALL) {
      targetId = (u16)menu->memberIds[targetIndex];
      target = Unit_Get(targetId);
    }

    amount = effect->power;
    switch (effect->flags & ABILITY_FORMULA_MASK) {
    case BATTLE_DAMAGE_HP_HEAL:
      if (fixedScale == 0) {
        if (effect->element != ELEMENT_NONE) {
          s32 elementIndex;

          caster = Unit_Get(sourceId);
          elementIndex = effect->element;
          elementPower = caster->element[elementIndex].power;
        } else
          elementPower = 100;
        amount = Battle_CalculateRecoveryFar(amount, elementPower, 0x100);
      }

      if (target->hp <= 0) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        if (target->hp == target->hpMax) {
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_HP_FULL;
        } else {
          target->hp += amount;
          if (target->hp > target->hpMax) {
            amount -= target->hp - target->hpMax;
            target->hp = target->hpMax;
            if (partyResultShown == 0)
              messageOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
          } else if (partyResultShown == 0) {
            messageOffset = ITEM_USE_RESULT_HP_RECOVERED;
          }
          Unit_RecalculateGaugesFar(targetId);
          changed = 1;
          if (effect->range == ABILITY_RANGE_ALL) {
            partyResultShown = 1;
            messageOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;
          }
        }
      }
      break;

    case ABILITY_FORMULA_NO_POWER:
      randomAdjust = Random_Below(4);
      if (randomAdjust == 0) {
        randomAdjust = -1;
      } else {
        randomDifference = 1 ^ randomAdjust;
        randomAdjust = (u32)((-randomDifference) | randomDifference) >> 31;
        randomAdjust = 1 - randomAdjust;
      }

      switch (effectId & PSYNERGY_ABILITY_MASK) {
      case ABILITY_POWER_BREAD:
        target->baseHp += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_MAX_HP_UP;
        changed = 1;
        break;
      case ABILITY_COOKIE:
        target->basePp += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_MAX_PP_UP;
        changed = 1;
        break;
      case ABILITY_MINT:
        target->baseAgility += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_AGILITY_UP;
        changed = 1;
        break;
      case ABILITY_LUCKY_PEPPER:
        target->baseLuck += amount;
        messageOffset = ITEM_USE_RESULT_LUCK_UP;
        changed = 1;
        break;
      case ABILITY_APPLE:
        target->baseAttack += amount + randomAdjust;
        MessageArgument_PushFar(3, MESSAGE_ARGUMENT_NUMBER);
        messageOffset = ITEM_USE_RESULT_ATTACK_UP;
        changed = 1;
        break;
      case ABILITY_HARD_NUT:
        target->baseDefense += amount + randomAdjust;
        MessageArgument_PushFar(4, MESSAGE_ARGUMENT_NUMBER);
        messageOffset = ITEM_USE_RESULT_DEFENSE_UP;
        changed = 1;
        break;
      }
      break;

    case BATTLE_DAMAGE_PP_HEAL:
      if (target->pp == target->ppMax) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          amount -= target->pp - target->ppMax;
          target->pp = target->ppMax;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_PP_RECOVERED;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
        if (effect->range == ABILITY_RANGE_ALL) {
          partyResultShown = 1;
          messageOffset = ITEM_USE_RESULT_PARTY_PP_RECOVERED;
        }
      }
      break;

    case ABILITY_FORMULA_EFFECT:
    case ABILITY_FORMULA_ATTACK:
    case ABILITY_FORMULA_ATTACK_MULTIPLIED:
    case ABILITY_FORMULA_DAMAGE:
    case ABILITY_FORMULA_ENEMY_DAMAGE:
    case ABILITY_FORMULA_EFFECT_ONLY:
    case ABILITY_FORMULA_SUMMON:
    case ABILITY_FORMULA_PP_DAMAGE:
      break;
    }

    switch (effect->effect) {
    case ABILITY_EFFECT_RESTORE_HP:
      if (target->hp <= 0 || target->hp == target->hpMax) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        target->hp += amount;
        if (target->hp > target->hpMax) {
          target->hp = target->hpMax;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_HP_RECOVERED;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
      }
      break;

    case ABILITY_EFFECT_RESTORE_PP:
      if (target->pp == target->ppMax) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          target->pp = target->ppMax;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_PP_RECOVERED;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
      }
      break;

    case ABILITY_EFFECT_REVIVE:
      if (target->hp == 0) {
        target->hp = target->hpMax;
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_REVIVED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->hpMax / 2;
        Unit_RecalculateGaugesFar(targetId);
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_REVIVED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_DEW:
      if (target->hp == 0) {
        target->hp = target->hpMax * 7 / 10;
        Unit_RecalculateGaugesFar(targetId);
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_REVIVED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0) {
        target->poison = 0;
        changed = 1;
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_POISON_PURGED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_POISONED;
      }
      break;
    }

    if (effect->range != ABILITY_RANGE_ALL)
      break;
  }

  if (changed == 0) {
    menu->messageOffset = messageOffset;
    return -1;
  }

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++)
    Unit_CalculateFar((u16)menu->memberIds[targetIndex]);
  menu->messageOffset = messageOffset;
  return 0;
}

/// Plays the use sound of item's action (Ability_PlayUseAnimation). Called by
/// ItemMenu_Use after a use that changed something.
s32 Item_PlayUseAnimation(s32 item) {
  return Ability_PlayUseAnimation(Item_GetDirectFar(item)->abilityId & 0x3fff);
}

/// Plays the sound of action used from the menu, and returns at once: no
/// animation is drawn and nothing waits. Formulas 1 and 11 play sound 126,
/// formula 1 twice; otherwise effects 1, 2, 31 and 32 play nothing, effect 5
/// sound 82, effect 3 sound 84, and every other effect sound 91.
s32 Ability_PlayUseAnimation(s32 action) {
  s32 animationType;
  u32 targetType;
  struct AbilityDefinition *actionDefinition;

  actionDefinition = Ability_Get(action);
  animationType = ABILITY_FORMULA_MASK & actionDefinition->flags;
  switch (animationType) {
  case ABILITY_FORMULA_HEAL_HP:
    // With no break the recovery sound is started twice, both times on music
    // player 2 (gSongTable's row for it), which Sound_Play never swaps for
    // another: the second start restarts the first before the sound driver
    // mixes a frame, so it is heard once.
    Sound_PlayReturnOne(SE_RECOVERY);
    // Fall through.
  case ABILITY_FORMULA_HEAL_PP:
    Sound_PlayReturnOne(SE_RECOVERY);
    return;
  default:
    targetType = actionDefinition->effect;
    switch (targetType) {
    case ABILITY_EFFECT_REVIVE:
      Sound_PlayReturnOne(MUS_REVIVE);
      return;
    case ABILITY_EFFECT_CURE_POISON:
      Sound_PlayReturnOne(MUS_CURE);
      return;
    default:
      Sound_PlayReturnOne(MUS_JINGLE_SMALL);

    // The HP and PP restores no ability has (ability_table.c).
    case ABILITY_EFFECT_RESTORE_HP:
    case ABILITY_EFFECT_RESTORE_PP:
    case ABILITY_EFFECT_DRAIN_HP:
    case ABILITY_EFFECT_DRAIN_PP:
      return;
    }
    break;
  }
}
