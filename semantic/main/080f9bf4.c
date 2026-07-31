#include "types.h"

/*
 * Sound-sequence command handler.  The handler table that begins at
 * 0x080fb7a0 publishes one of these per command byte and the interpreter
 * calls the entry with the music player in the first argument and the track
 * in the second.  This one stores the operand as the track field at 0x1b and raises no request bit.
 *
 * Func_080f9ab4 is the shared operand reader that lives inside the retained
 * 0x080f9a80 row: it reloads the byte-stream pointer at track+0x40,
 * post-increments it, reads the byte and passes it through the 0x080f9a9a
 * source-address check before returning it.  It preserves the player
 * argument, which it saves and restores around the check.
 */

struct Track_080f9bf4 {
    u8 status;
    u8 unknown_01[26];
    u8 parameter_1b;
    u8 unknown_1c[36];
    const u8 *command;
};

u8 Func_080f9ab4(struct Track_080f9bf4 *track);

void Func_080f9bf4(void *player, struct Track_080f9bf4 *track)
{
    track->parameter_1b = Func_080f9ab4(track);
}
