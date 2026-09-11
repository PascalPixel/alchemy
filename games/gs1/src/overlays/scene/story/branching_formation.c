#include "types.h"
#include "scene.h"
#include "staged_actor.h"
#include "staged_actor_probe.h"
#include "staged_actor_effect.h"
#include "staged_actor_movement.h"

/* overlays/scene/story/branching_formation/branching_formation.c */
/* overlays/scene/story/branching_formation/calculate_fixed.c */

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

struct PlacementResult {
    s32 words[6];
};

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Sub { u8 pad00[9]; u8 f09; u8 pad0a[28]; u8 f26; };

struct Obj {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

struct Struct288c {
    u8 pad00[8];
    s32 field08;
    u8 pad0c[4];
    s32 field10;
};

struct Struct5702 {
    u8 pad00[0x0c];
    s32 field0c;
    u8 pad10[0x55 - 0x10];
    u8 field55;
};

extern u8 *gWork;
extern u8 *gCam;
extern u8 gUnk[];
extern s32 gOv[];
extern s32 gOv2;
extern u8 gOv3[];
extern s32 gOv4;
extern s32 gOv5;
extern u8 gOv6[];

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *GetStagedActor(s32 actor_index);
struct StagedActor *FindNextStagedActor(s32 *position, struct StagedActor *actor);
struct StagedActor *FindBlockingStagedActor(s32 *position, struct StagedActor *actor);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *position, struct StagedActor *actor);

Ent *Story_unk7_4(Desc *, Ent *);

u8 *Story_unk8_4(s32);

u8 *Story_unk9_4(s32);

struct StagedActorEffect *GetStagedActorEffect(s32 actor_index);

struct Struct3848 *Story_unk10_4(s32 arg0);

struct Obj *Story_unk11_4(s32, s32, s32, s32);

struct Struct5702 *Story_unk12_4(s32 arg0);
struct Struct5702 *Story_unk13_4(s32 arg0);
struct Struct5702 *Story_unk14_4(s32 arg0);
struct Struct5702 *Story_unk15_4(s32 arg0);

/*
 * Distance between two three-component 16.16 fixed-point positions.  Each
 * argument walks three consecutive words in x, y, z order; the per-axis deltas
 * are shifted down to integers, squared, summed, and passed to the resident
 * IWRAM integer square root.  The walking-pointer form is what reproduces the
 * reference and must not become struct field access.
 */

/* Clear the pending object cell after restoring its mode. */

/*
 * The clamp store takes its zero from a local.  Written as a literal it is
 * materialised from the literal pool, which changes the store's source
 * register and costs a pool word.  The owner ends at 0x02000a4a; the veneer
 * that follows is not part of it.
 */

/* Placement query followed by the tile-(10,20) scene transition. */

/* Return this overlay's state block. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */

/* The scene-transition phase flag in the field-scene table. */
static __inline__ void DrawPlacement(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    Story_unk2_5(left, top, width, height, tile, palette);
}

s32 Story_unk16_4(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 *Story_unk17_4(s32 *arg0)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{
    extern u32 StagedActorStepTable[];

    s32 dest[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *block;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dest[0] = lead->x.value + (step & 0xffff0000);
    dest[1] = lead->y;
    step <<= 16;
    dest[2] = lead->z.value + step;
    next = FindNextStagedActor(dest, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    block = FindBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dest[0] = next->x.value;
    dest[1] = next->y + 0x100000;
    dest[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dest) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dest[0], dest[1], dest[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dest[0], dest[1], dest[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dest[0];
    next->z.value = dest[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}

s32 Story_unk18_4(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

s32 Actor_ResetMotionWhenAheadBlocked(Ent *a)
{
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
    r = Story_unk7_4(&d, a);
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
    if (Story_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 FindStagedActorProbePosition(struct StagedActorProbe *probe)
{
    struct StagedActorProbePosition position;
    s32 direction;
    struct StagedActor *actor;
    s32 step_count;
    s32 footprint_height;
    u8 *transition_mode;
    s32 footprint_width;
    s32 row;
    s32 column;
    s32 lower_bound;
    s32 upper_bound;
    s32 current_y;
    s32 unused_offset;
    u8 *unused_table;
    s32 footprint_index;

    probe->unknown_14 = 0;
    actor = FindStagedActorProbeTarget(
        &direction, &probe->actor_slot, probe);
    if (actor == 0) return 0;
    transition_mode = &actor->transition_mode;
    *transition_mode = 2;
    step_count = 0;

    footprint_index = probe->footprint_index;
    lower_bound = StagedActorFootprints[footprint_index].z0;
    if (lower_bound < 0) lower_bound = -lower_bound;
    upper_bound = StagedActorFootprints[footprint_index].z1;
    if (upper_bound < 0) upper_bound = -upper_bound;
    footprint_height = (lower_bound + upper_bound) >> 4;

    lower_bound = StagedActorFootprints[footprint_index].x0;
    if (lower_bound < 0) lower_bound = -lower_bound;
    upper_bound = StagedActorFootprints[footprint_index].x1;
    if (upper_bound < 0) upper_bound = -upper_bound;
    footprint_width = (lower_bound + upper_bound) >> 4;

    position.x = actor->x.value
        + (StagedActorDirectionSteps[direction] & 0xffff0000);
    current_y = actor->y;
    position.y = current_y;
    position.z = actor->z.value + (StagedActorDirectionSteps[direction] << 16);
    probe->position_y = current_y;

    for (;;) {
        probe->position_z = position.z
            + (StagedActorFootprints[probe->footprint_index].z0 << 16);
        for (row = 0; row < footprint_height; row++) {
            probe->position_x = position.x
                + (StagedActorFootprints[probe->footprint_index].x0 << 16);
            for (column = 0; column < footprint_width; column++) {
                if (ClassifyStagedActorProbePosition(actor, &probe->position_x) == 2) goto hit;
                probe->position_x += 0x100000;
            }
            probe->position_z += 0x100000;
        }
        step_count++;
        position.x += StagedActorDirectionSteps[direction] & 0xffff0000;
        position.z += StagedActorDirectionSteps[direction] << 16;
    }
hit:
    *transition_mode = 0;
    if (step_count == 0) return 0;
    probe->position_x = actor->x.value
        + (s32)(StagedActorDirectionSteps[direction] & 0xffff0000) * step_count;
    probe->position_y = actor->y;
    probe->position_z = actor->z.value
        + step_count *(s32)(StagedActorDirectionSteps[direction] << 16);
    return 1;
}

s32 OvObj_ClearPendingAndRestoreMode(u8 *object)
{
    u8 **pending = (u8 **)0x0200b390;
    if (*pending) {
        Story_Apply(object, 2);
        *pending = 0;
    }
    return 1;
}

s32 Actor_UpdateRandomCounterMode(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Story_unk19_4() * 100) >> 16));
    if ((s16)*counter > 1000) Story_Apply2(object, 7);
    else Story_Apply3(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}

u8 *SceneData_GetTableb06c(void) { return (u8 *)0x0200b06c; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetTableb0cc(void) { return (u8 *)0x0200b0cc; }

u8 *SceneData_GetTableb0e4(void) { return (u8 *)0x0200b0e4; }

void Scene_RunTile10x20Transition(void)
{
    struct PlacementResult res;
    Story_unk20_4();

    if (Story_unk10(&res)) {
        Story_unk4_2(res);
        if (res.words[1] == 10 && (res.words[2] >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Story_Apply4(10, 3);
            Story_unk8_3(10, -18, 6);
            Story_unk5_2(30);
            Story_unk6_2(240);
            Story_Apply5(10, 8);
            Story_unk8_4(10)[35] = 2;
            zero = 0;
            DrawPlacement(0, 17, 2, 4, 19, 17);
            Scene_RunBranchingFormationPresentation(2, 20, 17, 1, 4, zero);
            Story_unk7_2(0x200);
            actor = Story_unk9_4(10);
            Story_Apply6(actor, 0);
        }
    }

    Story_unk21_4();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 ret = CanStartStagedActorEffect(actor, request);

    if (ret == 0) {
        BeginStagedActorEffect();
        SetStagedActorEffectMode(actor, 6);
        SelectStagedActorEffectSlot(6);
        StartStagedActorEffectSound(152);
        SetStagedActorMotionMode(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        PrepareStagedActorEffect(actor, 0);
        TestStagedActorEffectCell(0, request->cell_x, request->cell_z);
        StartStagedActorEffectMove(actor, 6);
        SetStagedActorEffectTransition(actor, 1);
        *flags = (u8)ret;
        WaitStagedActorEffect(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        AdvanceStagedActorEffect(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitSceneFrames(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        RestoreStagedActorEffect(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        FinishStagedActorEffect();
        return 1;
    }
    return 0;
}

void Actor_PassSubjectOffsetPosition(void)
{
    extern u8 gCell[];

    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = Story_unk10_4(*(s32 *)(gCell + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    Story_unk8_2(buf);
}

u8 *SceneData_GetTableB294(void) { return (u8 *)0x0200b294; }

s32 Scene_RunScene391(void)
{
    extern s16 gCell[];

    s32 record;
    s32 zero;

    Story_unk22_4(10);
    if (Story_Check(0x200) != 0) {
        zero = 0;
        *(u8 *)(Story_unk11(10) + 35) = 2;
        Story_SetRect(0, 17, 2, 4, 19, 17);
        record = Story_unk12(2, 20, 17, 1, 4, zero);
        record = Story_unk13(10);
        Story_unk23_4(record, 0);
    }
    Story_unk24_4(8);
    Story_unk25_4(9);
    if (SceneTransition_Phase == 4) {
        if (Story_unk2(0x843) == 0) {
            Story_unk26_4();
        }
    }
    if (Story_unk3(0x845) != 0) {
        Story_unk27_4(17, 0, 0);
        Story_unk28_4(18, 0, 0);
        Story_unk29_4(19, 0, 0);
        Story_unk30_4(20, 0, 0);
        Story_unk31_4(21, 0, 0);
    }
    return 0;
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Story_Apply7(a, 0);
    Story_unk14(b);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Story_unk9_3(a, b, 0);
    Story_unk15(c);
}

s32 Effect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p)
{
    p->field18 += 0x1eb8;
    if (p->field38 == (s32)0x80000000
        && p->field3c == (s32)0x80000000
        && p->field40 == (s32)0x80000000) {
        Story_unk9_2(p);
    }
    return 1;
}

void Effect_SpawnObject26EveryEightFrames(void)
{
    extern s32 gIw;

    struct Obj *obj;
    struct Sub *sprite;
    s32 phase;
    s32 v;
    s32 w;
    s32 c1 = 0x00e70000;
    s32 c2 = 0x01cc0000;
    s32 c3 = 0x00e70000;
    s32 c4 = 0x02700000;

    phase = gIw & 7;
    if (phase != 0) return;
    if (gOv2 != 0) Story_unk10_2(200);
    obj = Story_unk11_4(26, c1, 0, c2);
    if (obj == 0) return;
    sprite = obj->f50;
    sprite->f26 = phase;
    v = 0xfe;
    v &= obj->f23;
    obj->f23 = v;
    w = ~12;
    w &= sprite->f09;
    w |= 4;
    sprite->f09 = w;
    obj->f18 = 0x1999;
    obj->f30 = 0x80000;
    obj->f34 = 0x80000;
    obj->f55 = phase;
    Story_Apply8(obj, 2);
    Story_SetMode(obj, c3, 0, c4);
    Story_Apply9(obj, gOv3);
}

s32 OvObj_SelectValueByFrameBit1(s32 obj)
{
    extern u32 gIw;

    if (((gIw >> 1) & 1) != 0) {
        Story_Apply10(obj, 10);
    } else {
        Story_Apply11(obj, 7);
    }
    return 0;
}

s32 Actor_CheckRegionTrigger(struct Struct288c *arg0)
{
    s32 x;

    if (gOv4 != 0) {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0x1120000
            && arg0->field10 > 0x2360000 && arg0->field10 < 0x2640000) {
            goto hit;
        }
        if (x > 0xca0000 && x < 0xff0000
            && arg0->field10 > 0x2250000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    } else {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0xf40000
            && arg0->field10 > 0x2250000 && arg0->field10 <= 0x248ffff) {
            goto hit;
        }
        if (x > 0xf40000 && x < 0x1120000
            && arg0->field10 > 0x23b0000 && arg0->field10 <= 0x25cffff) {
            goto hit;
        }
        if (x > 0xd30000 && x < 0xff0000
            && arg0->field10 > 0x2540000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    }
    return 0;
hit:
    Story_unk11_2(106);
    Story_Apply12(arg0, gOv6);
    gOv5 = 1;
    return 0;
}

void Actor_AlternateSlots13To16Field0c(void)
{
    extern u32 gIw;

    struct Struct5702 *p;

    p = Story_unk12_4(13);
    if (p != 0) {
        p->field55 = 0;
        if ((gIw & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Story_unk13_4(14);
    if (p != 0) {
        p->field55 = 0;
        if ((gIw & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Story_unk14_4(15);
    if (p != 0) {
        p->field55 = 0;
        if ((gIw & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Story_unk15_4(16);
    if (p != 0) {
        p->field55 = 0;
        if ((gIw & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
}

/* overlays/scene/story/branching_formation/run_branching_formation_presentation.c */

static __inline__ u8 * Record1(u8 * (*f)(), s32 a0)
{
    return f(a0);
}

extern s32 gOv7;
extern s32 gOv8;
extern s32 gOv9;
extern s32 gOv5;
extern s32 gOv4;
extern s32 gOv2;
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];

void Scene_RunBranchingFormationPresentation(void)
{
    u8 *record;
    s32 *flag_work;
    s32 entry_action;
    s32 value;
    s32 flag;
    s32 motion_action;
    s32 *party_flag;
    s32 *formation_flag;
    s32 *sequence_flag;
    s32 *finish_flag;
    s32 reset_action;
    s32 *effect_phase;
    s32 *formation_phase;
    s32 formation_action;
    s32 finish_action;
    s32 *sequence_phase;

    Story_unk32_4();
    Story_Run(-1, -1, -1, 0);
    Story_unk33_4(1);
    Story_Run(0xf60000, -1, 0x25c0000, 0);
    flag_work = &gOv4;
    flag = Story_unk4(3);
    *flag_work = flag;
    record = Story_BytePtr(13);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(14);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(15);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(16);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(17);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(18);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(19);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(20);
    Story_unk34_4((s32)record, 0);
    record = Story_BytePtr(21);
    Story_unk34_4((s32)record, 0);
    entry_action = (s32)gOv20;
    Story_unk35_4(17, entry_action);
    Story_unk35_4(18, entry_action);
    Story_unk35_4(19, entry_action);
    Story_unk35_4(20, entry_action);
    Story_unk35_4(21, entry_action);
    Story_Place(0, 0x740000, 0x25a0000);
    Story_unk33_4(1);
    Story_unk36_4();
    Story_Do(1);
    Story_unk37_4();
    Story_unk38_4();
    Story_unk2_3(0, 0xcccc, 0x6666);
    Story_unk3_3(0, 254, 0x251);
    Story_unk2_3(1, 0x9999, 0x4ccc);
    Story_unk2_3(2, 0x9999, 0x4ccc);
    {
        u8 *record = Record1(Story_BytePtr, 0);

        if (record != 0) {
            Story_unk39_4(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    {
        u8 *record = Record1(Story_BytePtr, 0);

        if (record != 0) {
            Story_unk39_4(2, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    Story_unk2_4(1, (s32)gOv13);
    Story_unk35_4(2, (s32)gOv14);
    if (*flag_work != 0) {
        Story_unk2_3(3, 0x9999, 0x4ccc);
        {
            u8 *record = Record1(Story_BytePtr, 0);

            if (record != 0) {
                Story_unk39_4(3, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
        }
        Story_unk35_4(3, (s32)gOv15);
    }
    Story_unk40_4(2);
    Actor_SetPairZeroAndValue(2, 0x2000, 40);
    Actor_SetPairZeroAndValue(2, 0x8000, 20);
    Actor_SetPairZeroAndValue(2, 0x4000, 40);
    Story_unk4_3(2, 0x101, 0);
    Story_unk41_4(60);
    Story_unk42_4(1, 0x4000, 0);
    Actor_SetPairZeroAndValue(0, 0x6000, 60);
    value = 160;
    Actor_SetPairZeroAndValue(3, 0x2000, 10);
    Story_unk42_4(1, 0x2000, 0);
    Actor_SetPairZeroAndValue(0, (value << 8), 10);
    Story_unk4_3(1, 0x101, 0);
    Story_unk4_3(0, 0x101, 0);
    Story_unk41_4(40);
    Story_unk42_4(1, 0x4000, 0);
    Actor_SetPairZeroAndValue(0, 0x6000, 10);
    Story_unk43_4(1, 2);
    Story_unk2_2(0x1474);
    Scene_RunSplitPairSteps(1, 10);
    Story_unk2_2(0x147c);
    Actor_SetPairZeroAndValue(2, 0xc000, 20);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(2, 20);
    Actor_SetPairZeroAndValue(1, 0, 20);
    Actor_SetPairZeroAndValue(0, (value << 8), 40);
    Actor_SetPairZeroAndValue(1, 0x4000, 20);
    Actor_SetPairZeroAndValue(0, 0x6000, 30);
    Actor_SetPairZeroAndValue(1, 0x6000, 20);
    Actor_SetPairZeroAndValue(0, 0xe000, 30);
    Story_unk43_4(2, 2);
    Story_unk4_3(2, 0x100, 0);
    Story_unk41_4(40);
    Story_unk42_4(1, 0x4000, 0);
    Actor_SetPairZeroAndValue(0, 0x6000, 20);
    Actor_SetPairZeroAndValue(2, 0xc000, 10);
    Story_unk45_4(17);
    Story_unk45_4(206);
    Story_unk3_4(0x7fff, 0);
    Story_unk46_4(1);
    Story_unk33_4(1);
    gOv2 = 1;
    Story_unk5((s32)gOv10, 0xc80);
    Story_unk33_4(20);
    Story_unk3_4(0x405210, 1);
    Story_unk3_4(0x10000, 2);
    Story_unk46_4(120);
    Story_unk33_4(60);
    motion_action = (s32)gOv16;
    Story_unk35_4(0, motion_action);
    Story_unk35_4(1, motion_action);
    Story_unk35_4(2, motion_action);
    Story_unk35_4(3, motion_action);
    Story_unk41_4(100);
    Scene_RunSplitPairSteps(1, 20);
    Scene_RunSplitPairSteps(2, 40);
    if (gOv4 != 0) {
        Story_unk41_4(40);
        Story_unk4_3(3, 0x102, 0);
        Story_unk41_4(40);
        Scene_RunSplitPairSteps(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk41_4(20);
    party_flag = &gOv4;
    if (*party_flag != 0) {
        value = 128;
        record = Story_BytePtr(3);
        *(s32 *)((s32)record + 40) = (value << 10);
        Story_unk41_4(10);
        Story_unk47_4(3, (value << 10), (value << 10));
        Story_unk5_3(3, -2, 0);
        Story_unk2_4(3, (s32)gOv17);
        record = Story_BytePtr(3);
        Story_unk34_4((s32)record, 0);
        Story_unk48_4(3, 19);
        Story_unk41_4(10);
    }
    value = 128;
    record = Story_BytePtr(0);
    *(s32 *)((s32)record + 40) = (value << 10);
    Story_unk41_4(10);
    Story_unk2_3(0, (value << 10), (value << 10));
    reset_action = (s32)gOv17;
    Story_unk35_4(0, reset_action);
    record = Story_BytePtr(0);
    Story_unk34_4((s32)record, 0);
    Story_unk48_4(0, 19);
    Story_unk41_4(20);
    record = Record1(Story_BytePtr, 1);
    *(s32 *)((s32)record + 40) = (value << 10);
    Story_unk41_4(10);
    Story_unk2_3(1, (value << 10), (value << 10));
    Story_unk35_4(1, reset_action);
    record = Story_BytePtr(1);
    Story_unk34_4((s32)record, 0);
    Story_unk48_4(1, 19);
    Story_unk41_4(40);
    record = Record1(Story_BytePtr, 2);
    *(s32 *)((s32)record + 40) = (value << 10);
    Story_unk41_4(10);
    Story_unk35_4(2, reset_action);
    record = Story_BytePtr(2);
    Story_unk34_4((s32)record, 0);
    Story_unk48_4(2, 19);
    gOv2 = 0;
    Story_unk41_4(160);
    Story_unk6((s32)gOv10);
    Story_unk41_4(120);
    Story_unk3_4(0x406218, 1);
    Story_unk46_4(60);
    Story_unk33_4(60);
    gOv8 = 0;
    effect_phase = &gOv9;
    gOv7 = 0x800000;
    *effect_phase = 1;
    Story_unk5((s32)gOv11, 0xc80);
    Story_unk41_4(180);
    Story_unk45_4(21);
    Scene_RunSplitPairSteps(1, 80);
    Scene_RunSplitPairSteps(2, 40);
    Story_unk4_3(0, 0x102, 0);
    Story_unk4_3(1, 0x102, 0);
    Story_unk4_3(2, 0x102, 0);
    Story_unk4_3(3, 0x102, 0);
    Story_unk41_4(60);
    Scene_RunSplitPairSteps(2, 20);
    *effect_phase = 2;
    Story_unk49_4(2, 2);
    Story_unk41_4(20);
    Story_unk49_4(1, 1);
    Story_unk41_4(40);
    Story_unk49_4(0, 2);
    Story_unk49_4(3, 1);
    Story_unk41_4(20);
    Story_unk49_4(2, 3);
    Story_unk41_4(40);
    Story_unk49_4(0, 1);
    Story_unk41_4(20);
    Story_unk49_4(1, 2);
    Story_unk41_4(20);
    Story_unk49_4(3, 2);
    Story_unk4_3(1, 0x102, 0);
    Scene_RunSplitPairSteps(1, 20);
    if (*party_flag != 0) {
        Story_unk4_3(3, 0x102, 0);
        Scene_RunSplitPairSteps(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    formation_phase = &gOv9;
    *formation_phase = 3;
    *(u8 *)(Story_BytePtr(0) + 35) &= 254;
    *(u8 *)(Story_BytePtr(1) + 35) &= 254;
    *(u8 *)(Story_BytePtr(2) + 35) &= 254;
    *(u8 *)(Story_BytePtr(3) + 35) &= 254;
    Story_unk50_4(0, 3);
    Story_unk50_4(1, 3);
    Story_unk50_4(2, 3);
    value = 0;
    Story_unk50_4(3, 3);
    gOv5 = value;
    Story_unk5((s32)gOv12, 0xc80);
    Story_unk45_4(220);
    *(u8 *)(Story_BytePtr(13) + 35) &= 254;
    Story_unk50_4(13, 2);
    Story_Place(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)gOv18;
    Story_unk35_4(13, formation_action);
    *(u8 *)(Story_BytePtr(14) + 35) &= 254;
    Story_unk50_4(14, 2);
    Story_Place(14, 0xe90000, 0x2750000);
    Story_unk35_4(14, formation_action);
    if (gOv4 != 0) {
        *(u8 *)(Story_BytePtr(15) + 35) &= 254;
        Story_unk50_4(15, 2);
        Story_Place(15, 0xcf0000, 0x2610000);
        Story_unk35_4(15, formation_action);
    }
    *(u8 *)(Story_BytePtr(16) + 35) &= 254;
    Story_unk50_4(16, 2);
    Story_Place(16, 0xe30000, 0x2440000);
    Story_unk35_4(16, formation_action);
    if (*formation_phase != 0) {
        do {
            Story_unk33_4(1);
        } while (gOv9 != 0);
    }
    Story_unk3_2(0x12c);
    Story_unk6((s32)gOv11);
    Story_unk41_4(120);
    Story_unk45_4(17);
    Story_unk3_4(0x10000, 1);
    Story_unk46_4(60);
    Story_unk33_4(60);
    Story_unk51_4(13);
    Story_unk51_4(14);
    formation_flag = &gOv4;
    if (*formation_flag != 0) {
        Story_unk51_4(15);
    }
    Story_unk51_4(16);
    Story_unk33_4(1);
    finish_action = (s32)gOv19;
    Story_unk35_4(13, finish_action);
    Story_unk35_4(14, finish_action);
    if (*formation_flag != 0) {
        Story_unk35_4(15, finish_action);
    }
    Story_unk52_3(16, finish_action);
    Story_unk41_4(80);
    Story_unk43_4(1, 2);
    Story_unk41_4(40);
    Story_unk7(1, 0);
    Story_Place(11, 0xdc0000, 0x1ee0000);
    Story_Place(12, 0xdc0000, 0x1ee0000);
    Story_unk33_4(1);
    if (Story_unk8(11, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk43_4(0, 1);
    Story_unk41_4(20);
    Story_unk43_4(2, 2);
    Scene_RunSplitPairSteps(2, 20);
    if (*formation_flag != 0) {
        Story_unk43_4(3, 2);
        Story_unk41_4(10);
        Story_unk2_2(0x1488);
        Scene_RunSplitPairSteps(3, 40);
    }
    Story_unk49_4(1, 1);
    Story_unk4_3(1, 0x101, 0);
    Story_unk41_4(80);
    Story_unk43_4(2, 2);
    Story_unk2_2(0x1489);
    Scene_RunSplitPairSteps(2, 40);
    Story_unk43_4(1, 3);
    Story_unk41_4(40);
    Story_unk50_4(1, 2);
    *(u8 *)(Story_BytePtr(1) + 35) |= 1;
    record = Story_BytePtr(1);
    Story_unk34_4((s32)record, 1);
    Story_unk53_3(1, 6, 0);
    Story_unk5_3(1, -3, 0);
    Story_unk48_4(1, 1);
    Actor_SetPairZeroAndValue(1, 0x4000, 60);
    Scene_RunSplitPairSteps(1, 20);
    Story_unk49_4(1, 2);
    Scene_RunSplitPairSteps(1, 10);
    Story_unk43_4(0, 3);
    Actor_SetPairZeroAndValue(1, 0x2000, 20);
    Story_unk4_3(1, 0x101, 0);
    Story_unk41_4(40);
    Actor_SetPairZeroAndValue(1, 0x6000, 40);
    Actor_SetPairZeroAndValue(1, 0x2000, 20);
    Actor_SetPairZeroAndValue(1, 0x6000, 20);
    Actor_SetPairZeroAndValue(1, 0x2000, 10);
    Story_unk53_3(1, 2, 0);
    Story_unk41_4(40);
    Story_unk53_3(1, 2, 0);
    Story_unk41_4(10);
    Story_unk53_3(1, 4, 0);
    Story_unk41_4(20);
    Scene_RunSplitPairSteps(1, 20);
    if (*formation_flag != 0) {
        Story_unk4_3(3, 0x100, 0);
        Story_unk41_4(60);
        Story_unk43_4(3, 2);
        Story_unk41_4(80);
        Story_unk50_4(3, 2);
        *(u8 *)(Story_BytePtr(3) + 35) |= 1;
        record = Story_BytePtr(3);
        Story_unk34_4((s32)record, 1);
        Story_unk53_3(3, 4, 0);
        Story_unk5_3(3, -2, 0);
        Story_unk48_4(3, 1);
        Actor_SetPairZeroAndValue(3, 0xe000, 60);
        Story_unk43_4(3, 2);
        Story_unk41_4(20);
        Scene_RunSplitPairSteps(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk6_3(1, 2, 0);
    Actor_SetPairZeroAndValue(1, 0x4000, 20);
    Story_unk44_4(1, 3);
    Actor_SetPairZeroAndValue(1, 0x2000, 10);
    Scene_RunSplitPairSteps(1, 20);
    Story_unk44_4(1, 3);
    Story_unk41_4(10);
    Story_unk4_4(2, 1);
    Story_unk41_4(40);
    Story_unk43_4(2, 2);
    Story_unk41_4(20);
    value = 1;
    Story_unk50_4(2, 2);
    *(u8 *)(Story_BytePtr(2) + 35) |= value;
    record = Story_BytePtr(2);
    Story_unk34_4((s32)record, 1);
    Story_unk53_3(2, 4, 0);
    Story_unk48_4(2, 1);
    Story_unk7_3(2, 0xc000, 0);
    Story_unk43_4(0, 2);
    Story_unk41_4(10);
    Story_unk50_4(0, 2);
    {
        u8 *record = Story_BytePtr(0);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    record = Story_BytePtr(0);
    Story_unk34_4((s32)record, 1);
    Story_unk53_3(0, 4, 0);
    Story_unk48_4(0, 1);
    Actor_SetPairZeroAndValue(0, 0x6000, 60);
    Story_unk4_3(0, 0x105, 0);
    Story_unk4_3(2, 0x105, 0);
    Story_unk41_4(60);
    Actor_SetPairZeroAndValue(0, 0xa000, 20);
    Story_unk44_4(1, 3);
    Story_unk44_4(0, 3);
    Actor_SetPairZeroAndValue(0, 0x6000, 10);
    Actor_SetPairZeroAndValue(1, 0x4000, 10);
    Story_unk7_3(2, 0xc000, 0);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(2, 20);
    Story_unk48_4(0, 3);
    Story_unk48_4(3, 3);
    Story_unk44_4(1, 3);
    Story_unk41_4(20);
    Story_unk43_4(2, 1);
    Story_unk41_4(20);
    Story_unk7(2, 0);
    Story_unk48_4(2, 3);
    Story_unk7_3(2, 0xe000, 0);
    Story_unk42_4(1, 0x2000, 0);
    if (Story_unk8(0, 0) == 0) {
        Story_unk48_4(2, 3);
        Story_unk44_4(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Story_unk43_4(1, 2);
        Actor_SetPairZeroAndValue(1, 0x2000, 10);
        Story_unk49_4(1, 2);
        Story_unk54_3(1, 0);
    }
    Actor_SetPairZeroAndValue(1, 0x4000, 10);
    Story_unk44_4(1, 4);
    Scene_RunSplitPairSteps(1, 20);
    Actor_SetPairZeroAndValue(2, 0xc000, 10);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(2, 10);
    if (gOv4 != 0) {
        Story_unk43_4(3, 2);
        Actor_SetPairZeroAndValue(3, 0, 20);
        Actor_SetPairZeroAndValue(3, 0x2000, 10);
        Story_unk48_4(3, 4);
        Scene_RunSplitPairSteps(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk7_3(1, 0x2000, 0);
    Actor_SetPairZeroAndValue(0, 0xa000, 10);
    Story_unk48_4(0, 3);
    Story_unk44_4(1, 3);
    value = 128;
    Story_unk41_4(20);
    Story_unk7_3(0, 0x6000, 0);
    Actor_SetPairZeroAndValue(1, (value << 7), 10);
    Story_unk44_4(2, 4);
    Story_unk41_4(20);
    Story_unk4_3(0, 0x102, 0);
    Story_unk4_3(1, 0x102, 0);
    Story_unk41_4(80);
    Actor_SetPairZeroAndValue(2, 0xe000, 10);
    Story_unk49_4(2, 2);
    Scene_RunSplitPairSteps(2, 20);
    Story_unk7_3(1, 0x2000, 0);
    Actor_SetPairZeroAndValue(0, 0xa000, 40);
    Story_unk42_4(1, (value << 7), 0);
    Actor_SetPairZeroAndValue(0, 0x6000, 10);
    Actor_SetPairZeroAndValue(2, 0xc000, 10);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(2, 10);
    Story_unk5_4(1, 0x102);
    Story_unk41_4(40);
    Scene_RunSplitPairSteps(1, 20);
    Story_unk44_4(2, 3);
    Story_unk41_4(20);
    Story_unk4_3(1, 0x102, 0);
    Story_unk41_4(40);
    Scene_RunSplitPairSteps(1, 20);
    Story_unk48_4(2, 3);
    Scene_RunSplitPairSteps(2, 10);
    Story_unk43_4(1, 2);
    Actor_SetPairZeroAndValue(1, 0x2000, 10);
    Story_unk7(1, 0);
    Story_unk42_4(0, 0xa000, 0);
    if (Story_unk8(0, 0) == 0) {
        Story_unk44_4(1, 3);
    } else {
        Story_unk41_4(20);
        Story_unk43_4(1, 2);
        Story_unk41_4(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk54_3(1, 0);
    Story_unk45_4(21);
    Story_unk3_4(0x406218, 1);
    Story_unk46_4(60);
    Story_unk33_4(60);
    gOv8 = 0;
    gOv7 = 0x800000;
    sequence_phase = &gOv9;
    *sequence_phase = 1;
    Story_unk5((s32)gOv11, 0xc80);
    Story_unk41_4(80);
    Story_unk6_4(0, 2);
    Story_unk49_4(1, 2);
    Story_unk6_4(3, 2);
    Story_unk43_4(2, 2);
    Story_unk41_4(60);
    Actor_SetPairZeroAndValue(2, 0xc000, 10);
    Story_unk2_2(0x149d);
    Scene_RunSplitPairSteps(2, 10);
    Actor_SetPairZeroAndValue(1, 0xc000, 10);
    Actor_SetPairZeroAndValue(0, 0xc000, 10);
    sequence_flag = &gOv4;
    if (*sequence_flag != 0) {
        Actor_SetPairZeroAndValue(3, 0xc000, 10);
    }
    *(u8 *)(Story_BytePtr(0) + 35) &= 254;
    *(u8 *)(Story_BytePtr(1) + 35) &= 254;
    *(u8 *)(Story_BytePtr(2) + 35) &= 254;
    *(u8 *)(Story_BytePtr(3) + 35) &= 254;
    Story_unk50_4(0, 3);
    Story_unk50_4(1, 3);
    Story_unk50_4(2, 3);
    Story_unk50_4(3, 3);
    *sequence_phase = 2;
    Story_unk45_4(220);
    Story_Place(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)gOv18;
    Story_unk35_4(13, formation_action);
    Story_Place(14, 0xe90000, 0x2750000);
    Story_unk35_4(14, formation_action);
    if (*sequence_flag != 0) {
        Story_Place(15, 0xcf0000, 0x2610000);
        Story_unk35_4(15, formation_action);
    }
    Story_Place(16, 0xe30000, 0x2440000);
    Story_unk2_4(16, formation_action);
    Story_unk41_4(120);
    *sequence_phase = 3;
    do {
        Story_unk33_4(1);
    } while (gOv9 != 0);
    Scene_RunSplitPairSteps(11, 80);
    Scene_RunSplitPairSteps(12, 20);
    Story_unk4_3(0, 0x101, 0);
    Story_unk4_3(1, 0x101, 0);
    Story_unk4_3(2, 0x101, 0);
    Story_unk4_3(3, 0x101, 0);
    Story_unk41_4(60);
    Scene_RunSplitPairSteps(12, 20);
    Story_unk48_4(0, 3);
    Story_unk48_4(1, 3);
    Story_unk48_4(3, 3);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk48_4(0, 3);
    Story_unk48_4(1, 3);
    Story_unk48_4(3, 3);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk4_3(0, 0x100, 0);
    Story_unk4_3(1, 0x100, 0);
    Story_unk4_3(2, 0x100, 0);
    Story_unk4_3(3, 0x100, 0);
    Story_unk41_4(40);
    Scene_RunSplitPairSteps(11, 10);
    Story_unk7_3(0, 0x8000, 0);
    Story_unk7_3(1, 0x4000, 0);
    Story_unk42_4(3, 0, 0);
    Actor_SetPairZeroAndValue(2, 0xc000, 40);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk4_3(0, 0x102, 0);
    Story_unk4_3(1, 0x102, 0);
    Story_unk4_3(2, 0x102, 0);
    Story_unk4_3(3, 0x102, 0);
    Story_unk42_4(0, 0xc000, 0);
    Story_unk42_4(1, 0xc000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0xc000, 80);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk7_3(0, 0x8000, 0);
    Story_unk7_3(1, 0x4000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0, 40);
    Scene_RunSplitPairSteps(11, 10);
    Story_unk42_4(0, 0xc000, 0);
    Story_unk42_4(1, 0xc000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0xc000, 10);
    Story_unk48_4(0, 4);
    Story_unk48_4(1, 4);
    Story_unk48_4(3, 4);
    Story_unk44_4(2, 4);
    Story_unk41_4(60);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk48_4(0, 3);
    Story_unk48_4(1, 3);
    Story_unk48_4(3, 3);
    Story_unk44_4(2, 3);
    Scene_RunSplitPairSteps(12, 20);
    Story_unk7_3(0, 0x8000, 0);
    Story_unk7_3(1, 0x4000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0, 20);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk49_4(0, 2);
    Story_unk49_4(1, 2);
    Story_unk49_4(3, 2);
    Story_unk43_4(2, 2);
    Story_unk42_4(0, 0xc000, 0);
    Story_unk42_4(1, 0xc000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Story_unk42_4(3, 0xc000, 0);
    Scene_RunSplitPairSteps(12, 20);
    Story_unk7_3(0, 0x8000, 0);
    Story_unk7_3(1, 0x4000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0, 20);
    Scene_RunSplitPairSteps(11, 20);
    Story_unk4_3(0, 0x102, 0);
    Story_unk4_3(1, 0x102, 0);
    Story_unk4_3(3, 0x102, 0);
    Story_unk4_3(2, 0x102, 0);
    Story_unk41_4(40);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk42_4(0, 0xc000, 0);
    Story_unk42_4(1, 0xc000, 0);
    Story_unk42_4(2, 0xc000, 0);
    Actor_SetPairZeroAndValue(3, 0xc000, 10);
    Scene_RunSplitPairSteps(12, 10);
    Story_unk48_4(0, 3);
    Story_unk48_4(1, 3);
    Story_unk48_4(3, 3);
    Story_unk44_4(2, 3);
    Story_unk41_4(60);
    Story_unk54_3(12, 0);
    Story_unk54_3(11, 0);
    Story_unk6((s32)gOv11);
    Story_unk41_4(80);
    Story_unk3_4(0x10000, 1);
    Story_unk46_4(60);
    Story_unk33_4(80);
    Story_unk51_4(13);
    Story_unk51_4(14);
    finish_flag = &gOv4;
    Story_unk51_4(15);
    Story_unk51_4(16);
    Story_unk33_4(1);
    finish_action = (s32)gOv19;
    Story_unk35_4(13, finish_action);
    Story_unk35_4(14, finish_action);
    if (*finish_flag != 0) {
        Story_unk35_4(15, finish_action);
    }
    Story_unk52_3(16, finish_action);
    Story_unk41_4(20);
    Story_unk50_4(0, 2);
    Story_unk50_4(1, 2);
    Story_unk50_4(2, 2);
    value = 1;
    Story_unk50_4(3, 2);
    *(u8 *)(Story_BytePtr(0) + 35) |= value;
    *(u8 *)(Story_BytePtr(1) + 35) |= value;
    *(u8 *)(Story_BytePtr(2) + 35) |= value;
    {
        u8 *record = Story_BytePtr(3);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    Story_unk43_4(2, 2);
    Actor_SetPairZeroAndValue(2, 0xe000, 10);
    Story_unk7(2, 0);
    Story_unk7_3(1, 0x2000, 0);
    Story_unk42_4(3, 0, 0);
    if (Story_unk8(0, 0) != 0) {
    } else {
        Story_unk43_4(1, 2);
        Story_unk41_4(10);
        Story_unk7(1, 0);
        if (Story_unk8(0, 0) == 0) {
            Actor_SetPairZeroAndValue(3, 0, 20);
            Story_unk4_3(1, 0x101, 0);
            Story_unk4_3(2, 0x101, 0);
            Story_unk4_3(3, 0x101, 0);
            Story_unk41_4(40);
            Actor_SetPairZeroAndValue(1, 0x4000, 20);
            Scene_RunSplitPairSteps(1, 10);
            Actor_SetPairZeroAndValue(2, 0xc000, 20);
            Actor_SetPairZeroAndValue(2, 0xe000, 20);
            Story_unk44_4(2, 3);
            Scene_RunSplitPairSteps(2, 20);
            Actor_SetPairZeroAndValue(1, 0x2000, 20);
        } else {
            Actor_SetPairZeroAndValue(3, 0, 20);
            Story_unk4_3(1, 0x102, 0);
            Story_unk4_3(2, 0x102, 0);
            Story_unk4_3(3, 0x102, 0);
            Story_unk41_4(40);
            Actor_SetPairZeroAndValue(1, 0x4000, 20);
            Story_unk2_2(0x14b4);
            Scene_RunSplitPairSteps(1, 20);
            Story_unk44_4(2, 3);
            Scene_RunSplitPairSteps(2, 20);
        }
        Story_unk48_4(3, 3);
        Story_unk44_4(1, 3);
        goto L_02002528;
    }
    Story_unk41_4(20);
    Story_unk44_4(1, 3);
    Story_unk41_4(10);
    Story_unk2_2(0x14b6);
    Scene_RunSplitPairSteps(1, 10);
    Story_unk7_3(1, 0x4000, 0);
    Actor_SetPairZeroAndValue(0, 0x6000, 20);
    Story_unk48_4(1, 3);
    Story_unk44_4(0, 3);
    Story_unk41_4(10);
    Story_unk44_4(2, 4);
    Story_unk7(2, 0);
    if (Story_unk8(0, 0) != 0) {
    } else {
        Story_unk41_4(20);
        Story_unk4_3(2, 0x103, 0);
        Story_unk41_4(40);
        Actor_SetPairZeroAndValue(2, 0xe000, 10);
        Scene_RunSplitPairSteps(2, 10);
        if (*finish_flag != 0) {
            Actor_SetPairZeroAndValue(3, 0, 10);
            Story_unk49_4(3, 3);
            Scene_RunSplitPairSteps(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Story_unk4_3(0, 0x102, 0);
        Story_unk4_3(1, 0x102, 0);
        Story_unk41_4(40);
        Story_unk43_4(1, 2);
        Scene_RunSplitPairSteps(1, 20);
        Story_unk4_3(1, 0x105, 0);
        Story_unk41_4(120);
        Scene_RunSplitPairSteps(2, 40);
        if (gOv4 != 0) {
            Actor_SetPairZeroAndValue(3, 0x2000, 10);
            Story_unk44_4(3, 4);
            Scene_RunSplitPairSteps(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Story_unk41_4(60);
        Story_unk43_4(2, 2);
        if (gOv4 != 0) {
            Actor_SetPairZeroAndValue(2, 0xa000, 40);
            Actor_SetPairZeroAndValue(2, 0xe000, 20);
        }
        Scene_RunSplitPairSteps(2, 10);
        Story_unk49_4(0, 2);
        Story_unk43_4(1, 2);
        Story_unk41_4(40);
        Scene_RunSplitPairSteps(2, 20);
        Story_unk44_4(0, 3);
        Story_unk44_4(1, 3);
        Story_unk41_4(20);
        Story_unk48_4(3, 3);
        L_02002528:;
        Story_unk44_4(2, 3);
        goto L_02002660;
    }
    Story_unk4_3(2, 0x105, 0);
    Story_unk41_4(40);
    Story_unk44_4(2, 3);
    Story_unk2_2(0x14bf);
    Scene_RunSplitPairSteps(2, 20);
    if (*finish_flag != 0) {
        Actor_SetPairZeroAndValue(3, 0, 10);
        Story_unk49_4(3, 1);
        Scene_RunSplitPairSteps(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk4_3(1, 0x102, 0);
    Story_unk4_3(0, 0x102, 0);
    Story_unk41_4(40);
    Story_unk43_4(1, 2);
    Scene_RunSplitPairSteps(1, 20);
    Story_unk4_3(2, 0x105, 0);
    Story_unk41_4(80);
    Scene_RunSplitPairSteps(2, 40);
    if (gOv4 != 0) {
        Actor_SetPairZeroAndValue(3, 0x2000, 20);
        Story_unk48_4(3, 4);
        Scene_RunSplitPairSteps(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Story_unk43_4(2, 2);
    Story_unk41_4(20);
    Scene_RunSplitPairSteps(2, 20);
    Story_unk49_4(1, 2);
    Story_unk43_4(0, 2);
    Story_unk41_4(40);
    Scene_RunSplitPairSteps(2, 20);
    L_02002660:;
    Story_unk45_4(17);
    Story_unk2_3(1, 0x13333, 0x9999);
    Story_unk2_3(2, 0x13333, 0x9999);
    Story_unk2_3(3, 0x13333, 0x9999);
    Story_unk48_4(1, 2);
    {
        u8 *record = Record1(Story_BytePtr, 0);

        if (record != 0) {
            Story_unk55_3(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Story_unk56_3(1);
    Story_unk39_4(1, 0, 0);
    Story_unk48_4(2, 2);
    {
        u8 *record = Record1(Story_BytePtr, 0);

        if (record != 0) {
            Story_unk55_3(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Story_unk56_3(2);
    Story_unk39_4(2, 0, 0);
    if (gOv4 != 0) {
        Story_unk48_4(3, 2);
        {
            u8 *record = Record1(Story_BytePtr, 0);

            if (record != 0) {
                Story_unk55_3(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
            }
        }
        Story_unk56_3(3);
        Story_unk39_4(3, 0, 0);
    }
    Story_unk9(0x843);
    Story_unk39_4(11, 0, 0);
    Story_unk39_4(12, 0, 0);
    Story_unk57_3();
    Story_unk58_3();
}

/* overlays/scene/story/branching_formation/move_and_redraw.c */
void Story_Run(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
