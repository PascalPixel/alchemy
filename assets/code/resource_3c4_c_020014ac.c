typedef signed int s32;
typedef unsigned char u8;

s32 Func_0200458e(s32);
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_020045f8(s32);

void Func_020014ac(void)
{
    u8 *flags;
    u8 value;

    Func_0200458e(0x201);
    if (Func_0200458e(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        flags = Func_020045f8(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}
