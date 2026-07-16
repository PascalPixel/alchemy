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

s32 Func_0808ec14();

s32 Func_0808ed4c(void) {
    register s32 base asm("r3");
    register s32 offset asm("r2");
    s32 temp_r0;

    temp_r0 = Func_0808ec14();
    if (temp_r0 == -1) {
        return 0;
    }
    base = *(s32 *)0x03001EBC;
    offset = temp_r0 * 8;
    return M2C_FIELD((base + offset), s32 *, 0x11C);
}
