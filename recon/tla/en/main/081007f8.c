// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 18 of 501 in tla-en. First difference: the native issues `str r1, [sp, #12]`
// earlier (row 9) than this does (row 14).
// First draft of BattleEffect_ApplyToTargets: applies healing, revival or
// permanent-stat effects of a menu action to one member or the whole party,
// the twin of The Broken Seal's function of the same name
// (games/tbs/src/menu/menu/use.c). ⚓️ takes the whole party's first target
// from gPartyState.currentOwner and draws the changed Attack or Defense with
// UiText_DrawQuantity. The result message offset is kept in the menu work;
// -1 reports that nothing changed.
// Differ 38 of 501 (2026-10-19), instruction order only: the native spills
// sourceId and effectId before it builds the heap address, and loads the menu
// from its stack slot before the offsets' moves throughout; the whole-party
// test is written == 9 first, as the native falls through to it (differ 46
// the other way round).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_unit.h"
#include "battle_effect.h"

struct InventoryMenuState {
  u8 unk000[0x204];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 unk216[0x48];
  u16 messageOffset;
};

struct BattleAct *BattleAction_Get(s32 action);
struct BattleUnit *Unit_Get(s32 owner);
s32 Battle_CalculateRecoveryFar(s32 value, s32 selector, s32 multiplier);
void Unit_UpdateRatiosFar(s32 unitId);
void Unit_CalculateFar(s32 unitId);
u32 Random16(void);
void UiText_DrawQuantity(s32 slot, s32 digits);

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
    target = Unit_Get(gPartyState.currentOwner);
  else
    target = Unit_Get(targetId);

  /* FAKEMATCH(dead code): the dead test loads effect->range ahead of the
   * count guard, as in The Broken Seal's twin; as plain code, 84 of 501
   * instructions differ in tla-en. */
  if (effect->range == 0xff && partyResultShown != 0)
    messageOffset = 3;

  for (targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
    if (effect->range == 0xff) {
      targetId = menu->ownerIds[targetIndex];
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
        UiText_DrawQuantity(3, 5);
        messageOffset = 0x14;
        changed = 1;
        break;
      case 0x107:
        target->baseDefense += amount + randomAdjust;
        UiText_DrawQuantity(4, 5);
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
    Unit_CalculateFar(menu->ownerIds[targetIndex]);
  menu->messageOffset = messageOffset;
  return 0;
}
