#include "OWNER_STATE.H"

u32 *Trade_AddOffer(u32 owner, u32 element, u32 djinn)
{
    struct TradeOfferState *state;
    u8 *entries;
    struct TradeOffer *entry;
    u32 *count_p;
    u32 count;
    u32 offset;

    Trade_RemoveOffer(owner, element, djinn);
    state = Trade_GetOfferState(owner > 7);
    entries = (u8 *)state->offers;
    count_p = &state->count;
    count = *count_p;
    /* FAKEMATCH: five typed append forms and a pointer handoff changed the
       first store or its registers. Byte strides preserve the 60-byte order. */
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

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerDjinnState *state = Owner_GetState(source);
    u32 mask;
    u32 present;

    if ((state->flags.banks.owned[index] & (mask = 1U << bit)) != 0) {
        present = Djinn_IsActive(source, index, bit);
        if (Djinn_AddToOwner(target, index, bit) == 0) {
            Djinn_Deactivate(source, index, bit);
            state->flags.banks.owned[index] &= ~mask;
            state->counts.banks.owned[index]--;

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
    struct TradeOfferState *state = Trade_GetOfferState(0);
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
        } while (index != (s32)state->count);
    }
    return found;
}

u16 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
    return *Djinn_GetDefinition(group, index);
}
