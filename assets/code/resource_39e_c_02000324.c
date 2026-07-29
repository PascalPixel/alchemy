typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02004730(s32);
s16 Func_020046a2(s32, s32);

s32 Func_02000324(void *a)
{
    void *b = Func_02004730(0);
    M2C_FIELD(a, u16, 6) = Func_020046a2(M2C_FIELD(b, s32, 0x10) - M2C_FIELD(a, s32, 0x10), M2C_FIELD(b, s32, 8) - M2C_FIELD(a, s32, 8));
    return 0;
}
