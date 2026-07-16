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

void *Func_08077080(s32);
s32 Func_0808a488(u8);

s32 Func_080a5fe0(void) {
    register s32 temp_r3 asm("r3");
    register s32 temp_r2 asm("r2");
    register u32 var_r0 asm("r0");
    void *temp_r0;

    temp_r0 = Func_08077080(0x3FFF & M2C_FIELD(*(void **)0x03001F2C, u16 *, 0x178));
    if (Func_0808a488(M2C_FIELD(temp_r0, u8 *, 0xC)) != 0) {
        return 0;
    }
    var_r0 = 2;
    if (M2C_FIELD(temp_r0, u8 *, 8) != 0xFF) {
        temp_r3 = M2C_FIELD(temp_r0, u8 *, 0);
        temp_r2 = 2;
        temp_r3 ^= temp_r2;
        var_r0 = -temp_r3;
        var_r0 |= temp_r3;
        var_r0 >>= 0x1F;
        temp_r3 = 1;
        var_r0 = temp_r3 - var_r0;
    }
    return (s32) var_r0;
}
