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

void Func_08015298(s32, u16, u8, s32);
void Func_080a3d24(u16 *);

void Func_080a3e28(u16 *arg0, s32 arg1) {
    s32 var_r7;
    u16 *var_r6;
    void **var_r5;
    s32 temp_r1;

    var_r5 = (void **)((u8 *)(*(void **)0x03001F2C) + 0x48);
    var_r6 = arg0;
    var_r7 = 14;
    do {
        temp_r1 = *var_r6++;
        if (temp_r1 != 0) {
            if (arg1 == 0) {
                Func_08015298(2, temp_r1, M2C_FIELD(*var_r5, u8 *, 0xE), 0);
            } else {
                Func_08015298(7, temp_r1, M2C_FIELD(*var_r5, u8 *, 0xE), 0);
            }
        }
        var_r5++;
        var_r7--;
    } while (var_r7 >= 0);
    Func_080a3d24(arg0);
}
