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

s32 Func_08002f40();
s32 Func_08005340(s32, s32);
s32 Func_080072f0(s32, s32, s32, s32);

void Func_080e0524(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_r6;

    var_r6 = Func_08002f40();
    if (arg3 != 0) {
        register s32 first asm("r0");
        register s32 fourth asm("r3");
        first = 0xA0;
        asm volatile("" : "+r" (first));
        fourth = 0x03001388;
        asm volatile("" : "+r" (fourth) : "r" (first));
        first <<= 19;
        Func_080072f0(first, var_r6, 0x80, fourth);
    }
    if (arg2 != 0) {
        var_r6 += 0x80;
    }
    Func_08005340(var_r6, arg1);
}
