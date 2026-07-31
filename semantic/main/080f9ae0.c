#include "types.h"

/*
 * Sound-sequence flow control: subroutine call.
 *
 * The current stream position plus four — the byte after the four-byte
 * destination that the jump handler is about to consume — is pushed onto the
 * track's three-deep return stack at track+0x44, the depth at track+0x02 is
 * incremented, and control tail-branches to the jump handler at 0x080f9ac0,
 * which reads that destination and installs it as the new stream position.
 *
 * A fourth nested call has nowhere to save its return, and instead of
 * overflowing the array the track is torn down: the overflow arm tail-branches
 * to Func_080f9a50, which releases every channel still hanging off the track's
 * channel list at track+0x20 and clears the track status byte.
 *
 * Both arms are tail branches taken with the return address still in lr and no
 * frame of this handler's own, so this handler's result is whatever the callee
 * returns, which is void in both cases.
 */

struct Track_080f9ae0 {
    u8 status;
    u8 unknown_01;
    u8 nesting_depth;
    u8 unknown_03[61];
    const u8 *command;
    const u8 *return_stack[3];
};

void Func_080f9ac0(void *player, struct Track_080f9ae0 *track);
void Func_080f9a50(void *player, struct Track_080f9ae0 *track);

void Func_080f9ae0(void *player, struct Track_080f9ae0 *track)
{
    u8 depth = track->nesting_depth;

    if (depth >= 3) {
        Func_080f9a50(player, track);
        return;
    }

    track->return_stack[depth] = track->command + 4;
    track->nesting_depth = track->nesting_depth + 1;
    Func_080f9ac0(player, track);
}
