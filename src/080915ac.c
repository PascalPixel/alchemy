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

extern u8 Data_02000240[];
u8 *Func_08091560(void);

s32 Func_080915ac(void) {
    u8 value;
    if (Data_02000240[0x20A] == 0 || (value = Func_08091560()[2]) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}
