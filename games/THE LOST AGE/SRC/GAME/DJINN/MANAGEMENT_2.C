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

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerLearnedState *)Owner_GetState(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}
