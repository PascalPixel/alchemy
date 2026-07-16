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

s32 Func_080091a8(s32, s32, s32);

void Func_0808e9a8(void *arg0) {
    s32 temp_r0;

    temp_r0 = Func_080091a8(0, M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0x10));
    M2C_FIELD(arg0, s32 *, 0xC) = temp_r0;
    M2C_FIELD(arg0, s32 *, 0x14) = temp_r0;
}
