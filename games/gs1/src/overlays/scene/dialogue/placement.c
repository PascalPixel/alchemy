/*
 * Overlay resource_386: actor placement, per-actor dialogue lines and the
 * scene initialiser that installs the per-frame task.
 */

#include "types.h"

#define WORKSPACE (*(u8 **)0x03001ebc)
#define State_CheckPositionWindow Func_02000030
#define SceneData_GetScriptTable Func_0200006c
#define SceneData_ReturnZero Func_02000074
#define SceneData_GetMessageTable Func_02000078
#define SceneData_InitAndGetTable87f4 Func_02000080
#define Actor_RunActorStep Func_02000098
#define Dialogue_RunActor9Line Func_020000bc
#define Dialogue_RunActor11Line Func_020000dc
#define Dialogue_RunActor12Line Func_020000fc
#define Scene_RunActor16Sequence Func_0200011c
#define Dialogue_RunActor16Line Func_020001c4
#define Dialogue_RunActor23Line Func_020001e4
#define Scene_RunActor18FlaggedSequence Func_02000204
#define Actor_RunActor16StepWithFlag91 Func_020002fc
#define Scene_RunActor18ConditionalCue Func_02000338
#define Scene_RunActor19StepByPlace Func_02000380
#define Scene_RunActor20StepByPlace Func_020003c8
#define Scene_RunActor21StepByPlace Func_02000410
#define Scene_RunActor22StepByPlace Func_02000458
#define Dialogue_RunActor18FlaggedLine Func_020004a0
#define SceneData_GetEffectTable Func_020004dc
#define Scene_InitSceneStateByStep Func_020004e4
#define OvObj_InitObject22 Func_02000570

#include "resource_386_state.h"

extern u8 Data_03001ebc[];
extern s16 Data_02000240[];

void Func_020006d4(u8 *);
void Func_020006da(s32);
void Func_0200071a(s32, s32);
void Func_02000752(s32, s32);
void Func_020006f6(void);
void Func_02000756(s32);
void Func_02000758(s32, s32, s32);
void Func_0200016a(s32);
void Func_02000776(s32);
void Func_02000778(s32, s32, s32);
void Func_0200018a(s32);
void Func_02000796(s32);
void Func_02000798(s32, s32, s32);
void Func_020001aa(s32);
void Func_0200075c();
void Func_02000784();
void Func_020007ac();
void Func_020007ba();
void Func_020007bc();
void Func_020007c2();
void Func_020007c6();
void Func_020007c8();
s32 Func_020007dc();
void Func_020007ee();
void Func_020007f4();
void Func_02000806();
s32 Func_02000814();
void Func_0200081c();
void Func_02000822();
void Func_0200084a();
void Func_0200085e(s32);
void Func_02000860(s32, s32, s32);
void Func_02000272(s32);
void Func_0200087e(s32);
void Func_02000292(s32);
s32 Func_0200082c();
void Func_02000844();
void Func_0200088e();
s32 Func_02000898();
void Func_0200089c();
s32 Func_0200089e();
void Func_020008b2();
void Func_020008b6();
void Func_020008c0();
void Func_020008c6();
void Func_020008c6_a();
void Func_020008d6();
void Func_020008e6();
void Func_020008f0();
void Func_020008f2();
void Func_020008f4();
void Func_020008f8();
void Func_020008fe();
void Func_02000918();
void Func_02000926();
void Func_02000930();
void Func_02000940();
void Func_0200095a();
void Func_02000992();
void Func_0200093c(void);
void Func_0200098c(s32, s32);
void Func_02000950(void);
void Func_02000438(void);
void Func_0200099c(s32, s32);
void Func_02000978(void);
s32 Func_0200096c(void);
void Func_020009ec(s32);
void Func_02000a04(s32, s32);
void Func_02000a2e(s32, s32);
void Func_020009c6(s32, s32);
void Func_020009ba(void);
u8 *Func_020009ea(s32);
void Func_02000a6a(s32, s32);
void Func_02000a3a(s32);
void Func_02000a52(s32, s32);
void Func_020009f6(void);
u8 *Func_02000a32(s32);
void Func_02000a10(void);
void Func_02000ab2(s32, s32);
void Func_02000a82(s32);
void Func_02000a9a(s32, s32);
void Func_02000a3e(void);
u8 *Func_02000a7a(s32);
void Func_02000a58(void);
void Func_02000afa(s32, s32);
void Func_02000aca(s32);
void Func_02000ae2(s32, s32);
void Func_02000a86(void);
u8 *Func_02000ac2(s32);
void Func_02000aa0(void);
void Func_02000b4a(s32, s32);
void Func_02000b12(s32);
void Func_02000b2a(s32, s32);
void Func_02000ace(void);
void Func_02000ae0(void);
void Func_02000b48(s32);
void Func_02000b50(s32);
void Func_02000b68(s32, s32);
void Func_02000b0c(void);
void Func_02000b1a(s32, s32, s32, s32, s32, s32);
u8  *Func_02000b80();
u8  *Func_02000b8c();
u8  *Func_02000b94();
void Func_02000b34();
u8 *Func_02000b76(s32);
u8 *Func_02000b98(s32, s32);
void Func_02000bc8(s32);
void Func_02000bc0(s32, s32, u8 *);
void Func_02000bbe(s32);

/* Each Func_ name is a loader-relocated call word, not a runtime address. */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers. A direct call instead precomputes a costly
 * constant into a temporary that is then shared with later uses in the same
 * block. A value-returning call sets r0 last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Advance the scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Func_02000880();
    void Func_02000976();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_02000880();
    void Func_02000976();

    return f(a0);
}

void State_CheckPositionWindow(void)
{
    s32 v1;
    s32 v0;

    v0 = ((struct Resource386FirstView *)Resource386_GetFirstView(0))->sample_08;
    v1 = (s32)((struct Resource386SecondView *)Resource386_GetSecondView(0))->sample_10 >> 0x14;
    if (((u32)((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A)) {
        Resource386_OnWindowMatch(0x250);
        return;
    }
    Resource386_OnWindowMiss(0x250);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020086dc;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020087cc;
}

u8 *SceneData_InitAndGetTable87f4(void)
{
    u8 *slot = (u8 *)0x020087F4;

    Func_020006d4(slot);
    return slot;
}

void Actor_RunActorStep(s32 arg0)
{
    Func_020006da(arg0);
    Func_0200071a(arg0, 1);
    Func_02000752(arg0, 0);
    Func_020006f6();
}

void Dialogue_RunActor9Line(void)
{
    Func_02000756(0x1CC9);
    Func_02000758(9, 0, 2);
    Func_0200016a(9);
}

void Dialogue_RunActor11Line(void)
{
    Func_02000776(0x1CCD);
    Func_02000778(11, 0, 2);
    Func_0200018a(11);
}

void Dialogue_RunActor12Line(void)
{
    Func_02000796(0x1CD0);
    Func_02000798(12, 0, 2);
    Func_020001aa(12);
}

void Scene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Func_0200075c();
    Call1(Func_020007ba, 0x1cd4);
    Func_020007bc(16, 0, 2);
    Func_020007ac(16, 1);
    Func_020007ee(16, 0, 20);
    Func_020007c6(16, 4);
    Func_02000784(20);
    Func_02000806(16, 0, 20);
    Call3(Func_02000822, 16, 0x102, 60);
    Func_0200081c(16, 0, 30);
    Value2(Func_02000814, 16, 0);
    if (Value2(Func_020007dc, 0, 0) != 0) {
        bump_step(1);
    }
    Func_0200084a(16, 0, 20);
    Call1(Func_020007c2, 0x300);
    Call1(Func_020007c8, 0x868);
    Func_020007f4();
}

void Dialogue_RunActor16Line(void)
{
    void Func_02000880(s32, s32, s32);

    Func_0200085e(0x1CDA);
    Func_02000860(16, 0, 2);
    Func_02000272(16);
}

void Dialogue_RunActor23Line(void)
{
    void Func_02000880(s32, s32, s32);

    Func_0200087e(0x1CEE);
    Func_02000880(23, 0, 2);
    Func_02000292(23);
}

void Scene_RunActor18FlaggedSequence(void)
{
    void Func_02000880_a();
    void Func_0200089e_a();
    void Func_02000976();

    u32 i;
    s32 record;

    Func_02000844();
    Func_0200089e_a(18, 0, 0);
    if (Value1(Func_0200082c, 0x85b) == 0) {
        Call1(Func_020008b6, 0x137c);
        Func_020008c6(18, 0);
    } else {
        Call1(Func_020008c6_a, 0x1385);
        Func_020008d6(18, 0);
    }
    if (Value2(Func_0200089e, 0, 0) == 0) {
        Func_02000880_a(20);
        Func_020008f8(18, 0);
        Func_0200088e(20);
        Func_020008e6(18, 2);
        Func_0200089c(20);
        if (Value0(Func_02000898) == 0) {
            Func_020008f4(18, 4);
            Func_020008b2(20);
            Call1(Func_02000918, 0x1384);
            Func_02000930(18, 0);
            goto L_020002d4;
        }
        Func_0200095a(231, 3);
        Func_020008f2(231, 0);
        Call1(Func_020008c0, 0x85b);
    } else {
        bump_step(1);
        Func_020008f0(20);
        Func_02000940(18, 3);
        Func_020008fe(20);
        Func_02000976(18, 0);
    }
    L_020002d4:;
    Call3(Func_02000992, 18, 0x4000, 0);
    Func_02000926();
}

void Actor_RunActor16StepWithFlag91(void)
{
    u8 *Func_02000976_a(s32);
    u8 *Func_02000986(s32);

    u8 *slot;
    u8 clear = 0;

    Func_0200093c();
    Func_0200098c(16, 1);
    Func_02000950();
    slot = Func_02000976_a(16) + 91;
    *slot = 1;
    Func_02000438();
    slot = Func_02000986(16) + 91;
    *slot = clear;
    Func_0200099c(16, 2);
}

void Scene_RunActor18ConditionalCue(void)
{
    void Func_02000986_a(s32);
    void Func_020009c8(s32, s32);

    Func_02000978();

    if (Func_0200096c() == 0) {
        Func_020009c8(18, 4);
        Func_02000986_a(20);
        Func_020009ec(0x1384);
        Func_02000a04(18, 0);
    } else {
        Func_02000a2e(0xE7, 3);
        Func_020009c6(0xE7, 0);
    }

    Func_020009ba();
}

void Scene_RunActor19StepByPlace(void)
{
    void Func_020009c8_a(void);

    u32 place;

    place = *(u16 *)(Func_020009ea(0) + 6);
    Func_020009c8_a();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000a6a(4, 19);
    } else {
        Func_02000a3a(0x1CE2);
        Func_02000a52(19, 0);
    }

    Func_020009f6();
}

void Scene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000a32(0) + 6);
    Func_02000a10();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000ab2(5, 20);
    } else {
        Func_02000a82(0x1CE4);
        Func_02000a9a(20, 0);
    }

    Func_02000a3e();
}

void Scene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000a7a(0) + 6);
    Func_02000a58();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000afa(6, 21);
    } else {
        Func_02000aca(0x1CE6);
        Func_02000ae2(21, 0);
    }

    Func_02000a86();
}

void Scene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000ac2(0) + 6);
    Func_02000aa0();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000b4a(1, 22);
    } else {
        Func_02000b12(0x1CEC);
        Func_02000b2a(22, 0);
    }

    Func_02000ace();
}

void Dialogue_RunActor18FlaggedLine(void)
{
    s32 Func_02000abe(s32);

    Func_02000ae0();
    if (Func_02000abe(0x85B) == 0) {
        Func_02000b48(0x1382);
    } else {
        Func_02000b50(0x1CF4);
    }
    Func_02000b68(18, 0);
    Func_02000b0c();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    s32 Func_02000abe(s32);

    return (u8 *)0x0200898c;
}

/*
 * Overlay entry point: selects the scene from the global block and runs the
 * matching setup. It returns a constant zero status.
 *
 * Data_02000240 is the cross-overlay RAM global block rather than an in-image
 * address, and the signed halfword read out of it selects the scene.
 */
s32 Scene_InitSceneStateByStep(void)
{
    void Func_02000abe_a();

    s32 scene;
    s32 zero;

    *(s32 *)(WORKSPACE + 448) = 521;
    scene = Data_02000240[225];

    if (scene == 5) {
        s32 fifth = 4;
        s32 sixth = 3;

        /*
         * The fifth and sixth arguments go on the stack. The two locals
         * are what put them there, so they must stay locals.
         */
        Func_02000b1a(0, 120, 8, 67, fifth, sixth);
        zero = 0;
        Func_02000b80(8)[0x55] = zero;
        *(s32 *)(Func_02000b8c(8) + 12) = zero;
        *(s32 *)(Func_02000b94(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Func_02000abe_a(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /*
         * 0x02008031 is Func_02000030 plus the Thumb bit, a task callback
         * rather than data; 200 << 4 is the period.
         */
        Func_02000b34(0x02008031, 0xc80);
    }

    return 0;
}

/*
 * Prepare object 22 for display. Call sites set three further registers that
 * this function does not read.
 */
void OvObj_InitObject22(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02000b76(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02000b98(17, 0x608);
        Func_02000bc8(a);
        v += 0x400;
        Func_02000bc0(q[28], 0x80, v);
        Func_02000bbe(17);
    }
}
