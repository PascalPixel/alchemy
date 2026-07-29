typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006b22(s32);

s32 Func_020014cc(void)
{
    Obj *o = Func_02006b22(0);
    s32 zz = o->z / 0x100000;
    s32 xx = o->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}
