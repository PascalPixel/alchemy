typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_080041d8(void (*)(void), s32);
void Func_080119cc(void);

void Func_08011a84(u16 *entry)
{
    u8 *base = *(u8 **)0x03001e70;
    u32 zero;
    u32 *source;
    u8 *slot;
    s32 changed;

    changed = 0;
    source = &zero;
    slot = base + 216;
    *source = 0;
    {
        u32 *dma = (u32 *)0x040000d4;

        dma[0] = (u32)source;
        dma[1] = (u32)slot;
        dma[2] = 0x85000003;
    }
    if (*entry != 0xFFFF) {
        *(u32 *)(slot + 0) = (u32)entry;
        *(u32 *)(slot + 4) = (u32)entry;
        *(u16 *)(slot + 8) = 0;
        *(u16 *)(slot + 10) = 0;
        changed = 1;
    }
    if (changed != 0) {
        Func_080041d8(Func_080119cc, 3200);
    }
}
