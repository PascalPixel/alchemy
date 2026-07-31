#include "types.h"

/*
 * Sound-sequence command handler that pokes a hardware sound register.
 *
 * Two operand bytes are consumed.  The first is a byte offset into the sound
 * register block that starts at 0x04000060; it is taken raw from the stream.
 * The second is fetched through the shared reader's source-address check and
 * is the value written.  The stream pointer is left just past both bytes.
 *
 * Func_080f9ab6 is the shared operand reader inside the retained 0x080f9a80
 * row entered one halfword past Func_080f9ab4, which is the entry that takes
 * the stream position already loaded in a register instead of reloading it
 * from track+0x40.  That is why the first byte here is read directly and the
 * second one goes through the helper: the helper is what stores the advanced
 * pointer back.
 *
 * Only the offset is attacker-controlled in the ordinary sense, and the code
 * range-checks nothing, so a malformed sequence can address past the sound
 * block; that is the behaviour of the image and is reproduced as written.
 */

#define SOUND_REGISTERS_080F9C2C 0x04000060

struct Track_080f9c2c {
    u8 unknown_00[64];
    const u8 *command;
};

u8 Func_080f9ab6(struct Track_080f9c2c *track, const u8 *position);

void Func_080f9c2c(void *player, struct Track_080f9c2c *track)
{
    const u8 *command = track->command;
    volatile u8 *sound_register =
        (volatile u8 *)(SOUND_REGISTERS_080F9C2C + command[0]);

    *sound_register = Func_080f9ab6(track, command + 1);
}
