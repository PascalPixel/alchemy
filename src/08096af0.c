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

s32 Func_080984c0();
s32 Func_08099738();
s32 Func_0809b648();

void Func_08096af0(void) {
    u32 temp_r3;

    temp_r3 = (s16) M2C_FIELD(*(void **)0x03001F30, s16 *, 0x1E);
    switch (temp_r3) {
    case 8:
        Func_080984c0();
        return;
    case 10:
        Func_08099738();
        return;
    case 16:
        Func_0809b648();
        return;
    }
}
