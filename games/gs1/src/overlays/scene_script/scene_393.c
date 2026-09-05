#include "types.h"

#define SceneEffect_AdjustColorChannels Func_02000cf4

#define SceneEffect_AdjustPaletteColors Func_02000c8c

#define SceneData_GetTable9098 Func_02000bf0

#define FieldScene_RunActorTenPlacementScene Func_020009f8

#define MapStagedScene_SelectTertiaryData Func_020009f0

#define MapStagedScene_SelectSecondaryData Func_020009e8

#define MapStagedScene_GetEmptyData Func_020009e4

#define MapStagedScene_SelectPrimaryData Func_020009dc

#define SceneState_FillGridAttributeRectangle Func_02000244

#define SceneActor_FindAtTileXZ Func_0200006c

#define SceneActor_CalculateFixedPointDistance Func_02000030
#define StagedActorStepTable Data_02008f10
#define StagedActor_AdvancePair Func_020000c4
#define GetStagedActor Func_02000f58
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02000fec
#define SetStagedActorMode Func_02000fe4
#define SelectStagedActorSlot Func_02000fe4
#define StartStagedActorEffect Func_02001092
#define StartNextStagedActorMove Func_0200100a
#define StartLeadStagedActorMove Func_0200101a
#define FinishStagedActorMove Func_02001028
#define FinishStagedActorEffect Func_020010b4
#define SetStagedActorTransition Func_02001050
#define StagedActorDirectionSteps Data_02008f10
#define StagedActorProbeValues Data_02008f50
#define StagedActor_CheckProbe Func_020002a8
#define FindStagedActorAtProbe Func_02000342
#define CheckStagedActorProbePosition Func_0200116e
#define TestActorPosition Func_020013a8
#define ActorProbeOffsets Data_02008f68
#define ActorSearchStep Data_02008f10
#define StagedActor_FindClearPosition Func_02000474
#define StagedActor_RunStepEffect Func_02000aac
#define ResolveActorSearchContext Func_020007de
#define GetStagedActorEffect Func_0200193e
#define CanStartStagedActorEffect Func_02001916
#define BeginStagedActorEffect Func_02001948
#define SetStagedActorEffectMode Func_02001908
#define SelectStagedActorEffectSlot Func_02001906
#define StartStagedActorEffectSound Func_020019b4
#define SetStagedActorMotionMode Func_0200191c
#define PrepareStagedActorEffect Func_02001966
#define TestStagedActorEffectCell Func_020019b4
#define StartStagedActorEffectMove Func_02001954
#define SetStagedActorEffectTransition Func_02001984
#define WaitStagedActorEffect Func_020019e8
#define AdvanceStagedActorEffect Func_02001974
#define WaitSceneFrames Func_02001986
#define RestoreStagedActorEffect Func_0200199c
#define FinishStagedActorEffect_02000aac Func_02001a08
#define GetResource393Object Func_02001a38
#define ApplyResource393Position Func_0200168a
#define Resource393SharedWork Data_02000240
#define SceneActor_ApplyOffsetObjectPosition Func_02000ba4
#define SCENE_WORKSPACE (*(u8 **)0x03001ebc)
#define SceneState_SetRuntimeWord448To516 Func_02000bf8
#define RunSceneBeat10 Func_020014ce
#define IsSceneFlag0201Set Func_02001a70
#define GetSceneBeatSubject Func_02001aa2
#define DrawSceneBeatRectangle Func_02001a7c
#define FillSceneTileAttributes Func_02000e90
#define GetScenePresentationSubject Func_02001ad6
#define SetScenePresentationMode Func_02001aac
#define RunSceneBeat8 Func_0200151e
#define RunSceneBeat9 Func_02001524
#define IsSceneFlag0845Set Func_02001ac6
#define RunPhase516Followup Func_02001900

#include "staged_actor.h"
#include "staged_actor_probe_state.h"
#include "staged_actor_effect.h"
#include "resource_393.h"

struct PlacementTail {
    s32 fifth;
    void (*callback)(void);
};

struct PlacementResult {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    struct PlacementTail tail;
};

struct SceneBeatSubject {
    u8 unknown_00[0x23];
    u8 marker;
};

extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;
extern u8 Data_02010000[];
extern s32 Data_02008f50[];
extern s32 Data_02008f68[];

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02000f58(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02000fec(struct StagedActor *arg0, s32 *arg1);
void Func_02000fe4();
void Func_02001092(s32 arg0);
void Func_0200100a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200101a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001028(struct StagedActor *arg0);
void Func_020010b4(void);
void Func_02001050(struct StagedActor *arg0, s32 arg1);
struct StagedActor *Func_02000342(struct StagedActorProbePoint *, struct StagedActor *);
s32 Func_0200116e(struct StagedActor *, struct StagedActorProbePoint *);
s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_020013a8(u8 *, s32 *);
void Func_0808a018(void); void Func_0808a020(void);
void Func_02001872(void);
s32 Func_02000e7a(struct PlacementResult *result);
void Func_02001026(struct PlacementResult result);
void Func_020018e0(s32,s32); void Func_020018dc(s32,s32,s32);
void Func_020018b2(s32); u8 *Func_02001918();
void Func_020018b8(s32,s32,s32,s32,s32,s32);
s32 Func_02000ccc(s32,s32,s32,s32,s32,s32);
void Func_020018f2(s32); void Func_020018ee(u8 *,s32);
void Func_0200191a(void);
struct StagedActorEffect *Func_0200193e(s32 actor_index);
s32 Func_02001916(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
void Func_02001948(void);
void Func_02001908(struct StagedActorEffect *actor, s32 mode);
void Func_02001906(s32 actor_index);
void Func_0200191c(struct StagedActorEffect *actor, s32 mode);
void Func_02001966(struct StagedActorEffect *actor, s32 mode);
void Func_02001954(struct StagedActorEffect *actor, s32 mode);
void Func_02001984(struct StagedActorEffect *actor, s32 mode);
void Func_020019e8(s32 frames, s32 mode);
void Func_02001974(s32 mode);
void Func_02001986(s32 frames);
void Func_0200199c(s32 mode);
void Func_02001a08(void);
s32 Func_020014ce();
s32 Func_02001a70();
struct SceneBeatSubject *Func_02001aa2();
void Func_02001a7c();
s32 Func_02000e90();
struct SceneBeatSubject *Func_02001ad6();
void Func_02001aac();
void Func_0200151e();
void Func_02001524();
s32 Func_02001ac6();
void Func_020019ee(void);
void Func_02001a72(void);
void Func_02001a56(void);
void Func_02001b98(s32, s32);

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

/* Contiguous unnamed leaf-owner run for resource_393. */

/*
 * Placement query followed by the tile-(10,12) scene transition.
 *
 * The six-word result is one aggregate, including the two-word tail forwarded
 * by value to Func_02000608.  This is the same source shape witnessed by the
 * resource_392 query wrapper.  The inline six-argument draw wrapper is also
 * witnessed by this overlay's exact 0x02000bf8 sibling: it preserves the ROM's
 * r2-before-r3 stacked-literal order.  Keeping zero live across that draw lets
 * the compiler reuse the dead r5 result pointer for the following stack slot.
 * Together these natural source lifetimes reproduce all 180 bytes.
 */

/* Raw overlay relocation spellings.  They are call-site evidence, so the two
 * logical actor-access calls deliberately use different names. */

/* Return this overlay's state block. */

/* Contiguous unnamed state-owner run for resource_393. */

/*
 * resource_393 owner at 0x02000cf4, 104 bytes: apply the asymmetric RGB555 colour
 * adjustment.
 *
 * TRANSPOSED from games/gs1/semantic/overlays/resource_394_c_02000ecc.c.  The two owners
 * are the same routine shared verbatim: over all 52 halfwords they differ in
 * exactly 3 places, and all three are BL halfwords.  No pool word differs.
 *
 * What was changed:
 *  - the entry symbol;
 *  - the calls, re-resolved with 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 *    resource_393 0cf4': three sites, ONE distinct target, the veneer publishing
 *    the ARM-mode IWRAM helper Func_03000380.  The 394 source predates the
 *    corrected 'bl' rule and spelled the three sites as three different callees
 *    (Func_02001ee2 / Func_02001ef0 / Func_02001efe); they are one import, which
 *    is also what the code shape says - the same per-channel scale applied three
 *    times.  resource_394's own site resolves to the same import, so this is a
 *    correction inherited by the transposition rather than a per-overlay change.
 */

/* One symbol PER CALL SITE, named at the site's PC-relative-decoded address
   (see resource_382:3ac for the rule, tools/bl-site-symbols to derive
   them). All three reach the same ARM-mode IWRAM helper that scales a
   channel by the adjustment, and each still needs its own name. */

/*
 * Apply the resource's asymmetric RGB555 colour adjustment.
 *
 * Control jumps over a mask literal inside the span and rejoins before the
 * common return.
 */

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Func_02001900(s32,s32); u8 *Func_020018de(s32);

    Func_020018b8(left, top, width, height, tile, palette);
}

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Func_02001900();

    DrawSceneBeatRectangle(left, top, width, height, tile, palette);
}

s32 Func_02001b32();   /* 0x02000d14 */

s32 Func_02001b40();   /* 0x02000d22 */

s32 Func_02001b4e();   /* 0x02000d30 */

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dx2 = dx * dx;
    s32 dy2 = dy * dy;
    s32 dz2 = dz * dz;

    return ((IwramIntegerSquareRoot) 0x030001D8)(dx2 + dy2 + dz2);
}

s32 *SceneActor_FindAtTileXZ(s32 *arg0) {
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

void StagedActor_AdvancePair(void) {
    extern u32 Data_02008f10[];

    s32 destination[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blk;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    destination[0] = lead->x.value + (step & 0xffff0000);
    destination[1] = lead->y;
    step <<= 16;
    destination[2] = lead->z.value + step;
    next = FindNextStagedActor(destination, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    destination[0] = next->x.value + (step & 0xffff0000);
    destination[1] = next->y;
    step <<= 16;
    destination[2] = next->z.value + step;
    blk = FindBlockingStagedActor(destination, next);
    if (blk != 0 && (blk->collision_flags & 1) != 0) return;

    destination[0] = next->x.value;
    destination[1] = next->y + 0x100000;
    destination[2] = next->z.value;
    blk = FindElevatedBlockingStagedActor(destination, next);
    if (blk != 0 && (blk->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    destination[0] = next->x.value + (step & 0xffff0000);
    destination[1] = next->y;
    step <<= 16;
    destination[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, destination) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SelectStagedActorSlot(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, destination[0], destination[1], destination[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = destination[0];
    next->z.value = destination[2];
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

s32 SceneState_FillGridAttributeRectangle(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5) {
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

s32 StagedActor_CheckProbe(struct StagedActor *actor)
{
    extern s32 Data_02008f10[];

    struct StagedActorProbePoint probe;
    u32 dir;
    s32 step;
    struct StagedActor *target;

    dir = actor->direction_and_kind >> 12;
    step = StagedActorDirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    target = FindStagedActorAtProbe(&probe, actor);
    if (target != 0) {
        u32 i = 0;
        s32 val =
            *(STAGED_ACTOR_PROBE_DETAILS(target)->unknown_28);
        s32 *vals = StagedActorProbeValues;

        do {
            if (val == *vals++) goto done;
            i++;
        } while (i <= 5);
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
    step = StagedActorDirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    if (CheckStagedActorProbePosition(actor, &probe) > 0) {
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
done:
    return 0;
}

s32 StagedActor_FindClearPosition(s32 *a)
{
    extern s32 Data_02008f10[];

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
    obj = (u8 *)ResolveActorSearchContext(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)ActorProbeOffsets;
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
        d = ActorSearchStep[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (ActorSearchStep[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)ActorProbeOffsets;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)ActorProbeOffsets;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (TestActorPosition(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (ActorSearchStep[sel] & 0xffff0000);
        buf[2] = buf[2] + (ActorSearchStep[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = ActorSearchStep[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m * cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

u8 *MapStagedScene_SelectPrimaryData(void) { return (u8 *)0x02008fc8; }

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void) { return (u8 *)0x02009028; }

u8 *MapStagedScene_SelectTertiaryData(void) { return (u8 *)0x02009038; }

void FieldScene_RunActorTenPlacementScene(void)
{
    void Func_02001900(s32,s32); u8 *Func_020018de(s32);

    struct PlacementResult result;
    Func_02001872();
    if (Func_02000e7a(&result)) {
        Func_02001026(result);
        if (result.second == 10 && (result.third >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Func_020018e0(10, 3);
            Func_020018dc(10, -18, 6);
            Func_020018b2(30);
            Func_02001918(240);
            Func_02001900(10, 8);
            Func_020018de(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            Func_02000ccc(2, 12, 16, 1, 4, zero);
            Func_020018f2(0x201);
            actor = Func_02001918(10);
            Func_020018ee(actor, 0);
        }
    }
    Func_0200191a();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request) {
    s32 Func_020019b4();

    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 result = CanStartStagedActorEffect(actor, request);

    if (result == 0) {
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
        *flags = (u8)result;
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
        FinishStagedActorEffect_02000aac();
        return 1;
    }
    return 0;
}

void SceneActor_ApplyOffsetObjectPosition(void)
{
    struct Resource393Position pos;
    struct Resource393Object *obj = GetResource393Object(Resource393SharedWork.object_id);
    u32 xb = obj->position_x & 0xfff00000;

    pos.x = xb + 0x80000;
    pos.y = obj->position_y;
    pos.z = (obj->position_z & 0xfff00000) + 0x80000;
    pos.x = xb + 0x280000;
    ApplyResource393Position(&pos);
}

u8 *SceneData_GetTable9098(void) { return (u8 *)0x02009098; }

s32 SceneState_SetRuntimeWord448To516(void)
{
    void Func_02001900();

    u8 *work = SCENE_WORKSPACE;

    *(s32 *)(work + 448) = 516;
    RunSceneBeat10(10);

    if (IsSceneFlag0201Set(0x201) != 0) {
        struct SceneBeatSubject *subj = GetSceneBeatSubject(10);

        subj->marker = 2;
        DrawSceneBeat(32, 20, 2, 4, 11, 16);
        FillSceneTileAttributes(2, 12, 16, 1, 4, 0);
        SetScenePresentationMode(GetScenePresentationSubject(10), 0);
    }

    RunSceneBeat8(8);
    RunSceneBeat9(9);

    if (IsSceneFlag0845Set(0x845) == 0) {
        RunPhase516Followup(6);
    }
    return 0;
}

void SceneEffect_AdjustPaletteColors(s32 a)
{
    s32 Func_020019b4(s32, s32);

    u32 x;

    Func_020019ee();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Func_020019b4(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    Func_02001a72();
    Func_02001a56();
    Func_02001b98(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{
    s32 Func_020019b4(s32, s32);

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_02001b32(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Func_02001b40(green, adj));
    blue = (s16)(blue - Func_02001b4e(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
