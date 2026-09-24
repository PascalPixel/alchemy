#include "TYPES.H"
#include "FIELD_EVENT.H"

#include "STAGED_ACTOR.H"

#include "TYPES.H"

void Func_02001ee2(s32, s32);

static __inline__ void ConfigureFirst(s32 actor, s32 angle, s32 zero)
{
    Actor_ShowEmote(actor, angle, zero);
}

static __inline__ void ConfigureSecond(s32 actor, s32 mode, s32 zero)
{
    Actor_Jump(actor, mode, zero);
}

static __inline__ void ConfigureThird(s32 actor, s32 value)
{
    Func_02001ee2(actor, value);
}

#include "TYPES.H"

#include "TYPES.H"

void Func_02001f1e(s32 actor, s32 mode);

#include "TYPES.H"

#include "TYPES.H"

u8 *Func_02001fc4();

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
extern u8 Data_020098ec[];

void Func_020016ba();
void Func_02001730();
void Func_020018b0();
void Func_020019c8();
void Func_020019fa();
void Func_02001a2e();
void Func_02001a5c();
void Func_02001a8c();
void Func_02001abc();
s32 Func_0200203e();
s32 Func_0200205a();
u8 *Func_02002062();
s32 Func_020020b4();
s32 Func_020020d0();
u8 *Func_020020d8();
void Func_02002108();
s32 Func_02002142();
u8 *Func_0200215e();
u8 *Func_0200216e();
u8 *Func_02002182();
void Func_0200218a();
u8 *Func_020021da();
u8 *Func_020021e4();
u8 *Func_02002216();
s32 Func_02002234();
s32 Func_02002250();
u8 *Func_02002258();
s32 Func_02002330();
s32 Func_02002342();
u8 *Func_02002360();
s32 Func_0200236a();
s32 Func_02002372();
s32 Func_02002382();
s32 Func_0200238a();
u8 *Func_02002392();
s32 Func_020023b6();
s32 Func_020023be();
s32 Func_020023c6();
s32 Func_020023e8();
s32 Func_020023f0();
s32 Func_020023f8();
s32 Func_0200241c();
s32 Func_02002424();
u8 *Func_0200242c();
s32 Func_0200244a();
s32 Func_02002452();
s32 Func_0200245a();
s32 Func_0200247a();
s32 Func_02002482();
s32 Func_0200248a();
void Func_02008bd1();
u8 *Func_020021e4_a();
void Func_0200251a();
s32 Func_020025f8();
u8 *Func_02002646();
void Func_02002046();
u8 *Func_0200268a();
s32 Func_02002730();
void Func_02002734();
s32 Func_0200273c();
u8 *Func_02002748();
s32 Func_0200279a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call1_0200115c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_0200121c(void (*f)(), s32 a0)
{
    f(a0);
}

#include "TYPES.H"

void Func_02001f5c(void);
void Func_02001f74(void);

#include "TYPES.H"

void Func_020025a8(void);

#include "TYPES.H"

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};


void ConfigureActorThirteenSceneParameters(void)
{
    ConfigureFirst(13, 256, 0);
    ConfigureSecond(13, 2, 0);
    ConfigureThird(12, 40);
}

void SceneEffect_AdvancePositionByAxisMode(u8 *o)
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

s32 OverlayObject_ApplyValue15(s32 actor)
{
    Func_02001f1e(actor, 15);
    return 0;
}

u8 *SceneData_GetTable9690(void) { return (u8 *)0x02009690; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetTable9750(void) { return (u8 *)0x02009750; }

u8 *SceneData_GetTable976c(void) { return (u8 *)0x0200976c; }

void ConfigureSceneForActorEightColumn(void)
{
    u8 *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Func_02001fc4(8);
    x = *(s32 *)(actor + 8);
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Event_Begin();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Map_CopyCellAttributes(18, 40, 6, 3, a4, a5);
        GameFlag_Clear(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Map_CopyCellAttributes(24, 40, 6, 3, a4, a5);
        GameFlag_Set(0x302);
    }
    Event_End();
}

void FieldScene_RunPrimarySequence(void)
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
        rec7a = Value1(Func_0200203e, 9);
        v1a = Value0(Engine_RandomNext);
        a0 = *(s32 *)(rec7a + 8);
        a0 = a0 + (s32)((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = Func_0200205a(9);
        reca = (s32)Func_02002062(9);
        a2 = *(s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Value0(Engine_RandomNext);
        ta = (s32)((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Random_Next();
        na = -na;
        Call7(Func_020016ba, a0, *(s32 *)(rec4 + 12), a2, 0, na,
              (s32)((u32)(v3a << 1) >> 16), flags);
        flags = *state & 15;
        if (flags == 0) {
            rec7b = Value1(Func_020020b4, 9);
            v1b = Value0(Engine_RandomNext);
            a0 = *(s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Func_020020d0(9);
            recb = (s32)Func_020020d8(9);
            a2 = *(s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Value0(Engine_RandomNext);
            tb = (s32)((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Random_Next();
            nb = -nb;
            Call7(Func_02001730, a0, *(s32 *)(rec4 + 12), a2, 0, nb,
                  (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}

void Func_02000ce0(void)
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

    r1 = Value1(Func_02002142, 9);
    kind = *(s32 *)(r1 + 8) / 0x100000;
    ((void (*)())Engine_EventBegin)();
    if (kind == 25) {
        p0 = Func_0200215e(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Func_0200216e(11);
        Actor_SetSpriteFlags(r2, 0);
        Call2((void (*)())Func_020021e4, 11, 14);
        r3 = (s32)Func_02002182(11);
        Object_SetBlendMode(r3, 1);
        Actor_SetPosition(11, 0x19e0000, 0xf00000);
        Event_Wait(10);
        base = (s32)Func_02008bd1;
        Func_02002108(base, 0xc80);
        Audio_PlayCue(141);
        Actor_SetDestinationOffset(9, 1, 0);
        Call1((void (*)())Engine_ActorWaitForMove, 9);
        Event_Wait(10);
        Actor_SetDestinationOffset(9, 2, 0);
        Actor_WaitForMove(9);
        r4 = (s32)Func_020021da(9);
        *(s32 *)(r4 + 68) = zero;
        r5 = (s32)Func_020021e4_a(9);
        *(s32 *)(r5 + 72) = 0x9999;
        Event_Wait(3);
        Actor_SetSpeed(9, 0x28000, 0x4000);
        Audio_PlayCue(0x120);
        Actor_SetDestination(9, 0x1a0, 200);
        r6 = (s32)Func_02002216(9);
        Actor_SetSpriteFlags(r6, 0);
        Func_0200218a(base);
        Event_Wait(12);
        Audio_PlayCue(189);
        rec7 = Value1(Func_02002234, 9);
        v1 = Value0(Engine_RandomNext);
        a0 = *(s32 *)(rec7 + 8);
        a0 = a0 + (s32)((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = Func_02002250(9);
        r7 = (s32)Func_02002258(9);
        a2 = *(s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Value0(Engine_RandomNext);
        t = (s32)((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Random_Next();
        n = -n;
        Call7(Func_020018b0, a0, *(s32 *)(rec4 + 12), a2, zero, n,
              (s32)((u32)(v3 << 1) >> 16), zero);
        Event_Wait(20);
        Audio_PlayCue(154);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        MapRender_WaitForValues();
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(11, 0, 0);
        GameFlag_Set(0x300);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    Event_End();
}

void Func_02000ecc(void)
{
    s32 a;
    s32 b;
    s32 x0, x1, x2;
    s32 c2;
    s32 q1, q2, q3, e1, e2;
    s32 g1a, g1b, g1c, g2a, g2b, g2c, g3a, g3b, g3c, g4a, g4b, g4c, g5a, g5b, g5c, g6a, g6b, g6c;

    q1 = Value1(Func_02002330, 10);
    a = *(s32 *)(q1 + 8) / 0x100000;
    q2 = Value1(Func_02002342, 10);
    b = *(s32 *)(q2 + 16) / 0x100000;
    if (a == 38) {
        if (b == 14) {
            q3 = (s32)Func_02002360(10);
            *(s32 *)(q3 + 12) = -0x20000;
            e1 = Value1(Func_0200236a, 10);
            e2 = Value1(Func_02002372, 10);
            *(s32 *)(e1 + 60) = *(s32 *)(e2 + 12);
            Audio_PlayCue(188);
            g1a = Value1(Func_02002382, 10);
            g1b = Value1(Func_0200238a, 10);
            g1c = (s32)Func_02002392(10);
            Func_020019c8(*(s32 *)(g1a + 8), *(s32 *)(g1b + 12),
                  *(s32 *)(g1c + 16), 0x8000, 0, 0, 1);
            g2a = Value1(Func_020023b6, 10);
            g2b = Value1(Func_020023be, 10);
            g2c = (s32)Func_020023c6(10);
            Func_020019fa(*(s32 *)(g2a + 8), *(s32 *)(g2b + 12),
                  *(s32 *)(g2c + 16), 0x6666, 0x6666, 0, 1);
            g3a = Value1(Func_020023e8, 10);
            g3b = Value1(Func_020023f0, 10);
            g3c = (s32)Func_020023f8(10);
            x0 = *(s32 *)(g3a + 8);
            x1 = *(s32 *)(g3b + 12);
            x2 = *(s32 *)(g3c + 16);
            c2 = -0x6666;
            Func_02001a2e(x0, x1, x2, c2, 0x6666, 0, 1);
            g4a = Value1(Func_0200241c, 10);
            g4b = Value1(Func_02002424, 10);
            g4c = (s32)Func_0200242c(10);
            Func_02001a5c(*(s32 *)(g4a + 8), *(s32 *)(g4b + 12),
                  *(s32 *)(g4c + 16), -0x8000, 0, 0, 1);
            g5a = Value1(Func_0200244a, 10);
            g5b = Value1(Func_02002452, 10);
            g5c = (s32)Func_0200245a(10);
            Func_02001a8c(*(s32 *)(g5a + 8), *(s32 *)(g5b + 12),
                  *(s32 *)(g5c + 16), 0x6666, c2, 0, 1);
            g6a = Value1(Func_0200247a, 10);
            g6b = Value1(Func_02002482, 10);
            g6c = (s32)Func_0200248a(10);
            Func_02001abc(*(s32 *)(g6a + 8), *(s32 *)(g6b + 12),
                  *(s32 *)(g6c + 16), c2, c2, 0, 1);
            GameFlag_Set(0x301);
        }
    }
}

void FieldScene_RunThreeCallSequence(void)
{
    Event_Begin();
    Func_02001f5c();
    Event_End();
}

void FieldScene_RunFourStepSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    Func_02001f74();
    Event_End();
}

void FieldScene_Forward25a8(void)
{
    Func_020025a8();
}

u8 *SceneData_GetTable9814(void)
{
    return (u8 *)0x02009814;
}

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunScene389_0200115c(void)
{
    Call1_0200115c(Func_0200251a, 0x20090c9);
    Event_Begin();
    Actor_ShowEmote(13, 0x100, 30);
    Actor_RunRepeatedMotion(13, 2);
    Actor_FaceDirection(0, 0xa000, 0);
    Event_SetMessage(MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE);
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(30);
    *(u8 *)(Func_020025f8(10) + 35) &= 253;
    Actor_SetSpeed(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    SetFlagBits(Func_02002646(10) + 35, 2);
    GameFlag_Set(0x869);
    Event_End();
}

s32 FieldScene_RunScene389_0200121c(void)
{
    u8 *record;
    u8 *work;

    work = *(u8 **)Data_03001ebc;
    *(s32 *)((s32)work + 0x1c0) = 0x204;
    *(s32 *)((s32)work + 0x1c8) = 24;
    SetFlagBits(Func_0200268a(9) + 89, 16);
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(8, 0x1580000, 0x680000);
        Map_CopyCellAttributes(24, 40, 6, 3, 18, 6);
    } else {
        Map_CopyCellAttributes(18, 40, 6, 3, 18, 6);
    }
    if (GameFlag_IsSet(0x300) != 0) {
        Actor_SetPosition(9, 0, 0);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    if (GameFlag_IsSet(0x301) != 0) {
        Actor_SetPosition(10, 0x2680000, 0xe80000);
        if ((u32)(((u16)Data_02000240[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Func_02002730(10) + 34) = 2;
        record = Value1(Func_0200273c, 10);
        *(s32 *)((s32)record + 12) = *(s32 *)((s32)record + 12) - 1;
        {
            u8 bits = 2;
            u8 *flags = Func_02002748(10) + 35;

            *flags |= bits;
        }
        Map_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
L_0200131c:
    if (Data_02000240[225] == 99) {
        Event_OpenScreen();
        Event_WaitForScreen();
        Actor_SetPosition(9, 0x1800000, 0xc00000);
        Event_Wait(60);
        *(u8 *)(Func_0200279a(9) + 34) = 2;
        Actor_MoveToAndWait(9, 0x198, 192);
        Call1_0200121c((void (*)())Engine_EventWait, 60);
        Func_02002046();
    }
    if (Data_02000240[282] != 0) {
        {
            s32 zero = 0;
            *(s32 *)Data_020098ec = zero;
        }
        Call2(Func_02002734, 0x20090c9, 0xc80);
    }
    return 0;
}
