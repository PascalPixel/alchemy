typedef signed int s32;
typedef unsigned char u8;

extern s32 Func_02005202(s32);

void Func_020015e0(void) {
    u8 *p;
    s32 v;

    p = (u8 *)(Func_02005202(0) + 35);
    v = 1;
    v |= *p;
    *p = v;
}
