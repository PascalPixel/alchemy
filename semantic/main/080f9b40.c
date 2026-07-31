#include "types.h"

/*
 * Sound-sequence command handler.  The handler table at 0x080fb7a0 publishes
 * one of these per command byte; the interpreter calls the entry with the
 * player in the first argument and the track in the second.
 *
 * This one consumes a single operand byte from the track's byte stream and
 * stores it in the track field at offset 0x1d.  Unlike its siblings it does
 * not raise any request bit in the track flags byte, so the new value is
 * picked up the next time the track is serviced rather than immediately.
 *
 * Func_080f9ab4 is the shared stream reader that lives inside the retained
 * 0x080f9a80 row: it reloads the stream pointer, post-increments it, reads the
 * byte and passes it through the 0x080f9a9a address check before returning it.
 * The player argument is not touched by this handler.
 */

struct Track_080f9b40 {
    u8 unknown_00[29];
    u8 parameter_1d;
    u8 unknown_1e[34];
    const u8 *stream;
};

u8 Func_080f9ab4(struct Track_080f9b40 *track);

void Func_080f9b40(void *player, struct Track_080f9b40 *track)
{
    track->parameter_1d = Func_080f9ab4(track);
}
