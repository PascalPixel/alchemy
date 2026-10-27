/// @file
/// Djinn: definitions, setting and standby, and their counts.
// Part of game.c.

#include "../game_internal.h"

/// Returns the definition of Djinni `index` of element `group`; anything out of
/// range gives DJINNI_FLINT's. Called by Unit_Calculate for a set Djinni's stat
/// bonuses and by Djinn_GetAbilityId.
const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index) {
  s32 entry;

  entry = 0;
  if ((group < ELEMENT_COUNT) && (index < DJINN_PER_ELEMENT)) {
    entry = (group * DJINN_PER_ELEMENT) + index;
  }
  return &gDjinn_DefinitionTable[entry];
}

/// Gives a newly found Djinni to the party member holding the fewest Djinn who
/// has room in that element, puts it on standby and sets its found flag.
/// Returns who got it, -1 if it was found before or -2 when nobody has room.
/// Called by Djinn_JoinParty when a Djinni joins in the field. Of party members
/// holding equally few, the earliest in the party order gets it.
s32 Djinn_GiveToParty(s32 element, s32 djinn) {
  s32 flag = djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST;
  s32 leastLoadedMember = 0;
  s32 fewestDjinn = 999;
  s32 partySize;
  s32 partyIndex;
  s32 elementIndex;

  if (GameFlag_Get(flag) != 0)
    return -1;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct Unit *unit = Unit_Get(gSaveBlock.party[partyIndex]);

    if (unit->djinnHeldCounts[element] <= 9) {
      s32 ownedDjinn = 0;

      for (elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++)
        ownedDjinn += unit->djinnHeldCounts[elementIndex];
      if (fewestDjinn > ownedDjinn) {
        fewestDjinn = ownedDjinn;
        leastLoadedMember = gSaveBlock.party[partyIndex];
      }
    }
  }

  if (fewestDjinn == 999)
    return -2;

  Djinn_Give(leastLoadedMember, element, djinn);
  DjinnRecovery_AddStandby(leastLoadedMember, element, djinn);
  GameFlag_Set(flag);
  return leastLoadedMember;
}

/// Gives Djinni `djinnIndex` of `element` to a unit: sets its bit in
/// djinnHeld and counts it. Returns -1 when the unit holds ten of the
/// element or has it already. Called by Djinn_GiveToParty and
/// Djinn_Transfer, by the events where a party member joins with Djinn, and by
/// the debug menus.
s32 Djinn_Give(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->djinnHeldCounts[element] > 9)
    return -1;
  if ((unit->djinnHeld[element] & (1 << djinnIndex)) != 0)
    return -1;
  unit->djinnHeldCounts[element]++;
  unit->djinnHeld[element] |= 1 << djinnIndex;
  return 0;
}

/// Returns 1 when the unit holds the Djinni and it can be set: it is not set
/// already, not recovering and not held by a summon. A count of set Djinn
/// above 9 is clamped to 10 on the way. Called by the Djinn menu to list what
/// can be set, and by battle to tell which Djinn can be unleashed.
s32 Djinn_CanSet(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);
  struct DjinnRecoveryList *list;
  s32 entryIndex;
  s32 recoveryTurns;

  if (unit->djinnHeldCounts[element] == 0)
    return 0;
  if (unit->djinnSetCounts[element] > 9) {
    unit->djinnSetCounts[element] = 10;
    return 0;
  }
  if ((unit->djinnHeld[element] & (1 << djinnIndex)) == 0)
    return 0;
  if ((unit->djinnSet[element] & (1 << djinnIndex)) != 0)
    return 0;

  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
               (unitId < 0 || unitId > PIERS)))
              ->list;
  for (entryIndex = 0; entryIndex < list->count; entryIndex++) {
    if (element == list->entries[entryIndex].element &&
        djinnIndex == list->entries[entryIndex].index)
      break;
  }
  // Ready when not listed, on standby (DJINN_RECOVERY_AFTER_BATTLE) or
  // recovered (0); not while recovering or held by a summon
  // (DJINN_RECOVERY_SUMMONED).
  if (entryIndex == list->count ||
      ((recoveryTurns = list->entries[entryIndex].turns) <= 0 &&
       recoveryTurns != DJINN_RECOVERY_SUMMONED))
    return 1;
  return 0;
}

/// Returns 1 when the Djinni is set on the unit.
u32 Djinn_IsSet(s32 unitId, s32 element, s32 djinnIndex) {
  s32 activeBit = Unit_Get(unitId)->djinnSet[element] & (1 << djinnIndex);

  return (u32)(-activeBit | activeBit) >> 31;
}

/// Sets a ready Djinni on the unit, counts it and works out the unit's class
/// again. Returns 1 when it was set. Called by the Djinn menu, by battle when
/// a Djinni recovers, and by Djinn_Transfer.
s32 Djinn_Set(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);
  s32 isReady = Djinn_CanSet(unitId, element, djinnIndex);

  if (isReady != 0) {
    if (unit->djinnHeld[element] & (1 << djinnIndex)) {
      unit->djinnSet[element] |= 1 << djinnIndex;
    } else {
      return 0;
    }
    unit->djinnSetCounts[element]++;
    Unit_UpdateClass(unitId);
  }
  return isReady;
}

/// Takes a set Djinni off the unit and works out its class again. Returns 1
/// when it was set. Called by the Djinn menu, by battle when a Djinni is
/// unleashed, and by Djinn_Transfer.
u32 Djinn_PutOnStandby(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);
  u32 wasActive = Djinn_IsSet(unitId, element, djinnIndex);

  if (wasActive != 0) {
    unit->djinnSetCounts[element]--;
    unit->djinnSet[element] &= ~(1 << djinnIndex);
    Unit_UpdateClass(unitId);
  }
  return wasActive;
}

/// Takes the Djinni off its side's standby and recovery list, closing the
/// gap. Returns 1 when it was there. Called when a Djinni is set again, by the
/// Djinn menu and by battle's recovery.
s32 DjinnRecovery_Remove(s32 unitId, s32 element, s32 djinnIndex) {
  struct DjinnRecoveryList *list;
  s32 removed = 0;
  s32 entryIndex;

  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
               (unitId < 0 || unitId > PIERS)))
              ->list;
  for (entryIndex = 0; entryIndex < list->count; entryIndex++) {
    if (element == list->entries[entryIndex].element &&
        djinnIndex == list->entries[entryIndex].index) {
      list->count--;
      removed = 1;
      break;
    }
  }
  for (; entryIndex < list->count; entryIndex++) {
    list->entries[entryIndex] = list->entries[entryIndex + 1];
  }
  return removed;
}

/// Puts unit unitId's Djinni at the end of its side's list on standby
/// (DJINN_RECOVERY_AFTER_BATTLE), moving it there if it was already listed.
/// Returns a pointer to the list's count. Nothing checks the list's 64-entry
/// capacity. Called when a Djinni joins, by the Djinn menu's standby command,
/// by battle and by the event where Flint joins.
u32 *DjinnRecovery_AddStandby(u32 unitId, u32 element, u32 djinnIndex) {
  struct DjinnRecoveryList *list;
  s32 entryIndex;

  DjinnRecovery_Remove(unitId, element, djinnIndex);
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(unitId > PIERS))
              ->list;
  entryIndex = list->count;
  list->entries[entryIndex].element = element;
  list->entries[entryIndex].index = djinnIndex;
  list->entries[entryIndex].unitId = unitId;
  list->entries[entryIndex].turns = DJINN_RECOVERY_AFTER_BATTLE;
  list->count = entryIndex + 1;
  return (u32 *)&list->count;
}

/// Moves a Djinni from one unit to another, keeping it set or on standby.
/// Returns 0 when it moved, and -1 when the source lacks it or the target
/// has no room or holds it already. Both units' classes are worked out
/// again when it was set. Called by the Djinn menu's Give and Trade.
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target) {
  struct Unit *sourceUnit = Unit_Get(source);
  u32 wasActive;

  if ((sourceUnit->djinnHeld[element] & (1 << djinnIndex)) != 0) {
    wasActive = Djinn_IsSet(source, element, djinnIndex);
    if (Djinn_Give(target, element, djinnIndex) == 0) {
      Djinn_PutOnStandby(source, element, djinnIndex);
      sourceUnit->djinnHeld[element] &= ~(1 << djinnIndex);
      sourceUnit->djinnHeldCounts[element]--;

      if (wasActive != 0) {
        Djinn_Set(target, element, djinnIndex);
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
/// BattleTurn_Build, the battle queue's ordering and the battle menu.
s32 Djinn_GetAbilityId(u32 group, u32 index) {
  return Djinn_GetDefinition(group, index)->abilityId;
}

/// Returns how many Djinn of `element` the party holds, or of all
/// elements for -1. Called by the save summary, the field Status screen, the
/// battle status screen and the battle command menu.
s32 Party_SumDjinnCounts(s32 element) {
  u16 unitIds[16];
  s32 djinnCount = 0;
  s32 partySize = Party_ListMembers(unitIds);

  if (djinnCount < partySize) {
    u16 *unitCursor = unitIds;
    s32 remainingUnits = partySize;

    do {
      struct Unit *unit = Unit_Get(*unitCursor++);

      if (element == -1) {
        djinnCount += unit->djinnHeldCounts[0];
        djinnCount += unit->djinnHeldCounts[1];
        djinnCount += unit->djinnHeldCounts[2];
        djinnCount += unit->djinnHeldCounts[3];
      } else {
        djinnCount += unit->djinnHeldCounts[element];
      }
      remainingUnits--;
    } while (remainingUnits != 0);
  }
  return djinnCount;
}
