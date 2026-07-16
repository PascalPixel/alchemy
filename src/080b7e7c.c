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
void *Func_080b7dd0(s32);

void *Func_080b7e7c(void) {
    s32 var_r0;
    s32 var_r6;
    void *temp_r0;

    var_r6 = 0;
    do {
        var_r0 = var_r6 + 0x78;
        if (var_r6 <= 7) {
            var_r0 = var_r6;
        }
        temp_r0 = Func_080b7dd0(var_r0);
        if ((temp_r0 != NULL) && (M2C_FIELD(temp_r0, s16 *, 0x28) != 0)) {
            Func_080090d0(M2C_FIELD(temp_r0, s32 *, 0));
            M2C_FIELD(temp_r0, s32 *, 0) = 0;
            M2C_FIELD(temp_r0, s16 *, 0x28) = 0;
        }
        var_r6 += 1;
    } while (var_r6 <= 0xD);
}
