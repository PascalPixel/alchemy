#include "types.h"

struct DmaTransfer_0800615c {
    const void *source;
    void *destination;
    u32 control;
};

struct SharedState_0800615c {
    u8 flags[4];
    u32 saved;
    u8 padding08[0x38];
    u32 matrix[8];
    u8 padding60[0x30];
    u16 *tables[2];
};

extern struct SharedState_0800615c Data_02002240;

u8 Func_0800615c(void *destination)
{
    volatile struct DmaTransfer_0800615c *dma =
        (volatile struct DmaTransfer_0800615c *)0x040000d4;
    u32 savedFlags;
    u32 *left;
    u32 *right;
    s32 column;
    s32 stream;
    u8 *output;

    *(volatile u16 *)0x04000208 = 0;
    left = &Data_02002240.matrix[0];
    right = &Data_02002240.matrix[4];
    column = 3;
    do {
        u32 value = *right;
        *right = *left;
        *left = value;
        left++;
        right++;
        column--;
    } while (column >= 0);

    savedFlags = Data_02002240.saved;
    Data_02002240.saved = 0;
    *(volatile u16 *)0x04000208 = 1;
    Data_02002240.flags[3] = 0;

    output = destination;
    stream = 0;
    do {
        u16 *table = Data_02002240.tables[stream];
        u8 *scan = (u8 *)table;
        s16 sum = 0;
        u32 index = 0;

        do {
            sum += *(u16 *)scan;
            scan += 2;
            index++;
        } while (index <= 13);

        if (((u8 *)&savedFlags)[stream] == 1 && sum == -1) {
            dma->source = table + 2;
            dma->destination = output;
            dma->control = 0x84000006;
            Data_02002240.flags[3] |= 1 << stream;
        }
        if (sum == -1) {
            table[1] = ~table[1];
        }
        output += 0x18;
        stream++;
    } while (stream <= 1);

    Data_02002240.flags[2] |= Data_02002240.flags[3];
    return Data_02002240.flags[3];
}
