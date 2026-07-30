typedef signed int s32;

extern void Func_02003d14(s32 a, s32 b);
extern s32 Func_02003d34(s32 a, s32 b);
extern void Func_02003d30(void);
extern void Func_02003c66(s32 a);
extern void Func_02001ce8(s32 a);
extern void Func_02003cfc(s32 a, s32 b);

void Func_02000ea8(s32 a, s32 b) {
    s32 k0 = 0x20000;
    s32 k1 = 0x4000;

    Func_02003d14(k0, k1);
    Func_02003d34(a, 1);
    Func_02003d30();
    Func_02003c66(30);
    Func_02001ce8(a);
    Func_02003cfc(a, b);
}
