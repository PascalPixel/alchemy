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

void Func_08004278(void *arg0);

void Func_080947e4(void) {
    void *temp_r2;

    Func_08004278((void *)0x080944ED);
    Func_08004278((void *)0x08094545);
    temp_r2 = (void *)0x040000B0;
    M2C_FIELD(temp_r2, volatile u16 *, 0xA) = (u16) (0xC5FF & M2C_FIELD(temp_r2, volatile u16 *, 0xA));
    M2C_FIELD(temp_r2, volatile u16 *, 0xA) = (u16) (0x7FFF & M2C_FIELD(temp_r2, volatile u16 *, 0xA));
    (void) M2C_FIELD(temp_r2, volatile u16 *, 0xA);
}
