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

s32 Func_02003b7e();
s32 Func_02003bac();
s32 Func_02003bba();
u8 Func_02003bcc(void *, s32);
u8 Func_02003bd6(void *, s32);

s32 Func_02000040(void *arg0) {
    u32 temp_r0;
    u32 temp_r3;

    if (M2C_FIELD(arg0, s16 *, 0x66) == 0) {
        temp_r0 = (u32) (Func_02003b7e() * 8) >> 0x10;
        switch (temp_r0) {
        case 0:
            Func_02003bcc(arg0, 3);
            break;
        case 1:
            Func_02003bd6(arg0, 4);
            break;
        case 3:
        case 4:
            M2C_FIELD(arg0, u16 *, 6) += (u32) (Func_02003bac() << 0xF) >> 0x10;
            break;
        }
        temp_r3 = (u32) (Func_02003bba() * 0x50) >> 0x10;
        M2C_FIELD(arg0, s16 *, 0x66) = (s16) temp_r3;
        if (temp_r3 != 0) {
            goto block_10;
        }
    } else {
block_10:
        --M2C_FIELD(arg0, s16 *, 0x66);
    }
    return 1;
}
