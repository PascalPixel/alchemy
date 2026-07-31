typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001ebc;

extern void Func_02000e86(s32, s32);
extern void Func_02000e8e(s32, s32);
extern s32 Func_02000e54(s32);
/* 0x02000e9a serves two imports in sibling arms: the three-argument setter in
 * the first and the one-argument record accessor in the second. */
extern void Func_02000e9a(s32, s32, s32);
extern u8 *Func_02000e9a_b(s32);
extern void Func_02000e68(u8 *, s32);

void Func_020004e8(void)
{
    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; reading it as one running offset is the natural mistake. */
    *(s32 *) (Data_03001ebc + 448) = 516;

    Func_02000e86(8, 1);
    Func_02000e8e(10, 2);

    if (Func_02000e54(0x882) != 0) {
        Func_02000e9a(9, 0, 0);
    } else {
        Func_02000e68(Func_02000e9a_b(9), 0);
    }
}
