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

extern void *Func_08077008(s32);
extern s32 Func_080c2454(s32);
extern s32 Func_080c23c0(s32);

u32 Func_080b8530(s32 arg0) {
    u32 var_r3;
    u8 b;

    b = M2C_FIELD(Func_08077008(arg0), u8 *, 0x128);
    var_r3 = (u32) (Func_080c2454((s32) b) << 0x18) >> 8;
    if (var_r3 == 0) {
        b = M2C_FIELD(Func_08077008(arg0), u8 *, 0x128);
        if (Func_080c23c0((s32) b) != 0) {
            var_r3 = 0x180000;
        } else {
            var_r3 = 0x300000;
        }
    }
    return var_r3;
}
