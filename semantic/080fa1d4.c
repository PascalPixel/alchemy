#include "types.h"

/*
 * Sound-sequence operand handler.  This player's handler table publishes it
 * at 0x080fa640 and at 0x080fb7e4; the interpreter calls the entry with the music
 * player in the first argument and the track in the second.
 *
 * One operand byte is consumed and stored as the depth of the first modulation at
 * track+0x19.  A zero operand means the first modulation is switched off, and that case
 * additionally calls Func_080fa1ac, which clears the running modulation
 * amount and phase at track+0x16 and track+0x1a and raises the matching
 * request bits in the track status byte.  A non-zero operand only stores the
 * new depth and leaves the accumulators running.
 *
 * Func_080fa1c8 is this player's operand reader.  Its exact C at
 * src/080fa1c8.c models only the pointer advance and the read, because the
 * byte it fetches is returned in r3 and the byte-exact source has no use for
 * it; the reconstruction here does use it, so it is declared with the byte
 * result it actually produces.
 */

struct Track_080fa1d4 {
    u8 unknown_00[25];
    u8 modulation_depth;
    u8 unknown_1a[38];
    const u8 *command;
};

u8 Func_080fa1c8(void *player, struct Track_080fa1d4 *track);
void Func_080fa1ac(void *player, struct Track_080fa1d4 *track);

void Func_080fa1d4(void *player, struct Track_080fa1d4 *track)
{
    u8 depth = Func_080fa1c8(player, track);

    track->modulation_depth = depth;

    if (depth == 0)
        Func_080fa1ac(player, track);
}
