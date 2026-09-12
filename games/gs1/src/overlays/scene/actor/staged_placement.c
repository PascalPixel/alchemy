#include "types.h"
#include "scene.h"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"
#include "staged_actor_probe_state.h"
#include "staged_actor_movement.h"

/* overlays/scene/actor/staged_placement/staged_placement.c */
/* overlays/scene/actor/staged_placement/actor_effect_motion.c */
void Effect_AdvancePositionByAxisMode(u8 *o)
{
    s16 v = *(s16 *)(o + 102);

    switch (v) {
    case 0:
        *(s32 *)(o + 8) += *(s32 *)(o + 48);
        *(s32 *)(o + 56) = *(s32 *)(o + 8);
        *(s32 *)(o + 12) += *(s32 *)(o + 52);
        *(s32 *)(o + 60) = *(s32 *)(o + 12);
        break;
    case 1:
        *(s32 *)(o + 8) += *(s32 *)(o + 48);
        *(s32 *)(o + 56) = *(s32 *)(o + 8);
        *(s32 *)(o + 16) += *(s32 *)(o + 52);
        *(s32 *)(o + 64) = *(s32 *)(o + 16);
        break;
    case 2:
        *(s32 *)(o + 12) += *(s32 *)(o + 48);
        *(s32 *)(o + 60) = *(s32 *)(o + 12);
        *(s32 *)(o + 16) += *(s32 *)(o + 52);
        *(s32 *)(o + 64) = *(s32 *)(o + 16);
        break;
    }
}

/* overlays/scene/actor/staged_placement/actor_position.c */

u8 *Actor_unk9_4();

void ConfigureSceneForActorEightColumn(void)
{
    u8 *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Actor_unk9_4(8);
    x = *(s32 *)(actor + 8);
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Actor_unk10_4();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Actor_unk11_4(18, 40, 6, 3, a4, a5);
        Actor_unk12_4(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Actor_unk13_4(24, 40, 6, 3, a4, a5);
        Actor_unk14_4(0x302);
    }
    Actor_unk15_4();
}

/* overlays/scene/actor/staged_placement/actor_presentation.c */

static __inline__ void ConfigureFirst(s32 actor, s32 angle, s32 zero)
{
    Actor_unk17_3(actor, angle, zero);
}

static __inline__ void ConfigureSecond(s32 actor, s32 mode, s32 zero)
{
    Actor_unk18_3(actor, mode, zero);
}

static __inline__ void ConfigureThird(s32 actor, s32 value)
{
    Actor_Apply(actor, value);
}

void ConfigureActorThirteenSceneParameters(void)
{
    ConfigureFirst(13, 256, 0);
    ConfigureSecond(13, 2, 0);
    ConfigureThird(12, 40);
}

/* overlays/scene/actor/staged_placement/overlay_object.c */

s32 OvObj_ApplyValue15(s32 actor)
{
    Actor_Apply2(actor, 15);
    return 0;
}

/* overlays/scene/actor/staged_placement/scene_data.c */
u8 *SceneData_GetTable9690(void) { return (u8 *)0x02009690; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetTable9750(void) { return (u8 *)0x02009750; }

u8 *SceneData_GetTable976c(void) { return (u8 *)0x0200976c; }

u8 *SceneData_GetTable9814(void)
{
    return (u8 *)0x02009814;
}

/* overlays/scene/actor/staged_placement/scene_primary_script.c */
extern u8 gWork[];
extern s16 gCell[];
extern u8 gOv[];

u8 *Actor_unk16_4();

u8 *Actor_unk17_4();

u8 *Actor_unk18_4();
u8 *Actor_unk19_4();
u8 *Actor_unk20_4();

u8 *Actor_unk21_4();

u8 *Actor_unk22_4();

u8 *Actor_unk23_4();

u8 *Actor_unk24_4();

u8 *Actor_unk25_4();

u8 *Actor_unk26_4();

u8 *Actor_unk27_4();

u8 *Actor_unk28_4();

u8 *Actor_unk29_4();

u8 *Actor_unk30_4();

u8 *Actor_unk31_4();

void Scene_RunPrimarySequence(void)
{
    volatile s32 *state = (volatile s32 *)0x03001e40;
    s32 flags;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 rec7a, reca, v1a, v2a, v3a, ta, ua, na;
    s32 rec7b, recb, v1b, v2b, v3b, tb, ub, nb;

    flags = *state & 7;
    if (flags == 0) {
        rec7a = Actor_Check(9);
        v1a = Actor_Run();
        a0 = *(volatile s32 *)(rec7a + 8);
        a0 = a0 + (s32)((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = Actor_unk25(9);
        reca = (s32)Actor_unk16_4(9);
        a2 = *(volatile s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Actor_unk2_4();
        ta = (s32)((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Actor_unk26();
        na = -na;
        Actor_SetRect(a0, *(volatile s32 *)(rec4 + 12), a2, 0, na, (s32)((u32)(v3a << 1) >> 16), flags);
        flags = *state & 15;
        if (flags == 0) {
            rec7b = Actor_unk2(9);
            v1b = Actor_unk3_4();
            a0 = *(volatile s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Actor_unk27(9);
            recb = (s32)Actor_unk17_4(9);
            a2 = *(volatile s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Actor_unk4_4();
            tb = (s32)((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Actor_unk28();
            nb = -nb;
            Actor_unk2_5(a0, *(volatile s32 *)(rec4 + 12), a2, 0, nb, (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}

void Scene_RunScene389SequenceA(void)
{
    s32 kind;
    s32 zero;
    s32 base;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 r1, r2, r3, r4, r5, r6, r7;
    s32 rec7, v1, v2, v3, t, u, n;
    u8 *p0;

    r1 = Actor_unk3(9);
    kind = *(volatile s32 *)(r1 + 8) / 0x100000;
    ((void (*)())Actor_unk29)();
    if (kind == 25) {
        p0 = Actor_unk18_4(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Actor_unk19_4(11);
        Actor_unk33_4(r2, 0);
        Actor_unk5_4(11, 14);
        r3 = (s32)Actor_unk20_4(11);
        Actor_unk34_4(r3, 1);
        Actor_Place(11, 0x19e0000, 0xf00000);
        Actor_unk35_4(10);
        base = (s32)Actor_unk36_4;
        Actor_unk37_4(base, 0xc80);
        Actor_unk38_4(141);
        Actor_unk39_4(9, 1, 0);
        Actor_Do(9);
        Actor_unk40_4(10);
        Actor_unk41_4(9, 2, 0);
        Actor_unk42_4(9);
        r4 = (s32)Actor_unk21_4(9);
        *(volatile s32 *)(r4 + 68) = zero;
        r5 = (s32)Actor_unk28_4(9);
        *(volatile s32 *)(r5 + 72) = 0x9999;
        Actor_unk43_4(3);
        Actor_unk2_3(9, 0x28000, 0x4000);
        Actor_unk2_2(0x120);
        Actor_unk3_3(9, 0x1a0, 200);
        r6 = (s32)Actor_unk23_4(9);
        Actor_unk44_4(r6, 0);
        Actor_unk45_4(base);
        Actor_unk46_4(12);
        Actor_unk47_4(189);
        rec7 = Actor_unk4(9);
        v1 = Actor_unk6_4();
        a0 = *(volatile s32 *)(rec7 + 8);
        a0 = a0 + (s32)((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = Actor_unk30(9);
        r7 = (s32)Actor_unk24_4(9);
        a2 = *(volatile s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Actor_unk7_4();
        t = (s32)((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Actor_unk31();
        n = -n;
        Actor_unk3_5(a0, *(volatile s32 *)(rec4 + 12), a2, zero, n, (s32)((u32)(v3 << 1) >> 16), zero);
        Actor_unk48_4(20);
        Actor_unk49_4(154);
        Actor_unk4_3(0x50000, 0x50000, 0x10000);
        Actor_unk5_3(-1, -1, 0xe666);
        Actor_unk50_4();
        Actor_unk51_4(9, 0, 0);
        Actor_unk6_3(11, 0, 0);
        Actor_unk3_2(0x300);
        Actor_unk4_5(21, 45, 4, 2, 21, 11);
    }
    Actor_unk52_4();
}

void Scene_RunScene389SequenceB(void)
{
    s32 a;
    s32 b;
    s32 x0, x1, x2;
    s32 c2;
    s32 q1, q2, q3, e1, e2;
    s32 g1a, g1b, g1c, g2a, g2b, g2c, g3a, g3b, g3c, g4a, g4b, g4c, g5a, g5b, g5c, g6a, g6b, g6c;

    q1 = Actor_unk5(10);
    a = *(volatile s32 *)(q1 + 8) / 0x100000;
    q2 = Actor_unk6(10);
    b = *(volatile s32 *)(q2 + 16) / 0x100000;
    if (a == 38) {
        if (b == 14) {
            q3 = (s32)Actor_unk25_4(10);
            *(volatile s32 *)(q3 + 12) = -0x20000;
            e1 = Actor_unk7(10);
            e2 = Actor_unk8(10);
            *(volatile s32 *)(e1 + 60) = *(volatile s32 *)(e2 + 12);
            Actor_unk54_4(188);
            g1a = Actor_unk9(10);
            g1b = Actor_unk10(10);
            g1c = (s32)Actor_unk26_4(10);
            Actor_unk55_4(*(s32 *)(g1a + 8), *(s32 *)(g1b + 12),
                  *(s32 *)(g1c + 16), 0x8000, 0, 0, 1);
            g2a = Actor_unk11(10);
            g2b = Actor_unk12(10);
            g2c = (s32)Actor_unk32(10);
            Actor_unk56_4(*(s32 *)(g2a + 8), *(s32 *)(g2b + 12),
                  *(s32 *)(g2c + 16), 0x6666, 0x6666, 0, 1);
            g3a = Actor_unk13(10);
            g3b = Actor_unk14(10);
            g3c = (s32)Actor_unk33(10);
            x0 = *(s32 *)(g3a + 8);
            x1 = *(s32 *)(g3b + 12);
            x2 = *(s32 *)(g3c + 16);
            c2 = -0x6666;
            Actor_unk57_4(x0, x1, x2, c2, 0x6666, 0, 1);
            g4a = Actor_unk15(10);
            g4b = Actor_unk16(10);
            g4c = (s32)Actor_unk27_4(10);
            Actor_unk58_4(*(s32 *)(g4a + 8), *(s32 *)(g4b + 12),
                  *(s32 *)(g4c + 16), -0x8000, 0, 0, 1);
            g5a = Actor_unk17(10);
            g5b = Actor_unk18(10);
            g5c = (s32)Actor_unk34(10);
            Actor_unk59_4(*(s32 *)(g5a + 8), *(s32 *)(g5b + 12),
                  *(s32 *)(g5c + 16), 0x6666, c2, 0, 1);
            g6a = Actor_unk19(10);
            g6b = Actor_unk20(10);
            g6c = (s32)Actor_unk35(10);
            Actor_unk60_4(*(s32 *)(g6a + 8), *(s32 *)(g6b + 12),
                  *(s32 *)(g6c + 16), c2, c2, 0, 1);
            Actor_unk36(0x301);
        }
    }
}

void Scene_RunScene389(void)
{
    u32 i;
    u8 *record;

    Actor_unk4_2(0x20090c9);
    Actor_unk61_4();
    Actor_unk7_3(13, 0x100, 30);
    Actor_unk62_4(13, 2);
    Actor_unk8_3(0, 0xa000, 0);
    Actor_unk5_2(0x132f);
    Actor_unk63_4(13, 0);
    Actor_unk64_4(13, 3);
    Actor_unk65_4(30);
    *(u8 *)(Actor_unk37(10) + 35) &= 253;
    Actor_unk9_3(13, 0x20000, 0x10000);
    Actor_unk10_3(13, 0x258, 216);
    Actor_unk11_3(13, 0x258, 248);
    Actor_unk12_3(13, 0x238, 0x128);
    Actor_unk66_4(13, 0, 0);
    {
        u8 *record = Actor_unk29_4(10);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Actor_unk6_2(0x869);
    Actor_unk67_4();
}

void Scene_RunScene389(void)
{
    u8 *record;
    u8 *work;

    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x204;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    {
        u8 *record = Actor_unk30_4(9);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 16);
    }
    if (Actor_unk21(0x302) != 0) {
        Actor_unk13_3(8, 0x1580000, 0x680000);
        Actor_unk5_5(24, 40, 6, 3, 18, 6);
    } else {
        Actor_unk6_5(18, 40, 6, 3, 18, 6);
    }
    if (Actor_unk22(0x300) != 0) {
        Actor_unk68_4(9, 0, 0);
        Actor_unk7_5(21, 45, 4, 2, 21, 11);
    }
    if (Actor_unk23(0x301) != 0) {
        Actor_unk14_3(10, 0x2680000, 0xe80000);
        if ((u32)(((u16)gCell[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Actor_unk38(10) + 34) = 2;
        record = Actor_unk24(10);
        *(volatile s32 *)((s32)record + 12) = (*(volatile s32 *)((s32)record + 12) - 1);
        {
            u8 mask = 2;
            u8 *record = Actor_unk31_4(10);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | mask);
        }
        Actor_unk8_5(36, 48, 5, 1, 36, 14);
    } else {
    }
    L_0200131c:;
    if (gCell[225] == 99) {
        Actor_unk69_4();
        Actor_unk70_4();
        Actor_unk15_3(9, 0x1800000, 0xc00000);
        Actor_unk71_4(60);
        *(u8 *)(Actor_unk39(9) + 34) = 2;
        Actor_unk16_3(9, 0x198, 192);
        Actor_unk7_2(60);
        Actor_unk72_4();
    }
    if (gCell[282] != 0) {
        {
            s32 zero = 0;
            *(volatile s32 *)gOv = zero;
        }
        Actor_unk8_4(0x20090c9, 0xc80);
    }
    return 0;
}

/* overlays/scene/actor/staged_placement/scene_setup.c */

void Scene_RunThreeCallSequence(void)
{
    Actor_unk73_4();
    Actor_unk74_4();
    Actor_unk75_4();
}

void Scene_RunFourStepSequence(void)
{
    Actor_unk76_4();
    Actor_unk77_4();
    Actor_unk78_4();
    Actor_unk79_4();
}

/* overlays/scene/actor/staged_placement/shared.c */

void Scene_Forward(void)
{
    Actor_unk80_4();
}

/* overlays/scene/actor/staged_placement/staged_actor.c */

s32 StagedActor_FindClearPosition(s32 *a)
{
}

/* overlays/scene/actor/staged_placement/staged_actor_pair.c */

extern u32 ActorSearchStep[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void StagedActor_AdvanceActorPair(void)
{
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

/* overlays/scene/actor/staged_placement/reset_motion_if_blocked_ahead.c */
extern s32 StagedActorDirectionSteps[];
extern s32 StagedActorProbeValues[];
extern struct StagedActor *FindStagedActorAtProbe(
    struct StagedActorProbePoint *, struct StagedActor *);

s32 StagedActor_ResetMotionIfBlockedAhead(struct StagedActor *actor)
{
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
        s32 v =
            *(STAGED_ACTOR_PROBE_DETAILS(target)->unknown_28);
        s32 *p = StagedActorProbeValues;

        do {
            if (v == *p++) goto done;
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

/* overlays/scene/actor/staged_placement/move_and_redraw.c */
void Actor_Run(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
