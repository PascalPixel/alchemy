typedef signed int s32;

extern void Func_02001a3c(void);
extern s32 Func_02001a12(s32);
extern void Func_02001ae4(s32);
extern void Func_02001af4(s32, s32);
extern s32 Func_02001a2c(s32);
extern void Func_02001afe(s32);
extern void Func_02001b16(s32, s32);
extern void Func_02001b0e(s32);
extern void Func_02001b26(s32, s32);
extern void Func_02001a8a(void);

void Func_020004b8(void)
{
    Func_02001a3c();

    if (Func_02001a12(0x8A7) != 0) {
        Func_02001ae4(0x1D1F);
        Func_02001af4(13, 0);
    } else if (Func_02001a2c(0x8A5) != 0) {
        Func_02001afe(0x1D1B);
        Func_02001b16(13, 0);
    } else {
        Func_02001b0e(0x1D19);
        Func_02001b26(13, 0);
    }

    Func_02001a8a();
}
