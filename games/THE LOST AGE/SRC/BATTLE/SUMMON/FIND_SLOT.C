#include "TYPES.H"
#include "OWNER_STATE.H"

/* ☀️'s, but ⚓️ reaches the owner's state with a direct call. */
s32 Summon_FindSlot(void)
{
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 0x80;
        if (((u8 *)Owner_GetState(id))[0x12A] == 0)
            break;
    }
    if (i == 6)
        return -1;
    return id;
}
