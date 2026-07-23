typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 Func_0801965c(s32, s16 *, s32);
s32 Func_08017aa4(s16 *, s32, s32, s32);

/*
 * The r9 static chain and matching save identify this as a GNU C nested
 * function. The two captured pointers preserve the evidenced outer-frame
 * layout; the public alias gives the reconstructed entry its address name.
 */
extern s32 Func_080270d8(void)
    __attribute__((alias("Nested_080270d8.0")));

static __inline__ s32 Scope_080270d8(void)
{
    u8 *padding;
    u8 *context;

    s32 Nested_080270d8(void)
    {
        s16 data[64];

        (void)&padding;
        Func_0801965c(0x80D, data, 0x34);
        return Func_08017aa4(data, *(s32 *)(context + 0x44), 0, 4);
    }

    return 0;
}
