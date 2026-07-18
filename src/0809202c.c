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

s32 Func_080f9010(s32);

void Func_0809202c(void) {
    s16 value = *(s16 *)(*(u8 **)0x03001EBC + 0xCC8);
    if (value != -1) {
        Func_080f9010(value);
    }
}
