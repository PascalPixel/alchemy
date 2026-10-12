#include "types.h"
#include "party_state.h"

struct BattleFxDefinition {
    u16 id;
    u8 value;
    u8 flags;
};

struct BattleFxDefinition *Func_080d1e60(s32 id);

s32 BattleFx_GetAnimationValue(void);

s32 Func_080d1eac(s32 id)
{
    struct PartyState *party = &gPartyState;
    u8 value;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    if (party->battle_fx_enabled == 0 || (value = Func_080d1e60(id)->value) == 0xff)
        return 0;
    return value + 0x100;
}

u8 BattleFx_GetFlags(void)
{
    return Func_080d1e60(BattleFx_GetAnimationValue())->flags;
}
