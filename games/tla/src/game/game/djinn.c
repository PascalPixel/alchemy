/// @file
/// Djinn: definitions, setting and standby, and their counts.
// Part of game.c.

#include "../game_internal.h"
#include "module/game/game.h"

const u16 *Djinn_GetDefinition(u32 group, u32 index) {
  s32 entry;

  entry = 0;
  if (group <= 3 && index <= 19) {
    entry = group * 20 + index;
  }
  return (const u16 *)&gDjinn_DefinitionTable[entry];
}

/// Gives a newly found Djinni to the party member holding the fewest Djinn who
/// has room in that element, puts it on standby and sets its found flag.
/// Returns who got it, -1 if it was found before or -2 when nobody has room.
/// Called by Djinn_JoinParty when a Djinni joins in the field. Of party members
/// holding equally few, the earliest in the party order gets it.
s32 Djinn_GiveToParty(s32 index, s32 bit) {
  s32 flag = bit + index * 20 + 48;
  s32 bestNo = 0;
  s32 bestVal = 999;
  s32 count;
  s32 result;
  u8 *owners;

  if (GameFlag_Get(flag) != 0)
    return -1;

  result = Party_CountMembers();
  if (bestNo < result) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    owners = party->activeOwners;
    count = result;
    do {
      struct OwnerDjinnState *state = Unit_Get(*owners);

      if (state->counts.banks.owned[index] <= 9) {
        u8 *p = state->counts.banks.owned;
        s32 value = 0;
        s32 i = 3;

        do {
          u8 byte = *p;
          p++;
          value += byte;
          i--;
        } while (i >= 0);

        if (bestVal > value) {
          bestVal = value;
          bestNo = *owners;
        }
      }
      count--;
      owners++;
    } while (count != 0);
  }

  if (bestVal == 999)
    return -2;

  Djinn_Give(bestNo, index, bit);
  DjinnRecovery_AddStandby(bestNo, index, bit);
  GameFlag_SetBit__game(flag);
  return bestNo;
}

/// Gives Djinni `djinnIndex` of `element` to a unit: sets its bit in
/// djinnHeld and counts it. Returns -1 when the unit holds ten of the
/// element or has it already. Called by Djinn_GiveToParty and
/// Djinn_Transfer, by the events where a party member joins with Djinn, and by
/// the debug menus.
s32 Djinn_Give(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = Unit_Get(owner);
  u32 owned;

  if (state->counts.slots[index] > 9)
    return -1;
  owned = state->flags.slots[index];
  if ((owned & (1 << bit)) != 0)
    return -1;
  state->counts.slots[index]++;
  state->flags.slots[index] |= 1 << bit;
  return 0;
}

/// Whether the djinn can be set: it is owned, fewer than ten are set, it is
/// not set already, and no trade offer holds it unless the offer has run out.
s32 Djinn_CanSet(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = Unit_Get(owner);
  struct TradeOfferList *table;
  s32 i;
  s32 status;
  u32 owned;

  if (state->counts.banks.owned[index] == 0)
    return 0;
  if (state->counts.banks.active[index] > 9) {
    state->counts.banks.active[index] = 10;
    return 0;
  }
  owned = state->flags.banks.owned[index];
  if ((owned & (1 << bit)) == 0)
    return 0;
  if ((state->flags.banks.active[index] & (1 << bit)) != 0)
    return 0;

  table = &((struct TradeOfferTable *)Djinn_GetRecoveryTable(
                (owner < 0 || owner > 7)))
               ->list;
  for (i = 0; i < table->count; i++) {
    if (index == table->offers[i].element && bit == table->offers[i].djinn)
      break;
  }
  if (i == table->count ||
      ((status = table->offers[i].status) <= 0 && status != -2))
    return 1;
  return 0;
}

/// Returns 1 when the Djinni is set on the unit.
u32 Djinn_IsSet(s32 owner, s32 index, s32 bit) {
  s32 value =
      ((struct OwnerDjinnState *)Unit_Get(owner))->flags.banks.active[index] &
      (1 << bit);

  return (u32)(-value | value) >> 31;
}

/// Sets a ready Djinni on the unit, counts it and works out the unit's class
/// again. Returns 1 when it was set. Called by the Djinn menu, by battle when
/// a Djinni recovers, and by Djinn_Transfer.
s32 Djinn_Set(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = Unit_Get(owner);
  s32 result = Djinn_CanSet(owner, index, bit);

  if (result != 0) {
    u32 owned = state->flags.banks.owned[index];
    if (owned & (1 << bit)) {
      state->flags.banks.active[index] |= 1 << bit;
    } else {
      return 0;
    }
    state->counts.banks.active[index]++;
    Unit_UpdateClass(owner);
  }
  return result;
}

/// Takes a set Djinni off the unit and works out its class again. Returns 1
/// when it was set. Called by the Djinn menu, by battle when a Djinni is
/// unleashed, and by Djinn_Transfer.
u32 Djinn_PutOnStandby(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = Unit_Get(owner);
  u32 present = Djinn_IsSet(owner, index, bit);

  if (present != 0) {
    state->counts.banks.active[index]--;
    state->flags.banks.active[index] &= ~(1 << bit);
    Unit_UpdateClass(owner);
  }
  return present;
}

s32 DjinnRecovery_Remove(s32 owner, s32 index, s32 bit) {
  struct TradeOfferList *table;
  s32 found = 0;
  s32 i;

  table = &((struct TradeOfferTable *)Djinn_GetRecoveryTable(
                (owner < 0 || owner > 7)))
               ->list;
  for (i = 0; i < table->count; i++) {
    if (index == table->offers[i].element && bit == table->offers[i].djinn) {
      table->count--;
      found = 1;
      break;
    }
  }
  for (; i < table->count; i++)
    table->offers[i] = table->offers[i + 1];
  return found;
}

u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinn) {
  struct TradeOfferState *state;
  u8 *entries;
  struct TradeOffer *entry;
  u32 *count_p;
  u32 count;
  u32 offset;

  DjinnRecovery_Remove(owner, element, djinn);
  state = Djinn_GetRecoveryTable(owner > 7);
  entries = (u8 *)state->offers;
  count_p = &state->count;
  count = *count_p;
  /* FAKEMATCH(temporary): the entry's offset as a byte stride of its own, its
   * first field stored through it before the entry pointer is formed, which
   * keeps the first store's order and registers; as the fields stored through
   * &state->offers[count], 29 of 27 instructions differ in tla-en (the
   * draft has 26 and shares 12). */
  offset = count * sizeof(struct TradeOffer);
  entries[offset] = element;
  count++;
  entry = (struct TradeOffer *)(entries + offset);
  entry->djinn = djinn;
  entry->owner = owner;
  entry->status = TRADE_OFFER_PENDING;
  *count_p = count;
  return count_p;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target) {
  struct OwnerDjinnState *state = Unit_Get(source);
  u32 mask;
  u32 present;

  if ((state->flags.banks.owned[index] & (mask = 1U << bit)) != 0) {
    present = Djinn_IsSet(source, index, bit);
    if (Djinn_Give(target, index, bit) == 0) {
      Djinn_PutOnStandby(source, index, bit);
      state->flags.banks.owned[index] &= ~mask;
      state->counts.banks.owned[index]--;

      if (present != 0) {
        Djinn_Set(target, index, bit);
      } else {
        DjinnRecovery_Remove(source, index, bit);
        DjinnRecovery_AddStandby(target, index, bit);
      }
      return 0;
    }
  }
  return -1;
}

s32 DjinnRecovery_CountStandby(u8 *counts) {
  s32 found = 0;
  struct TradeOfferState *state = Djinn_GetRecoveryTable(0);
  struct TradeOffer *entry = state->offers;
  s32 index;

  if (counts != 0) {
    u8 *slot;

    slot = counts + 3;
    *slot = found;
    slot = counts + 2;
    *slot = found;
    slot = counts + 1;
    *slot = found;
    counts[0] = found;
  }
  index = 0;
  if (state->count != 0) {
    do {
      if (entry->status == TRADE_OFFER_PENDING) {
        if (counts != 0)
          counts[entry->element]++;
        found++;
      }
      index++;
      entry++;
    } while (index != state->count);
  }
  return found;
}

u16 Djinn_GetDefinitionHeader(u32 group, u32 index) {
  return *Djinn_GetDefinition(group, index);
}

s32 Owner_SumDjinnCounts(s32 owner, s32 index) {
  struct OwnerDjinnState *state;
  s32 result;

  state = Unit_Get(owner);
  if (index == -1) {
    result = state->counts.banks.owned[0];
    result += state->counts.banks.owned[1];
    result += state->counts.banks.owned[2];
    result += state->counts.banks.owned[3];
  } else {
    result = state->counts.banks.owned[index];
  }
  return result;
}

s32 Party_SumDjinnCounts(s32 index) {
  u16 owners[16];
  s32 result = 0;
  s32 count = Party_ListMembers((s16 *)owners);

  if (result < count) {
    u16 *owner = owners;
    s32 remaining = count;

    do {
      struct OwnerDjinnState *state = Unit_Get(*owner++);

      if (index == -1) {
        result += state->counts.banks.owned[0];
        result += state->counts.banks.owned[1];
        result += state->counts.banks.owned[2];
        result += state->counts.banks.owned[3];
      } else {
        result += state->counts.banks.owned[index];
      }
      remaining--;
    } while (remaining != 0);
  }
  return result;
}

void Djinn_ActivateOfferedForParty(void) {
  u16 owners[16];
  s32 count = Party_ListMembers((s16 *)owners);
  s32 i;
  s32 element;
  s32 djinn;

  DjinnRecovery_CountDownTwentyFar();
  for (i = 0; i < count; i++) {
    for (element = 0; element <= 3; element++) {
      for (djinn = 0; djinn <= 19; djinn++) {
        if (Djinn_CanSet(owners[i], element, djinn)) {
          Djinn_Set(owners[i], element, djinn);
          DjinnRecovery_Remove(owners[i], element, djinn);
        }
      }
    }
  }
}
