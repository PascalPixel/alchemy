#include "types.h"

#define OverlayObject_UpdateEveryFourFrames Func_020005ac
#define OverlayObject_ApplyZero Func_02000690
#define OverlayObject_WaitUntilIdle Func_0200094c
#define OverlayObject_ResetObjectWhenFlags12Set Func_02001a50
struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

typedef struct {
    u8 filler0[9];
    u8 unk9;
} Struct_Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[48];
    s32 unk44;
    u8 filler48[8];
    Struct_Sub *unk50;
} Struct_1a50;

void Func_020054fa(void *, s32);
void Func_02005502(void *, s32);
void Func_020055d4(s32);
void Func_020054da(void *, s32);
void Func_020056fe(s32);
s32 Func_02001ad4(s32, s32, s32, s32);
void Func_020023d0(Struct_1a50 *);
void Func_0200687a(s32);
void Func_02003454(void);

s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)0x03001e40 & 3) == 0)
        Func_020054fa(obj, 7);
    else
        Func_02005502(obj, 0);

    if ((*(u32 *)0x03001e40 & 7) == 0)
        Func_020055d4(138);
    return 0;
}

s32 OverlayObject_ApplyZero(void *obj)
{
    Func_020054da(obj, 0);
    return 0;
}

void OverlayObject_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Func_020056fe(1);
        busy = obj->busy;
        cnt--;
        if (busy == 0) break;
    }
}

void OverlayObject_ResetObjectWhenFlags12Set(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = Func_02001ad4(o->unk8, 0, 0x2000000, 223);
        Func_020023d0(o);
        o->unk8 = z;
        o->unk10 = z;
        Func_0200687a(t);
    } else {
        Func_02003454();
    }
}
