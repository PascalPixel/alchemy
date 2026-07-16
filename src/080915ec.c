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

extern u8 Data_03001f54;

void Func_080915ec(void) {
    register void *temp_r1 asm("r1");
    register u8 *flag asm("r3");
    register volatile s32 *keys asm("r0");
    register s32 mask asm("r2");
    register s32 bits asm("r3");
    register s32 *target asm("r2");

    temp_r1 = *(void **)0x03001EBC;
    flag = &Data_03001f54;
    asm volatile("" : "+r" (temp_r1), "+r" (flag));
    if (*flag != 0) {
        keys = (volatile s32 *)0x03001C94;
        mask = 0x80;
        bits = *keys;
        mask <<= 2;
        asm volatile("" : "+r" (keys), "+r" (mask), "+r" (bits));
        if (bits & mask) {
            M2C_FIELD(temp_r1, s32 *, 0x1CC) = 0;
        }
        bits = *keys;
        asm volatile("" : "+r" (bits) : "r" (keys));
        mask = 0x80;
        mask <<= 1;
        asm volatile("" : "+r" (keys), "+r" (mask), "+r" (bits));
        if (bits & mask) {
            target = (s32 *)((u8 *)temp_r1 + 0x1CC);
            asm volatile("" : "+r" (target));
            *target = -1;
        }
    }
}
