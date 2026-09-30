#include "TYPES.H"
extern u8 Flash_Handler3[];

struct AudioTrackSlotWork {
    u8 unknown0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 unknown4404[0x34];
    u32 input_cursor;
    s32 unknown443c;
    u32 input_limit;
};

extern struct AudioTrackSlotWork *Data_02004c00;

void AudioTrack_ResetSlotBuckets(void)
{
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    limit = 0x3FF;
    index = 0;
    zero = 0;
    record = *(u8 **)Flash_Handler3 + 4;
    do {
        *(s32 *)(record + 4) = index;
        index++;
        *(s32 *)record = zero;
        record += 12;
    } while (index <= limit);
    slot = (s32 *)(*(u8 **)Flash_Handler3 + 0x3000);
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}
