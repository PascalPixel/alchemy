/* Near miss: differ 10 of 102, ported from The Broken Seal's AudioTrack_ConsumeSlotBytes. Three scheduling swaps remain: the cursor's increment comes ahead of the limit offset's split constant, the work pointer's pool load ahead of 0x3ff's shift, and the work pointer's copy ahead of the bucket offset's constant in the second loop. Empty asm barriers at nine statement boundaries (three permuter minutes over their choices) reach differ 6 at best. */
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
            u32                        read_offset;
            u32                        next_offset;
            u8                         value;

            AudioTrack_RemoveSlotNode(removal & mask);
            state = Flash_Handler3;
            read_offset = state->input_cursor;
            value = input[read_offset];
            next_offset = read_offset + 1;
            state->input_cursor = next_offset;
            if (next_offset == state->input_limit)
            {
                state->bucket_by_slot[slot & mask] = -1;
                break;
            }

            state->bucket_by_slot[slot & mask] = value;
            current++;
            AudioTrack_InsertSlotNode(slot & mask);
            removal++;
        } while (current < limit);
    }

    current++;
    if (current < limit)
    {
        u32                         mask = 0x3ff;
        struct AudioTrackSlotWork **root = &Flash_Handler3;
        s32                         empty = -1;

        do
        {
            u32                        slot = (base + current) & mask;
            struct AudioTrackSlotWork *state;

            AudioTrack_RemoveSlotNode(slot);
            state = *root;
            current++;
            state->bucket_by_slot[slot] = empty;
        } while (current < limit);
    }
}
