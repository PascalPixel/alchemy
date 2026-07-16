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

s32 Func_08009080(s32, s32);
s32 Func_08009158();
s32 Func_0808ba1c();

void Func_080923c4(void) {
    s32 temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != 0) {
        Func_08009158();
        Func_08009080(temp_r0, 1);
    }
}
