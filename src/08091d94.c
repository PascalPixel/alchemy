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

s32 Func_08015040(void *, s32);
s32 Func_08015120(s32, s32);
s32 Func_08077088();
s32 Func_080f9010(s32);
extern u8 Value_0000001e[];

void Func_08091d94(s32 arg0, s32 arg1) {
    register void *value asm("r0");
    register s32 three asm("r1");

    Func_08077088();
    Func_080f9010(0x53);
    Func_08015120(arg0, 1);
    Func_08015120(arg1, 4);
    value = Value_0000001e;
    asm volatile("" : "+r" (value));
    three = 3;
    asm volatile("" : "+r" (three) : "r" (value));
    Func_08015040(value, three);
}
