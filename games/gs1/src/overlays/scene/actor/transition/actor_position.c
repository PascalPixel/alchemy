#include "types.h"

#define SceneActor_PlacePairAtOffset Func_02000180
#define SceneActor_PlaceAtTileAndMark Func_02001004
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_39a owner at 0x02001004, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_39a {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

extern s32 Data_02000240[];

Obj *Func_020024f2(s32);
Obj *Func_020024fa(s32);
void Func_020024e8(void);
void Func_020024b2(Obj *, s32, s32, s32);
void Func_02002492(Obj *, s32);
void Func_020024de(Obj *, s32, s32, s32);
void Func_020024ca(Obj *, s32);
void Func_020024d4(Obj *, s32);
void Func_0200250a(Obj *);
void Func_0200257e(void);
struct Rec_39a *Func_02003362();
void Func_020033b8();

void SceneActor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020024f2(Data_02000240[125]);
    q = Func_020024fa(a0);
    Func_020024e8();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020024b2(p, x, p->f0c, y);
    }
    Func_02002492(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020024de(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020024ca(q, 4);
    } else {
        Func_020024d4(q, 3);
    }
    Func_0200250a(p);
    Func_0200257e();
}

void SceneActor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Func_02003362(id);

    if (rec != 0) {
        Func_020033b8(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}
