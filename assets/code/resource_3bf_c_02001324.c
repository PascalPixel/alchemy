#include "types.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_0200697a(s32);
Obj *Func_02006982(s32);

s32 Func_02001324(void)
{
    Obj *o = Func_0200697a(0);
    Obj *p = Func_02006982(17);
    s32 ox = o->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = p->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = p->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}
