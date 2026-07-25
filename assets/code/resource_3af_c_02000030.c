typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_020042e6(s32, s16);
s32 Func_020042f8(void);

s32 Func_02000030(void *arg0) {
    switch (M2C_FIELD(arg0, s16, 0x64)) {
    case 6:
        M2C_FIELD(arg0, s32, 0x18) += (s32) 0xFFFFC000;
        M2C_FIELD(arg0, s32, 0x1C) += 0x2000;
        break;
    case 4:
        M2C_FIELD(arg0, s32, 0x18) += 0x2000;
        /* The load at 0x0200006A owns the Thumb-like data word at 0x020000AC. */
        M2C_FIELD(arg0, s32, 0x1C) += (s32) 0xF856F001;
        break;
    case 2:
        M2C_FIELD(arg0, s32, 0x18) += 0x1000;
        M2C_FIELD(arg0, s32, 0x1C) += (s32) 0xFFFFF800;
        break;
    case 0:
        M2C_FIELD(arg0, s32, 0x18) = 0x10000;
        M2C_FIELD(arg0, s32, 0x1C) = 0x10000;
        M2C_FIELD(arg0, s16, 0x64) = (s16) (Func_020042e6(Func_020042f8(), 90) + 60);
        break;
    }
    M2C_FIELD(arg0, s16, 0x64)--;
    return 1;
}
