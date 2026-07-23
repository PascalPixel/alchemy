typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

static __inline__ void CopyEntry(u8 *map, u8 *destination) {
    u32 palette = *(u16 *)map;
    volatile u16 *colors = (volatile u16 *)0x02010000;
    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

void Func_080110e0(u32 value) {
    u8 *map;
    u8 *destination;
    u32 counter;

    map = (u8 *)(0x02020000 + ((((s32)value / 2) & 31) << 7));
    destination = (u8 *)(0x06004000 + ((value & 62) << 6));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);

    destination += 0xFC0;
    map += 0xF80;
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);
}
