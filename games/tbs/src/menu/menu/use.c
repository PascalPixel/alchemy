/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
// Part of menu.c.

#include "../menu_internal.h"

/// Copies the first sprite palette into background palette 14 and makes a
/// highlight palette in bank 13. Nothing calls it.
void Item_PrepareUsePalette(void) {
  Palette_CopyObjectBankToBackground14();
  Palette_LightenBankHighlight(13);
}

/// Nothing calls it.
void Item_UseNoOpCallback(void) {}

/// Uses the item in ownerId's inventory slot on targetId, or on every member
/// when its action's range is ABILITY_RANGE_ALL: applies its action at its
/// plain power (BattleEffect_ApplyToTargets with fixedScale 1). When the action
/// changed something, an item that is used up (ITEM_USE_CONSUMED) loses one
/// from its stack and the menu's list is rebuilt, and Hermes' Water
/// (ITEM_USE_EMPTIES) becomes an Empty Bottle. Returns 0, or
/// -1 when nothing changed; the result message is left in the work's
/// messageOffset. Called by ItemMenu_Use.
s32 Item_Use(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *menu;
  s32 useResult;
  s32 itemId;
  struct BattleUnit *unit;
  struct ItemDefinition *item;

  unit = UnitGetFar(ownerId);
  menu = gMenuWork;
  itemId = ITEM_ID_MASK & unit->inventory[slot];
  item = Item_GetDirectFar(itemId);
  useResult = BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->actionId,
                                          ownerId, targetId, 1);
  if (useResult != -1) {
    item = Item_GetDirectFar(unit->inventory[slot]);
    if (item->useType == ITEM_USE_CONSUMED) {
      Inventory_RemoveFar(ownerId, slot);
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

/// Nothing calls it.
s32 Item_ReturnOne(void) { return 1; }

/// Applies the healing, curing, revival or lasting stat rise of action
/// effectId, used by member sourceId, to targetId, or to every member when the
/// action's range is ABILITY_RANGE_ALL. Item_Use passes fixedScale 1, the
/// action's plain power; the Psynergy screen 0, which scales HP healing by the
/// user's Power in the action's element (BattleRecoveryCalc). Leaves the result
/// message in the work's messageOffset, counted from MsgItemUseResult ("You
/// recovered all HP!"). Returns 0 after recalculating every member, or -1 when
/// nothing changed.
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

  effect = BattleActGet(effectId);
  menu = gMenuWork;
  changed = 0;
  messageOffset = 0;
  partyResultShown = 0;

  if (targetId != MENU_TARGET_WHOLE_PARTY)
    target = UnitGetFar(targetId);
  else
    target = UnitGetFar(0);

  /* FAKEMATCH(dead): a test that is never true (partyResultShown is still
   * 0), which emits nothing but loads effect->range ahead of the loop's count
   * guard; without it, 21 of 485 instructions differ in tbs-en. */
  if (effect->range == ABILITY_RANGE_ALL && partyResultShown != 0)
    messageOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;

  // A party-wide action keeps the first useful message while visiting targets.
  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == ABILITY_RANGE_ALL) {
      targetId = menu->ownerIds[targetIndex];
      target = UnitGetFar(targetId);
    }

    // The ability kind first; the result messages are enum ItemUseResult.
    amount = effect->power;
    switch (effect->flags & ABILITY_KIND_MASK) {
    case ABILITY_KIND_HEAL_HP:
      if (fixedScale == 0) {
        if (effect->element != ELEMENT_NONE) {
          s32 elementIndex;

          caster = UnitGetFar(sourceId);
          elementIndex = effect->element;
          elementPower = caster->element[elementIndex].power;
        } else
          elementPower = 100;
        amount = BattleRecoveryCalc(amount, elementPower, 0x100);
      }

      if (target->hp <= 0) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        if (target->hp == target->maxHp) {
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_HP_FULL;
        } else {
          target->hp += amount;
          if (target->hp > target->maxHp) {
            amount -= target->hp - target->maxHp;
            target->hp = target->maxHp;
            if (partyResultShown == 0)
              messageOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
          } else if (partyResultShown == 0) {
            messageOffset = ITEM_USE_RESULT_HP_RECOVERED;
          }
          UnitRatioFar(targetId);
          changed = 1;
          if (effect->range == ABILITY_RANGE_ALL) {
            partyResultShown = 1;
            messageOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;
          }
        }
      }
      break;

    // The stat items. The stat rises by power, less one a quarter of the time
    // and plus one a quarter of the time (a roll of 0 to 3: 0 gives -1, 1 gives
    // +1, 2 and 3 give 0); Luck rises by power alone.
    case ABILITY_KIND_NO_POWER:
      randomAdjust = (Random16() * 4) >> 16;
      if (randomAdjust == 0) {
        randomAdjust = -1;
      } else {
        randomDifference = 1 ^ randomAdjust;
        randomAdjust = (u32)((-randomDifference) | randomDifference) >> 31;
        randomAdjust = 1 - randomAdjust;
      }

      switch (effectId & OWNER_ACTION_ID_MASK) {
      case 0x104: // Power Bread
        target->baseHp += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_MAX_HP_UP;
        changed = 1;
        break;
      case 0x105: // Cookie
        target->basePp += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_MAX_PP_UP;
        changed = 1;
        break;
      case 0x108: // Mint
        target->baseAgility += amount + randomAdjust;
        messageOffset = ITEM_USE_RESULT_AGILITY_UP;
        changed = 1;
        break;
      case 0x109: // Lucky Pepper
        target->baseLuck += amount;
        messageOffset = ITEM_USE_RESULT_LUCK_UP;
        changed = 1;
        break;
      case 0x106: // Apple
        target->baseAttack += amount + randomAdjust;
        UiWork_PushValueSlotFar(3, MSG_ARG_NUMBER);
        messageOffset = ITEM_USE_RESULT_ATTACK_UP;
        changed = 1;
        break;
      case 0x107: // Hard Nut
        target->baseDefense += amount + randomAdjust;
        UiWork_PushValueSlotFar(4, MSG_ARG_NUMBER);
        messageOffset = ITEM_USE_RESULT_DEFENSE_UP;
        changed = 1;
        break;
      }
      break;

    case ABILITY_KIND_HEAL_PP:
      if (target->pp == target->maxPp) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          amount -= target->pp - target->maxPp;
          target->pp = target->maxPp;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_PP_RECOVERED;
        }
        UnitRatioFar(targetId);
        changed = 1;
        if (effect->range == ABILITY_RANGE_ALL) {
          partyResultShown = 1;
          messageOffset = ITEM_USE_RESULT_PARTY_PP_RECOVERED;
        }
      }
      break;

    case ABILITY_KIND_EFFECT:
    case ABILITY_KIND_ATTACK:
    case ABILITY_KIND_ATTACK_MULTIPLIED:
    case ABILITY_KIND_DAMAGE:
    case ABILITY_KIND_UNLEASHED_DAMAGE:
    case ABILITY_KIND_EFFECT_ONLY:
    case ABILITY_KIND_SUMMON:
    case ABILITY_KIND_PP_DAMAGE:
      break;
    }

    // Then the ability's effect (enum AbilityEffect). The first two cases are
    // an HP and a PP restore no ability has (actions.tsv), so they have no
    // enumerator yet.
    switch (effect->effect) {
    case 1:
      if (target->hp <= 0 || target->hp == target->maxHp) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
      } else {
        target->hp += amount;
        if (target->hp > target->maxHp) {
          target->hp = target->maxHp;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_HP_RECOVERED;
        }
        UnitRatioFar(targetId);
        changed = 1;
      }
      break;

    case 2:
      if (target->pp == target->maxPp) {
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_PP_FULL;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          target->pp = target->maxPp;
          if (partyResultShown == 0)
            messageOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
        } else if (partyResultShown == 0) {
          messageOffset = ITEM_USE_RESULT_PP_RECOVERED;
        }
        UnitRatioFar(targetId);
        changed = 1;
      }
      break;

    case ABILITY_EFFECT_REVIVE:
      if (target->hp == 0) {
        target->hp = target->maxHp;
        UnitRatioFar(targetId);
        changed = 1;
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_REVIVED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    // Quartz's and Dew's revivals set no change, so they would report -1
    // after reviving; no item has either action (items.tsv), and no Djinni is
    // used from the menus. Dew gives 7/10 of maximum HP here.
    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->maxHp / 2;
        UnitRatioFar(targetId);
        if (partyResultShown == 0)
          messageOffset = ITEM_USE_RESULT_REVIVED;
      } else if (partyResultShown == 0) {
        messageOffset = ITEM_USE_RESULT_NOT_DOWN;
      }
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp == 0) {
        target->hp = target->maxHp * 7 / 10;
        UnitRatioFar(targetId);
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
    UnitCalcFar(menu->ownerIds[targetIndex]);
  menu->messageOffset = messageOffset;
  return 0;
}

/// Plays the use sound of item's action (Ability_PlayUseAnimation). Called by
/// ItemMenu_Use after a use that changed something.
void Item_PlayUseAnimation(u32 item) {
  Ability_PlayUseAnimation(Item_GetDirectFar(item)->actionId &
                           OWNER_ACTION_ID_MASK);
}

/// Plays the sound of action used from the menu, and returns at once: no
/// animation is drawn and nothing waits. HP and PP healing
/// (ABILITY_KIND_HEAL_HP and ABILITY_KIND_HEAL_PP) play SE_RECOVERY, HP healing
/// twice; otherwise Revive's effect plays MUS_082, Cure Poison's MUS_084,
/// Drain, Psy Drain and the unused HP and PP restores nothing, and every other
/// effect, ABILITY_EFFECT_NONE of the stat items among them, MUS_091. Called
/// after a successful use by Item_PlayUseAnimation (for ItemMenu_Use) and by
/// the Psynergy screen.
void Ability_PlayUseAnimation(s32 action) {
  s32 animationType;
  u32 targetType;
  struct BattleAct *actionDefinition;

  actionDefinition = BattleActGet(action);
  animationType = ABILITY_KIND_MASK & actionDefinition->flags;
  switch (animationType) {
  case ABILITY_KIND_HEAL_HP:
    // XXX: with no break the recovery sound is started twice; whether that
    // was meant would show only in the sound driver's handling of a cue
    // started again in the same frame.
    Audio_PlayCueReturnOne(SE_RECOVERY);
    // Fall through.
  case ABILITY_KIND_HEAL_PP:
    Audio_PlayCueReturnOne(SE_RECOVERY);
    return;
  default:
    targetType = actionDefinition->effect;
    switch (targetType) {
    case ABILITY_EFFECT_REVIVE:
      Audio_PlayCueReturnOne(MUS_082);
      return;
    case ABILITY_EFFECT_CURE_POISON:
      Audio_PlayCueReturnOne(MUS_084);
      return;
    default:
      Audio_PlayCueReturnOne(MUS_091);

    // The HP and PP restores no ability has (actions.tsv), which have no
    // enumerator yet.
    case 1:
    case 2:
    case ABILITY_EFFECT_DRAIN_HP:
    case ABILITY_EFFECT_DRAIN_PP:
      return;
    }
    break;
  }
}

/// Nothing calls it.
s32 Menu_ReservedStatusZero(void) { return 0; }
