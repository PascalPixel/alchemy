typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080160fc(void)
{
    u8 *base;
    u8 *source;
    u8 *destination;
    u8 *status;
    u32 *dma;
    u32 pending;
    u8 state;

    base = *(u8 **)0x03001E8C;
    status = base + 0xEA6;
    if (*status != 0)
        return;

    state = status[-3];
    if (state == 0)
        return;

    destination = (u8 *)0x06002000;
    source = base;
    if (state & 1)
        state = 0x3F;
    pending = (state & 0x3F) >> 1;
    dma = (u32 *)0x040000D4;

    do {
        if (pending & 1) {
            dma[0] = (u32)source;
            dma[1] = (u32)destination;
            dma[2] = 0x84000040;
        }
        pending >>= 1;
        source += 0x100;
        destination += 0x100;
    } while (pending != 0);

    status[-3] = pending;
}
