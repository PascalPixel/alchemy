typedef signed int s32; typedef unsigned int u32;

extern void Func_02009280(s32, s32, s32, s32, s32, s32);
extern u32 Func_0200923a(void);
extern u32 Func_0200924e(void);
extern void Func_02003a32(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern void Func_020093a8(s32);
extern void Func_0200931a(s32, s32, s32, s32, s32, s32);
extern void Func_02009330(s32, s32, s32, s32, s32, s32);

void Func_02003860(void)
{
    s32 buf[10];
    u32 i, j;

    Func_02009280(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = ((Func_0200923a() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = ((Func_0200924e() << 3) >> 16) * 0x3333 + 0xffff3334;

                Func_02003a32(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Func_020093a8(1);
            }
        }
        Func_0200931a(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Func_02009330(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}
