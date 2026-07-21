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

void Func_080c1084(void) {
    s32 _c0 = 0x03001E74;
    s8 *table;
    u16 temp_r2;
    s32 var_r1;
    void *temp_r0;

    temp_r0 = *(void **)_c0;
    if ((temp_r0 != NULL) && (M2C_FIELD(temp_r0, u16 *, 0x650) != 0)) {
        M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F90;
        M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x10;
        table = (s8 *)0x080C5C10;
        *(s16 *)0x04000054 = table[M2C_FIELD(temp_r0, u16 *, 0x64E)];
        temp_r2 = M2C_FIELD(temp_r0, u16 *, 0x64E);
        var_r1 = (temp_r2 + 1) & 0xF;
        if ((u32) temp_r2 > 0xEU) {
            var_r1 |= 0x10;
        }
        M2C_FIELD(temp_r0, u16 *, 0x64E) = var_r1;
    }
}
