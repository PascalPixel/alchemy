/// @file
/// Using an item or Psynergy from the menu: what it does to the party, and the
/// sound it plays.
/// Part of menu.c, the menu object, compiled only through it.

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
/// (ITEM_USE_EMPTIES, item 0xb8) becomes an Empty Bottle (0xb9). Returns 0, or
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
    if (item->useType == 1) {
      Inventory_RemoveFar(ownerId, slot);
      menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
    }
    if (item->useType == 4) {
      if (itemId == 0xb8)
        itemId = 0xb9;
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
  if (effect->range == 0xff && partyResultShown != 0)
    messageOffset = 3;

  // A party-wide action keeps the first useful message while visiting targets.
  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == 0xff) {
      targetId = menu->ownerIds[targetIndex];
      target = UnitGetFar(targetId);
    }

    // The ability's kind (flags bits 3..0). The result messages, counted from
    // MsgItemUseResult: 0 all HP recovered, 1 some, 2 none, 3 everyone's, 4 HP
    // already full; 5 all PP recovered, 6 some, 7 PP already full, 8
    // everyone's; 10 and 11 poison purged or none; 12 and 13 revived or not
    // down; 16 to 21 a stat raised.
    amount = effect->power;
    switch (effect->flags & 0xf) {
    case BATTLE_DAMAGE_HP_HEAL:
      if (fixedScale == 0) {
        if (effect->element != 4) {
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
          UnitRatioFar(targetId);
          changed = 1;
          if (effect->range == 0xff) {
            partyResultShown = 1;
            messageOffset = 3;
          }
        }
      }
      break;

    // Kind 9: the stat items. The stat rises by power, less one a quarter of
    // the time and plus one a quarter of the time (a roll of 0 to 3: 0 gives
    // -1, 1 gives +1, 2 and 3 give 0); Luck rises by power alone.
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
      case 0x104: // Power Bread
        target->baseHp += amount + randomAdjust;
        messageOffset = 0x10;
        changed = 1;
        break;
      case 0x105: // Cookie
        target->basePp += amount + randomAdjust;
        messageOffset = 0x11;
        changed = 1;
        break;
      case 0x108: // Mint
        target->baseAgility += amount + randomAdjust;
        messageOffset = 0x12;
        changed = 1;
        break;
      case 0x109: // Lucky Pepper
        target->baseLuck += amount;
        messageOffset = 0x13;
        changed = 1;
        break;
      case 0x106: // Apple
        target->baseAttack += amount + randomAdjust;
        UiWork_PushValueSlotFar(3, 5);
        messageOffset = 0x14;
        changed = 1;
        break;
      case 0x107: // Hard Nut
        target->baseDefense += amount + randomAdjust;
        UiWork_PushValueSlotFar(4, 5);
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
        UnitRatioFar(targetId);
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

    // Then the ability's effect (enum AbilityEffect).
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
        UnitRatioFar(targetId);
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
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
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
          messageOffset = 0xc;
      } else if (partyResultShown == 0) {
        messageOffset = 0xd;
      }
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp == 0) {
        target->hp = target->maxHp * 7 / 10;
        UnitRatioFar(targetId);
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
/// animation is drawn and nothing waits. HP and PP healing (kinds 1 and 11)
/// play SE_RECOVERY, kind 1 twice; otherwise Revive's effect plays MUS_082,
/// Cure Poison's MUS_084, effects 1, 2, 31 and 32 nothing, and every other
/// effect, the stat items' 0 among them, MUS_091. Called after a successful
/// use by Item_PlayUseAnimation (for ItemMenu_Use) and by the Psynergy screen.
void Ability_PlayUseAnimation(s32 action) {
  s32 animationType;
  u32 targetType;
  struct BattleAct *actionDefinition;

  actionDefinition = BattleActGet(action);
  animationType = 0xf & actionDefinition->flags;
  switch (animationType) {
  case 1:
    // XXX: with no break the recovery sound is started twice; whether that
    // was meant would show only in the sound driver's handling of a cue
    // started again in the same frame.
    Audio_PlayCueReturnOne(SE_RECOVERY);
    // Fall through.
  case 11:
    Audio_PlayCueReturnOne(SE_RECOVERY);
    return;
  default:
    // The effect less one: effect 0 wraps to the u32 maximum, which takes
    // the default case.
    targetType = actionDefinition->effect - 1;
    switch (targetType) {
    case 4:
      Audio_PlayCueReturnOne(MUS_082);
      return;
    case 2:
      Audio_PlayCueReturnOne(MUS_084);
      return;
    default:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
      Audio_PlayCueReturnOne(MUS_091);

    case 0:
    case 1:
    case 30:
    case 31:
      return;
    }
    break;
  }
}

/// Nothing calls it.
s32 Menu_ReservedStatusZero(void) { return 0; }
