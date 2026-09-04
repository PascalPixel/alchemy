#include "types.h"

#define SceneActor_CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_02008ec8
#define StagedActor_AdvancePair Func_020000c4
#define GetStagedActor Func_02000f08
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02000f8c
#define SetStagedActorMode Func_02000f84
#define SelectStagedActorSlot Func_02000f54
#define StartStagedActorEffect Func_0200104a
#define StartNextStagedActorMove Func_02000faa
#define StartLeadStagedActorMove Func_02000fba
#define FinishStagedActorMove Func_02000fc8
#define FinishStagedActorEffect Func_0200106c
#define SetStagedActorTransition Func_02000ff0
#define StagedActorDirectionSteps Data_02008ec8
#define StagedActorProbeValues Data_02008f08
#define StagedActor_CheckProbe Func_020002a8
#define FindStagedActorAtProbe Func_02000342
#define CheckStagedActorProbePosition Func_0200110e
#define TestActorPosition Func_02001348
#define ActorProbeOffsets Data_02008f20
#define ActorSearchStep Data_02008ec8
#define StagedActor_FindClearPosition Func_02000474
#define ResolveActorSearchContext Func_020007de
#define GetOrbitingSceneObject Func_02001af4
#define NormalizeOrbitingSceneObject Func_02001ae6
#define IsGameFlagSet Func_02001b0c
#define AllocateEffectTransfer Func_02001aec
#define LoadEffectResource Func_02001b3c
#define ConfigurePaletteTransfer Func_02001b14
#define ReleaseEffectTransfer Func_02001b12
#define UpdateOrbitingSceneObject Value_02008c4d
#define SceneEffect_InitOrbitingParticle Func_02000cb4

#include "staged_actor.h"
#include "staged_actor_probe_state.h"

#define SceneEffect_UpdateOrbitingParticle Func_02000c4c

#define SceneEffect_AdjustColorChannels Func_02000b24

#define SceneEffect_AdjustPaletteWindow Func_02000abc

#define FieldScene_SetupEntryActors8To11 Func_02000a68

#define SceneData_GetTable9068 Func_02000a60

#define FieldScene_SetupActor11Effect181 Func_02000a2c

#define SceneActor_RunPlacementQuery Func_020009f8

#define SceneData_GetTable8ff0 Func_020009f0

#define SceneData_GetTable8fe0 Func_020009e8

#define SceneData_ReturnZero Func_020009e4

#define SceneData_GetTable8f80 Func_020009dc

#define SceneState_FillGridAttributeRectangle Func_02000244

#define SceneActor_FindActorAtFixedPointPosition Func_0200006c

typedef struct { s32 lo, hi; } Pair;

typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c */
    u8 unknown_10[0x20];
    s32 angle;              /* +0x30, 0x10000 to the turn */
    u8 unknown_34[4];
    s32 base_x;             /* +0x38 */
    s32 base_y;             /* +0x3c */
    u8 unknown_40[0x10];
    u16 *sprite;            /* +0x50 */
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 pal;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;
extern u8 Data_02010000[];
extern s32 Data_02008f08[];
extern s32 Data_02008f20[];
extern u8 Value_02008c4d;

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02000f08(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02000f8c(struct StagedActor *arg0, s32 *arg1);
void Func_02000f84(struct StagedActor *arg0, s32 arg1);
void Func_02000f54(s32 arg0);
void Func_0200104a(s32 arg0);
void Func_02000faa(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02000fba(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02000fc8(struct StagedActor *arg0);
void Func_0200106c(void);
void Func_02000ff0(struct StagedActor *arg0, s32 arg1);
struct StagedActor *Func_02000342(struct StagedActorProbePoint *, struct StagedActor *);
s32 Func_0200110e(struct StagedActor *, struct StagedActorProbePoint *);
s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_02001348(u8 *, s32 *);
void Func_0200181a(void);
void Func_02001846(void);
s32 Func_02000e7a(Query *result);
void Func_02001026(Query result);
void Func_0200184c();
void Func_02001878(void);
void Func_0200188e(s32,s32,s32);
void Func_020018bc(s32,s32);
s32 Func_0200187c(s32,s32);
void Func_020008c0(s32 id);
s32 Func_020018a2(s32 flagId);
void Func_0200164e();
u16 Func_02001614(u16, s32);
void Func_020016d2();
void Func_020016b6();
void Func_02001978();
s32 Func_020019fa(s32);
s32 Func_02001a10(s32);
s32 Func_02001a2a(s32);
s32 Func_02001a28(void);
s32 Func_02001a2e(void);
OrbitingSceneObject *Func_02001af4(void);
void Func_02001ae6(OrbitingSceneObject *, s32);
s32 Func_02001b0c(s32);
u8 *Func_02001aec(s32, s32);
void Func_02001b3c(s32);
s32 Func_02001b14(u8, s32, u8 *);
void Func_02001b12(s32);

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

/* Constant getter; pool word belongs to the owner. */

/* Contiguous unnamed leaf-owner run for resource_392. */

/* Run the six-word placement query and forward a successful result. */

/* Scene setup for slot 11 and effect 181. */

/* Raw call-site relocation spellings from this overlay. */

/*
 * resource_392 owner at 0x02000a68, 84 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay-driver resource_392), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Body: write 516 to the workspace's +448 s32; unless story flag
 * 0xfd3 is set, run Func_02000cb4(11) (this overlay's drafted row);
 * always run Func_02001358 on records 8, 9 and 10; unless flag 0x845
 * is set, run Func_02001564(11); return 0.  Both gates are
 * "flag clear -> act", which is the shape a scene uses for work that
 * must not repeat once its flag is raised.
 *
 * Complete owner: `push {lr}` at 0x02000a68 through `movs r0, #0 /
 * pop {r1} / bx r1` at 0x02000aaa-0x02000aae, then the three-word
 * literal pool 0x02000ab0-0x02000abb (0x03001ebc, 0xfd3, 0x845);
 * the next owner is 0x02000abc, which this driver itself calls.
 * Returns a literal 0.
 *
 * Local callees: Func_0200173a is already drafted
 * (resource_392_c_02000cb4.c); Func_02001352 and SceneEffect_AdjustPaletteWindow are
 * not yet owned and are declared here only.
 *
 * Uncertainty: the flag ids are literal, and the record ids 8/9/10
 * and 11 are passed through unexamined -- what Func_0200134c and
 * SceneEffect_AdjustPaletteWindow do with them is open until those rows are drafted.
 */

/* Protected-window pal adj owner, 0x02000abc-0x02000b23. */

/* Raw overlay relocation spellings for the five calls in this owner. */

/*
 * resource_392 owner at 0x02000b24, 104 bytes: apply the asymmetric RGB555 colour
 * adj.
 *
 * TRANSPOSED from games/gs1/semantic/overlays/resource_394_c_02000ecc.c.  The two owners
 * are the same routine shared verbatim: over all 52 halfwords they differ in
 * exactly 3 places, and all three are BL halfwords.  No pool word differs.
 *
 * What was changed:
 *  - the entry symbol;
 *  - the calls, re-resolved with 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 *    resource_392 0b24': three sites, ONE distinct target, the veneer publishing
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
   channel by the adj, and each still needs its own name. */

/*
 * Apply the resource's asymmetric RGB555 colour adj.
 *
 * Control jumps over a mask literal inside the span and rejoins before the
 * common return.
 */

/* The no-argument source is reached from two sites, so it carries two names. */

s32 Func_020018d2();   /* 0x02000b44 */

s32 Func_020018e0();   /* 0x02000b52 */

s32 Func_020018ee();   /* 0x02000b60 */

s32 SceneActor_CalculateFixedPointPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dx2 = dx * dx;
    s32 dy2 = dy * dy;
    s32 dz2 = dz * dz;

    return ((IwramIntegerSquareRoot) 0x030001D8)(dx2 + dy2 + dz2);
}

s32 *SceneActor_FindActorAtFixedPointPosition(s32 *arg0) {
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
    extern u32 Data_02008ec8[];

    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = FindNextStagedActor(dst, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    blocker = FindBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    blocker = FindElevatedBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dst[0], dst[1], dst[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dst[0];
    next->z.value = dst[2];
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
    extern s32 Data_02008ec8[];

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
        s32 *tbl = StagedActorProbeValues;

        do {
            if (val == *tbl++) goto done;
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
    extern s32 Data_02008ec8[];

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

u8 *SceneData_GetTable8f80(void) { return (u8 *)0x02008f80; }

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable8fe0(void) { return (u8 *)0x02008fe0; }

u8 *SceneData_GetTable8ff0(void) { return (u8 *)0x02008ff0; }

void SceneActor_RunPlacementQuery(void)
{
    Query result;
    Func_0200181a();
    if (Func_02000e7a(&result))
        Func_02001026(result);
    Func_02001846();
}

void FieldScene_SetupActor11Effect181(void)
{
    Func_0200184c();
    Func_0200188e(11, 0, 0);
    Func_0200184c(0xfd3);
    Func_020018bc(181, 3);
    Func_0200187c(181, 0);
    Func_02001878();
}

u8 *SceneData_GetTable9068(void) { return (u8 *)0x02009068; }

s32 FieldScene_SetupEntryActors8To11(void)
{
    void SceneEffect_AdjustPaletteWindow(s32 id);
    void Func_02000cb4(s32 id);

    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_02001880(0xfd3) == 0) {
        Func_0200173a(11);
    }
    Func_0200134c(8);
    Func_02001352(9);
    Func_02001358(10);
    if (Func_020018a2(0x845) == 0) {
        Func_02001564(11);
    }
    return 0;
}

void SceneEffect_AdjustPaletteWindow(s32 adj)
{
    volatile u16 *pal = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next;
    Func_0200164e();
    phase = 0;
    do {
        u32 idx = phase >> 16;
        u32 win;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            win = (idx + 0xff3f) << 16;
            if (win > 0x70000)
                pal[idx] = Func_02001614(pal[idx], adj);
        }
        next = phase + 0x10000;
        phase = next;
    } while (next <= 0x00df0000);
    Func_020016d2(); Func_020016b6(); Func_02001978(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_020018d2(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Func_020018e0(green, adj));
    blue = (s16)(blue - Func_020018ee(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}

s32 SceneEffect_UpdateOrbitingParticle(struct Particle_02000c4c *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Func_020019fa(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Func_02001a10(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Func_02001a2a(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Func_02001a28() << 9) >> 16);
    jitter += (s32)(((u32)Func_02001a2e() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}

void SceneEffect_InitOrbitingParticle(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->pal, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
