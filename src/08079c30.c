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

s32 Func_08079b24(s32, s32);

s32 Func_08079c30(s32 arg0, s32 arg1, s32 arg2) {
    register s32 call_result asm("r0");
    register s32 inner asm("r3");
    register s32 saved_arg0 asm("r6") = arg0;
    register s32 saved_arg2 asm("r5") = arg2;
    s32 var_r0;

    call_result = Func_08079b24(arg1, 0);
    inner = saved_arg0;
    asm volatile("" : "+r" (inner) : "r" (call_result));
    inner *= call_result;
    call_result = saved_arg2;
    asm volatile("" : "+r" (call_result) : "r" (inner));
    call_result *= inner;
    var_r0 = call_result;
    if (var_r0 < 0) {
        var_r0 += 0xFFFF;
    }
    return var_r0 >> 0x10;
}
