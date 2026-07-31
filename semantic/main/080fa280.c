#include "types.h"

/*
 * Snap a live sound track to a new value and give it a fresh glide state.
 *
 * The third member of the family whose other two entries are reconstructed at
 * 0x080fa4cc: all three check the track's 'Smsh' signature word first, then
 * write the same current/target pair, and differ only in the state word left
 * behind — 0x0101 for 0x080fa4cc, 2 for 0x080fa4ec, 0x0100 here.
 *
 * The value argument is narrowed to sixteen bits before either store, which
 * the image spells as lsls r1,r1,#16 followed by lsrs r1,r1,#16, so a caller
 * passing a wider value has its high half discarded rather than saturated.
 *
 * Called from one site, 0x080fa4c2.
 */

#define SIGNATURE_080FA280 0x68736D53

struct Track_080fa280 {
    u8 unknown_00[36];
    u16 target;
    u16 current;
    u16 state;
    u8 unknown_2a[10];
    u32 signature;
};

void Func_080fa280(struct Track_080fa280 *track, u32 value)
{
    u16 narrowed = (u16)value;

    if (track->signature != SIGNATURE_080FA280)
        return;

    track->current = narrowed;
    track->target = narrowed;
    track->state = 0x100;
}
