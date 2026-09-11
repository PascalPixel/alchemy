#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_placement/staged_placement.h"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"

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

u8 *Actor_Run9();

void ConfigureSceneForActorEightColumn(void)
{
    u8 *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Actor_Run9(8);
    x = *(s32 *)(actor + 8);
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Actor_Run10();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Actor_Run11(18, 40, 6, 3, a4, a5);
        Actor_Run12(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Actor_Run13(24, 40, 6, 3, a4, a5);
        Actor_Run14(0x302);
    }
    Actor_Run15();
}

/* overlays/scene/actor/staged_placement/actor_presentation.c */

static __inline__ void ConfigureFirst(s32 actor, s32 angle, s32 zero)
{
    Actor_Place17(actor, angle, zero);
}

static __inline__ void ConfigureSecond(s32 actor, s32 mode, s32 zero)
{
    Actor_Place18(actor, mode, zero);
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

u8 *Actor_Run16();

u8 *Actor_Run17();

u8 *Actor_Run18();
u8 *Actor_Run19();
u8 *Actor_Run20();

u8 *Actor_Run21();

u8 *Actor_Run22();

u8 *Actor_Run23();

u8 *Actor_Run24();

u8 *Actor_Run25();

u8 *Actor_Run26();

u8 *Actor_Run27();

u8 *Actor_Run28();

u8 *Actor_Run29();

u8 *Actor_Run30();

u8 *Actor_Run31();

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
        rec4 = Actor_Check25(9);
        reca = (s32)Actor_Run16(9);
        a2 = *(volatile s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Actor_Run2();
        ta = (s32)((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Actor_Check26();
        na = -na;
        Actor_SetRect(a0, *(volatile s32 *)(rec4 + 12), a2, 0, na, (s32)((u32)(v3a << 1) >> 16), flags);
        flags = *state & 15;
        if (flags == 0) {
            rec7b = Actor_Check2(9);
            v1b = Actor_Run3();
            a0 = *(volatile s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Actor_Check27(9);
            recb = (s32)Actor_Run17(9);
            a2 = *(volatile s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Actor_Run4();
            tb = (s32)((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Actor_Check28();
            nb = -nb;
            Actor_SetRect2(a0, *(volatile s32 *)(rec4 + 12), a2, 0, nb, (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}

void Actor_Run32(void)
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

    r1 = Actor_Check3(9);
    kind = *(volatile s32 *)(r1 + 8) / 0x100000;
    ((void (*)())Actor_Check29)();
    if (kind == 25) {
        p0 = Actor_Run18(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Actor_Run19(11);
        Actor_Run33(r2, 0);
        Actor_Run5(11, 14);
        r3 = (s32)Actor_Run20(11);
        Actor_Run34(r3, 1);
        Actor_Place(11, 0x19e0000, 0xf00000);
        Actor_Run35(10);
        base = (s32)Actor_Run36;
        Actor_Run37(base, 0xc80);
        Actor_Run38(141);
        Actor_Run39(9, 1, 0);
        Actor_Do(9);
        Actor_Run40(10);
        Actor_Run41(9, 2, 0);
        Actor_Run42(9);
        r4 = (s32)Actor_Run21(9);
        *(volatile s32 *)(r4 + 68) = zero;
        r5 = (s32)Actor_Run28(9);
        *(volatile s32 *)(r5 + 72) = 0x9999;
        Actor_Run43(3);
        Actor_Place2(9, 0x28000, 0x4000);
        Actor_Do2(0x120);
        Actor_Place3(9, 0x1a0, 200);
        r6 = (s32)Actor_Run23(9);
        Actor_Run44(r6, 0);
        Actor_Run45(base);
        Actor_Run46(12);
        Actor_Run47(189);
        rec7 = Actor_Check4(9);
        v1 = Actor_Run6();
        a0 = *(volatile s32 *)(rec7 + 8);
        a0 = a0 + (s32)((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = Actor_Check30(9);
        r7 = (s32)Actor_Run24(9);
        a2 = *(volatile s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Actor_Run7();
        t = (s32)((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Actor_Check31();
        n = -n;
        Actor_SetRect3(a0, *(volatile s32 *)(rec4 + 12), a2, zero, n, (s32)((u32)(v3 << 1) >> 16), zero);
        Actor_Run48(20);
        Actor_Run49(154);
        Actor_Place4(0x50000, 0x50000, 0x10000);
        Actor_Place5(-1, -1, 0xe666);
        Actor_Run50();
        Actor_Run51(9, 0, 0);
        Actor_Place6(11, 0, 0);
        Actor_Do3(0x300);
        Actor_SetRect4(21, 45, 4, 2, 21, 11);
    }
    Actor_Run52();
}

void Actor_Run53(void)
{
    s32 a;
    s32 b;
    s32 x0, x1, x2;
    s32 c2;
    s32 q1, q2, q3, e1, e2;
    s32 g1a, g1b, g1c, g2a, g2b, g2c, g3a, g3b, g3c, g4a, g4b, g4c, g5a, g5b, g5c, g6a, g6b, g6c;

    q1 = Actor_Check5(10);
    a = *(volatile s32 *)(q1 + 8) / 0x100000;
    q2 = Actor_Check6(10);
    b = *(volatile s32 *)(q2 + 16) / 0x100000;
    if (a == 38) {
        if (b == 14) {
            q3 = (s32)Actor_Run25(10);
            *(volatile s32 *)(q3 + 12) = -0x20000;
            e1 = Actor_Check7(10);
            e2 = Actor_Check8(10);
            *(volatile s32 *)(e1 + 60) = *(volatile s32 *)(e2 + 12);
            Actor_Run54(188);
            g1a = Actor_Check9(10);
            g1b = Actor_Check10(10);
            g1c = (s32)Actor_Run26(10);
            Actor_Run55(*(s32 *)(g1a + 8), *(s32 *)(g1b + 12),
                  *(s32 *)(g1c + 16), 0x8000, 0, 0, 1);
            g2a = Actor_Check11(10);
            g2b = Actor_Check12(10);
            g2c = (s32)Actor_Check32(10);
            Actor_Run56(*(s32 *)(g2a + 8), *(s32 *)(g2b + 12),
                  *(s32 *)(g2c + 16), 0x6666, 0x6666, 0, 1);
            g3a = Actor_Check13(10);
            g3b = Actor_Check14(10);
            g3c = (s32)Actor_Check33(10);
            x0 = *(s32 *)(g3a + 8);
            x1 = *(s32 *)(g3b + 12);
            x2 = *(s32 *)(g3c + 16);
            c2 = -0x6666;
            Actor_Run57(x0, x1, x2, c2, 0x6666, 0, 1);
            g4a = Actor_Check15(10);
            g4b = Actor_Check16(10);
            g4c = (s32)Actor_Run27(10);
            Actor_Run58(*(s32 *)(g4a + 8), *(s32 *)(g4b + 12),
                  *(s32 *)(g4c + 16), -0x8000, 0, 0, 1);
            g5a = Actor_Check17(10);
            g5b = Actor_Check18(10);
            g5c = (s32)Actor_Check34(10);
            Actor_Run59(*(s32 *)(g5a + 8), *(s32 *)(g5b + 12),
                  *(s32 *)(g5c + 16), 0x6666, c2, 0, 1);
            g6a = Actor_Check19(10);
            g6b = Actor_Check20(10);
            g6c = (s32)Actor_Check35(10);
            Actor_Run60(*(s32 *)(g6a + 8), *(s32 *)(g6b + 12),
                  *(s32 *)(g6c + 16), c2, c2, 0, 1);
            Actor_Check36(0x301);
        }
    }
}

void Scene_RunScene389(void)
{
    u32 i;
    u8 *record;

    Actor_Do4(0x20090c9);
    Actor_Run61();
    Actor_Place7(13, 0x100, 30);
    Actor_Run62(13, 2);
    Actor_Place8(0, 0xa000, 0);
    Actor_Do5(0x132f);
    Actor_Run63(13, 0);
    Actor_Run64(13, 3);
    Actor_Run65(30);
    *(u8 *)(Actor_Check37(10) + 35) &= 253;
    Actor_Place9(13, 0x20000, 0x10000);
    Actor_Place10(13, 0x258, 216);
    Actor_Place11(13, 0x258, 248);
    Actor_Place12(13, 0x238, 0x128);
    Actor_Run66(13, 0, 0);
    {
        u8 *record = Actor_Run29(10);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Actor_Do6(0x869);
    Actor_Run67();
}

s32 Scene_RunScene389(void)
{
    u8 *record;
    u8 *work;

    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x204;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    {
        u8 *record = Actor_Run30(9);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 16);
    }
    if (Actor_Check21(0x302) != 0) {
        Actor_Place13(8, 0x1580000, 0x680000);
        Actor_SetRect5(24, 40, 6, 3, 18, 6);
    } else {
        Actor_SetRect6(18, 40, 6, 3, 18, 6);
    }
    if (Actor_Check22(0x300) != 0) {
        Actor_Run68(9, 0, 0);
        Actor_SetRect7(21, 45, 4, 2, 21, 11);
    }
    if (Actor_Check23(0x301) != 0) {
        Actor_Place14(10, 0x2680000, 0xe80000);
        if ((u32)(((u16)gCell[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Actor_Check38(10) + 34) = 2;
        record = Actor_Check24(10);
        *(volatile s32 *)((s32)record + 12) = (*(volatile s32 *)((s32)record + 12) - 1);
        {
            u8 mask = 2;
            u8 *record = Actor_Run31(10);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | mask);
        }
        Actor_SetRect8(36, 48, 5, 1, 36, 14);
    } else {
    }
    L_0200131c:;
    if (gCell[225] == 99) {
        Actor_Run69();
        Actor_Run70();
        Actor_Place15(9, 0x1800000, 0xc00000);
        Actor_Run71(60);
        *(u8 *)(Actor_Check39(9) + 34) = 2;
        Actor_Place16(9, 0x198, 192);
        Actor_Do7(60);
        Actor_Run72();
    }
    if (gCell[282] != 0) {
        {
            s32 zero = 0;
            *(volatile s32 *)gOv = zero;
        }
        Actor_Run8(0x20090c9, 0xc80);
    }
    return 0;
}

/* overlays/scene/actor/staged_placement/scene_setup.c */

void Scene_RunThreeCallSequence(void)
{
    Actor_Run73();
    Actor_Run74();
    Actor_Run75();
}

void Scene_RunFourStepSequence(void)
{
    Actor_Run76();
    Actor_Run77();
    Actor_Run78();
    Actor_Run79();
}

/* overlays/scene/actor/staged_placement/shared.c */

void Scene_Forward(void)
{
    Actor_Run80();
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
