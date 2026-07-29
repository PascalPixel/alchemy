typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_02004792(void);
extern s32 Func_020047b0(void);

s32 Func_02000514(u8 *a)
{
    s16 *flag = (s16 *)(a + 0x66);
    s32 v;
    s32 t;

    if (*flag != 0) {
        v = *(s32 *)(a + 0xc) - (((u32)(Func_02004792() << 15)) >> 16) - 0x8000;
        *(s32 *)(a + 0xc) = v;
        if (v >= 0x40000)
            goto done;
        t = 0;
    } else {
        v = *(s32 *)(a + 0xc) + (((u32)(Func_020047b0() << 15)) >> 16) + 0x8000;
        *(s32 *)(a + 0xc) = v;
        if (v <= 0xC0000)
            goto done;
        t = 1;
    }
    *flag = t;
done:
    return 1;
}
