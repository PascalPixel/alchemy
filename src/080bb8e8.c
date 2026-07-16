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

s32 Func_080090d0(s32);
void *Func_08077008();
s32 Func_08077130(void *, s32);
void *Func_080b7dd0(s32);
s32 Func_080b7e60(s32);
s32 Func_080bac6c(s32);

s32 Func_080bb8e8(s32 arg0) {
    s32 result;
    void *temp_r0;
    void *temp_r0_2;

    temp_r0 = Func_08077008();
    if (M2C_FIELD(temp_r0, u8 *, 0x12A) == 1) {
        Func_08077130(temp_r0, 0);
        Func_080bac6c(arg0);
        Func_080b7e60(arg0);
        temp_r0_2 = Func_080b7dd0(arg0);
        result = Func_080090d0(M2C_FIELD(temp_r0_2, s32 *, 0));
        M2C_FIELD(temp_r0_2, s32 *, 0) = 0;
        M2C_FIELD(temp_r0_2, s16 *, 0x28) = 0;
        return result;
    }
    return (s32) temp_r0;
}
