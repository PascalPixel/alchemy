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

void Func_08077088(void);
void Func_080f9010(s32);
void Func_08015120(s32, s32);
void Func_08015040(void *, s32);
extern u8 Value_0000001e;

void Func_08091d94(s32 arg0, s32 arg1) {
    Func_08077088();
    Func_080f9010(0x53);
    Func_08015120(arg0, 1);
    Func_08015120(arg1, 4);
    Func_08015040(&Value_0000001e, 3);
}
