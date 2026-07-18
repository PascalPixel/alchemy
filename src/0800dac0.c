typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_0800d14c(void *, s32, s32, s32);

s32 Func_0800dac0(u8 *arg0)
{
    s32 offset[3];

    Func_0800d14c(
        arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    *(u16 *)(arg0 + 4) += 3;
    return 1;
}
