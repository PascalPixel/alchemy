#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Summon_FindSlot(void)
{
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 128;
        if (((u8 *)Owner_GetState(id))[298] == 0)
            break;
    }
    if (i == 6)
        return -1;
    return id;
}
