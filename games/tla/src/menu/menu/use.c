/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
// Part of menu.c.

#include "../menu_internal.h"
#include "sounds.h"

/// Uses the item in memberId's inventory slot on targetId, or on every party
/// member when its action's range is ABILITY_RANGE_ALL: applies its action at
/// its plain power (Ability_ApplyToTargets with fixedScale 1).
s32 Item_Use(s32 slot, s32 memberId, s32 targetId) {
  struct ItemMenuWork *menu;
  s32 useResult;
  s32 itemId;
  struct OwnerInventoryState *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(memberId);
  menu = Ram_HeapSlots->menuWork;
  itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
  item = Item_GetDirectFar(itemId);
  useResult = Ability_ApplyToTargets(PSYNERGY_ABILITY_MASK & item->abilityId,
                                     memberId, targetId, 1);
  if (useResult != -1) {
    item = Item_GetDirectFar(unit->inventory[slot]);
    if (item->useType == 1) {
      Inventory_RemoveFar(memberId, slot);
      menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
    }
    if (item->useType == 4) {
      if (itemId == 184)
        itemId = 185;
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
  if (effect->range == 0xff && partyResultShown != 0)
    messageOffset = 3;

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == 0xff) {
      targetId = (u16)menu->memberIds[targetIndex];
      target = Unit_Get(targetId);
    }

    amount = effect->power;
    switch (effect->flags & 0xf) {
    case BATTLE_DAMAGE_HP_HEAL:
      if (fixedScale == 0) {
        if (effect->element != 4) {
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
          messageOffset = 2;
      } else {
        if (target->hp == target->hpMax) {
          if (partyResultShown == 0)
            messageOffset = 4;
        } else {
          target->hp += amount;
          if (target->hp > target->hpMax) {
            amount -= target->hp - target->hpMax;
            target->hp = target->hpMax;
            if (partyResultShown == 0)
              messageOffset = 0;
          } else if (partyResultShown == 0) {
            messageOffset = 1;
          }
          Unit_RecalculateGaugesFar(targetId);
          changed = 1;
          if (effect->range == 0xff) {
            partyResultShown = 1;
            messageOffset = 3;
          }
        }
      }
      break;

    case 9:
      randomAdjust = (Random16() * 4) >> 16;
      if (randomAdjust == 0) {
        randomAdjust = -1;
      } else {
        randomDifference = 1 ^ randomAdjust;
        randomAdjust = (u32)((-randomDifference) | randomDifference) >> 31;
        randomAdjust = 1 - randomAdjust;
      }

      switch (effectId & PSYNERGY_ABILITY_MASK) {
      case 0x104:
        target->baseHp += amount + randomAdjust;
        messageOffset = 0x10;
        changed = 1;
        break;
      case 0x105:
        target->basePp += amount + randomAdjust;
        messageOffset = 0x11;
        changed = 1;
        break;
      case 0x108:
        target->baseAgility += amount + randomAdjust;
        messageOffset = 0x12;
        changed = 1;
        break;
      case 0x109:
        target->baseLuck += amount;
        messageOffset = 0x13;
        changed = 1;
        break;
      case 0x106:
        target->baseAttack += amount + randomAdjust;
        MessageArgument_PushFar(3, MESSAGE_ARGUMENT_NUMBER);
        messageOffset = 0x14;
        changed = 1;
        break;
      case 0x107:
        target->baseDefense += amount + randomAdjust;
        MessageArgument_PushFar(4, MESSAGE_ARGUMENT_NUMBER);
        messageOffset = 0x15;
        changed = 1;
        break;
      }
      break;

    case BATTLE_DAMAGE_PP_HEAL:
      if (target->pp == target->ppMax) {
        if (partyResultShown == 0)
          messageOffset = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          amount -= target->pp - target->ppMax;
          target->pp = target->ppMax;
          if (partyResultShown == 0)
            messageOffset = 5;
        } else if (partyResultShown == 0) {
          messageOffset = 6;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
        if (effect->range == 0xff) {
          partyResultShown = 1;
          messageOffset = 8;
        }
      }
      break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
      break;
    }

    switch (effect->effect) {
    case 1:
      if (target->hp <= 0 || target->hp == target->hpMax) {
        if (partyResultShown == 0)
          messageOffset = 2;
      } else {
        target->hp += amount;
        if (target->hp > target->hpMax) {
          target->hp = target->hpMax;
          if (partyResultShown == 0)
            messageOffset = 0;
        } else if (partyResultShown == 0) {
          messageOffset = 1;
        }
        Unit_RecalculateGaugesFar(targetId);
        changed = 1;
      }
      break;

    case 2:
      if (target->pp == target->ppMax) {
        if (partyResultShown == 0)
          messageOffset = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->ppMax) {
          target->pp = target->ppMax;
          if (partyResultShown == 0)
            messageOffset = 5;
        } else if (partyResultShown == 0) {
          messageOffset = 6;
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
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->hpMax / 2;
        Unit_RecalculateGaugesFar(targetId);
        if (partyResultShown == 0)
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp == 0) {
        target->hp = target->hpMax * 7 / 10;
        Unit_RecalculateGaugesFar(targetId);
        if (partyResultShown == 0)
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0) {
        target->poison = 0;
        changed = 1;
        if (partyResultShown == 0)
          messageOffset = 0xa;
      } else if (partyResultShown == 0) {
        messageOffset = 0xb;
      }
      break;
    }

    if (effect->range != 0xff)
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
  u8 *definition = Ability_Get(action);

  switch (definition[1] & 15) {
  case 1:
    Sound_PlayReturnOne(SE_RECOVERY);
  case 11:
    Sound_PlayReturnOne(SE_RECOVERY);
    break;
  default:
    switch (definition[3]) {
    case 1:
    case 2:
    case 31:
    case 32:
      break;
    case 5:
      Sound_PlayReturnOne(MUS_REVIVE);
      break;
    case 3:
      Sound_PlayReturnOne(MUS_CURE);
      break;
    default:
      Sound_PlayReturnOne(MUS_JINGLE_SMALL);
      break;
    }
    break;
  }
}
