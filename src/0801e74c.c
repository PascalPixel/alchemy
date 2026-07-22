typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_08018038(s32, s32);
void Func_08017aa4(u8 *, s32, s32, s32);

void Func_0801e74c(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *base = *(u8 **)0x03001E8C;
    u16 *counter = (u16 *)(base + 0x12B2);
    s32 idx;
    s32 zero = 0;

    *counter = zero;
    Func_08018038(arg0, 1);
    idx = *counter * 2 + 0xEB0;
    *(u16 *)(base + idx) = zero;
    *counter = (*counter + 1) & 0x1FF;
    Func_08017aa4(base + 0xEB0, arg1, arg2, arg3);
}
