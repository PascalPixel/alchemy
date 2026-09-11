#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_palette/staged_palette.h"
#include "staged_actor.h"
#include "staged_actor_probe_state.h"
#include "staged_actor_effect.h"
#include "resource_393.h"

/* overlays/scene/actor/staged_palette/move_and_redraw.c */

void Actor_Run(
    StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_palette/scene.c */
/*
 * resource_393 scene script: staged-actor motion, scene beats, and the
 * overlay's palette adjustment.
 */

/*
 * Several aliases below resolve to the same relocation.  The spellings are
 * call-site evidence and are deliberately kept distinct.
 */
#define SCENE_WORKSPACE (*(u8 **)0x03001ebc)

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

extern u8 *gWork;
extern u8 *gCam;
extern u8 gUnk[];
extern s32 StagedActorProbeValues[];
extern s32 Actor_Far[];

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

struct StagedActor *FindStagedActorAtProbe(struct StagedActorProbePoint *, struct StagedActor *);

void Actor_Run2(void); void Actor_Run3(void);

void Actor_Run4(s32, s32); void Actor_Run5(s32, s32, s32);
void Actor_Run6(s32); u8 *Actor_Run7();

void Actor_Run8(s32); void Actor_Run9(u8 *, s32);

struct StagedActorEffect *GetStagedActorEffect(s32 actor_index);

struct SceneBeatSubject *GetSceneBeatSubject();

struct SceneBeatSubject *GetScenePresentationSubject();

/*
 * Six-argument draw wrapper.  Inlining it here preserves the reference's
 * r2-before-r3 stacked-literal order at the call site.
 */

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Actor_Run10(s32, s32); u8 *Actor_Run11(s32);

    Actor_SetRect(left, top, width, height, tile, palette);
}

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Actor_Run10();

    DrawSceneBeatRectangle(left, top, width, height, tile, palette);
}

/*
 * One symbol per call site, named at the site's own address.  All three
 * reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.  The sites are 0x02000d14,
 * 0x02000d22 and 0x02000d30.
 */

/*
 * Distance between two three-component 16.16 fixed-point positions.  Each
 * argument walks three consecutive words in x, y, z order; the per-axis
 * deltas are shifted down to integers, squared, summed, and passed to the
 * resident IWRAM integer square root.  The walking-pointer form is
 * load-bearing and must not become struct field access.
 */

s32 Actor_CalculateFixedPointDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dx2 = dx *dx;
    s32 dy2 = dy *dy;
    s32 dz2 = dz *dz;

    return ((IwramIntegerSquareRoot) 0x030001D8)(dx2 + dy2 + dz2);
}

s32 *Actor_FindAtTileXZ(s32 *arg0)
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
    Actor_Do(15);
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

u8 *MapStagedScene_SelectPrimaryData(void) { return (u8 *)0x02008fc8; }

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void) { return (u8 *)0x02009028; }

u8 *MapStagedScene_SelectTertiaryData(void) { return (u8 *)0x02009038; }

/*
 * Placement query followed by the tile-(10,12) scene transition.  The
 * six-word result is one aggregate and its two-word tail is forwarded by
 * value.  Keeping `zero' live across the draw is load-bearing: it lets the
 * dead result pointer be reused for the following stack slot.
 */

void Scene_RunActorTenPlacementScene(void)
{
    void Actor_Run10(s32, s32); u8 *Actor_Run11(s32);

    struct PlacementResult result;
    Actor_Run12();
    if (Actor_Check(&result)) {
        Actor_Do2(result);
        if (result.second == 10 && (result.third >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Actor_Run4(10, 3);
            Actor_Run5(10, -18, 6);
            Actor_Run6(30);
            Actor_Do3(240);
            Actor_Run10(10, 8);
            Actor_Run11(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            Actor_SetRect2(2, 12, 16, 1, 4, zero);
            Actor_Run8(0x201);
            actor = Actor_Run7(10);
            Actor_Run9(actor, 0);
        }
    }
    Actor_Run13();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    s32 StartStagedActorEffectSound();
    s32 TestStagedActorEffectCell();

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
        FinishStagedActorEffect();
        return 1;
    }
    return 0;
}

void Actor_ApplyOffsetObjectPosition(void)
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

/* Set workspace word 448 to 516, then run the scene's beat sequence. */
s32 State_SetRuntimeWord448To516(void)
{
    void RunPhase516Followup();

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

void Effect_AdjustPaletteColors(s32 a)
{
    s32 Actor_Run14(s32, s32);

    u32 x;

    Actor_Run15();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Actor_Run14(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    Actor_Run16();
    Actor_Run17();
    Actor_Apply(0x10000, 0);
}

/*
 * resource_393 owner at 0x02000cf4, 104 bytes: the asymmetric RGB555 colour
 * adjustment.  Red rises while green and blue fall, each through the same
 * per-channel scale.
 */
u16 Effect_AdjustColorChannels(u16 color, s32 adj)
{
    s32 StartStagedActorEffectSound(s32, s32);

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Actor_Check2(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Actor_Check3(green, adj));
    blue = (s16)(blue - Actor_Check4(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
