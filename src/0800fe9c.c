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

s32 Func_08010000();
s32 Func_08010230(s32, s32);

void Func_0800fe9c(void) {
    s32 var_r0;
    s32 var_r1;
    s32 var_r2;
    void *temp_r3;

    temp_r3 = **(void ***)0x03001E70;
    var_r0 = 0;
    var_r2 = 0;
    var_r1 = 0;
    if (temp_r3 != NULL) {
        var_r0 = M2C_FIELD(temp_r3, s32 *, 0);
        temp_r3 += 4;
        var_r2 = M2C_FIELD(temp_r3, s32 *, 0);
        temp_r3 += 4;
        var_r1 = M2C_FIELD(temp_r3, s32 *, 0);
    }
    Func_08010230(var_r0, var_r1 - var_r2);
    Func_08010000();
}
