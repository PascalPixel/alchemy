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

s32 Func_08017364(void) {
    s32 var_r0;
    s32 var_r1;
    void **var_r2;
    void *temp_r3;

    var_r2 = *(s32 *)0x03001E8C + 0x620;
    var_r1 = 0;
loop_1:
    temp_r3 = *var_r2;
    if ((temp_r3 == NULL) || (var_r0 = 0, (M2C_FIELD(temp_r3, u16 *, 0x14) != 0))) {
        var_r1 += 1;
        var_r2 += 0xA;
        if (var_r1 == 3) {
            var_r0 = 1;
        } else {
            goto loop_1;
        }
    }
    return var_r0;
}
