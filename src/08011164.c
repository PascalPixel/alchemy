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

void Func_08011164(u32 value) {
    u8 *map;
    u8 *destination;
    u32 counter;

    map = (u8 *)(0x02020000 + ((((s32)value / 2) & 31) << 2));
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}
