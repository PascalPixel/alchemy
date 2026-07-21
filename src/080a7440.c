typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080a77a4(s32);

s32 Func_080a7440(void) {
    s16 *b;
    s32 t;
    s32 v;
    b = *(s16 **)0x03001F2C;
    __asm__("");
    v = 0;
    __asm__("");
    b[186] = v;
    t = Func_080a77a4(v);
    if (t == -1) {
        v = t;
    } else {
        v = ((u8 *)b)[0x21A];
    }
    return v;
}
