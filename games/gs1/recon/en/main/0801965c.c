#include "types.h"

extern u8 *Data_03001e8c;
extern u8 Data_00000000[];

s32 Func_08018038(s32 key, s32 mode);

s32 Func_0801965c(s32 key, u16 *destination, u32 capacity)
{
    u8 *work = Data_03001e8c;
    u16 *source;
    u32 count;
    u32 destination_offset;

    *(u16 *)(work + 0x12b2) = 0;
    Func_08018038(key, 1);

    capacity--;
    count = 0;
    if (count < capacity) {
        source = (u16 *)(work + 0xeb0);
        *destination = *source;
        if (*destination != 0) {
            destination_offset = 0;
            do {
                count++;
                destination_offset += 2;
                if (count >= capacity)
                    break;
                source++;
                *(u16 *)((u8 *)destination + destination_offset) = *source;
            } while (*source != 0);
        }
    }
    *(u16 *)((u8 *)destination + count * 2) = (u16)(u32)Data_00000000;
    return count;
}
