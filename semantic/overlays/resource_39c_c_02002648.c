typedef unsigned short u16;
typedef unsigned int u32;

/*
 * resource_39c palette-rotation owner at 0x02002648, complete 60-byte span
 * through its four-word pool. Once every eight frames, rotate the eight
 * BGR555 entries at palette indices 40-47 one place toward the front.
 */

void Func_02002648(void)
{
    volatile u16 *palette = (volatile u16 *)0x05000050;
    u16 first;
    unsigned int index;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    first = palette[0];
    for (index = 0; index <= 6; index++) {
        palette[index] = palette[index + 1];
    }
    palette[7] = first;
}
