/// @file
/// An ability classified for target selection, and a reserve member chosen for
/// a party slot.
// Part of screens.c.

#include "window_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "battle/motobj.h"

/// Classifies ability `act`'s effect for target selection: the hint
/// BattleTarget_RunSelection shows on a party member and starts on (enum
/// BattleTargetHint).
s32 Battle_ClassifyEntryKind(const struct AbilityDefinition *act) {
  s32 abilityKind;
  s32 targetKind;

  targetKind = TARGET_HINT_NONE;
  abilityKind = act->flags & ABILITY_FORMULA_MASK;
  if (abilityKind == ABILITY_FORMULA_HEAL_HP)
    targetKind = TARGET_HINT_HP;
  if (abilityKind == ABILITY_FORMULA_HEAL_PP)
    targetKind = TARGET_HINT_PP;
  if (act->effect == ABILITY_EFFECT_CURE_POISON)
    targetKind = TARGET_HINT_CURE_POISON;
  if (act->effect == ABILITY_EFFECT_RESTORE)
    targetKind = TARGET_HINT_RESTORE;
  if (act->effect == ABILITY_EFFECT_CURE_ALL)
    targetKind = TARGET_HINT_CURE_ALL;
  if (Battle_IsReviveEffectFar(act->effect) != 0)
    targetKind = TARGET_HINT_REVIVE;
  return targetKind;
}

/// Lets the player choose which reserve member takes a party slot in battle,
/// previewing each choice in place: left and right cycle through the reserve,
/// A confirms and swaps the two in gMainRoutineState.party, and B puts
/// the slot's member back.
s32 BattleParty_ChooseReserve(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleState *battle;
  s32 count;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  {
    s32 sel;
    s32 redraw;
    s32 result;
    struct BattleMenuState *menu;
    s32 current;
    s32 newId;
    s32 i;

    sel = 0;
    redraw = 1;
    battle = Ram_HeapSlots->battleState;
    result = 0;
    menu = Ram_HeapSlots->battleMenuState;
    if (count <= 0)
      return -1;
    // BattleTarget_RunSelection's PLACE_PARTY | place: the place is its low
    // byte.
    slot &= 0xff;
    current = battle->partyUnits[slot];
    Sound_PlayFar(SE_MENU_CONFIRM);
    /* FAKEMATCH(dead): a once-only block; as its body alone. */
    do {
      goto test;
      for (;;) {
        // Japanese also takes A on the first frame, before the first preview
        // has been drawn.
#if TLA_PATCH_1
        if ((gInput.pressed & KEY_A) && !redraw) {
#else
        if (gInput.pressed & KEY_A) {
#endif
          // The preview already stands in the slot: a swap that would leave
          // no one standing is refused.
          if (BattleUnit_ListLivingIdsFar(BATTLE_SIDE_MASK_PARTY, 0) == 0) {
            Sound_PlayFar(SE_MENU_ERROR);
            goto test;
          }
          Sound_PlayFar(SE_CHIME);
          menu->swappedUnit = current;
          menu->swappedPlace = slot;
          for (i = 0; i < 8; i++) {
            if (gMainRoutineState.party[i] == current)
              gMainRoutineState.party[i] = newId;
            else if (gMainRoutineState.party[i] == newId)
              gMainRoutineState.party[i] = current;
          }
          goto done;
        }
        if (gInput.repeat & KEY_LEFT) {
          sel = (sel + count - 1) % count;
          Sound_PlayFar(SE_MENU_CONFIRM);
          redraw = 1;
        } else if (gInput.repeat & KEY_RIGHT) {
          sel = (sel + 1) % count;
          Sound_PlayFar(SE_MENU_CONFIRM);
          redraw = 1;
        }
        if (redraw) {
          struct BattleObject *from;
          struct BattleObject *to;
          struct Actor *actor;
          struct Actor *place;

          redraw = 0;
          newId = battle->partyUnits[slot] = owners[sel];
          from = BattleObject_GetFar(current);
          to = BattleObject_GetFar(newId);
          to->anchorX = from->anchorX;
          place = (struct Actor *)from->object;
          actor = (struct Actor *)to->object;
          to->anchorZ = from->anchorZ;
          Actor_SetPositionAndResetMotionFar(actor, place->x.fixed,
                                             place->y.fixed, place->z.fixed);
          PartyStatus_Draw(Ram_HeapSlots->battleState->partyStatus);
          BattleObject_PutFar();
          selected[0] = newId;
          selected[1] = BATTLE_UNIT_LIST_END;
          BattleHighlight_SetModesFar(selected, HIGHLIGHT_PULSE);
          PartyStatus_HighlightMembers(selected);
        }
        wait(1);
      test:
        newId = battle->partyUnits[slot] = owners[sel];
        if (gInput.pressed & KEY_B)
          break;
      }
      battle->partyUnits[slot] = current;
      result = -1;
      Sound_PlayFar(SE_MENU_CANCEL);
    done:
      selected[0] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(selected, HIGHLIGHT_OFF);
      PartyStatus_HighlightMembers(selected);
      PartyStatus_Draw(Ram_HeapSlots->battleState->partyStatus);
      BattleObject_PutFar();
      wait(1);
    } while (0);
    return result;
  }
}

/// Lists the units still standing on the sides in sideMask, party first, into
/// unitIds when it is not null, and returns how many.
s32 BattleUnit_ListLivingFar(s32 sideMask, u16 *unitIds);

/// The first half of The Broken Seal's Battle_BuildPartyActions: each action
/// of a living party member who is asleep, confused, charmed or stunned is lost
/// (COMMAND_NONE) and written at *cursor, which moves past them, their count
/// going to *forcedCount; every other member's actions are listed in ready to
/// be chosen. Returns how many are listed.
s32 BattleParty_SplitChoosers(u16 *ready, struct BattleActionRecord **cursor,
                              s32 *forcedCount) {
  u16 *unitIds = Heap_AllocEwram(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleUnit_ListLivingFar(BATTLE_SIDE_MASK_PARTY, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct Unit *unit = Unit_Get(unitIds[i]);

    for (action = 0; action < unit->actionsPerTurn; action++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleActionRecord *entry = &entries[forced];

        entry->unitId = unitIds[i];
        entry->priority = unit->agility;
        entry->command = COMMAND_NONE;
        entry->parameter = 0;
        // The other side's place 0, as a lost action's target.
        entry->target = PLACE_ENEMY;
        forced++;
      } else {
        ready[readyCount++] = unitIds[i];
      }
    }
  }
  *forcedCount = forced;
  entries += forced;
  *cursor = entries;
  return readyCount;
}
