/*
 * Draft: Djinn_AddToLeastLoadedOwner does not yet match; 6 bytes differ from +0x1e.
 * Links as recon/tla/raw/080b0ab8.s.
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

    Djinn_AddToOwner(best_no, index, bit);
    Trade_AddOffer((u32)best_no, (u32)index, (u32)bit);
    GameFlag_Set(flag);
    return best_no;
}
