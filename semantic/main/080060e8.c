typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Value_000000c0;

void Func_080060e8(void *frame)
{
    u8 *state = (u8 *)0x02002240;
    u8 *packet;
    u16 *word;
    s32 checksum;
    s32 index;
    u16 zero;

    packet = *(u8 **)(state + 40);
    checksum = 0;
    packet[0] = state[11];
    packet[1] = state[2] ^ state[3];
    *(u16 *)(packet + 2) = checksum;
    packet += 4;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)frame;
        u32 destination = (u32)packet;
        u32 control = 0x84000006;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    word = *(u16 **)(state + 40);
    index = 0;
    do {
        checksum += *word;
        index++;
        word++;
    } while ((u32)index <= 13);
    *(u16 *)(*(u8 **)(state + 40) + 2) = ~checksum;
    if (state[0] != 0) {
        zero = 0;
        *(volatile u16 *)0x0400010e = zero;
    }
    *(s32 *)(state + 20) = -1;
    if (state[0] != 0 && state[8] != 0) {
        *(volatile u16 *)0x0400010e = (u16)(u32)&Value_000000c0;
    }
}
