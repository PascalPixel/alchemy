typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_08077394(s32);
extern u8 *Func_08078414(s32);

s32 Func_080787dc(s32 arg0, s32 arg1)
{
    u8 *base = Func_08077394(arg0);
    s32 index;
    s32 offset;
    u8 *data;

    for (index = 0, offset = 216; index <= 14; index++) {
        if (*(u16 *)((u8 *)offset + (s32)base) & 0x200) {
            data = Func_08078414(*(u16 *)((u8 *)offset + (s32)base));
            if (data[2] == arg1) break;
        }
        offset += 2;
    }
    if (index == 15) index = -1;
    return index;
}
