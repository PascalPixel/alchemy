#include "types.h"

/*
 * Sound-sequence command handler.  The handler table that begins at
 * 0x080fb7a0 publishes one of these per command byte and the interpreter
 * calls the entry with the music player in the first argument and the track
 * in the second.
 *
 * This one reads an operand byte into the track field at 0x18 but only when
 * the value actually changes, and only then raises request bits 0x0f in the
 * track status byte.  Repeating the same value therefore costs a stream byte
 * and nothing else, which is what keeps a held setting from re-triggering the
 * work the request bits ask for.
 *
 * Func_080f9ab4 is the shared operand reader inside the retained 0x080f9a80
 * row; the player argument is dead here — the assembly reuses r0 as the
 * scratch register holding the old field value.
 */

struct Track_080f9c00 {
    u8 status;
    u8 unknown_01[23];
    u8 parameter_18;
    u8 unknown_19[39];
    const u8 *command;
};

u8 Func_080f9ab4(struct Track_080f9c00 *track);

void Func_080f9c00(void *player, struct Track_080f9c00 *track)
{
    u8 value = Func_080f9ab4(track);

    if (track->parameter_18 == value)
        return;

    track->parameter_18 = value;
    track->status |= 15;
}
