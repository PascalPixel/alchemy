/* Near miss: differ 6 of 102, The Broken Seal's AudioTrack_ConsumeSlotBytes. The two one-pass blocks (found by a greedy device search, differ 10 without them) fix the byte read and the removal call; three swaps remain, each a load the native issues earlier: the cursor store before the limit offset's shift, the work pointer's pool load before 0x3ff's shift, and the root's load inside the bucket offset's split constant. */
#include "trackbuf.h"

void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input)
{
    s32 limit = count;
    s32 current = 0;
    u32 base = start;

    if (current < limit)
    {
        u32 mask = 0x3ff;
        u32 removal = base + 0x124;

        do
        {
            u32                        slot = base + current;
            struct AudioTrackSlotWork *state;
            u32                        readOffset;
            u32                        nextOffset;
            u8                         value;

            AudioTrack_RemoveSlotNode(removal & mask);
            state = gFlash_Handler3;
            readOffset = state->inputCursor;
            do {
                value = input[readOffset];
            } while (0);
            nextOffset = readOffset + 1;
            state->inputCursor = nextOffset;
            if (nextOffset == state->inputLimit)
            {
                state->bucketBySlot[slot & mask] = -1;
                break;
            }

            state->bucketBySlot[slot & mask] = value;
            current++;
            AudioTrack_InsertSlotNode(slot & mask);
            removal++;
        } while (current < limit);
    }

    current++;
    if (current < limit)
    {
        u32                         mask = 0x3ff;
        struct AudioTrackSlotWork **root = &gFlash_Handler3;
        s32                         empty = -1;

        do
        {
            u32                        slot = (base + current) & mask;
            struct AudioTrackSlotWork *state;

            do {
                AudioTrack_RemoveSlotNode(slot);
            } while (0);
            state = *root;
            current++;
            state->bucketBySlot[slot] = empty;
        } while (current < limit);
    }
}
