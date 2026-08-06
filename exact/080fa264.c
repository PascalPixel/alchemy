#include "types.h"

/*
 * Release the sustain hold on a live sound track.
 *
 * Like the retune entries at 0x080fa4cc the track's 'Smsh' signature word is
 * checked before anything is touched, so a slot that has been torn down or
 * never initialised is ignored rather than corrupted.  When the signature
 * matches, the top bit of the flag word is cleared and nothing else changes;
 * that bit is the same one the 0x080fa4ec sibling clears as part of its larger
 * retune, so this is that half of it on its own.
 *
 * Called from four sites in the same block, 0x080fa3e8, 0x080fa446, 0x080fa486
 * and 0x080fa4a2.
 */

#define SIGNATURE_080FA264 0x68736D53

struct Track_080fa264 {
    u8 unknown_00[4];
    u32 flags;
    u8 unknown_08[44];
    u32 signature;
};

void Func_080fa264(struct Track_080fa264 *track)
{
    if (track->signature != SIGNATURE_080FA264)
        return;

    track->flags &= 0x7FFFFFFF;
}
