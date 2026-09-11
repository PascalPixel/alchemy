#include "scene.h"
#include "battle_random.h"
#include "types.h"
#include "inventory.h"
#include "item.h"
#include "battle_effect_chance.h"
#include "runtime_interfaces.h"
#include "game_flags.h"
#include "owner_state.h"
#include "party_state.h"

/* battle/random16.c */
/* battle/random_16.c */
u32 BattleRandom16(void)
{
    s32 battle_value;
    battle_value = (*(s32 *)0x020023a8 * 0x41c64e6d) + 0x3039;
    *(s32 *)0x020023a8 = battle_value;
    return (u32)(battle_value << 8) >> 16;
}

/* battle/random_percent.c */
u32 BattleRandomPercent(void);

u32 BattleRandomPercent(void)
{
    return (u32)(BattleRandom16() * 0x64) >> 0x10;
}

/* owner/scale_adjusted_amount.c */
/* owner/scale_adjusted_amount.c */
/* owner/scale_adjusted_amount.c */
s32 Owner_ScaleAdjustedAmount(s32 amount, s32 reduction, s32 adjustment, s32 selector)
{
    s32 result;
    s32 product;

    amount -= reduction;
    result = selector;
    if (amount < 0) {
        amount = 0;
    }
    product = Sys_Apply(result, 1) * (amount + (adjustment * 2));
    if (product < 0) {
        product += 0x1FF;
    }
    result = product >> 9;
    if (result < 0) {
        result = 0;
    }
    return result;
}

/* owner/scale_value_by_curve.c */
s32 Owner_ScaleValueByCurve(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    s32 shifted;
    s32 zero;

    result = (s32)((u32)Sys_Apply(no, 0) *
        (u32)value *(u32)multiplier);
    zero = 0;
    do {
        if (result < zero) {
            result = (s32)((u32)result + 0xFFFF);
        }
        shifted = result >> 0x10;
        return shifted;
    } while (zero);
}

/* owner/scale_value_by_offset_curve.c */
s32 Owner_ScaleValueByOffsetCurve(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    u32 product;

    product = (u32)Sys_Apply(
        (s32)((u32)no * 2 - 0xC8), 0) * (u32)value;
    product = (u32)multiplier *product;
    result = (s32)product;
    if (result < 0) {
        result = (s32)((u32)result + 0xFFFF);
    }
    return result >> 0x10;
}

/* item/get_equipped_element.c */
/* item/get_equipped_element.c */
s32 Owner_GetDefaultElement(void *state);

s32 Item_GetEquippedElement(void)
{
    struct ItemDefinition *item;
    void *owner;

    owner = OwnerState_Get();
    if (FIELD_AT_OFFSET(owner, u8 *, 0x129) == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item != NULL) {
        return FIELD_AT_OFFSET(item, s32 *, 0x14);
    }
    return 4;
}

/* item/get_unleash_rate_bonus.c */
s32 Equipment_GetUnleashRateBonus(s32 owner)
{
    s32 sum;
    s32 offset;
    s32 index;
    u8 *data;
    s32 j;
    s32 mask;
    u16 v;

    sum = 0;
    offset = 216;
    mask = 0x200;
    index = 15;
    while (--index >= 0) {
        v = *(u16 *)((u8 *)offset + owner);
        if (v & mask) {
            data = (u8 *)Item_GetDirect(
                *(u16 *)((u8 *)offset + owner)) + 24;
            j = 4;
            while (--j >= 0) {
                if (data[0] == 23) { sum += (s8)data[1]; }
                data += 4;
            }
        }
        offset += 2;
    }
    if (sum < 0) sum = 0;
    return sum;
}

/* item/roll_weapon_unleash.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);

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
    rate = FixedPoint_Ratio(
        (Equipment_GetUnleashRateBonus((s32)owner) +
         (FIELD_AT_OFFSET(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return FIELD_AT_OFFSET(item, u16, 0xE);
    }
    return 1;
}

/* battle/effects/calc/fx_get_base_success_rate.c */
/* battle/effects/calc/get_base_success_rate.c */
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

/* battle/effects/calc/target_is_weak.c */
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
    entries = Battle_Check(*field) + 0x50;
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

/* battle/effects/calc/is_revive.c */
s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}

/* battle/effects/calc/roll_success.c */
s32 FixedPoint_Ratio(s32, s32);
u8 *Owner_GetState(s32);
s32 Owner_GetResistanceValue(s32, s32);
s32 BattleRandomPercent(void);

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
        if (FixedPoint_Ratio(score *success_scale, 100) >= BattleRandomPercent()) {
            return 1;
        }
    }
fail:
    return 0;
}

/* djinn/get_definition.c */
/* djinn/get_definition.c */
/* djinn/get_definition.c */
extern const u16 gRom[];

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)gRom + entry * 0xC);
}

/* djinn/add_to_least_loaded_owner.c */
struct OwnerState_0807a0f4 {
    u8 padding[280];
    u8 values[4];
};

s32 Djinn_AddToOwner(s32 owner, s32 index, u8 *state);
u32 *Trade_AddOffer(s32 owner, s32 index, u8 *state);

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
    void *entry = state + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 result;
    u8 *owners;

    if (GameFlag_Test(entry)!= 0)
        return -1;

    result = Party_CountActiveOwners();
    if (best_no < result) {
        s32 off = 252;

        owners = (u8 *)&gCell + off * 2;
        count = result;
        do {
            u8 *p = OwnerState_Get(*owners);

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

    Djinn_AddToOwner(best_no, index, state);
    Trade_AddOffer(best_no, index, state);
    GameFlag_Set(entry);
    return best_no;
}

/* djinn/add_to_owner.c */
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

/* trade/can_offer_djinn.c */
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

struct OwnerTradeState *Owner_GetState(s32 owner);
u8 *Trade_GetOfferState(s32 which);

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct OwnerTradeState *state = Owner_GetState(owner);
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

/* djinn/is_active_2.c */
/* djinn/is_active.c */
/* djinn/is_active.c */
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerLearnedState *)Owner_GetState(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

/* djinn/activate.c */
s32 Trade_CanOfferDjinn(s32, s32, s32);
void Owner_RefreshDerivedData(s32 owner);

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)OwnerState_Get(owner);
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

/* djinn/deactivate.c */
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
void Owner_RefreshDerivedData(s32 owner);

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)OwnerState_Get(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->active_counts[index]--;
        state->active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}

/* trade/trade_remove_offer.c */
/* trade/remove_offer.c */
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

u8 *Trade_GetOfferState(s32 which);

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

/* trade/add_offer.c */
s32 Trade_RemoveOffer(s32 kind, s32 first, s32 second);
s32 Trade_GetOfferState(s32 arg0);

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

/* djinn/transfer.c */
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerTransferState *state = Owner_GetState(source);
    /* Retained across calls for the two accesses to available[index]. */
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

/* trade/count_pending_offers.c */
u8 *Trade_GetOfferState(s32);

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
                if (counts != 0) {
                    counts[entry[0]]++;
                }
                found++;
            }
            index++;
            entry += 4;
        } while (index != (s32)*((u32 *)(base + 264)));
    }
    return found;}

/* djinn/get_definition_header.c */
const u16 *Djinn_GetDefinition();

u16 Djinn_GetDefinitionHeader(void)
{
    return *Djinn_GetDefinition();
}

/* party/sum_djinn_counts.c */
s32 Party_SumDjinnCounts(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners(owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct OwnerValueState *state = OwnerState_Get(*owner++);

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

/* inventory/add_and_equip.c */
void Inventory_AddAndEquip(s32 owner, s32 target)
{
    s32 state = (s32)OwnerState_Get(owner);
    u16 *entry;
    s32 index;

    Inventory_Add(owner, target);
    index = 0;
    entry = (u16 *)(state + 0xd8);
    do {
        if (*entry++ == target)
            Inventory_Equip(owner, index);
        index++;
    } while (index <= 14);
}
