#include "types.h"

#define EffectDescriptorTable Data_0200ad58
#define GetPartyEffect Func_02002c7a
#define SpawnEffect Func_02002bf8
#define SetEffectVariant Func_02002c02
#define SetEffectDescriptor Func_02002c1c
#define SetEffectMode Func_02002d86
#define ScaleEffectDeltaFromAccumulated Func_02002ca8
#define ScaleEffectDeltaFromOrigin Func_02002cc0
#define ScaleEffectVerticalDelta Func_02002cce
#define SetEffectCallbackMode Func_02002d1c
#define SetEffectCallbackArgument Func_02002d2c
#define StagedActorStepTable Data_0200aca0
#define GetStagedActor Func_02002ed4
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02002f50
#define SetStagedActorMode Func_02002f00
#define SelectStagedActorSlot Func_02002ed8
#define StartStagedActorEffect Func_0200305e
#define StartNextStagedActorMove Func_02002f46
#define StartLeadStagedActorMove Func_02002f56
#define FinishStagedActorMove Func_02002f64
#define FinishStagedActorEffect Func_02003080
#define SetStagedActorTransition Func_02002f6c
#define SpawnConfiguredEffect Func_0200013c
#define AdvanceStagedActorPair Func_020003a8
#define SceneActor_ApplyPlacementQueryAndTag Func_02000ec8
#define SceneActor_ApplyPlacementQuery Func_02000f58

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

extern s32 Data_0200ace0[];
extern s32 Data_0200acf8[];

struct StagedActor *Func_02002ed4(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_02002f50(struct StagedActor *arg0, s32 *arg1);
void Func_02002f00(struct StagedActor *arg0, s32 arg1);
void Func_02002ed8(s32 arg0);
void Func_0200305e(s32 arg0);
void Func_02002f46(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f56(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f64(struct StagedActor *arg0);
void Func_02003080(void);
void Func_02002f6c(struct StagedActor *arg0, s32 arg1);
Ent *Func_0200090a(Desc *, Ent *);
s32 Func_020030d2(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_0200330c(u8 *, s32 *);
u8 *Func_020039ec();
s32 Func_02001cc4();
void Func_020039c8();
void Func_02003970();
void Func_020039d8();
void Func_020039ee();
u8 *Func_02003a7c();
s32 Func_02001d54();
void Func_02003a5c();
void Func_020014d4();
void Func_02003a18();

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. Func_02001cc4 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. Func_02001d54 is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void AdvanceStagedActorPair(void)
{
    extern u32 Data_0200aca0[];

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
    extern s32 Data_0200aca0[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200aca0[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_0200090a(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200ace0;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200aca0[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_020030d2(a, &d) > 0) {
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
    extern s32 Data_0200aca0[];

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
    E = (u8 *)Data_0200acf8;
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
        d = Data_0200aca0[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_0200aca0[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_0200acf8;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_0200acf8;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_0200330c(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_0200aca0[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_0200aca0[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_0200aca0[sel];
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

s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Func_020039ec(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Func_02001cc4(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Func_020039c8(2, 2, out20, out16, r2, r4);

    Func_02003970(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Func_020039d8(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Func_020039ee(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Func_02003a7c(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Func_02001d54(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Func_02003a5c(x, z, out20, out16, rec[2], rec[4]);
        Func_020014d4(0, rec[2], rec[4], out20, out16, 255);
    }

    Func_02003a18(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}
