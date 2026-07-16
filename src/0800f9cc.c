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

void Func_0800f9cc(u8 *arg0, s32 arg1) {
    register s32 var_r1 asm("r1");
    register u8 *var_r0 asm("r0");
    register const u8 *table asm("r2");
    register s32 sentinel asm("r3");

    var_r0 = arg0;
    sentinel = -1;
    var_r1 = arg1 - 1;
    table = (const u8 *)0x080097B8;
    if (var_r1 != -1) {
        do {
            var_r1 -= 1;
            *var_r0 = table[*var_r0];
            var_r0 += 1;
        } while (var_r1 != -1);
    }
}
