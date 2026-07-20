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

u8 *Func_08077080(s32);
s32 Func_0808a488(u8);

s32 Func_080a5fe0(void) {
    u8 *r5;
    s32 temp_r3;
    s32 var_r0;

    r5 = Func_08077080((s32) (0x3FFF & *(u16 *)(*(u8 **)0x03001F2C + 0x178)));
    if (Func_0808a488(r5[0xC]) != 0) {
        return 0;
    }
    var_r0 = 2;
    if (r5[8] != 0xFF) {
        u8 b = r5[0];
        temp_r3 = b ^ 2;
        var_r0 = (0 - temp_r3) | temp_r3;
        var_r0 = (s32) ((u32) var_r0 >> 0x1F);
        var_r0 = 1 - var_r0;
    }
    return var_r0;
}
