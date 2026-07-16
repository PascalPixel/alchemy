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

void Func_0808b9f8(void) {
    register s32 *var_r3 asm("r3");
    register s32 var_r2 asm("r2");
    register s32 var_r1 asm("r1");
    register s32 var_r0 asm("r0");

    var_r0 = 0x8C;
    var_r1 = 0;
    var_r2 = 0x41;
    var_r3 = (s32 *)((u8 *)*(void **)0x03001EBC + (var_r0 << 1));
    do {
        var_r2 -= 1;
        *var_r3 = var_r1;
        var_r3 -= 1;
    } while (var_r2 >= 0);
}
