#include "types.h"

/*
 * Sound-sequence command handler.  The handler table that begins at
 * 0x080fb7a0 publishes one of these per command byte and the interpreter
 * calls the entry with the music player in the first argument and the track
 * in the second.  This one stores the operand as the track field at 0x0f and raises the two middle request bits.
 *
 * Func_080f9ab4 is the shared operand reader that lives inside the retained
 * 0x080f9a80 row: it reloads the byte-stream pointer at track+0x40,
 * post-increments it, reads the byte and passes it through the 0x080f9a9a
 * source-address check before returning it.  It preserves the player
 * argument, which it saves and restores around the check.
 *
 * The status byte at track+0x00 is read back after the store, so the
 * request bits are set on the value the reader may itself have changed.
 */

struct Track_080f9be0 {
    u8 status;
    u8 unknown_01[14];
    u8 parameter_0f;
    u8 unknown_10[48];
    const u8 *command;
};

u8 Func_080f9ab4(struct Track_080f9be0 *track);

void Func_080f9be0(void *player, struct Track_080f9be0 *track)
{
    track->parameter_0f = Func_080f9ab4(track);
    track->status |= 12;
}
