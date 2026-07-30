typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_0801e318(void)
{
    u8 *base;
    u16 *entry;
    u8 enabled;
    s32 rows;
    register s32 width;
    s32 counter;
    u32 value;
    u32 reduced;
    s32 index;
    u8 *marker;
    s32 cleanup;

    base = *(u8 **)0x03001E8C;
    enabled = base[0xEA2];
    entry = (u16 *)base;
    width = 30;
    rows = 20;
    do {
        if (width != 0) {
            counter = width;
            do {
                value = *entry++ & 0x3FF;
                reduced = value - 0x80;
                if (reduced <= 0x7F ||
                    (enabled != 0 && value > 0x1FF && value <= 0x27F)) {
                    index = ((value & 0xFF) ^ 0x80) + 0xDA0;
                    base[index] |= 2;
                }
                counter--;
            } while (counter != 0);
        }
        rows--;
    } while (rows != 0);

    cleanup = 0xFF;
    marker = base + 0xDA0;
    do {
        if (*marker == 1)
            *marker = 0;
        cleanup--;
        marker++;
    } while (cleanup >= 0);
}
