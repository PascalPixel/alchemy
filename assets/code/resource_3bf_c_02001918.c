typedef signed int s32;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006f6c(void);
Obj *Func_02006f74(s32);

s32 Func_02001918(void)
{
    Obj *a = Func_02006f6c();
    Obj *b = Func_02006f74(0);
    s32 az = a->z / 0x100000;
    s32 ax = a->x / 0x100000;
    s32 bz = b->z / 0x100000;
    s32 bx = b->x / 0x100000;
    s32 dx = ax - bx;
    s32 dz;

    az += 1;
    if (dx < 0) {
        dx = -dx;
    }
    dz = az - bz;
    if (dz < 0) {
        dz = -dz;
    }
    if (dx + dz <= 4) {
        return 1;
    }
    return 0;
}
