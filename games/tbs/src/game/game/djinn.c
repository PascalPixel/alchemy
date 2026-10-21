/// @file
/// Djinn: definitions, setting and standby, and their counts.
// Part of game.c.

#include "../game_internal.h"

/// Returns the definition of Djinni `index` of element `group`; anything out of
/// range gives DJINNI_FLINT's. Called by UnitCalc for a set Djinni's stat
/// bonuses and by Djinn_GetAbilityId.
const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index) {
  s32 entry;

  entry = 0;
  if ((group < NUM_ELEMENTS) && (index < DJINN_PER_ELEMENT)) {
    entry = (group * DJINN_PER_ELEMENT) + index;
  }
  return &gDjinn_DefinitionTable[entry];
}

/// Gives a newly found Djinni to the party member holding the fewest Djinn who
/// has room in that element, puts it on standby and sets its found flag.
/// Returns who got it, -1 if it was found before or -2 when nobody has room.
/// Called by Djinn_JoinParty when a Djinni joins in the field. Of members
/// holding equally few, the earliest in the party order gets it.
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn) {
  s32 flag = djinn + element * DJINN_PER_ELEMENT + FLAG_DJINNI_0;
  s32 leastLoadedOwner = 0;
  s32 fewestDjinn = 999;
  s32 partySize;
  s32 partyIndex;
  s32 elementIndex;

  if (GameFlag_Test(flag) != 0)
    return -1;

  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct BattleUnit *unit = UnitGet(gGameState.activeOwners[partyIndex]);

    if (unit->djinnOwnedCounts[element] <= 9) {
      s32 ownedDjinn = 0;

      for (elementIndex = 0; elementIndex < NUM_ELEMENTS; elementIndex++)
        ownedDjinn += unit->djinnOwnedCounts[elementIndex];
      if (fewestDjinn > ownedDjinn) {
        fewestDjinn = ownedDjinn;
        leastLoadedOwner = gGameState.activeOwners[partyIndex];
      }
    }
  }

  if (fewestDjinn == 999)
    return -2;

  Djinn_AddToOwner(leastLoadedOwner, element, djinn);
  DjinnRecovery_AddStandby(leastLoadedOwner, element, djinn);
  GameFlag_SetBit(flag);
  return leastLoadedOwner;
}

/// Gives Djinni `djinnIndex` of `element` to a unit: sets its bit in
/// djinnAvailable and counts it. Returns -1 when the unit holds ten of the
/// element or has it already. Called by Djinn_AddToLeastLoadedOwner and
/// Djinn_Transfer, by the scenes where a member joins with Djinn, and by the
/// debug menus.
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);

  if (unit->djinnOwnedCounts[element] > 9)
    return -1;
  if ((unit->djinnAvailable[element] & (1 << djinnIndex)) != 0)
    return -1;
  unit->djinnOwnedCounts[element]++;
  unit->djinnAvailable[element] |= 1 << djinnIndex;
  return 0;
}

/// Returns 1 when the unit holds the Djinni and it can be set: it is not set
/// already, not recovering and not held by a summon. A count of set Djinn
/// above 9 is clamped to 10 on the way. Called by the Djinn menu to list what
/// can be set, and by battle to tell which Djinn can be unleashed.
s32 Djinn_IsReady(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  struct DjinnRecoveryList *recoveryList;
  s32 entryIndex;
  s32 recoveryTurns;

  if (unit->djinnOwnedCounts[element] == 0)
    return 0;
  if (unit->djinnActiveCounts[element] > 9) {
    unit->djinnActiveCounts[element] = 10;
    return 0;
  }
  if ((unit->djinnAvailable[element] & (1 << djinnIndex)) == 0)
    return 0;
  if ((unit->djinnActive[element] & (1 << djinnIndex)) != 0)
    return 0;

  recoveryList = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
                       (owner < 0 || owner > PIERS)))
                      ->list;
  for (entryIndex = 0; entryIndex < recoveryList->count; entryIndex++) {
    if (element == recoveryList->entries[entryIndex].element &&
        djinnIndex == recoveryList->entries[entryIndex].index)
      break;
  }
  // Ready when not listed, on standby (-1) or recovered (0); not while
  // recovering or held by a summon (-2).
  if (entryIndex == recoveryList->count ||
      ((recoveryTurns = recoveryList->entries[entryIndex].turns) <= 0 &&
       recoveryTurns != -2))
    return 1;
  return 0;
}

/// Returns 1 when the Djinni is set on the unit.
u32 Djinn_IsActive(s32 owner, s32 element, s32 djinnIndex) {
  s32 activeBit = UnitGet(owner)->djinnActive[element] & (1 << djinnIndex);

  return (u32)(-activeBit | activeBit) >> 31;
}

/// Sets a ready Djinni on the unit, counts it and works out the unit's class
/// again. Returns 1 when it was set. Called by the Djinn menu, by battle when
/// a Djinni recovers, and by Djinn_Transfer.
s32 Djinn_Activate(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 isReady = Djinn_IsReady(owner, element, djinnIndex);

  if (isReady != 0) {
    if (unit->djinnAvailable[element] & (1 << djinnIndex)) {
      unit->djinnActive[element] |= 1 << djinnIndex;
    } else {
      return 0;
    }
    unit->djinnActiveCounts[element]++;
    Unit_UpdateClass(owner);
  }
  return isReady;
}

/// Takes a set Djinni off the unit and works out its class again. Returns 1
/// when it was set. Called by the Djinn menu, by battle when a Djinni is
/// unleashed, and by Djinn_Transfer.
u32 Djinn_Deactivate(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  u32 wasActive = Djinn_IsActive(owner, element, djinnIndex);

  if (wasActive != 0) {
    unit->djinnActiveCounts[element]--;
    unit->djinnActive[element] &= ~(1 << djinnIndex);
    Unit_UpdateClass(owner);
  }
  return wasActive;
}

/// Takes the Djinni off its side's standby and recovery list, closing the
/// gap. Returns 1 when it was there. Called when a Djinni is set again, by the
/// Djinn menu and by battle's recovery.
s32 DjinnRecovery_Remove(s32 owner, s32 element, s32 djinnIndex) {
  struct DjinnRecoveryList *recoveryList;
  s32 removed = 0;
  s32 entryIndex;

  recoveryList = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
                       (owner < 0 || owner > PIERS)))
                      ->list;
  for (entryIndex = 0; entryIndex < recoveryList->count; entryIndex++) {
    if (element == recoveryList->entries[entryIndex].element &&
        djinnIndex == recoveryList->entries[entryIndex].index) {
      recoveryList->count--;
      removed = 1;
      break;
    }
  }
  for (; entryIndex < recoveryList->count; entryIndex++) {
    recoveryList->entries[entryIndex] = recoveryList->entries[entryIndex + 1];
  }
  return removed;
}

/// Puts the owner's Djinni at the end of its side's list on standby (turns
/// -1), moving it there if it was already listed. Returns a pointer to the
/// list's count. Nothing checks the list's 64-entry capacity. Called when a
/// Djinni joins, by the Djinn menu's standby command, by battle and by the
/// scene where Flint joins.
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinnIndex) {
  struct DjinnRecoveryList *recoveryList;
  s32 entryIndex;

  DjinnRecovery_Remove(owner, element, djinnIndex);
  recoveryList =
      &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(owner > PIERS))
           ->list;
  entryIndex = recoveryList->count;
  recoveryList->entries[entryIndex].element = element;
  recoveryList->entries[entryIndex].index = djinnIndex;
  recoveryList->entries[entryIndex].unitId = owner;
  recoveryList->entries[entryIndex].turns = -1;
  recoveryList->count = entryIndex + 1;
  return (u32 *)&recoveryList->count;
}

/// Moves a Djinni from one unit to another, keeping it set or on standby.
/// Returns 0 when it moved, and -1 when the source lacks it or the target
/// has no room or holds it already. Both units' classes are worked out
/// again when it was set. Called by the Djinn menu's Give and Trade.
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target) {
  struct BattleUnit *sourceUnit = UnitGet(source);
  u32 wasActive;

  if ((sourceUnit->djinnAvailable[element] & (1 << djinnIndex)) != 0) {
    wasActive = Djinn_IsActive(source, element, djinnIndex);
    if (Djinn_AddToOwner(target, element, djinnIndex) == 0) {
      Djinn_Deactivate(source, element, djinnIndex);
      sourceUnit->djinnAvailable[element] &= ~(1 << djinnIndex);
      sourceUnit->djinnOwnedCounts[element]--;

      if (wasActive != 0) {
        Djinn_Activate(target, element, djinnIndex);
      } else {
        DjinnRecovery_Remove(source, element, djinnIndex);
        DjinnRecovery_AddStandby(target, element, djinnIndex);
      }
      return 0;
    }
  }
  return -1;
}

/// Counts the party's Djinn on standby, by element into `counts` when it is
/// given. Returns the total. Only GAME.S's bare veneer reaches it; nothing
/// calls it by name.
s32 DjinnRecovery_CountStandby(u8 *counts) {
  s32 standbyCount = 0;
  struct DjinnRecoveryTable *recoveryTable = Djinn_GetRecoveryTable(0);
  struct DjinnRecoveryEntry *entry = recoveryTable->list.entries;
  s32 entryIndex;

  if (counts != 0) {
    u8 *slot;

    slot = counts + 3;
    *slot = standbyCount;
    slot = counts + 2;
    *slot = standbyCount;
    slot = counts + 1;
    *slot = standbyCount;
    counts[0] = standbyCount;
  }
  entryIndex = 0;
  if (recoveryTable->list.count != 0) {
    do {
      if (entry->turns == -1) {
        if (counts != 0)
          counts[entry->element]++;
        standbyCount++;
      }
      entryIndex++;
      entry++;
    } while (entryIndex != recoveryTable->list.count);
  }
  return standbyCount;
}

/// Returns the ability ID used when the Djinni is unleashed. Called by the
/// battle plan, the battle queue's ordering and the battle menu.
s32 Djinn_GetAbilityId(u32 group, u32 index) {
  return Djinn_GetDefinition(group, index)->abilityId;
}

/// Returns how many Djinn of `element` the party holds, or of all
/// elements for -1. Called by the save summary, the field Status screen, the
/// battle status screen and the battle command menu.
s32 Party_SumDjinnCounts(s32 element) {
  u16 owners[16];
  s32 djinnCount = 0;
  s32 partySize = Party_ListActiveOwners(owners);

  if (djinnCount < partySize) {
    u16 *ownerCursor = owners;
    s32 remainingOwners = partySize;

    do {
      struct BattleUnit *unit = UnitGet(*ownerCursor++);

      if (element == -1) {
        djinnCount += unit->djinnOwnedCounts[0];
        djinnCount += unit->djinnOwnedCounts[1];
        djinnCount += unit->djinnOwnedCounts[2];
        djinnCount += unit->djinnOwnedCounts[3];
      } else {
        djinnCount += unit->djinnOwnedCounts[element];
      }
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return djinnCount;
}
