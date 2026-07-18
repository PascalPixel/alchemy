typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001e8c;

void Func_08015ef4(void)
{
    s32 count;
    u8 *base;
    u8 *item;
    u8 *next;

    base = Data_03001e8c;
    item = base + 0x698;
    *(u8 **)(base + 0xd98) = item;
    count = 0x3e;
    do {
        next = item + 0x1c;
        count--;
        *(u8 **)item = next;
        item = next;
    } while (count >= 0);
    *(s32 *)next = 0;
    *(u8 **)(base + 0xd9c) = next;
}
