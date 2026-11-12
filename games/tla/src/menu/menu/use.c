/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
// Part of menu.c.

#include "../menu_internal.h"

s32 Func_08100738(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *menu;
  s32 useResult;
  s32 itemId;
  s32 entry;
  struct OwnerInventoryState *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(temporary): the inventory entry read into its own local first, so
   * the barrier sits between the mask and the and; as itemId &=
   * unit->inventory[slot] after the asm, 24 of 90 instructions differ in
   * tla-en. */
  entry = unit->inventory[slot];
  itemId = ITEM_ID_MASK;
  itemId &= entry;
  item = Item_Get(itemId);
  useResult = BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->actionId,
                                          ownerId, targetId, 1);
  if (useResult != -1) {
    item = Item_Get(unit->inventory[slot]);
    if (item->useType == 1) {
      Inventory_RemoveFar(ownerId, slot);
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

s32 BattleEffect_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                                s32 fixedScale) {
  struct BattleAct *effect;
  struct InventoryMenuState *menu;
  struct BattleUnit *target;
  struct BattleUnit *caster;
  s32 partyResultShown;
  s32 changed;
  u8 targetIndex;
  s16 elementPower;
  s32 randomAdjust;
  s32 randomDifference;
  s32 amount;
  s32 messageOffset;

  menu = Ram_HeapSlots->menuRuntime;
  effect = BattleAction_Get(effectId);
  changed = 0;
  messageOffset = 0;
  partyResultShown = 0;

  if (targetId == 9)
    target = Unit_Get(gMainRoutineWork.currentOwner);
  else
    target = Unit_Get(targetId);

  /* FAKEMATCH(dead): the dead test loads effect->range ahead of the
   * count guard, as in The Broken Seal's twin; as plain code, 84 of 501
   * instructions differ in tla-en. */
  if (effect->range == 0xff && partyResultShown != 0)
    messageOffset = 3;

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == 0xff) {
      // Read unsigned (ldrh): the party's ids, as this routine's bytes load them.
      targetId = (u16)menu->ownerIds[targetIndex];
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
        if (target->hp == target->maxHp) {
          if (partyResultShown == 0)
            messageOffset = 4;
        } else {
          target->hp += amount;
          if (target->hp > target->maxHp) {
            amount -= target->hp - target->maxHp;
            target->hp = target->maxHp;
            if (partyResultShown == 0)
              messageOffset = 0;
          } else if (partyResultShown == 0) {
            messageOffset = 1;
          }
          Unit_UpdateRatiosFar(targetId);
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

      switch (effectId & OWNER_ACTION_ID_MASK) {
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
        MessageArgument_PushFar(3, 5);
        messageOffset = 0x14;
        changed = 1;
        break;
      case 0x107:
        target->baseDefense += amount + randomAdjust;
        MessageArgument_PushFar(4, 5);
        messageOffset = 0x15;
        changed = 1;
        break;
      }
      break;

    case BATTLE_DAMAGE_PP_HEAL:
      if (target->pp == target->maxPp) {
        if (partyResultShown == 0)
          messageOffset = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          amount -= target->pp - target->maxPp;
          target->pp = target->maxPp;
          if (partyResultShown == 0)
            messageOffset = 5;
        } else if (partyResultShown == 0) {
          messageOffset = 6;
        }
        Unit_UpdateRatiosFar(targetId);
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
      if (target->hp <= 0 || target->hp == target->maxHp) {
        if (partyResultShown == 0)
          messageOffset = 2;
      } else {
        target->hp += amount;
        if (target->hp > target->maxHp) {
          target->hp = target->maxHp;
          if (partyResultShown == 0)
            messageOffset = 0;
        } else if (partyResultShown == 0) {
          messageOffset = 1;
        }
        Unit_UpdateRatiosFar(targetId);
        changed = 1;
      }
      break;

    case 2:
      if (target->pp == target->maxPp) {
        if (partyResultShown == 0)
          messageOffset = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          target->pp = target->maxPp;
          if (partyResultShown == 0)
            messageOffset = 5;
        } else if (partyResultShown == 0) {
          messageOffset = 6;
        }
        Unit_UpdateRatiosFar(targetId);
        changed = 1;
      }
      break;

    case ABILITY_EFFECT_REVIVE:
      if (target->hp == 0) {
        target->hp = target->maxHp;
        Unit_UpdateRatiosFar(targetId);
        changed = 1;
        if (partyResultShown == 0)
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->maxHp / 2;
        Unit_UpdateRatiosFar(targetId);
        if (partyResultShown == 0)
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp == 0) {
        target->hp = target->maxHp * 7 / 10;
        Unit_UpdateRatiosFar(targetId);
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
    Unit_CalculateFar((u16)menu->ownerIds[targetIndex]);
  menu->messageOffset = messageOffset;
  return 0;
}

s32 Func_08100d40(s32 item) {
  return Ability_PlayUseAnimation(Item_Get(item)->actionId & 0x3fff);
}

/// Plays the sound of action used from the menu, and returns at once: no
/// animation is drawn and nothing waits. Formulas 1 and 11 play sound 126,
/// formula 1 twice; otherwise effects 1, 2, 31 and 32 play nothing, effect 5
/// sound 82, effect 3 sound 84, and every other effect sound 91.
s32 Ability_PlayUseAnimation(s32 action) {
  u8 *definition = BattleAction_Get(action);

  switch (definition[1] & 15) {
  case 1:
    Sound_PlayReturnOne(126);
  case 11:
    Sound_PlayReturnOne(126);
    break;
  default:
    switch (definition[3]) {
    case 1:
    case 2:
    case 31:
    case 32:
      break;
    case 5:
      Sound_PlayReturnOne(82);
      break;
    case 3:
      Sound_PlayReturnOne(84);
      break;
    default:
      Sound_PlayReturnOne(91);
      break;
    }
    break;
  }
}
