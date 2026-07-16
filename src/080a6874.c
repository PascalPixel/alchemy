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

s32 Func_08015018(s32, s32);
s32 Func_080a195c();

void Func_080a6874(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_080a195c();
    Func_08015018(M2C_FIELD(temp_r5, s32 *, 0x10), 1);
    Func_08015018(M2C_FIELD(temp_r5, s32 *, 0x20), 1);
    Func_08015018(M2C_FIELD(temp_r5, s32 *, 0x10C), 1);
}
