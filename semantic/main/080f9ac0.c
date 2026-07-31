#include "types.h"

/*
 * Sound-sequence flow control: unconditional jump.
 *
 * The four bytes at the current stream position are assembled little-endian
 * into a new stream pointer and stored back over track+0x40, so the next byte
 * the interpreter fetches comes from the destination.  The stream is not
 * advanced past the operand; it is replaced by it.
 *
 * The low byte is the one that goes through the shared source-address check
 * at Func_080f9a98 (inside the retained 0x080f9a80 row, the entry that reads
 * the byte at the supplied position and screens it), while bytes 1..3 are
 * taken raw.  The check screens the position being read, not the value, so
 * for a stream that lives in ROM or RAM the assembled pointer is exactly the
 * stored word and only a stream pointer below 0x02000000 loses its low byte.
 *
 * The 0x080f9ae0 subroutine-call handler tail-branches to this entry with its
 * own return address still in lr, so the push {lr} here is what saves it and
 * this epilogue is what returns to 0x080f9ae0's caller.  The player argument
 * is dead in both and r0 is used as scratch, with the pop {r0}; bx r0
 * epilogue making the return type void.
 */

struct Track_080f9ac0 {
    u8 unknown_00[64];
    const u8 *command;
};

u8 Func_080f9a98(const u8 *position);

void Func_080f9ac0(void *player, struct Track_080f9ac0 *track)
{
    const u8 *command = track->command;
    u32 destination;

    destination = command[3];
    destination = (destination << 8) | command[2];
    destination = (destination << 8) | command[1];
    destination = (destination << 8) | Func_080f9a98(command);

    track->command = (const u8 *)destination;
}
