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

void Func_08003810(void) {
    if (*(u16 *)0x02002000 == 0) {
        register u32 value asm("r2");
        value = 0xC3FF;
        asm volatile("" : "+r" (value));
        *(s16 *)0x04000132 = value;
        *(s8 *)0x03001CB8 = 1;
    }
}
