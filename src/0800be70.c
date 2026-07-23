typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct MapBase {
    u16 unused;
    u16 offset;
};

void Func_0800be70(u8 *object, u32 position)
{
    u16 *destination;
    s32 count;
    u32 row;
    u8 *table;
    u32 high_mask;
    u32 index_mask;
    u8 offset_mask;

    destination = (u16 *)(0x06010000
        + ((struct MapBase *)0x03001B10)[object[0x1C]].offset);
    count = (object[0x20] * object[0x21]) / 64;
    row = 0;

    if (row < (u32)count) {
        table = (u8 *)0x0801314C;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        for (; row < (u32)count; row++, destination += 32, position++) {
            if ((u32)(position - 0x40) <= 0x3F) {
                u8 entry;
                u16 *cell;

                entry = table[(position + row * 16) & index_mask];
                cell = (u16 *)((u8 *)destination + (entry & offset_mask));
                if (((entry >> 1) * 2) != entry)
                    *cell = *(u8 *)cell;
                else
                    *cell &= high_mask;
            }
        }
    }
}
