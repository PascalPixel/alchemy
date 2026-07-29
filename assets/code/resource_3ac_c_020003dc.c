typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern u8 Data_02000240[];
extern u8 Value_00000069;
extern void Func_020008a6(s32);
extern s32 Func_020008d6(s32);
extern void Func_020008ac(s32, s32);
extern s32 Func_020008e2(s32);
extern void Func_020008b8(s32, s32);
extern s32 Func_020008ee(s32);
extern void Func_020008c4(s32, s32);

s32 Func_020003dc(void)
{
    u8 *state;
    s32 *dst;
    u8 *tbl;
    s32 off;
    s32 v;
    s32 n;

    state = *(u8 **)0x03001ebc;
    dst = (s32 *)(state + 0x1c0);
    off = 0x209;
    *dst = off;
    tbl = Data_02000240;
    v = *(s16 *)(tbl + off - 71);
    if (v == 10) {
        Func_020008a6(0x12f);
        n = (s32)&Value_00000069;
        *(u16 *)(tbl + 0x1c4) = n;
        *(u16 *)(tbl + 0x1c6) = v;
    }
    Func_020008ac(Func_020008d6(23), 0);
    Func_020008b8(Func_020008e2(24), 0);
    Func_020008c4(Func_020008ee(25), 0);
    return 0;
}
