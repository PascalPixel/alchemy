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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct Unk { u8 unk_00[0x16]; u16 unk_16 : 9; u16 unk_16b : 7; };

void Func_080a17c4(void *arg0) {
    if (arg0 != NULL) {
        M2C_FIELD(arg0, s8, 5) = 1;
        ((struct Unk *) arg0)->unk_16 = M2C_FIELD(arg0, u16, 6);
        M2C_FIELD(arg0, s8, 0x14) = M2C_FIELD(arg0, u16, 8);
        M2C_FIELD(arg0, s8, 0x17) = -0x3F & M2C_FIELD(arg0, s8, 0x17);
        M2C_FIELD(arg0, s8, 0x15) = -4 & M2C_FIELD(arg0, s8, 0x15);
    }
}
