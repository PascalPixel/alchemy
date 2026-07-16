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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08004458();
s32 Func_08009098(void *, s32);

void Func_080999a8(void *arg0) {
    s32 temp_r5;

    M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 0xFFFFB334);
    temp_r5 = Func_08004458();
    M2C_FIELD(arg0, s32 *, 8) = (s32) (M2C_FIELD(arg0, s32 *, 8) + (temp_r5 - Func_08004458()));
    if ((s32) M2C_FIELD(arg0, s32 *, 0xC) <= (s32) M2C_FIELD(arg0, s32 *, 0x14)) {
        Func_08009098(arg0, 0x0809F0B0);
    }
}
