#include "owner_state.h"
#include "iwram_call.h"

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    struct BtlUnit *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = UnitGet(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (state->hp == 0 && requested == 1) {
        output[0] = requested;
        count = 1;
    }

    mode = state->doku;
    if (mode != 0) {
        if (mode == 1)
            output[1] = mode;
        else
            output[2] = 1;
        count++;
    }
    if (state->restraint != 0) {
        output[3] = 1;
        count++;
    }
    if (state->akuryo != 0) {
        output[4] = 1;
        count++;
    }
    return count;
}
