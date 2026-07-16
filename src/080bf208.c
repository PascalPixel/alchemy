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

void *Func_08077008();
s32 Func_080771a0();

s32 Func_080bf208(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r5;
    void *temp_r0;

    temp_r0 = Func_08077008();
    if (arg1 <= 5) {
        temp_r5 = (((M2C_FIELD(temp_r0, u8 *, 0x42) * 3) - (arg1 * 5)) + arg2) * 0x28F;
        if (temp_r5 >= (Func_080771a0() & 0xFFFF)) {
            return 1;
        }
    }
    return 0;
}
