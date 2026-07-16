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

s32 Func_080f9010();

void Func_0809202c(void) {
    register s32 value asm("r0");
    register u8 *address asm("r3");
    register s32 offset asm("r2");
    register s32 minus_one asm("r3");

    address = *(u8 **)0x03001EBC + 0xCC8;
    offset = 0;
    asm volatile("ldrsh %0, [%1, %2]"
                 : "=r" (value) : "r" (address), "r" (offset));
    minus_one = 1;
    minus_one = -minus_one;
    if (value != minus_one) {
        Func_080f9010();
    }
}
