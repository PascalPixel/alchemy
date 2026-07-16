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

s32 Func_080a23f4(s32, s32, s32, s32);

void Func_080a4eb8(void) {
    register s32 first asm("r0");

    first = M2C_FIELD(*(void **)0x03001F2C, s32 *, 0x10C);
    asm volatile(
        "sub sp, #4\n"
        "mov r3, #7\n"
        "str r3, [sp]"
        : "+r" (first)
        :
        : "r3", "memory");
    Func_080a23f4(first, 0xD, 0, 0x11);
    asm volatile("add sp, #4" : : : "memory");
}
