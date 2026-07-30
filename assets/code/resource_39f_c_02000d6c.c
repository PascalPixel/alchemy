typedef signed int s32;
typedef signed short s16;

extern void Func_02003ba4(s32 a, s32 b);

s32 Func_02000d6c(s32 a) {
    Func_02003ba4(a, *(s16 *)(a + 100));
    return 0;
}
