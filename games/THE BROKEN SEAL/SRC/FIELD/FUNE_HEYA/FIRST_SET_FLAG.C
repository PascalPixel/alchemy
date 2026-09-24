#include "TYPES.H"

s32 Engine_GameFlagIsSet();

/* Return the actor id (from 8, or 18 for the alternate set) of the first of
 * nine consecutive flags of a group that is set, or 0 when none is. */
s32 FuneHeya_FindFirstSetFlag(u32 group, s32 alternate)
{
    s32 flag = 0;
    s32 id = 8;
    u32 i;

    if (alternate == 0) {
        id = 18;
    }
    switch (group) {
    case 0:
        flag = 0x92c;
        break;
    case 1:
        flag = 0x935;
        break;
    case 2:
        flag = 0x917;
        break;
    case 3:
        flag = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++, flag++, id++) {
        if (Engine_GameFlagIsSet(flag) != 0) {
            return id;
        }
    }
    return 0;
}
