typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Dma_08005c68 {
    u32 source;
    u32 destination;
    u32 control;
};

extern u8 *Data_03001f1c;
s32 Func_08005b24(s32);
void Func_08006ba8(u16, s32, void *, s32);

s32 Func_08005c68(void) {
    u8 *destination;
    s32 zero;
    u32 i;
    s32 count;

    destination = Data_03001f1c + (130 << 5);
    i = 0;
    zero = 0;
    count = 0;
    do {
        struct Dma_08005c68 transfer;
        u32 value;

        transfer.source = (u32)&zero;
        transfer.destination = (u32)destination;
        transfer.control = 0x85000010;
        *(volatile struct Dma_08005c68 *)0x040000D4 = transfer;

        value = Func_08005b24(i);
        if (value <= 15) {
            Func_08006ba8((u16)value, 0, destination, 64);
            count++;
        }

        value = Func_08005b24(i + 3);
        if (value <= 15)
            Func_08006ba8((u16)value, 272, destination + 56, 4);
        else
            *(s32 *)(destination + 56) = 0;

        i++;
        destination += 64;
    } while (i <= 2);

    return count;
}
