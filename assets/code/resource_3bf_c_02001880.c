typedef signed int s32;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006ed4(void);
Obj *Func_02006edc(s32);

s32 Func_02001880(void)
{
    Obj *a = Func_02006ed4();
    Obj *b = Func_02006edc(0);
    s32 az = a->z / 0x100000;
    s32 ax = a->x / 0x100000;
    s32 bz = b->z / 0x100000;
    s32 bx = b->x / 0x100000;
    s32 d = ax - bx;

    if (d < -6 || d > 6) {
        return 0;
    }
    if (az - 2 < bz && az + 2 > bz) {
        return 1;
    }
    return 0;
}
