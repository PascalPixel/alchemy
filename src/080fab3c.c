typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

void Func_080f9ef8(void *, void *);

void Func_080fab3c(u8 *entry)
{
    u32 current = *(u32 *)(entry + 52);
    s32 count;
    u8 *item;

    if (current != 0x68736d53)
        return;

    *(u32 *)(entry + 52) = current + 1;
    *(u32 *)(entry + 4) |= 0x80000000;

    count = entry[8];
    item = *(u8 **)(entry + 44);
    if (count > 0) {
        do {
            Func_080f9ef8(entry, item);
            count--;
            item += 80;
        } while (count > 0);
    }

    *(u32 *)(entry + 52) = 0x68736d53;
}
