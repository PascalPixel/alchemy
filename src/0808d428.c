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

s32 Func_080770c0();

s32 Func_0808d428(s32 arg0) {
    s32 temp_r0;

    if (arg0 == -1) {
        return 1;
    }
    if (0x1000 & arg0) {
        return Func_080770c0();
    }
    temp_r0 = Func_080770c0();
    return 1 - ((u32) ((0 - temp_r0) | temp_r0) >> 0x1F);
}
