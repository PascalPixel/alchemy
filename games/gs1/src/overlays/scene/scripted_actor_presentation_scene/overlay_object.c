#include "types.h"

#define CreateOverlayObject Func_02005d1e
#define SetOverlayObjectMode Func_02005d88
#define SetOverlayObjectSlot Func_02005ed0
#define OverlayObject_SetRecordField1 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_StepScaleByCounter Func_02000314
#define OverlayObject_DecayRecordField1e Func_02000400
#define OverlayObject_UpdateHeadingTimer Func_02000424

#include "create_configured_overlay_object.h"

typedef struct {
    u8 filler0[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} Sub;

typedef struct {
    u8 filler0[0x50];
    Sub *sub;
} Obj;

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj_02000400;

typedef struct {
    u8 filler0[0x50];
    Obj_02000400 *obj;
} Spr_02000400;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr_02000424;

s32 Func_02005f8e(Spr *);
s32 Func_02005f74(s32, s32);
s32 Func_0200604c(void);
s32 Func_0200605a(void);

void OverlayObject_SetRecordField1(Obj *o, u32 v)
{
    o->sub->f1 = v;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

s32 OverlayObject_StepScaleByCounter(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = Func_02005f74(Func_02005f8e(s), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

void OverlayObject_DecayRecordField1e(Spr_02000400 *s)
{
    Obj_02000400 *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OverlayObject_UpdateHeadingTimer(Spr_02000424 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Func_0200604c() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Func_0200605a() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}
