#include "OWNER_STATE.H"

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerDjinnState *)Owner_GetState(owner))->flags.banks.active[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}
