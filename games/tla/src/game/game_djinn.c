/// @file
/// Djinn: definitions, setting and standby, and their counts.
// Part of game.c.

#include "game_internal.h"
#include "game/game.h"

/// The most Djinn of one element a party member holds or sets.
#define DJINN_HELD_PER_ELEMENT_MAX 10
/// Above any party member's count of Djinn: the fewest found so far before the
/// search, and still when nobody has room.
#define DJINN_COUNT_NONE 999

/// Returns the definition of Djinni `djinn` of `element`; anything out of
/// range gives DJINNI_FLINT's.
const struct DjinnDefinition *Djinn_GetDefinition(u32 element, u32 djinn) {
  s32 entry = 0;

  if (element < ELEMENT_COUNT && djinn < DJINN_PER_ELEMENT)
    entry = element * DJINN_PER_ELEMENT + djinn;
  return &gDjinn_DefinitionTable[entry];
}

/// Gives a newly found Djinni to the party member holding the fewest Djinn who
/// has room, on standby. Returns who got it, -1 if found before, -2 when nobody
/// has room.
s32 Djinn_GiveToParty(s32 element, s32 djinn) {
  s32 flag = djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST;
  s32 leastLoadedMember = 0;
  s32 fewestDjinn = DJINN_COUNT_NONE;
  s32 partySize;
  s32 partyIndex;
  s32 elementIndex;

  if (GameFlag_Get(flag) != 0)
    return -1;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct Unit *unit = Unit_Get(gMainRoutineState.party[partyIndex]);

    if (unit->djinnHeldCounts[element] < DJINN_HELD_PER_ELEMENT_MAX) {
      s32 ownedDjinn = 0;

      for (elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++)
        ownedDjinn += unit->djinnHeldCounts[elementIndex];
      if (fewestDjinn > ownedDjinn) {
        fewestDjinn = ownedDjinn;
        leastLoadedMember = gMainRoutineState.party[partyIndex];
      }
    }
  }

  if (fewestDjinn == DJINN_COUNT_NONE)
    return -2;

  Djinn_Give(leastLoadedMember, element, djinn);
  DjinnRecovery_AddStandby(leastLoadedMember, element, djinn);
  GameFlag_Set(flag);
  return leastLoadedMember;
}

/// Gives a Djinni to a unit. Returns -1 when the unit holds
/// DJINN_HELD_PER_ELEMENT_MAX of the element or has it already.
s32 Djinn_Give(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->djinnHeldCounts[element] >= DJINN_HELD_PER_ELEMENT_MAX)
    return -1;
  if ((unit->djinnHeld[element] & (1 << djinnIndex)) != 0)
    return -1;
  unit->djinnHeldCounts[element]++;
  unit->djinnHeld[element] |= 1 << djinnIndex;
  return 0;
}

/// Returns 1 when the unit holds the Djinni and it can be set: not set, not
/// recovering and not held by a summon.
s32 Djinn_CanSet(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);
  struct DjinnRecoveryList *list;
  s32 entryIndex;
  s32 recoveryTurns;

  if (unit->djinnHeldCounts[element] == 0)
    return 0;
  if (unit->djinnSetCounts[element] >= DJINN_HELD_PER_ELEMENT_MAX) {
    unit->djinnSetCounts[element] = DJINN_HELD_PER_ELEMENT_MAX;
    return 0;
  }
  if ((unit->djinnHeld[element] & (1 << djinnIndex)) == 0)
    return 0;
  if ((unit->djinnSet[element] & (1 << djinnIndex)) != 0)
    return 0;

  list = &Djinn_GetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
  for (entryIndex = 0; entryIndex < list->count; entryIndex++) {
    if (element == list->entries[entryIndex].element &&
        djinnIndex == list->entries[entryIndex].index)
      break;
  }
  // Ready when not listed, on standby or recovered.
  if (entryIndex == list->count ||
      ((recoveryTurns = list->entries[entryIndex].turns) <= 0 &&
       recoveryTurns != DJINN_RECOVERY_SUMMONED))
    return 1;
  return 0;
}

/// Returns 1 when the Djinni is set on the unit.
u32 Djinn_IsSet(s32 unitId, s32 element, s32 djinnIndex) {
  s32 set = Unit_Get(unitId)->djinnSet[element] & (1 << djinnIndex);

  return NONZERO(set);
}

/// Sets a ready Djinni on the unit, counts it and works out the unit's class
/// again. Returns 1 when it was set.
s32 Djinn_Set(s32 unitId, s32 element, s32 djinnIndex) {
  struct Unit *unit = Unit_Get(unitId);
  s32 isReady = Djinn_CanSet(unitId, element, djinnIndex);

  if (isReady != 0) {
    if (unit->djinnHeld[element] & (1 << djinnIndex))
      unit->djinnSet[element] |= 1 << djinnIndex;
    else
      return 0;
    unit->djinnSetCounts[element]++;
    Unit_UpdateClass(unitId);
  }
  return isReady;
}

/// Takes a set Djinni off the unit and works out its class again. Returns 1
/// when it was set.
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

/// Takes the Djinni off its side's standby and recovery list, closing the gap.
/// Returns 1 when it was there.
s32 DjinnRecovery_Remove(s32 unitId, s32 element, s32 djinnIndex) {
  struct DjinnRecoveryList *list;
  s32 removed = 0;
  s32 entryIndex;

  list = &Djinn_GetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
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

/// Puts a unit's Djinni at the end of its side's list on standby. Nothing
/// checks the list's capacity.
u32 *DjinnRecovery_AddStandby(u32 unitId, u32 element, u32 djinnIndex) {
  struct DjinnRecoveryList *list;
  s32 count;

  DjinnRecovery_Remove(unitId, element, djinnIndex);
  list = &Djinn_GetRecoveryTable(unitId > PIERS)->list;
  count = list->count;
  list->entries[count].element = element;
  list->entries[count].index = djinnIndex;
  list->entries[count].unitId = unitId;
  list->entries[count].turns = DJINN_RECOVERY_AFTER_BATTLE;
  list->count = count + 1;
  return (u32 *)&list->count;
}

/// Moves a Djinni from one unit to another, set or on standby as it was.
/// Returns -1 when it cannot.
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

/// Counts the party's Djinn on standby, and by element into `counts` when it
/// is given.
s32 DjinnRecovery_CountStandby(u8 *counts) {
  s32 standbyCount = 0;
  struct DjinnRecoveryTable *recoveryTable =
      Djinn_GetRecoveryTable(DJINN_RECOVERY_PARTY);
  struct DjinnRecoveryEntry *entry = recoveryTable->list.entries;
  s32 entryIndex;

  if (counts != 0) {
    s32 element;

    // Each element's count starts at 0.
    for (element = 0; element < ELEMENT_COUNT; element++)
      counts[element] = 0;
  }
  for (entryIndex = 0; entryIndex != recoveryTable->list.count; entryIndex++) {
    if (entry->turns == DJINN_RECOVERY_AFTER_BATTLE) {
      if (counts != 0)
        counts[entry->element]++;
      standbyCount++;
    }
    entry++;
  }
  return standbyCount;
}

/// Returns the ability the Djinni performs when it is unleashed.
u16 Djinn_GetAbilityId(u32 element, u32 djinn) {
  return Djinn_GetDefinition(element, djinn)->abilityId;
}

/// Returns how many Djinn of `element` the unit holds, or of every element for
/// -1.
s32 Owner_SumDjinnCounts(s32 unitId, s32 element) {
  struct Unit *unit;
  s32 result;

  unit = Unit_Get(unitId);
  if (element == -1) {
    result = unit->djinnHeldCounts[0];
    result += unit->djinnHeldCounts[1];
    result += unit->djinnHeldCounts[2];
    result += unit->djinnHeldCounts[3];
  } else {
    result = unit->djinnHeldCounts[element];
  }
  return result;
}

/// Returns how many Djinn of `element` the party holds, or of all elements for
/// -1.
s32 Party_SumDjinnCounts(s32 element) {
  u16 unitIds[16];
  s32 djinnCount = 0;
  s32 partySize = Party_ListMembers(unitIds);
  s32 i;

  for (i = 0; i < partySize; i++) {
    struct Unit *unit = Unit_Get(unitIds[i]);

    if (element == -1) {
      djinnCount += unit->djinnHeldCounts[0];
      djinnCount += unit->djinnHeldCounts[1];
      djinnCount += unit->djinnHeldCounts[2];
      djinnCount += unit->djinnHeldCounts[3];
    } else {
      djinnCount += unit->djinnHeldCounts[element];
    }
  }
  return djinnCount;
}

/// Counts the Djinn recovery down, then sets every Djinni each party member
/// holds and can set, taking it off the recovery list.
void Djinn_SetAllForParty(void) {
  u16 owners[16];
  s32 count = Party_ListMembers((s16 *)owners);
  s32 i;
  s32 element;
  s32 djinn;

  DjinnRecovery_CountDownTwentyFar();
  for (i = 0; i < count; i++) {
    for (element = 0; element < ELEMENT_COUNT; element++) {
      for (djinn = 0; djinn < DJINN_PER_ELEMENT; djinn++) {
        if (Djinn_CanSet(owners[i], element, djinn)) {
          Djinn_Set(owners[i], element, djinn);
          DjinnRecovery_Remove(owners[i], element, djinn);
        }
      }
    }
  }
}
