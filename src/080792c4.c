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

void *Func_08077394();
u32 Func_08079008(s32, s32);
s32 Func_0807905c(s32, s32);

s32 Func_080792c4(s32 arg0, s32 arg1) {
    void *temp_r0;

    temp_r0 = Func_08077394();
    if (((u32) M2C_FIELD(temp_r0, u32 *, 0x124) >= Func_08079008(arg0, M2C_FIELD(temp_r0, u8 *, 0xF) + 1)) && (Func_0807905c(arg0, arg1) != 0)) {
        return arg1;
    }
    return 0;
}
