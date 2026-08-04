#include "types.h"

struct PartyState_0802281c {
    u8 padding00[0x58];
    s16 active_ids[4];
};

extern struct PartyState_0802281c *Data_03001e74;

void Func_08022768(s32, s32, s32, s32, s32);
s32 Func_080b5090(s32, s16 *);

/*
 * Shade the party strip, then restore the brighter palette on each requested
 * active member.  Both lists contain at most four IDs and use 0xff as their
 * terminator.  Horizontal positions are relative to the current party count.
 */
s32 Func_0802281c(const u16 *requested_ids)
{
    s32 party_count = Func_080b5090(1, 0);
    s32 requested_index;

    Func_08022768(29 - party_count * 6, 0, 25, 5, 15);

    for (requested_index = 0;
         requested_index < 4 && requested_ids[requested_index] != 0xff;
         requested_index++) {
        s32 active_index;

        for (active_index = 0; active_index < 4; active_index++) {
            if (Data_03001e74->active_ids[active_index] ==
                requested_ids[requested_index]) {
                break;
            }
            if (Data_03001e74->active_ids[active_index] == 0xff) {
                active_index = 4;
                break;
            }
        }

        if (active_index != 4) {
            Func_08022768(
                29 - (party_count - active_index) * 6,
                0,
                7,
                5,
                14);
        }
    }

    return 0;
}
