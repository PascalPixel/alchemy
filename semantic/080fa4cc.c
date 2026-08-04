#include "types.h"

/*
 * Two sibling entry points that retune a live sound track.  Each checks the
 * track's 'Smsh' signature word before touching anything, so a slot that has
 * been torn down is ignored rather than corrupted; the two differ only in the
 * state they leave behind.  Caller unknown for both: no direct call, branch or
 * pool word in the image reaches either entry.
 */

#define SIGNATURE_080FA4CC 0x68736D53

struct Track_080fa4cc {
    u8 unknown_00[4];
    u32 flags;
    u8 unknown_08[28];
    u16 target;
    u16 current;
    u16 state;
    u8 unknown_2a[10];
    u32 signature;
};

void Func_080fa4cc(struct Track_080fa4cc *track, u16 value)
{
    if (track->signature != SIGNATURE_080FA4CC)
        return;

    track->current = value;
    track->target = value;
    track->state = 0x101;
}

void Func_080fa4ec(struct Track_080fa4cc *track, u16 value)
{
    if (track->signature != SIGNATURE_080FA4CC)
        return;

    track->current = value;
    track->target = value;
    track->state = 2;
    track->flags &= 0x7FFFFFFF;
}
