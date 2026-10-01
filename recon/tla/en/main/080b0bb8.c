/* Near miss: score 160 with temporaries for the owned and pledged words. ⚓️
   sets the 0 result between loading the pledged word and testing its bit;
   this draft after with the approved game flags. */
/*
 * Draft: Trade_CanOfferDjinn does not yet match; 8 bytes differ from +0x30.
 * Links as recon/tla/raw/080b0bb8.s.
 */
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

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct OwnerTradeState *state = (struct OwnerTradeState *)Owner_GetState(owner);
    struct TradeOfferTable *table;
    s32 i;
    s32 status;
    u32 owned;
    u32 pledged;

    if (state->owned_counts[index] == 0)
        return 0;
    if (state->offer_counts[index] > 9) {
        state->offer_counts[index] = 10;
        return 0;
    }
    owned = state->owned[index];
    if ((owned & (1 << bit)) == 0)
        return 0;
    pledged = state->pledged[index];
    if ((pledged & (1 << bit)) != 0)
        return 0;
    table = (struct TradeOfferTable *)(Trade_GetOfferState((u32)owner > 7) + 8);
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].index && bit == table->offers[i].bit)
            break;
    }
    if (i == table->count || ((status = (s8)table->offers[i].status) <= 0 && status != -2))
        return 1;
    return 0;
}
