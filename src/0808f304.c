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

u32 Func_0808f304(void) {
    register s32 temp_r3_2 asm("r3");
    register u32 var_r0 asm("r0");
    register s32 offset asm("r2");

    temp_r3_2 = (s32)*(void **)0x03001EBC;
    var_r0 = 0;
    if (temp_r3_2 != 0) {
        offset = 0xCB8;
        asm volatile(
            "add %0, %0, %1\n\t"
            "mov %1, #0\n\t"
            "ldrsh %0, [%0, %1]"
            : "+r" (temp_r3_2), "+r" (offset));
        var_r0 = (u32) ((0 - temp_r3_2) | temp_r3_2) >> 0x1F;
    }
    return var_r0;
}
