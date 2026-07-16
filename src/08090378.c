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

void Func_08090378(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r4;

    temp_r4 = *(void **)0x03001E70;
    if (temp_r4 != NULL) {
        if (arg2 != 0) {
            M2C_FIELD(temp_r4, u16 *, 0x14) = (u16) (0xFDFF & M2C_FIELD(temp_r4, u16 *, 0x14));
        }
        if (arg1 != 0) {
            M2C_FIELD(temp_r4, u16 *, 0x14) = (u16) (0xFBFF & M2C_FIELD(temp_r4, u16 *, 0x14));
        }
        if (arg0 != 0) {
            M2C_FIELD(temp_r4, u16 *, 0x14) = (u16) (0xF7FF & M2C_FIELD(temp_r4, u16 *, 0x14));
        }
    }
}
