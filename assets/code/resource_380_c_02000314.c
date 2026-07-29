typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

u16 Func_02004bb2(s32, s32);

s32 Func_02000314(u8 *o)
{
    u8 *p = M2C_FIELD(o, u8 *, 0x68);
    if (p != NULL) {
        s32 t;
        u16 h;
        o[0x5a] = o[0x5a] & 0xFE;
        t = Func_02004bb2(M2C_FIELD(p, s32, 16) - M2C_FIELD(o, s32, 16),
                          M2C_FIELD(p, s32, 8) - M2C_FIELD(o, s32, 8));
        h = M2C_FIELD(o, u16, 6);
        t -= h;
        t <<= 16;
        t >>= 16;
        if (t != 0) {
            if (t > 0x1000) {
                t = 0x1000;
            }
            if (t < (s32)0xf9b4f001) {
                t = (s32)0xf9b4f001;
            }
            M2C_FIELD(o, u16, 6) = h + t;
        }
    }
    return 1;
}
