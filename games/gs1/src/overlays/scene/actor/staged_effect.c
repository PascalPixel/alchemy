#include "types.h"
#include "scene.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_effect/staged_effect.c */
/* overlays/scene/actor/staged_effect/actor_facing.c */
u8 *Actor_unk6_4(s32 id);

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 Actor_FaceLeaderWhileGrounded(u8 *object)
{
    u8 *leader = Actor_unk6_4(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Actor_Apply(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Actor_unk7_3(3, 0xc000, 0);
    }
    return 0;
}

/* overlays/scene/actor/staged_effect/effect.c */
/*
 * resource_39c owner at 0x020051b0, 64 bytes.
 * Advances a frame counter and, while it is still within 31, reseeds the two
 * accumulators from the counter and re-anchors the position on the source
 * record. Returns whether the step ran.
 */
struct Src_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct Src_39c *f104;       /* +104 */
};

s32 Effect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
{
    struct Src_39c *source = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Actor_unk9(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = source->f8;
    work->f12 += 0x10000;
    work->f16 = source->f16;
    return 1;
}

/* overlays/scene/actor/staged_effect/overlay_object.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *AcquireOverlayObject(s32, s32, s32, s32);

u8 *Actor_unk7_4();

u8 **Actor_unk8_4(s32, s32);

void *OvObj_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* Spawn and configure the compact companion object at a source position. */
void OvObj_SpawnKind24AtActor(u8 *src)
{
    u8 *obj = Actor_unk7_4(24, *(int *)(src + 8),
                              *(int *)(src + 12), *(int *)(src + 16));
    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        Actor_unk9_4(obj, (void *)0x0200a7b8);
        obj[85] = 0;
        obj[34] = 1;
        obj[35] = 2;
        if (rec != 0) {
            Actor_unk10_4(rec, 2);
            rec[38] = 0;
            rec[9] |= 0x0c;
        }
    }
}

s32 OvObj_RampWords24And28Over16Frames(u8 *obj)
{
    u16 *cnt = (u16 *)(obj + 100);
    s16 n;

    *cnt = *cnt + 1;
    n = (s16)*cnt;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(obj + 24) = (n * 3) << 10;
    *(s32 *)(obj + 28) = (n * 3) << 10;
    return 1;
}

s32 OvObj_AdvanceScaleCounter(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}

/* Release the optional published attachment; complete owner, no pool. */
void OvObj_ReleasePublishedAttachment(void)
{
    u8 **pub = Actor_unk8_4(35, 4);
    u8 *state;
    u8 *obj;

    if (pub == 0)
        return;
    state = *pub;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;
    Actor_unk4_2(obj);
    *(u8 **)(state + 20) = 0;
}

/* overlays/scene/actor/staged_effect/scene_audio.c */

/* Play the footprint-motion completion cue. */

s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Actor_unk5_2(118);
    return 0;
}

/* overlays/scene/actor/staged_effect/scene_data.c */

/* Contiguous unnamed leaf-owner run for resource_39b. */
void *SceneData_GetTablea928(void) { return (void *)0x0200a928; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetTableabf8(void) { return (void *)0x0200abf8; }

void *SceneData_GetTableac58(void) { return (void *)0x0200ac58; }

/* Apply the overlay's common actor-0 presentation preset. */
void Scene_RunStepWithValue1632(void)
{
    Actor_unk11_4();
    Actor_unk12_4(0, 1);
    Actor_unk13_4(0x1632, 1);
    Actor_unk14_4();
}

/* overlays/scene/actor/staged_effect/scene_primary_script.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 gWork[];

/* A value-returning call sets r0 last of its arguments. */

void Scene_RunScene39b(s32 a0)
{
    s32 value;
    s32 magic;
    s32 base5_3001e40;
    s32 none;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    base5_3001e40 = 0x3001e40;
    if ((*(volatile s32 *)base5_3001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    none = 0;
    FIELD(rec, s32, 0) = none;
    value = Actor_unk10();
    magic = -((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) + ((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) << 8));
    Actor_unk15_4((*(volatile s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)base5_3001e40 & 15)) << 16)), (*(volatile s32 *)(a0 + 12) + 0x1a0000), *(volatile s32 *)(a0 + 16), none, magic, 0, 0xb0000, rec);
    return 0;
}

void Scene_RunScene39b(s32 a0)
{
    u32 i;
    s32 record;

    Actor_unk16_4();
    Actor_unk17_4(228);
    Actor_Place(0, 0x6666, 0x3333);
    Actor_unk18_4(0, 2);
    Actor_unk2_3(0, 0, -8);
    record = Actor_unk11(0);
    Actor_unk19_4(record, 0);
    Actor_unk20_4(8);
    Actor_unk21_4(0, ((a0 << 19) + 0x80000), 0);
    Actor_unk22_4(30);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = *(volatile s32 *)(Actor_Check(0) + 8) / 0x100000;
    b = *(volatile s32 *)(Actor_unk2(0) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Actor_unk23_4();
        Actor_Run(0x10000, 0);
        Actor_unk24_4(60);
        Actor_unk25_4(120);
        Actor_unk2_4(0x10005, 1);
        Actor_unk26_4(60);
        Actor_unk27_4(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = 0x200af4c;
            Actor_unk28_4(246);
            x = 208 - ((u32)(Actor_unk12() << 4) >> 16);
            y = 560 - ((u32)(Actor_unk13() << 4) >> 16);
            t = ((u32)(Actor_unk3_4() << 2) >> 16);
            record = Actor_unk14((((t << 4) - t) << 16) + 0x3c0000, 100);
            Actor_unk29_4(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Actor_unk30_4(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Actor_unk31_4(220);
        Actor_unk32_4(60);
        Actor_Do(0x875);
        Actor_unk3(0x2008d99, 0xc80);
        Actor_SetRect(37, 98, 10, 97, 5, 3);
        Actor_unk2_5(70, 32, 13, 7, 6, 32);
        Actor_unk4_4(0x10000, 0);
        Actor_unk33_4(60);
        Actor_unk34_4(120);
        Actor_unk35_4();
    }
}

void Scene_RunScene39b(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Actor_unk4(0);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    Actor_unk36_4(0x205);
    if (quotient == 7) {
        Actor_unk37_4(8, 0x200a874);
    } else {
        Actor_unk38_4(8, 0x200a8c8);
    }
}

void Scene_RunScene39b(void)
{
    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Actor_unk5(0);
    rec8 = Actor_unk6(0x109);
    if (rec8 == 0) {
        Actor_unk39_4();
        Actor_unk5_4(-1, -1, -1, 0);
        rec7[85] = rec8;
        Actor_unk3_3(0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Actor_unk40_4(0, 15);
        record = Actor_unk15(0);
        Actor_unk41_4(record, 0);
        Actor_unk42_4();
        Actor_unk43_4();
        Actor_unk44_4(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cc1;
        Actor_unk4_3(0, 0x6666, 0x3333);
        Actor_unk45_4(0, 0, 8);
        Actor_unk46_4(0, 0);
        record = Actor_unk16(0);
        Actor_unk47_4(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Actor_unk48_4(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Actor_unk49_4();
        Actor_unk50_4();
    }
}

void Scene_RunScene39b(void)
{
    u32 i;
    s32 record;

    if (Actor_unk7(0x250) == 0) {
        Actor_unk2_2(0x250);
        Actor_unk51_4();
        record = Actor_unk17(12);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Actor_unk8(13);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Actor_unk18(14);
        *(volatile s32 *)(record + 24) = -0x10000;
        Actor_unk5_3(3, 0x880000, 0x900000);
        Actor_unk6_3(3, 0x4000, 10);
        *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
        Actor_unk52_4();
        Actor_unk53_4();
        Actor_unk54_4(60);
        Actor_unk55_4(3, 0, 0);
        Actor_unk56_4(3, 3);
        Actor_unk57_4(30);
        Actor_unk58_4(3, 136, 72);
        Actor_unk59_4(40);
        Actor_unk60_4(0, 1);
        Actor_unk61_4(3);
        Actor_unk62_4(3, 0, 0);
        Actor_unk3_2(0x872);
        *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x204;
        Actor_unk63_4();
    }
}

/* overlays/scene/actor/staged_effect/scene_setup.c */

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */

void Scene_RunIndexedStep17(void)
{
    Actor_unk6_2(17);
}

void Scene_RunIndexedStep18(void)
{
    Actor_unk7_2(18);
}

void Scene_RunIndexedStep19(void)
{
    Actor_unk8_2(19);
}

void *SceneData_GetTablead60(void) { return (void *)0x0200ad60; }

void Scene_RunIndexedStep63(void)
{
    Actor_unk64_4(63);
}

void Scene_RunActor8StepWithTableA820(void)
{
    Actor_unk65_4(0x205);
    Actor_unk66_4(8, (void *)0x0200a820);
}

/* overlays/scene/actor/staged_effect/scene_wrapper.c */

void Scene_CallHelper(void)
{
    Actor_unk67_4();
}

void Scene_CallHelper2(void)
{
    Actor_unk68_4();
}

/* overlays/scene/actor/staged_effect/shared.c */

void Scene_Forward(void)
{
    Actor_unk69_4();
}

void Scene_RunSingleStep(void)
{
    Actor_unk70_4();
}

/* overlays/scene/actor/staged_effect/staged_actor_effect_sequence.c */
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

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

extern s32 gOv[];
extern s32 gOv2[];
extern u8 Value;
extern s32 **gIw;

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

Ent *Actor_unk71_4(Desc *, Ent *);

u8 *Actor_unk72_4(s32);

/*
 * Staged actor effect sequence for resource_39b.  gIw is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 StagedActorStepTable[];

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

s32 stop_blocked_actor_motion(Ent *a)
{
    extern s32 StagedActorStepTable[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = StagedActorStepTable[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Actor_unk71_4(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = StagedActorStepTable[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Actor_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 find_clear_actor_position(s32 *a)
{
    extern s32 StagedActorStepTable[];

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
    obj = (u8 *)Actor_unk8_3(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)gOv2;
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
        d = StagedActorStepTable[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (StagedActorStepTable[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)gOv2;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)gOv2;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Actor_Apply2(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (StagedActorStepTable[sel] & 0xffff0000);
        buf[2] = buf[2] + (StagedActorStepTable[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = StagedActorStepTable[sel];
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

    Actor_unk75_4();
    Actor_unk9_2(0xe4);
    F(v, s32, 0x6c) = (s32)&Value;
    F(v, s32, 0x30) = 0x3333;
    Actor_Apply3(0, 2);
    Actor_unk9_3(0, 0, -6);
    Actor_unk10_2(0);
    Actor_Apply4(0, 15);
    Actor_Apply5(Actor_unk19(0), 0);
    F(v, s32, 0x6c) = 0;
    Actor_unk11_2(30);
    Actor_unk76_4();
    Actor_unk77_4();
    Actor_unk12_2(a);
    Actor_unk78_4();
}

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Actor_Apply6 and
 * Actor_Apply7 are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void clear_linked_scene_record(void)
{
    s32 *record = *gIw;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    Actor_unk13_2(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Actor_Apply6(target, (s32)0x0200a7dc);
        Actor_Apply7(target, 7);
        record[5] = 0;
    }
}

/* overlays/scene/actor/staged_effect/move_and_redraw.c */
#define ValueMoveAndRedraw Value2

#include "staged_actor_movement.h"

void Actor_Run(
    StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
