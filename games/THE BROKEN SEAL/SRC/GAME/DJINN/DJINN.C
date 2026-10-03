#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFFECT_CHANCE.H"
#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "GAME_FLAGS.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
#include "CHARACTER.H"
#include "BATTLE_PARTY.H"

s32 Owner_GetResistanceValue(s32, s32);
s32 BattleRandomPercent(void);

extern const u16 Djinn_DefinitionTable[];

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 GameFlag_SetBit(u32 flag);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
s32 Trade_CanOfferDjinn(s32, s32, s32);
void Owner_RefreshDerivedData(s32 owner);
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
const u16 *Djinn_GetDefinition(u32 group, u32 index);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u16 RollWeaponUnleash(void *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (((struct BattleUnit *)owner)->class_index == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (item->description_message == 0) {
        return 1;
    }
    rate = ((Equipment_GetUnleashRateBonus((s32)owner) +
         (item->secondary_flags * 5)) << 0x10) / 100;
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return item->description_message;
    }
    return 1;
}

#undef FIELD_AT_OFFSET

s32 BattleFx_GetBaseSuccessRate(s32 effect_id)
{
    s32 battle_result;
    u32 entry_index;

    entry_index = effect_id - 8;
    switch (entry_index) {
    case 4:
    case 5:
        return 0x46;
    case 8:
    case 9:
        return 0x4B;
    case 14:
        return 0x1E;
    case 15:
        return 0x28;
    case 16:
        return 0x2D;
    case 10:
    case 11:
    case 17:
        return 0x37;
    case 18:
        return 0x19;
    case 19:
        return 0x14;
    case 12:
    case 23:
        return 0x41;
    case 13:
    case 26:
        return 0x23;
    case 27:
        return 0x32;
    case 48:
        battle_result = 0x3C;
        goto block_18;
    case 49:
        battle_result = 0x5A;
        goto block_18;
    case 0:
    case 1:
    case 20:
    case 24:
        return 0x3C;
    default:
        battle_result = 0x64;
        break;
    }
block_18:
    return 0 - battle_result;
}

s32 BattleTarget_IsWeakToEffect(const u8 *state, s32 effect_id)
{
    u8 *entries;
    const u8 *field;
    s32 entry_index;
    const struct BattleUnit *unit = (const struct BattleUnit *)state;
    s32 battle_value;

    field = &unit->class_index;
    if (*field == 0) {
        field = &unit->class_id;
        entries = (u8 *)Owner_GetRecord(*field) + 0x48;
        entry_index = 0;
first_loop:
        if (*entries != effect_id) {
            entry_index++;
            entries++;
            if (entry_index > 2) {
                goto not_found;
            }
            goto first_loop;
        }
        goto found;
    }

    field = &unit->class_index;
    entries = (u8 *)Owner_GetRecordStride84(*field) + 0x50;
    entry_index = 0;
second_loop:
    battle_value = *entries++;
    if (battle_value == effect_id) {
found:
        return 1;
    }
    entry_index++;
    if (entry_index > 2) {
not_found:
        return 0;
    }
    goto second_loop;
}

s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 BattleFx_RollSuccess(
    s32 caster,
    s32 target,
    s32 resistance_category,
    s32 effect_id,
    s32 success_scale) {
    struct BattleUnit *state = Owner_GetState(target);
    s32 attempts = 1;
    s32 score;
    s32 attempt;
    s8 *flag131;
    u8 *flag138;
    u8 *flag139;
    u8 *flag_13a;
    u8 *flag_13b;
    u8 *flag_13c;

    if (BattleFx_IsRevive(effect_id)!= 0 &&
        state->hp != 0) {
        return 0;
    }

    if (effect_id == 3 && state->poison == 0) {
        goto fail;
    }

    goto action4_check;
action4_tail:
    if (state->stun == 0 && state->sleep == 0 &&
        state->psy_seal == 0 && state->death_count == 0) {
        goto fail;
    }
    goto action4_done;
action4_check:
    if (effect_id == 4) {
        if (state->delusion == 0 && (u8)state->confusion == 0 && state->charm == 0) {
            goto action4_tail;
        }
    }

action4_done:
    flag131 = &state->poison;
    flag138 = (u8 *)&state->delusion;
    flag139 = (u8 *)&state->confusion;
    flag_13a = (u8 *)&state->charm;
    flag_13b = (u8 *)&state->stun;
    flag_13c = (u8 *)&state->sleep;
    if (effect_id == 0x40 &&
        *flag131 == 0 &&
        *flag138 == 0 &&
        *flag139 == 0 &&
        *flag_13a == 0 &&
        *flag_13b == 0 &&
        *flag_13c == 0 &&
        state->psy_seal == 0 &&
        state->death_count == 0 &&
        state->evil_spirit == 0) {
        return 0;
    }

    if (effect_id == 0x1C && state->death_count == 1) {
        return 0;
    }

    score = BattleFx_GetBaseSuccessRate(effect_id);
    if (score > 0) {
        s32 difference = Owner_GetResistanceValue(caster, resistance_category) -
            Owner_GetResistanceValue(target, resistance_category) -
            (state->luck >> 1);
        score += difference * 3;
        if (BattleTarget_IsWeakToEffect((const u8 *)state, effect_id) != 0) {
            score += 25;
        }
    } else {
        score = -score;
    }

    if (effect_id == 0x43) {
        attempts = 3;
    }

    for (attempt = 0; attempt < attempts; attempt++) {
        if (score *success_scale / 100 >= BattleRandomPercent()) {
            return 1;
        }
    }
fail:
    return 0;
}

#undef FIELD_AT_OFFSET

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)Djinn_DefinitionTable + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
    void *entry = state + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 result;
    u8 *owners;

    if (GameFlag_Test(entry) != 0)
        return -1;

    result = Party_CountActiveOwners();
    if (best_no < result) {
        s32 off = 252;

        owners = gGameState.active_owners;
        count = result;
        do {
            struct BattleUnit *unit = Owner_GetState(*owners);
            u8 *p = unit->djinn_owned_counts;

            if (unit->djinn_owned_counts[index] <= 9) {
                s32 value = 0;
                s32 i = 3;

                do {
                    u8 byte = *p;
                    p++;
                    value += byte;
                    i--;
                } while (i >= 0);

                if (best_val > value) {
                    best_val = value;
                    best_no = *owners;
                }
            }
            count--;
            owners++;
        } while (count != 0);
    }

    if (best_val == 999)
        return -2;

    Djinn_AddToOwner(best_no, index, (s32)(u32)state);
    Trade_AddOffer((u32)best_no, (u32)index, (u32)state);
    GameFlag_SetBit((u32)entry);
    return best_no;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
    struct BattleUnit *state = Owner_GetState(owner);

    if (state->djinn_owned_counts[index] > 9)
        return -1;
    if ((state->djinn_available[index] & (1 << bit)) != 0)
        return -1;
    state->djinn_owned_counts[index]++;
    state->djinn_available[index] |= 1 << bit;
    return 0;
}

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct BattleUnit *state =
        (struct BattleUnit *)Owner_GetState(owner);
    struct DjinnRecoveryList *table;
    s32 i;
    s32 status;

    if (state->djinn_owned_counts[index] == 0)
        return 0;
    if (state->djinn_active_counts[index] > 9) {
        state->djinn_active_counts[index] = 10;
        return 0;
    }
    if ((state->djinn_available[index] & (1 << bit)) == 0)
        return 0;
    if ((state->djinn_active[index] & (1 << bit)) != 0)
        return 0;

    table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((u32)owner > 7))->list;
    for (i = 0; i < table->count; i++) {
        if (index == table->entries[i].element && bit == table->entries[i].index)
            break;
    }
    if (i == table->count ||
        ((status = (s8)table->entries[i].turns) <= 0 && status != -2))
        return 1;
    return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct BattleUnit *)Owner_GetState(owner))->djinn_active[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct BattleUnit *state =
        (struct BattleUnit *)Owner_GetState(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        if (state->djinn_available[index] & (1 << bit)) {
            state->djinn_active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->djinn_active_counts[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct BattleUnit *state =
        (struct BattleUnit *)Owner_GetState(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->djinn_active_counts[index]--;
        state->djinn_active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct DjinnRecoveryList *table;
    s32 found = 0;
    s32 i;

    table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((u32)owner > 7))->list;
    for (i = 0; i < table->count; i++) {
        if (index == table->entries[i].element && bit == table->entries[i].index) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < table->count; i++) {
        table->entries[i] = table->entries[i + 1];
    }
    return found;
}

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
    struct DjinnRecoveryTable *state;
    struct DjinnRecoveryEntry *entries;
    struct DjinnRecoveryEntry *entry;
    u32 *count_p;
    u32 count;
    u32 offset;

    Trade_RemoveOffer(kind, first, second);
    state = Trade_GetOfferState(kind > 7);
    entries = state->list.entries;
    count_p = (u32 *)&state->list.count;
    count = *count_p;
    offset = count;
    entries[offset].element = first;
    count++;
    entry = entries + offset;
    entry->index = second;
    entry->unit_id = kind;
    entry->turns = 0xFF;
    *count_p = count;
    return count_p;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct BattleUnit *state = Owner_GetState(source);
    u32 mask = 1U << bit;
    u32 present;

    if ((state->djinn_available[index] & mask) != 0) {
        present = Djinn_IsActive(source, index, bit);
        if (Djinn_AddToOwner(target, index, bit) == 0) {
            Djinn_Deactivate(source, index, bit);
            state->djinn_available[index] &= ~mask;
            state->djinn_owned_counts[index]--;

            if (present != 0) {
                Djinn_Activate(target, index, bit);
            } else {
                Trade_RemoveOffer(source, index, bit);
                Trade_AddOffer(target, index, bit);
            }
            return 0;
        }
    }
    return -1;
}

s32 Trade_CountPendingOffers(u8 *counts)
{
    s32 found = 0;
    struct DjinnRecoveryTable *base = Trade_GetOfferState(0);
    struct DjinnRecoveryEntry *entry = base->list.entries;
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
    if (*(u32 *)&base->list.count != 0) {
        do {
            if (entry->turns == -1) {
                if (counts != 0)
                    counts[entry->element]++;
                found++;
            }
            index++;
            entry++;
        } while (index != (s32)*(u32 *)&base->list.count);
    }
    return found;
}

u16 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
    return *Djinn_GetDefinition(group, index);
}

s32 Party_SumDjinnCounts(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners((s16 *)owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct BattleUnit *state = Owner_GetState(*owner++);

            if (index == -1) {
                result += state->djinn_owned_counts[0];
                result += state->djinn_owned_counts[1];
                result += state->djinn_owned_counts[2];
                result += state->djinn_owned_counts[3];
            } else {
                result += state->djinn_owned_counts[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
