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

extern u8 Data_02000240[];

void Func_0801ce90(s32 arg0) {
    u8 *sp;
    u16 sel;
    s32 off;

    sel = M2C_FIELD(arg0, u16 *, 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = &Data_02000240[off];
        break;
    case 1:
        off = 0x205;
        sp = &Data_02000240[off];
        break;
    case 2:
        off = 0x206;
        sp = &Data_02000240[off];
        break;
    default:
        return;
    }
    if (*sp) {
        (*sp)--;
    }
}
