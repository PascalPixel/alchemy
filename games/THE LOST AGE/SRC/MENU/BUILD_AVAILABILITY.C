#include "IWRAM_CALL.H"

u8 *Owner_GetState(s32 owner);

#if defined(TBS_EDITION_JA)
#define GROUP_LEN 6
#else
#define GROUP_LEN 5
#endif

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    u8 *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = Owner_GetState(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (*(s16 *)(state + 56) == 0 && requested == 1) {
        output[0] = requested;
        count = 1;
    }

    mode = *(s8 *)(state + 0x131);
    if (mode != 0) {
        if (mode == 1)
            output[1] = mode;
        else
            output[2] = 1;
        count++;
    }
    if (*(s8 *)(state + 304) != 0) {
        output[3] = 1;
        count++;
    }
    if (state[320] != 0) {
        output[4] = 1;
        count++;
    }
    return count;
}
