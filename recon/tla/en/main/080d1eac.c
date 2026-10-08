#include "types.h"
#include "party_state.h"

struct BattleFxDefinition {
    u16 id;
    u8 value;
    u8 flags;
};

struct BattleFxDefinition *Func_080d1e60(s32 id);

s32 Func_080d1eac(s32 id)
{
    u8 value;

    if (gPartyState.battle_fx_enabled == 0 || (value = Func_080d1e60(id)->value) == 0xff)
        return 0;
    return value + 0x100;
}
