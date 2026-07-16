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

void Func_080c9138(void) {
    s32 temp_r3;
    void *temp_r4;

    temp_r4 = *(void **)0x03001EEC;
    temp_r3 = M2C_FIELD(temp_r4, s32 *, 0x7790) + 1;
    M2C_FIELD(temp_r4, s32 *, 0x7790) = temp_r3;
    if (temp_r3 == M2C_FIELD(temp_r4, s32 *, 0x7794)) {
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (s32) M2C_FIELD(temp_r4, s32 *, 0x77D0);
        M2C_FIELD((void *)0x04000028, s32 *, 4) = (s32) M2C_FIELD(temp_r4, s32 *, 0x77D4);
        M2C_FIELD(temp_r4, s32 *, 0x77D0) = (s32) (M2C_FIELD(temp_r4, s32 *, 0x77D0) + M2C_FIELD(temp_r4, s32 *, 0x7798));
        M2C_FIELD(temp_r4, s32 *, 0x77D4) = (s32) (M2C_FIELD(temp_r4, s32 *, 0x77D4) + M2C_FIELD(temp_r4, s32 *, 0x779C));
        M2C_FIELD(temp_r4, s32 *, 0x7790) = 0;
    }
}
