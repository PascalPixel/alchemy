#include "TYPES.H"

struct SummonOwnerState {
    u8 unknown_000[0x14a];
    u16 summon;
};

struct SummonOwnerState *Owner_GetState(s32 owner);
u8 Summon_GetEntryByte4(s32 index);
s32 Summon_IsEntryFlagged(s32 index);

s32 Func_0811c37c(s32 owner)
{
    s32 value = Summon_GetEntryByte4(Owner_GetState(owner)->summon) << 16;

    if (value == 0) {
        if (Summon_IsEntryFlagged(Owner_GetState(owner)->summon) != 0)
            value = 0x180000;
        else
            value = 0x300000;
    }
    return value;
}
