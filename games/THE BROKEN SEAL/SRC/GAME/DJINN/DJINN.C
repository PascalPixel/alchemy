#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFFECT_CHANCE.H"
#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "GAME_FLAGS.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

s32 Owner_GetRecordStride84(s32 arg0);
s32 Owner_GetResistanceValue(s32, s32);
s32 BattleRandomPercent(void);

extern const u16 Djinn_DefinitionTable[];

struct OwnerState_0807a0f4 {
    u8 padding[280];
    u8 values[4];
};

struct OwnerTradeState {
    u8 unknown_000[0xf8];
    u32 owned[4];
    u32 pledged[4];
    u8 owned_counts[4];
    u8 offer_counts[4];
};

struct TradeOffer {
    u8 index;
    u8 bit;
    u8 unknown_02;
    u8 status;
};

struct TradeOfferTable {
    struct TradeOffer offers[64];
    s32 count;
};

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 GameFlag_SetBit(u32 flag);
u8 *Trade_GetOfferState(s32 which);
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

    if (FIELD_AT_OFFSET(owner, u8, 0x129) == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (FIELD_AT_OFFSET(item, u16, 0xE) == 0) {
        return 1;
    }
    rate = ((Equipment_GetUnleashRateBonus((s32)owner) +
         (FIELD_AT_OFFSET(item, u8, 0xB) * 5)) << 0x10) / 100;
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return FIELD_AT_OFFSET(item, u16, 0xE);
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
    s32 offset = 0x129;
    s32 battle_value;

    field = state + offset;
    if (*field == 0) {
        offset--;
        field = state + offset;
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

    offset = 0x129;
    field = state + offset;
    entries = Owner_GetRecordStride84(*field) + 0x50;
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
    u8 *state = Owner_GetState(target);
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
        FIELD_AT_OFFSET(state, s16 *, 0x38) != 0) {
        return 0;
    }

    if (effect_id == 3 && FIELD_AT_OFFSET(state, s8 *, 0x131) == 0) {
        goto fail;
    }

    goto action4_check;
action4_tail:
    if (state[0x13B] == 0 && state[0x13C] == 0 &&
        state[0x13D] == 0 && state[0x141] == 0) {
        goto fail;
    }
    goto action4_done;
action4_check:
    if (effect_id == 4) {
        if (state[0x138] == 0 && state[0x139] == 0 && state[0x13A] == 0) {
            goto action4_tail;
        }
    }

action4_done:
    flag131 = (s8 *)(state + 0x131);
    flag138 = state + 0x138;
    flag139 = state + 0x139;
    flag_13a = state + 0x13A;
    flag_13b = state + 0x13B;
    flag_13c = state + 0x13C;
    if (effect_id == 0x40 &&
        *flag131 == 0 &&
        *flag138 == 0 &&
        *flag139 == 0 &&
        *flag_13a == 0 &&
        *flag_13b == 0 &&
        *flag_13c == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x13D) == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x141) == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x140) == 0) {
        return 0;
    }

    if (effect_id == 0x1C && FIELD_AT_OFFSET(state, u8 *, 0x141) == 1) {
        return 0;
    }

    score = BattleFx_GetBaseSuccessRate(effect_id);
    if (score > 0) {
        s32 difference = Owner_GetResistanceValue(caster, resistance_category) -
            Owner_GetResistanceValue(target, resistance_category) -
            (FIELD_AT_OFFSET(state, u8 *, 0x42) >> 1);
        score += difference * 3;
        if (BattleTarget_IsWeakToEffect(state, effect_id) != 0) {
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

        owners = (u8 *)&gGameState + off * 2;
        count = result;
        do {
            u8 *p = Owner_GetState(*owners);

            if (((struct OwnerState_0807a0f4 *)p)->values[index] <= 9 &&
                (p += 280, 1)) {
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
    struct OwnerBitState *state = Owner_GetState(owner);

    if (state->bit_counts[index] > 9)
        return -1;
    if ((state->bits[index] & (1 << bit)) != 0)
        return -1;
    state->bit_counts[index]++;
    state->bits[index] |= 1 << bit;
    return 0;
}

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct OwnerTradeState *state =
        (struct OwnerTradeState *)Owner_GetState(owner);
    struct TradeOfferTable *table;
    s32 i;
    s32 status;

    if (state->owned_counts[index] == 0)
        return 0;
    if (state->offer_counts[index] > 9) {
        state->offer_counts[index] = 10;
        return 0;
    }
    if ((state->owned[index] & (1 << bit)) == 0)
        return 0;
    if ((state->pledged[index] & (1 << bit)) != 0)
        return 0;

    table = (struct TradeOfferTable *)(Trade_GetOfferState((u32)owner > 7) + 8);
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].index && bit == table->offers[i].bit)
            break;
    }
    if (i == table->count ||
        ((status = (s8)table->offers[i].status) <= 0 && status != -2))
        return 1;
    return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerLearnedState *)Owner_GetState(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)Owner_GetState(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        if (state->available[index] & (1 << bit)) {
            state->active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->active_counts[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)Owner_GetState(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->active_counts[index]--;
        state->active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct TradeOfferTable *table;
    s32 found = 0;
    s32 i;

    table = (struct TradeOfferTable *)(Trade_GetOfferState((u32)owner > 7) + 8);
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].index && bit == table->offers[i].bit) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < table->count; i++) {
        table->offers[i] = table->offers[i + 1];
    }
    return found;
}

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
    u8 *state;
    u8 *entries;
    u8 *entry;
    u32 *count_p;
    u32 count;
    u32 offset;

    Trade_RemoveOffer(kind, first, second);
    state = Trade_GetOfferState(kind > 7);
    entries = state + 8;
    count_p = (u32 *)(state + 0x108);
    count = *count_p;
    offset = count * 4;
    entries[offset] = first;
    count++;
    entry = entries + offset;
    entry[1] = second;
    entry[2] = kind;
    entry[3] = 0xFF;
    *count_p = count;
    return count_p;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerTransferState *state = Owner_GetState(source);
    s32 avail_off = index * 4 + 0xf8;
    u32 mask = 1U << bit;
    u32 present;

    if ((*(u32 *)((u8 *)state + avail_off) & mask) != 0) {
        present = Djinn_IsActive(source, index, bit);
        if (Djinn_AddToOwner(target, index, bit) == 0) {
            Djinn_Deactivate(source, index, bit);
            *(u32 *)((u8 *)state + avail_off) &= ~mask;
            state->owned_counts[index]--;

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
    u8 *base = Trade_GetOfferState(0);
    u8 *entry = base + 8;
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
    if (*((u32 *)(base + 264)) != 0) {
        do {
            if (*(s8 *)(entry + 3) == -1) {
                if (counts != 0)
                    counts[entry[0]]++;
                found++;
            }
            index++;
            entry += 4;
        } while (index != (s32)*((u32 *)(base + 264)));
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
            struct OwnerValueState *state = Owner_GetState(*owner++);

            if (index == -1) {
                result += state->values[0];
                result += state->values[1];
                result += state->values[2];
                result += state->values[3];
            } else {
                result += state->values[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
