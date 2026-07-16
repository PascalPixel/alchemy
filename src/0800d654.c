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

s32 Func_0800d654(void *arg0) {
    register s32 value asm("r3");
    register u8 *destination asm("r2");

    value = M2C_FIELD(((M2C_FIELD(arg0, s16 *, 4) * 4) + M2C_FIELD(arg0, s32 *, 0)), s32 *, 4);
    destination = arg0;
    asm volatile("" : "+r" (destination) : "r" (value));
    value -= 1;
    destination += 0x5E;
    *(s16 *)destination = value;
    M2C_FIELD(arg0, s16 *, 4) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 4) + 2);
    return 0;
}
