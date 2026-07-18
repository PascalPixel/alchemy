typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

void Func_0800d14c(void *, s32, s32, s32);

s32 Func_0800da40(u8 *arg0)
{
    u8 *entry = (u8 *)(*(s32 *)arg0 + *(s16 *)(arg0 + 4) * 4);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Func_0800d14c(arg0,
        *(s32 *)(arg0 + 8) + first,
        *(s32 *)(arg0 + 0xc) + second,
        *(s32 *)(arg0 + 0x10) + third);
    *(s16 *)(arg0 + 4) = *(u16 *)(arg0 + 4) + 4;
    return 1;
}
