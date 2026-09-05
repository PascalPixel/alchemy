#include "types.h"

#define CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_0200ad68
#define StagedActor_PushActorAhead Func_020000c4
#define GetStagedActor Func_02002cf8
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002d84
#define SetStagedActorMode Func_02002d5c
#define SelectStagedActorSlot Func_02002d44
#define StartStagedActorEffect Func_02002eea
#define StartNextStagedActorMove Func_02002da2
#define StartLeadStagedActorMove Func_02002db2
#define FinishStagedActorMove Func_02002dc0
#define FinishStagedActorEffect Func_02002f0c
#define SetStagedActorTransition Func_02002dc8
#define SceneActor_ResetMotionWhenAheadBlocked Func_020002a8
#define FindStagedActorProbePosition Func_02000474
#define OverlayObject_ClearPendingAndRestoreMode Func_020009dc
#define SceneActor_UpdateRandomCounterMode Func_020009fc
#define SceneData_GetTableb06c Func_02000a4c
#define SceneData_ReturnZero Func_02000a54
#define SceneData_GetTableb0cc Func_02000a58
#define SceneData_GetTableb0e4 Func_02000a60
#define FieldScene_RunTile10x20Transition Func_02000a68
#define StagedActor_RunStepEffect Func_02000b1c
#define GetStagedActorEffect Func_0200374e
#define CanStartStagedActorEffect Func_0200371e
#define BeginStagedActorEffect Func_02003750
#define SetStagedActorEffectMode Func_020036f0
#define SelectStagedActorEffectSlot Func_020036d6
#define StartStagedActorEffectSound Func_0200387c
#define SetStagedActorMotionMode Func_02003704
#define PrepareStagedActorEffect Func_0200376e
#define TestStagedActorEffectCell Func_020037ec
#define StartStagedActorEffectMove Func_0200373c
#define SetStagedActorEffectTransition Func_0200378c
#define WaitStagedActorEffect Func_02003830
#define AdvanceStagedActorEffect Func_02003744
#define WaitSceneFrames Func_02003756
#define RestoreStagedActorEffect Func_0200376c
#define FinishStagedActorEffect_02000b1c Func_02003810
#define SceneActor_PassSubjectOffsetPosition Func_02000c14
#define SceneData_GetTableB294 Func_02000c60
#define FieldScene_RunScene391_02000c68 Func_02000c68
#define SceneTransition_Phase Data_02000240[225]
#define FieldScene_RunSplitPairSteps Func_02002768
#define SceneActor_SetPairZeroAndValue Func_02002780
#define SceneEffect_AdvanceAngleAndFinishWhenParked Func_02002798
#define SceneEffect_SpawnObject26EveryEightFrames Func_020027c8
#define OverlayObject_SelectValueByFrameBit1 Func_02002864
#define SceneActor_CheckRegionTrigger Func_0200288c
#define SceneActor_AlternateSlots13To16Field0c Func_02002ad8

#include "staged_actor.h"
#include "staged_actor_probe.h"
#include "staged_actor_effect.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

struct PlacementResult_02000a68 {
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

extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;
extern u8 Data_02010000[];
extern s32 Data_0200ada8[];
extern s32 Data_0200b398;
extern u8 Data_0200b2d0[];
extern s32 Data_0200b394;
extern s32 Data_0200b390;
extern u8 Data_0200b2e4[];

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02002cf8(s32 actor_index);
struct StagedActor *Func_02000176(s32 *position, struct StagedActor *actor);
struct StagedActor *Func_020001a2(s32 *position, struct StagedActor *actor);
struct StagedActor *Func_020001ce(s32 *position, struct StagedActor *actor);
s32 Func_02002d84(struct StagedActor *actor, s32 *position);
void Func_02002d5c(struct StagedActor *actor, s32 mode);
void Func_02002d44(s32 actor_index);
void Func_02002eea(s32 effect_id);
void Func_02002da2(struct StagedActor *actor, s32 x, s32 y, s32 z);
void Func_02002db2(struct StagedActor *actor, s32 x, s32 y, s32 z);
void Func_02002dc0(struct StagedActor *actor);
void Func_02002f0c(void);
void Func_02002dc8(struct StagedActor *actor, s32 mode);
Ent *Func_02000342(Desc *, Ent *);
s32 Func_02002f06(Ent *, Desc *);
void Func_0200358e(u8 *object, s32 mode);
u32 Func_0200359e(void);
void Func_020036d2(u8 *object, s32 mode);
void Func_020036dc(u8 *object, s32 mode);
void Func_0200367a(void);
void Func_02003724(void);
s32 Func_02000eea(struct PlacementResult_02000a68 *res);
void Func_02001096(struct PlacementResult_02000a68 res);
void Func_02003728(s32,s32);
void Func_0200371c(s32,s32,s32);
void Func_020036ba(s32);
void Func_020037e0(s32);
void Func_02003748(s32,s32);
u8 *Func_020036ee(s32);
void Func_020036c0(s32,s32,s32,s32,s32,s32);
s32 Func_02000d3c(s32,s32,s32,s32,s32,s32);
void Func_020036fc(s32);
u8 *Func_0200372a(s32);
void Func_020036f8(u8 *,s32);
struct StagedActorEffect *Func_0200374e(s32 actor_index);
s32 Func_0200371e(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
void Func_02003750(void);
void Func_020036f0(struct StagedActorEffect *actor, s32 mode);
void Func_020036d6(s32 actor_index);
void Func_0200387c(s32 effect_id);
void Func_02003704(struct StagedActorEffect *actor, s32 mode);
void Func_0200376e(struct StagedActorEffect *actor, s32 mode);
s32 Func_020037ec(s32 layer, s32 cell_x, s32 cell_z);
void Func_0200373c(struct StagedActorEffect *actor, s32 mode);
void Func_0200378c(struct StagedActorEffect *actor, s32 mode);
void Func_02003830(s32 frames, s32 mode);
void Func_02003744(s32 mode);
void Func_02003756(s32 frames);
void Func_0200376c(s32 mode);
void Func_02003810(void);
struct Struct3848 *Func_02003848(s32 arg0);
void Func_0200176a(u32 *arg0);
s32 Func_02000ef4();
void Func_02001530();
void Func_02001582();
void Func_02001588();
void Func_02001a22();
s32 Func_0200386c();
void Func_02003878();
s32 Func_020038a6();
void Func_020038a8();
s32 Func_020038d2();
s32 Func_020038da();
s32 Func_020038e0();
void Func_02003976();
void Func_02003980();
void Func_0200398a();
void Func_02003994();
void Func_0200399e();
void Func_02005434(s32 arg0, s32 arg1);
s32 Func_0200537a(s32 arg0);
void Func_02005454(s32 a, s32 b, s32 c);
s32 Func_02005392(s32 a);
void Func_02005376(struct Struct2798 *p);
void Func_02005506(s32);
struct Obj *Func_020053a6(s32, s32, s32, s32);
void Func_020053d6(struct Obj *, s32);
void Func_0200540e(struct Obj *, s32, s32, s32);
void Func_020053f6(struct Obj *, u8 *);
void Func_02005524(s32 arg0, s32 arg1);
void Func_0200552c(s32 arg0, s32 arg1);
void Func_0200563a(s32 arg0);
s32 Func_020054ca(struct Struct288c *arg0, u8 *arg1);
struct Struct5702 *Func_02005702(s32 arg0);
struct Struct5702 *Func_0200572a(s32 arg0);
struct Struct5702 *Func_02005752(s32 arg0);
struct Struct5702 *Func_0200577a(s32 arg0);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Clear the pending object cell after restoring its mode. */

/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the clamp store
 * had to take its zero from a local.  Written as a literal it was materialised
 * from the literal pool, which both changed the store's source register and
 * added a pool word; a `u16 z = 0' gives the reference's `movs r3, #0'.  The
 * owner ends at 0x02000a4a -- the veneer that follows is not its code.
 */

/* Contiguous unnamed leaf-owner run for resource_391. */

/* Placement query followed by the tile-(10,20) scene transition. */

/* Raw overlay relocation spellings for this owner's fifteen call sites. */

/* Return this overlay's state block. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene-transition phase flag in the field-scene table. */

static __inline__ void DrawPlacement_02000a68(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    Func_020036c0(left, top, width, height, tile, palette);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02000030(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x * delta_x;
    s32 delta_y_squared = delta_y * delta_y;
    s32 delta_z_squared = delta_z * delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 *Func_0200006c(s32 *arg0) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
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

void StagedActor_PushActorAhead(void) {
    extern u32 Data_0200ad68[];

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

s32 Func_02000244(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5) {
    u8 *g = Data_03001e70;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = Data_02010000;
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

s32 SceneActor_ResetMotionWhenAheadBlocked(Ent *a)
{

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200ad68[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200ada8;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200ad68[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_02002f06(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 FindStagedActorProbePosition(struct StagedActorProbe *probe) {
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
        + step_count * (s32)(StagedActorDirectionSteps[direction] << 16);
    return 1;
}

s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
    u8 **pending = (u8 **)0x0200b390;
    if (*pending) {
        Func_0200358e(object, 2);
        *pending = 0;
    }
    return 1;
}

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Func_0200359e() * 100) >> 16));
    if ((s16)*counter > 1000) Func_020036d2(object, 7);
    else Func_020036dc(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}

u8 *SceneData_GetTableb06c(void) { return (u8 *)0x0200b06c; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetTableb0cc(void) { return (u8 *)0x0200b0cc; }

u8 *SceneData_GetTableb0e4(void) { return (u8 *)0x0200b0e4; }

void FieldScene_RunTile10x20Transition(void)
{
    struct PlacementResult_02000a68 res;
    Func_0200367a();

    if (Func_02000eea(&res)) {
        Func_02001096(res);
        if (res.words[1] == 10 && (res.words[2] >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Func_02003728(10, 3);
            Func_0200371c(10, -18, 6);
            Func_020036ba(30);
            Func_020037e0(240);
            Func_02003748(10, 8);
            Func_020036ee(10)[35] = 2;
            zero = 0;
            DrawPlacement_02000a68(0, 17, 2, 4, 19, 17);
            Func_02000d3c(2, 20, 17, 1, 4, zero);
            Func_020036fc(0x200);
            actor = Func_0200372a(10);
            Func_020036f8(actor, 0);
        }
    }

    Func_02003724();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request) {
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
        FinishStagedActorEffect_02000b1c();
        return 1;
    }
    return 0;
}

void SceneActor_PassSubjectOffsetPosition(void) {
    extern u8 Data_02000240[];

    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = Func_02003848(*(s32 *)(Data_02000240 + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    Func_0200176a(buf);
}

u8 *SceneData_GetTableB294(void) { return (u8 *)0x0200b294; }

s32 FieldScene_RunScene391_02000c68(void)
{
    extern s16 Data_02000240[];

    s32 record;
    s32 zero;

    Func_02001530(10);
    if (Value1(Func_0200386c, 0x200) != 0) {
        zero = 0;
        *(u8 *)(Func_020038a6(10) + 35) = 2;
        Call6(Func_02003878, 0, 17, 2, 4, 19, 17);
        record = Func_02000ef4(2, 20, 17, 1, 4, zero);
        record = Func_020038da(10);
        Func_020038a8(record, 0);
    }
    Func_02001582(8);
    Func_02001588(9);
    if (SceneTransition_Phase == 4) {
        if (Value1(Func_020038d2, 0x843) == 0) {
            Func_02001a22();
        }
    }
    if (Value1(Func_020038e0, 0x845) != 0) {
        Func_02003976(17, 0, 0);
        Func_02003980(18, 0, 0);
        Func_0200398a(19, 0, 0);
        Func_02003994(20, 0, 0);
        Func_0200399e(21, 0, 0);
    }
    return 0;
}

void FieldScene_RunSplitPairSteps(s32 a, s32 b) {
    Func_02005434(a, 0);
    Func_0200537a(b);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c) {
    Func_02005454(a, b, 0);
    Func_02005392(c);
}

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p) {
    p->field18 += 0x1eb8;
    if (p->field38 == (s32)0x80000000
        && p->field3c == (s32)0x80000000
        && p->field40 == (s32)0x80000000) {
        Func_02005376(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void) {
    extern s32 Data_03001e40;

    struct Obj *obj;
    struct Sub *sprite;
    s32 phase;
    s32 v;
    s32 w;
    s32 c1 = 0x00e70000;
    s32 c2 = 0x01cc0000;
    s32 c3 = 0x00e70000;
    s32 c4 = 0x02700000;

    phase = Data_03001e40 & 7;
    if (phase != 0) return;
    if (Data_0200b398 != 0) Func_02005506(200);
    obj = Func_020053a6(26, c1, 0, c2);
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
    Func_020053d6(obj, 2);
    Func_0200540e(obj, c3, 0, c4);
    Func_020053f6(obj, Data_0200b2d0);
}

s32 OverlayObject_SelectValueByFrameBit1(s32 obj) {
    extern u32 Data_03001e40;

    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02005524(obj, 10);
    } else {
        Func_0200552c(obj, 7);
    }
    return 0;
}

s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0) {
    s32 x;

    if (Data_0200b394 != 0) {
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
    Func_0200563a(106);
    Func_020054ca(arg0, Data_0200b2e4);
    Data_0200b390 = 1;
    return 0;
}

void SceneActor_AlternateSlots13To16Field0c(void) {
    extern u32 Data_03001e40;

    struct Struct5702 *p;

    p = Func_02005702(13);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200572a(14);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_02005752(15);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200577a(16);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
}
