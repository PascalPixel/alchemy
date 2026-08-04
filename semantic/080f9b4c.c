#include "types.h"

/*
 * Sound-sequence command handler.  The handler table that begins at
 * 0x080fb7a0 publishes one of these per command byte and the interpreter
 * calls the entry with the music player in the first argument and the track
 * in the second.
 *
 * This one is the only handler in the table that writes the player rather
 * than the track.  It reads one operand byte, doubles it into the player's
 * requested level at 0x1c, scales that by the player's master level at 0x1e
 * and stores the 8.8 product's whole part as the effective level at 0x20.
 *
 * Func_080f9ab4 is the shared operand reader inside the retained 0x080f9a80
 * row; it saves and restores r0 around the 0x080f9a9a source-address check,
 * so the player pointer is still live when the three halfword accesses below
 * run.  The multiply is a 32-bit muls on two zero-extended halfwords, so the
 * >> 8 is an unsigned shift and cannot borrow from a sign bit.
 */

struct Player_080f9b4c {
    u8 unknown_00[28];
    u16 requested_level;
    u16 master_level;
    u16 effective_level;
};

struct Track_080f9b4c;

u8 Func_080f9ab4(struct Track_080f9b4c *track);

void Func_080f9b4c(struct Player_080f9b4c *player, struct Track_080f9b4c *track)
{
    u32 level = (u32)Func_080f9ab4(track) << 1;

    player->requested_level = level;
    player->effective_level = (level * player->master_level) >> 8;
}
