#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_particle/staged_particle.h"
#include "staged_actor.h"
#include "staged_actor_probe_state.h"

/* overlays/scene/actor/staged_particle/move_and_redraw.c */

void Actor_Run(StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_particle/scene_script.c */
#define UpdateOrbitingSceneObject Value

typedef struct { s32 lo, hi; } Pair;

typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;

struct Particle {
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

extern u8 *gWork;
extern u8 *gCam;
extern u8 gUnk[];
extern s32 StagedActorProbeValues[];
extern s32 Actor_Far[];
extern u8 Value;

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

struct StagedActor *FindStagedActorAtProbe(struct StagedActorProbePoint *, struct StagedActor *);

OrbitingSceneObject *GetOrbitingSceneObject(void);

u8 *AllocateEffectTransfer(s32, s32);

/*
 * The Func_ symbols declared above name the pre-relocation call words the
 * overlay image holds, not runtime addresses. A source reached from two
 * sites carries two such names.
 */

/*
 * One symbol per call site, named at the site's PC-relative decoded
 * address. All three reach the same ARM-mode IWRAM helper that scales one
 * channel by the adjustment, and each still needs its own name.
 */
s32 Actor_Run2();   /* 0x02000b44 */

s32 Actor_Run3();   /* 0x02000b52 */

s32 Actor_Run4();   /* 0x02000b60 */

/*
 * Distance between two three-component 16.16 fixed-point positions. Each
 * argument walks three consecutive words in x, y, z order; the per-axis
 * deltas are shifted down to integers, squared, summed, and passed to the
 * resident IWRAM integer square root. The walking-pointer form is what
 * reproduces the reference and must not become struct field access.
 */
s32 Actor_CalculateFixedPointPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dx2 = dx *dx;
    s32 dy2 = dy *dy;
    s32 dz2 = dz *dz;

    return ((IwramIntegerSquareRoot) 0x030001D8)(dx2 + dy2 + dz2);
}

s32 *Actor_FindActorAtFixedPointPosition(s32 *arg0)
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

void StagedActor_AdvancePair(void)
{
    extern u32 Actor_Far2[];

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

s32 State_FillGridAttributeRectangle(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
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

s32 StagedActor_CheckProbe(struct StagedActor *actor)
{
    extern s32 Actor_Far2[];

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
    extern s32 Actor_Far2[];

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

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable8f80(void) { return (u8 *)0x02008f80; }

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable8fe0(void) { return (u8 *)0x02008fe0; }

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable8ff0(void) { return (u8 *)0x02008ff0; }

/* Runs the six-word placement query and forwards a successful result. */
void Actor_RunPlacementQuery(void)
{
    Query result;
    Actor_Run5();
    if (Actor_Check(&result))
        Actor_Do(result);
    Actor_Run6();
}

/* Scene setup for slot 11 and effect 181. */
void Scene_SetupActor11Effect181(void)
{
    Actor_Run7();
    Actor_Place(11, 0, 0);
    Actor_Run8(0xfd3);
    Actor_Apply(181, 3);
    Actor_Apply2(181, 0);
    Actor_Run9();
}

/* Constant getter; the owner includes its own pool word. */
u8 *SceneData_GetTable9068(void) { return (u8 *)0x02009068; }

/*
 * The overlay's entry driver: the loader enters here through the header
 * veneer, and no call inside the image reaches it. Sets the workspace +448
 * word to 516, sets up records 8, 9 and 10, and runs two steps for slot 11
 * that each act only while their flag is clear. The 84-byte owner includes
 * its three pool words.
 */
s32 Scene_SetupEntryActors8To11(void)
{
    void Effect_AdjustPaletteWindow(s32 id);
    void Effect_InitOrbitingParticle(s32 id);

    *(s32 *)(gWork + 448) = 516;
    if (Actor_Run10(0xfd3) == 0) {
        Actor_Run11(11);
    }
    Actor_Run12(8);
    Actor_Run13(9);
    Actor_Run14(10);
    if (Actor_Check2(0x845) == 0) {
        Actor_Run15(11);
    }
    return 0;
}

/* Applies the adjustment to the protected palette window only. */
void Effect_AdjustPaletteWindow(s32 adj)
{
    volatile u16 *pal = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next;
    Actor_Run16();
    phase = 0;
    do {
        u32 idx = phase >> 16;
        u32 win;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            win = (idx + 0xff3f) << 16;
            if (win > 0x70000)
                pal[idx] = Actor_Apply3(pal[idx], adj);
        }
        next = phase + 0x10000;
        phase = next;
    } while (next <= 0x00df0000);
    Actor_Run17(); Actor_Run18(); Actor_Run19(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and
 * blue fall. Control jumps over a mask literal inside the span and rejoins
 * before the common return.
 */
u16 Effect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Actor_Run2(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Actor_Run3(green, adj));
    blue = (s16)(blue - Actor_Run4(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}

s32 Effect_UpdateOrbitingParticle(struct Particle *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Actor_Check3(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Actor_Check4(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Actor_Check5(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Actor_Check6() << 9) >> 16);
    jitter += (s32)(((u32)Actor_Check7() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}

void Effect_InitOrbitingParticle(void)
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
