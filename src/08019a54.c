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

void Func_08016418(s32, s32);
void Func_080030f8(u32);

void Func_08019a54(s32 arg0) {
    s32 var_r6;
    void **var_r5;
    void *temp_r0;

    var_r5 = (void **)(*(s32 *)0x03001E8C + 0x620);
    var_r6 = 0;
    do {
        temp_r0 = *var_r5;
        if ((temp_r0 != NULL) && (M2C_FIELD(temp_r0, s32, 0x18) == 0) &&
            (M2C_FIELD(temp_r0, u16, 0x16) != 0) &&
            (M2C_FIELD(temp_r0, u16, 0x14) != 0)) {
            Func_08016418((s32) temp_r0, (s32) (u16) (2 & M2C_FIELD(temp_r0, u16, 0x16)));
        }
        var_r6 += 1;
        var_r5 = (void **)((u8 *)var_r5 + 40);
    } while (var_r6 != 3);
    Func_080030f8(10U);
}
