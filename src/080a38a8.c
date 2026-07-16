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

s32 Func_08015270(s32);
s32 Func_080a9a5c(s32, s32, s32);
s32 Func_080a9cbc();

void Func_080a38a8(s32 arg0) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_080a9cbc();
    Func_08015270(M2C_FIELD(temp_r5, s32 *, 0x20));
    Func_080a9a5c(M2C_FIELD(temp_r5, s32 *, 0x20), arg0, 0);
}
