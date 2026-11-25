/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
// Part of menu.c.

#include "menu_internal.h"
#include "sounds.h"

/// Uses the item in memberId's inventory slot on targetId, or on the whole
/// party when its ability's range is ABILITY_RANGE_ALL. An item heals by its
/// ability's plain power, not by its user's elemental power as Psynergy does.
/// A consumed item leaves the bag and Hermes' Water leaves its Empty Bottle;
/// returns -1 when it did nothing, keeping the item.
s32 Item_Use(s32 slot, s32 memberId, s32 targetId) {
  struct ItemMenuState *menu;
  s32 useResult;
  s32 itemId;
  struct Unit *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(memberId);
  menu = Ram_HeapSlots->menuState;
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

/// Applies the healing, curing, revival or lasting stat rise of ability
/// abilityId, used by party member userId, to targetId, or to the whole party
/// when its range is ABILITY_RANGE_ALL. HP healing is scaled by the user's
/// power in the ability's element unless plainPower is set, as it is for an
/// item. Leaves the result's message in the menu's state; returns 0, or -1 when
/// nothing changed.
s32 Ability_ApplyToTargets(s32 abilityId, s32 userId, s32 targetId,
                           s32 plainPower) {
  struct AbilityDefinition *ability;
  struct ItemMenuState *menu;
  struct Unit *target;
  struct Unit *caster;
  s32 partyMessageChosen;
  s32 changed;
  u8 targetIndex;
  s16 elementPower;
  s32 randomAdjust;
  s32 amount;
  s32 result;

  menu = Ram_HeapSlots->menuState;
  ability = Ability_Get(abilityId);
  changed = 0;
  result = 0;
  partyMessageChosen = 0;

  if (targetId == MENU_TARGET_WHOLE_PARTY)
    target = Unit_Get(gMainRoutineState.leaderActorId);
  else
    target = Unit_Get(targetId);

  /* FAKEMATCH(dead): the dead test loads ability->range ahead of the count
   * guard. */
  if (ability->range == ABILITY_RANGE_ALL && partyMessageChosen != 0)
    result = ITEM_USE_RESULT_PARTY_HP_RECOVERED;

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (ability->range == ABILITY_RANGE_ALL) {
      targetId = (u16)menu->memberIds[targetIndex];
      target = Unit_Get(targetId);
    }

    amount = ability->power;
    switch (ability->flags & ABILITY_FORMULA_MASK) {
    case ABILITY_FORMULA_HEAL_HP:
      if (plainPower == 0) {
        if (ability->element != ELEMENT_NONE) {
          s32 elementIndex;

          caster = Unit_Get(userId);
          elementIndex = ability->element;
          elementPower = caster->element[elementIndex].power;
        } else
          elementPower = HEALING_BASE_POWER;
        amount = Battle_CalculateRecoveryFar(amount, elementPower,
                                             BATTLE_MULTIPLIER_ONE);
      }

      if (target->hp <= 0) {
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        if (target->hp == target->hpMax) {
          if (partyMessageChosen == 0)
            result = ITEM_USE_RESULT_HP_FULL;
        } else {
          target->hp += amount;
          if (target->hp > target->hpMax) {
            amount -= target->hp - target->hpMax;
            target->hp = target->hpMax;
            if (partyMessageChosen == 0)
              result = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
          } else if (partyMessageChosen == 0) {
            result = ITEM_USE_RESULT_HP_RECOVERED;
          }
          Unit_RecalculateGaugesFar(targetId);
          changed = 1;
          if (ability->range == ABILITY_RANGE_ALL) {
            partyMessageChosen = 1;
            result = ITEM_USE_RESULT_PARTY_HP_RECOVERED;
          }
        }
      }
      break;

    // The lasting rises: the item's power, give or take one, except Lucky
    // Pepper's. One time in four it is one more, one time in four one less.
    case ABILITY_FORMULA_NO_POWER:
      randomAdjust = Random_Below(4);
      if (randomAdjust == 0) {
        randomAdjust = -1;
      } else {
        if (randomAdjust == 1)
          randomAdjust = 1;
        else
          randomAdjust = 0;
      }

      switch (abilityId & PSYNERGY_ABILITY_MASK) {
      case ABILITY_POWER_BREAD:
        target->baseHp += amount + randomAdjust;
        result = ITEM_USE_RESULT_MAX_HP_UP;
        changed = 1;
        break;
      case ABILITY_COOKIE:
        target->basePp += amount + randomAdjust;
        result = ITEM_USE_RESULT_MAX_PP_UP;
        changed = 1;
        break;
      case ABILITY_MINT:
        target->baseAgility += amount + randomAdjust;
        result = ITEM_USE_RESULT_AGILITY_UP;
        changed = 1;
        break;
      case ABILITY_LUCKY_PEPPER:
        target->baseLuck += amount;
        result = ITEM_USE_RESULT_LUCK_UP;
        changed = 1;
        break;
      case ABILITY_APPLE:
        target->baseAttack += amount + randomAdjust;
        // "Your Attack increased!" shows no number: neither this 3 nor the
        // Apple's power is seen.
        MessageArgument_PushFar(3, MESSAGE_ARGUMENT_NUMBER);
        result = ITEM_USE_RESULT_ATTACK_UP;
        changed = 1;
        break;
      case ABILITY_HARD_NUT:
        target->baseDefense += amount + randomAdjust;
        // Nor does "Your Defense increased!" show this 4.
        MessageArgument_PushFar(4, MESSAGE_ARGUMENT_NUMBER);
        result = ITEM_USE_RESULT_DEFENSE_UP;
        changed = 1;
        break;
      }
      break;

    case ABILITY_FORMULA_HEAL_PP:
      if (target->pp == target->ppMax) {
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          amount -= target->pp - target->ppMax;
          target->pp = target->ppMax;
          if (partyMessageChosen == 0)
            result = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyMessageChosen == 0) {
          result = ITEM_USE_RESULT_PP_RECOVERED;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
        if (ability->range == ABILITY_RANGE_ALL) {
          partyMessageChosen = 1;
          result = ITEM_USE_RESULT_PARTY_PP_RECOVERED;
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

    switch (ability->effect) {
    case ABILITY_EFFECT_RESTORE_HP:
      if (target->hp <= 0 || target->hp == target->hpMax) {
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        target->hp += amount;
        if (target->hp > target->hpMax) {
          target->hp = target->hpMax;
          if (partyMessageChosen == 0)
            result = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
        } else if (partyMessageChosen == 0) {
          result = ITEM_USE_RESULT_HP_RECOVERED;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
      }
      break;

    case ABILITY_EFFECT_RESTORE_PP:
      if (target->pp == target->ppMax) {
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          target->pp = target->ppMax;
          if (partyMessageChosen == 0)
            result = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyMessageChosen == 0) {
          result = ITEM_USE_RESULT_PP_RECOVERED;
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
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_REVIVED;
      } else if (partyMessageChosen == 0) {
        result = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    // Quartz's and Dew's revivals, which only battle uses: the menu never
    // reaches these two.
    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->hpMax / 2;
        Unit_RecalculateGaugesFar(targetId);
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_REVIVED;
      } else if (partyMessageChosen == 0) {
        result = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_DEW:
      if (target->hp == 0) {
        target->hp = target->hpMax * 7 / 10;
        Unit_RecalculateGaugesFar(targetId);
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_REVIVED;
      } else if (partyMessageChosen == 0) {
        result = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0) {
        target->poison = 0;
        changed = 1;
        if (partyMessageChosen == 0)
          result = ITEM_USE_RESULT_POISON_PURGED;
      } else if (partyMessageChosen == 0) {
        result = ITEM_USE_RESULT_NOT_POISONED;
      }
      break;
    }

    if (ability->range != ABILITY_RANGE_ALL)
      break;
  }

  if (changed == 0) {
    menu->messageOffset = result;
    return -1;
  }

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++)
    Unit_CalculateFar((u16)menu->memberIds[targetIndex]);
  menu->messageOffset = result;
  return 0;
}

/// Plays the sound of using item from the menu (Ability_PlayUseSound),
/// after a use that changed something.
void Item_PlayUseSound(s32 item) {
  Ability_PlayUseSound(Item_GetDirectFar(item)->abilityId &
                       PSYNERGY_ABILITY_MASK);
}

/// Plays the sound of a Psynergy or item used from the menu; unlike in battle,
/// no animation is drawn and nothing waits. Healing HP or PP plays
/// SE_RECOVERY, revival MUS_RECOVERING_AFTER_COMBAT, curing poison
/// MUS_POISON_CURED, and any other effect MUS_LUCKY.
void Ability_PlayUseSound(s32 abilityId) {
  s32 formula;
  u32 effect;
  struct AbilityDefinition *ability;

  ability = Ability_Get(abilityId);
  formula = ABILITY_FORMULA_MASK & ability->flags;
  switch (formula) {
  case ABILITY_FORMULA_HEAL_HP:
    // Started twice on the same music player, the second start restarts the
    // first in the same frame, so it is heard once.
    Sound_PlayReturnOne(SE_RECOVERY);
    // Fall through.
  case ABILITY_FORMULA_HEAL_PP:
    Sound_PlayReturnOne(SE_RECOVERY);
    return;
  default:
    effect = ability->effect;
    switch (effect) {
    case ABILITY_EFFECT_REVIVE:
      Sound_PlayReturnOne(MUS_RECOVERING_AFTER_COMBAT);
      return;
    case ABILITY_EFFECT_CURE_POISON:
      Sound_PlayReturnOne(MUS_POISON_CURED);
      return;
    default:
      Sound_PlayReturnOne(MUS_LUCKY);
      // Fall through.

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
