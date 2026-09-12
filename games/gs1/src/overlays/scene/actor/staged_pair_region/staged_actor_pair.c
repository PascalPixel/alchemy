#include "types.h"

#define EffectDescriptorTable Data_02009f60
#define GetPartyEffect Func_02001e1a
#define SpawnEffect Func_02001dc8
#define SetEffectVariant Func_02001dd2
#define SetEffectDescriptor Func_02001dec
#define SetEffectMode Func_02001f26
#define ScaleEffectDeltaFromAccumulated Func_02001e90
#define ScaleEffectDeltaFromOrigin Func_02001ea8
#define ScaleEffectVerticalDelta Func_02001eb6
#define SetEffectCallbackMode Func_02001eec
#define SetEffectCallbackArgument Func_02001efc
#define StagedActorStepTable Data_02009e48
#define GetStagedActor Func_02002074
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_020020f8
#define SetStagedActorMode Func_020020d0
#define SelectStagedActorSlot Func_020020c0
#define StartStagedActorEffect Func_02002206
#define StartNextStagedActorMove Func_0200210e
#define StartLeadStagedActorMove Func_0200211e
#define FinishStagedActorMove Func_0200212c
#define FinishStagedActorEffect Func_02002228
#define SetStagedActorTransition Func_0200213c
#define SpawnConfiguredEffect Func_0200013c
#define AdvanceStagedActorPair Func_020003a8

#include "configured_effect_spawn.h"
#include "staged_actor.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

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

extern s32 Data_02009e88[];
extern s32 Data_02009ea0[];
extern s32 Data_02000240[];

struct StagedActor *Func_02002074(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_020020f8(struct StagedActor *arg0, s32 *arg1);
void Func_020020d0(struct StagedActor *arg0, s32 arg1);
void Func_020020c0(s32 arg0);
void Func_02002206(s32 arg0);
void Func_0200210e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200211e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200212c(struct StagedActor *arg0);
void Func_02002228(void);
void Func_0200213c(struct StagedActor *arg0, s32 arg1);
Ent *Func_0200090a(Desc *, Ent *);
s32 Func_0200227a(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_020024b4(u8 *, s32 *);
Obj *Func_020034b6_a();
Obj *Func_020034b6_b();
Obj *Func_020034be(s32);
void Func_020034b4(void);
void Func_02003496(Obj *, s32, s32, s32);
void Func_0200347e(Obj *, s32);
void Func_020034c2(Obj *, s32, s32, s32);
void Func_020034c0(Obj *, s32);
void Func_020034ee(Obj *);
void Func_0200354a(void);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void AdvanceStagedActorPair(void)
{
    extern u32 Data_02009e48[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 Func_0200058c(Ent *a)
{
    extern s32 Data_02009e48[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_02009e48[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_0200090a(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_02009e88;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_02009e48[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_0200227a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 Func_02000758(s32 *a)
{
    extern s32 Data_02009e48[];

    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Func_02000da6(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Data_02009ea0;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = Data_02009e48[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_02009e48[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_02009ea0;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_02009ea0;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_020024b4(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_02009e48[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_02009e48[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_02009e48[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

void Func_020017d8(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020034b6_a(Data_02000240[125]);
    q = Func_020034be(a0);
    Func_020034b4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003496(p, x, p->f0c, y);
    }
    Func_0200347e(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020034c2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020034b6_b(q, 4);
    } else {
        Func_020034c0(q, 3);
    }
    Func_020034ee(p);
    Func_0200354a();
}
