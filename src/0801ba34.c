typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_080b50e0(u16 *, s32);

void Func_0801ba34(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Func_080b50e0(data, 0);
}
