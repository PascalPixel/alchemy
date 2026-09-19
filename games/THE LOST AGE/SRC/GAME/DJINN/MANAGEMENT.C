#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32);
u32 GameFlag_Set(s32);
u8 *Trade_GetOfferState(s32);
void Owner_RefreshDerivedData(s32);

extern const u16 Djinn_Definitions[];

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
    struct TradeOffer offers[72];
    s32 count;
};

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);

u8 *Trade_GetOfferState(s32 which);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)Djinn_Definitions + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, s32 bit)
{
    s32 flag = bit + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 result;
    u8 *owners;

    if (GameFlag_Test(flag) != 0)
        return -1;

    result = Party_CountActiveOwners();
    if (best_no < result) {
        s32 off = 268;

        owners = (u8 *)&gPartyState + off * 2;
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

    Djinn_AddToOwner(best_no, index, bit);
    Trade_AddOffer((u32)best_no, (u32)index, (u32)bit);
    GameFlag_Set(flag);
    return best_no;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
    struct OwnerBitState *state = OwnerState_Get(owner);

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
        (struct OwnerTradeState *)OwnerState_Get(owner);
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
        ((struct OwnerLearnedState *)OwnerState_Get(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

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
    count_p = (u32 *)(state + 0x128);
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

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerTransferState *state = OwnerState_Get(source);
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
    if (*((u32 *)(base + 296)) != 0) {
        do {
            if (*(s8 *)(entry + 3) == -1) {
                if (counts != 0)
                    counts[entry[0]]++;
                found++;
            }
            index++;
            entry += 4;
        } while (index != (s32)*((u32 *)(base + 296)));
    }
    return found;
}

const u16 *Djinn_GetDefinition(u32 group, u32 index);

u16 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
    return *Djinn_GetDefinition(group, index);
}
