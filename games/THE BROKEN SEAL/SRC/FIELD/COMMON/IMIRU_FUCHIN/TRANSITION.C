#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    /* Message 0x182 + 230. */
    ITEM_DRAGONS_EYE = 230
};

enum TransitionMessage {
    MSG_DRAGONS_FLAME_ILLUMINATES_PATH_TRUTH = 0x17e1,
    MSG_SECRET_KI_SHALL_REVEALED_DISCIPLES = 0x17e2,
    MSG_RAYS_LIGHT_GIVE_BIRTH_SHADOWS = 0x17e3
};

#define NULL ((void *)0)

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService_02000fcc {
    u16 unknown00[3];
    u16 value06;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_39a owner at 0x02001004, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_39a {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject_02001bdc {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[4];
    s32 sub_x;
    s32 sub_y;
    u8 unknown20[0x10];
    s32 sub_dx;
    s32 sub_dy;
    u8 unknown38[0x0c];
    s32 dx;
    s32 dy;
    s32 dz;
};

/*
 * resource_39a owner at 0x02000ed8, 88 bytes.
 *
 * Creates an actor and, when creation succeeded, initialises it: mode field of
 * the linked record to 1, state byte at +85 cleared, two setup calls, and bit 1
 * of the flag byte at +35 raised. Returns the actor, or null.
 *
 * The mode write is a bitfield assignment; as explicit mask arithmetic the
 * compiler narrows -13 to a byte and the `movs r3,#13 / negs r3,r3` pair is
 * lost.
 */
struct Mode_39a {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Actor_39a {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Mode_39a *f80;       /* +80 */
    u8 pad51[1];
    u8 f85;                     /* +85 */
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02002094 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern u8 *Data_03001ee0;
extern u8 *Data_0200b328;
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000042;
extern u8 Value_00000043;
extern u8 Data_0200a4bc[];
extern u8 Data_0200a504[];
extern u8 Data_0200a5f4[];
extern u8 Data_0200a63c[];
extern u8 Data_0200a6cc[];
extern u8 Data_0200a744[];
extern u8 Data_0200a7bc[];
extern u8 Data_0200a48c[];
extern u8 Data_0200a8f4;
extern u8 Data_0200a9bc[];
extern u8 Data_0200a9ec[];
extern u8 Data_0200aa4c[];
extern u8 Data_0200aac4[];
extern u8 Data_0200ab3c[];
extern u8 Data_0200ab9c[];
extern u8 Data_0200a9a4[];
extern u8 Data_0200abd8[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ad1c[];
extern u8 Data_0200ae24[];
extern u8 Data_0200b058[];
extern u8 Data_0200b130[];
extern u8 Data_0200b184[];
extern u8 Data_0200abcc[];
extern u8 Data_00000034[];
extern s16 Data_02000240_t[][1];
extern s16 Data_0200a424[];
extern s16 Data_0200a464[];
extern u32 Data_03001ae8;

void Func_02002442(s32, s32, s32);
void Func_020023f0(s32, s32);
struct SceneService_02000fcc *Func_02003326(s32 index);
s32 Func_02003a6e(s32);
void Func_0200359a(s32, s32);
s32 Func_02004290();
Obj *Func_020024f2(s32);
Obj *Func_020024fa(s32);
void Func_020024b2(Obj *, s32, s32, s32);
void Func_020024de(Obj *, s32, s32, s32);
void Func_0200250a(Obj *);
struct Rec_39a *Func_02003362();
struct Actor_39a *Func_020031a2();
void Func_02002f72_a();
s32 Func_02003b02();
s32 Func_02003de6();
s32 Func_02003df4();
void Func_0200421a();
void Func_020040fa();
void Func_02004122();
void Func_0200420c();
void Func_020042e0();
void Func_02009d78();
void Func_02004328(T *, s32);
void Func_0200a014();
struct Subject_02002094 *Func_02004498();
s32 Func_020043ec();
void Func_02004392();
s32 Func_02004406();
s32 Func_02004410();
void Func_02004426();
void Func_02004416();
void Func_02004444();
s32 Func_0200446e();
void Func_02004480();
void Func_0200448e();
void Func_02004450();
s32 Func_020044c4_b();
void Func_020044c2();
void Func_020044d0();

/* Imports; the queried ones are typed for their return value. */

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Engine_AudioPlayCue.
 */

/* Imports, named by the address each call site computes. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */
static __inline__ void AdvanceProbe_02002094(s32 heading, s32 *probe)
{
    /*
     * Keep this call behind an inline boundary: it is what makes sp+8 be
     * rematerialized for argument 2 before the split 0x100000 constant is
     * completed.
     */
    Func_02004450((s32)0x100000, heading, probe);
}

/* 0x03001ebc is the overlay's workspace pointer. */

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */
void SceneState_ApplyValues8And2And1(void)
{
    Func_02002442(8, 2, 1);
}

void SceneState_ApplyValues11And62(void)
{
    Func_020023f0(0xB, 0x3E);
}

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a4bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a504;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200a5f4;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200a63c;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200a6cc;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200a744;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200a7bc;
    }
    return (s32)Data_0200a48c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableA8f4(void)
{
    return &Data_0200a8f4;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a9bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a9ec;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200aa4c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200aac4;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200ab3c;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200ab9c;
    }
    return (s32)Data_0200a9a4;
}

void SceneActor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020024f2(gGameState.selected_actor);
    q = Func_020024fa(a0);
    Event_Begin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020024b2(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020024de(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Func_0200250a(p);
    Event_End();
}

void ActorPresentation_SetupActorEightForFlag301(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x301);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void SceneState_RunSlot8OffsetStep(void)
{
    s32 offset = 112;

    offset = -offset;
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, offset, 0);
    SceneActor_PlacePairAtOffset(8, offset, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x301);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

/* Imports; the queried ones are typed for their return value. */
void FieldScene_RunActor9Transition302(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x302);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorNineForFlag302(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x302);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorTenForFlag303(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x303);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor10Transition303(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x303);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor8Transition304(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x90, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x304);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition304And305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -144, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x304);
    GameFlag_Set(0x305);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -14, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x305);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8FlaggedSequence(void)
{
    Audio_PlayCue((s32) 0xF1);

    if (GameFlag_IsSet((s32) 0x306) != 0) {
        SceneActor_PlacePairAtOffset(8, 16, 0);
        GameFlag_Clear((s32) 0x305);
    } else {
        SceneActor_PlacePairAtOffset(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        GameFlag_Set((s32) 0x304);
    }

    Audio_PlayCue((s32) 0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void ActorPresentation_SetupActorNineForFlag306(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x306);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor9Flag306Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x306);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void ActorPresentation_SetupActorTenForFlag307(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(10, 0, 144);
    SceneActor_PlacePairAtOffset(10, 0, 128);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x307);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

/* Three sites of one import, so three names. */
void FieldScene_RunActorTenDepthSequence(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Audio_PlayCue((s32) 0xF1);
    SceneActor_PlacePairAtOffset(10, 0, depth);
    SceneActor_PlacePairAtOffset(10, 0, depth);
    SceneActor_PlacePairAtOffset(10, 0, -80);
    Audio_PlayCue((s32) 0x121);
    GameFlag_Set((s32) 0x307);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_PlaceActorEightByFlags(void)
{
    Audio_PlayCue((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (GameFlag_IsSet((s32) 0x310) != 0 || GameFlag_IsSet((s32) 0x30D) != 0) {
        SceneActor_PlacePairAtOffset(8, -48, 0);
        GameFlag_Clear((s32) 0x308);
        GameFlag_Set((s32) 0x309);
    } else {
        SceneActor_PlacePairAtOffset(8, -96, 0);
        GameFlag_Set((s32) 0x308);
        GameFlag_Clear((s32) 0x309);
    }

    Audio_PlayCue((s32) 0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor8Transition308And309(void)
{
    Audio_PlayCue(0xF1);
    GameFlag_Clear(0x308);
    GameFlag_Clear(0x309);
    SceneActor_PlacePairAtOffset(8, 0x30, 0);
    Audio_PlayCue(0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlags308And309Guarded(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (GameFlag_IsSet((s32) 0x310) != 0) {
        return;
    }
    if (GameFlag_IsSet((s32) 0x30D) != 0) {
        return;
    }

    Audio_PlayCue((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    GameFlag_Set((s32) 0x308);
    GameFlag_Clear((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    SceneActor_PlacePairAtOffset(8, -48, 0);
    Audio_PlayCue((s32) 0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlags308And309(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    GameFlag_Clear(0x308);
    GameFlag_Clear(0x309);
    SceneActor_PlacePairAtOffset(8, 0x60, 0);
    Audio_PlayCue(0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorNineForFlag30a(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -32, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30A);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor9Transition30A(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x20, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30A);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_PlaceActorTenByFlags(void)
{
    Audio_PlayCue(241);
    if (GameFlag_IsSet(0x308) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, -64);
        GameFlag_Clear(0x30b);
        GameFlag_Set(0x30c);
        GameFlag_Clear(0x30d);
        GameFlag_Clear(0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, -128);
        GameFlag_Set(0x30b);
        GameFlag_Clear(0x30c);
        GameFlag_Clear(0x30d);
        GameFlag_Clear(0x30e);
    }
    Audio_PlayCue(0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  The tail call shared by the first
 * three arms is written out in each arm rather than adding control flow the
 * reference does not have.  Imports are named by the address their call site
 * computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag308DialogueBranch(void)
{
    Audio_PlayCue((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (GameFlag_IsSet((s32)0x308) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 16);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 16);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 64);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Set((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, 128);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    }
    Audio_PlayCue((s32)0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Transition30BTo30E(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, -16);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Engine_GameFlagClear serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag311DialogueBranch(void)
{
    Audio_PlayCue((s32)0xf1);
    if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 48);
        GameFlag_Clear((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        GameFlag_Clear((s32)0x30c);
        GameFlag_Set((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 32);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, 112);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    }
    Audio_PlayCue((s32)0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorTenForFlags30bAnd30d(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30B);
    GameFlag_Clear(0x30D);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceA(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, 64);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceB(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, -80);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceC(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30B);
    GameFlag_Clear(0x30C);
    GameFlag_Clear(0x30D);
    GameFlag_Clear(0x30E);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Field scene step for overlay resource_39a.  Imports are named by the address
 * their call site computes, not by a location in this image, and their
 * interfaces are left open.  Engine_GameFlagClear is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Engine_GameFlagClear is spelled out in each arm.
 */
void FieldScene_RunActorElevenFlaggedSteps(void)
{
    Audio_PlayCue((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (GameFlag_IsSet((s32)0x308) != 0 || GameFlag_IsSet((s32)0x30d) != 0) {
        SceneActor_PlacePairAtOffset(11, 0, -64);
        GameFlag_Clear((s32)0x30f);
        GameFlag_Clear((s32)0x310);
        GameFlag_Set((s32)0x311);
        GameFlag_Clear((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (GameFlag_IsSet((s32)0x30c) != 0) {
        SceneActor_PlacePairAtOffset(11, 0, -112);
        GameFlag_Clear((s32)0x30f);
        GameFlag_Set((s32)0x310);
        GameFlag_Clear((s32)0x311);
        GameFlag_Clear((s32)0x312);
    } else {
        SceneActor_PlacePairAtOffset(11, 0, -128);
        GameFlag_Set((s32)0x30f);
        GameFlag_Clear((s32)0x310);
        GameFlag_Clear((s32)0x311);
        GameFlag_Clear((s32)0x312);
    }
    Audio_PlayCue((s32)0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunSteps30FTo312(void)
{
    GameFlag_Clear(0x30F);
    GameFlag_Clear(0x310);
    GameFlag_Clear(0x311);
    GameFlag_Clear(0x312);
}

void FieldScene_RunActor11Offset128Sequence(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 128);
    FieldScene_RunSteps30FTo312();
    Audio_PlayCue(0x121);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor11Flags30fTo312Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, -16);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30F);
    GameFlag_Clear(0x310);
    GameFlag_Clear(0x311);
    GameFlag_Clear(0x312);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_112(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 112);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_64(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x40);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_80(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x50);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor11Transition(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 48);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlag313(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, 0x70);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x313);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor8Transition313(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, -112);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x313);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor9Flag314Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -128, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x314);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorNineForFlag314(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x80, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x314);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorTenForFlag315(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x315);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor10Flag315Sequence(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, -160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x315);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_AdvanceActorEightStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(8, 2);
}

void FieldScene_SetActor9Values1And2(void)
{
    Actor_SetAnimation(9, 1);
    Actor_SetAnimation(9, 2);
}

void ActorPresentation_AdvanceActorTenStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(10, 2);
}

void SceneActor_SetActor11Values1And2(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(11, 2);
}

struct Actor_39a *OverlayObject_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Func_020031a2(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Actor_SetSpriteFlags(actor, 0);
        Object_SetPalette(actor, 15);
        actor->f35 |= 2;
        return actor;
    }
    return 0;
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200abd8;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200ac08;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200ad1c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200ae24;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200b058;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200b130;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200b184;
    }
    return (s32)Data_0200abcc;
}

void SceneState_SetServiceZeroValue06(void)
{
    struct SceneService_02000fcc *work;

    Event_Begin();
    work = Func_02003326(0);
    work->value06 = 0x4000;
    Audio_PlayCue(123);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(1);
}

void FieldScene_RunSingleStep(void)
{
    SceneActor_StepSubjectAlongHeading();
}

void SceneActor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Func_02003362(id);

    if (rec != 0) {
        Actor_SetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Imports; the three queried ones are typed for their return value. */

/* Dialogue layout for resource_39a. */

/*
 * 0x02000240 is below the link base, so it is a resident table; entry 225 is
 * read here as an unsigned halfword.
 */

/* Imports; the queried ones are typed for their return value. */
void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Map_CopyCellAttributes(8, 42, 15, 5,  f1, g1); }

    if (GameFlag_IsSet((s32)0x301) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x302) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f4, g4); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f5, g5); }
    }

    if (GameFlag_IsSet((s32)0x303) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f6, g6); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Map_CopyCellAttributes(0, 28, 10, 18,  f1, g1); }

    if (GameFlag_IsSet((s32)0x304) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x305) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Map_CopyCellAttributes(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f5, g5); }
        if (GameFlag_IsSet((s32)0x304) != 0) {
            SceneActor_PlaceAtTileAndMark(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (GameFlag_IsSet((s32)0x306) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Map_CopyCellAttributes(14, 18, 3, 1,  f8, g8); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Map_CopyCellAttributes(14, 18, 3, 1,  f9, g9); }
    }

    if (GameFlag_IsSet((s32)0x307) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Map_CopyCellAttributes(14, 18, 3, 1,  f10, g10); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Map_CopyCellAttributes(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Map_CopyCellAttributes(12, 3, 9, 16, k5, k6); }

    if (GameFlag_IsSet((s32)0x308) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    } else if (GameFlag_IsSet((s32)0x309) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 17, 25);
        { s32 k6 = 24;
          Map_CopyCellAttributes(18, 24, 1, 3, 20, k6);
          Map_CopyCellAttributes(18, 24, 1, 3, 14, k6);
          Map_CopyCellAttributes(8, 41, 1, 3, 17, k6);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30a) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30b) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 22);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 41, 3, 1, k5, 22);
        }
    } else if (GameFlag_IsSet((s32)0x30c) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(5, 42, 3, 1, k5, 23);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(10, 44, 3, 1, k5, 21);
        }
    } else if (GameFlag_IsSet((s32)0x30d) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 26);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(5, 43, 3, 1, k5, 26);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
        }
    } else if (GameFlag_IsSet((s32)0x30e) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 44, 3, 1, k5, 27);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 15, 30);
    }

    if (GameFlag_IsSet((s32)0x30f) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 40, 3, 1, k5, 23);
        }
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 24);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 41, 3, 1, k5, 24);
        }
    } else if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 42, 3, 1, k5, 27);
        }
    } else if (GameFlag_IsSet((s32)0x312) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 28);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 43, 3, 1, k5, 28);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(11, 15, 31);
    }
}

/*
 * Four flag-branched layout steps.  Nothing is returned; the three pool words
 * after the return belong to the owner.  The eight bytes of frame are the
 * fifth and sixth arguments of the six-argument layout calls.  Imports are
 * named by the address their call site computes, and are old-style because
 * arity varies between sites.
 */
void FieldScene_RunFlagBranchedLayoutSteps(void)
{
    extern u16 Data_02000240[];

    /*
     * The byte offset 450 is built by shifting, giving entry 225.  The test
     * is (entry - 1) << 16 against 0x10000 with an unsigned compare, which
     * selects exactly entries 1 and 2.
     */
    if ((u32)((u32)(Data_02000240[225] - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Map_CopyCellAttributes(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Map_CopyCellAttributes(20, 45, 11, 4,  f2, g2); }
    }

    if (GameFlag_IsSet((s32)0x313) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Map_CopyCellAttributes(19, 11, 3, 1,  f3, g3); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Map_CopyCellAttributes(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (GameFlag_IsSet((s32)0x314) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f5, g5); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f6, g6); }
    }

    if (GameFlag_IsSet((s32)0x315) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f7, g7); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f8, g8); }
    }
}

void SceneState_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = Data_03001ee0;

        *(s32 *)(state + 24) = Func_02003a6e(0);
    }
}

void SceneState_ClearWorkWord24(void)
{
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}

s32 Func_02001750(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x109) == 0 && Data_02000240_t[224][0] == (s32)Data_00000034) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        Func_02002f72_a();
    }
    return 0;
}

void FieldScene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    record = Func_02003b02(8);
    Actor_SetSpriteFlags(record, 0);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x108, 196);
    Event_End();
}

void FieldScene_ApplyOffset0Neg32(void)
{
    FieldScene_RunScene39a_02001ad0(0, -32);
}

void SceneState_ApplyOffsetMinus32(void)
{
    Func_0200359a(-32, 0);
}

void FieldScene_RunScene39a_02001ad0(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Value3(Engine_ActorSetDestinationOffset, 0, a0, a1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 7);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Event_End();
}

void FieldScene_RunScene39a_02001b1c(void)
{
    extern u8 Data_0200a488[];

    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Value4(Func_02003de6, 22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(u8 **)(rec + 80);
        p6[38] = record;
        p6[39] = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Value2(Func_02003df4, 17, 0x608);
        Item_LoadIcon(ITEM_DRAGONS_EYE);
        Vram_Load(p6[28], 128, (rec7 + 0x400));
        Heap_Release(17);
        *(s32 *)Data_0200a488 = (s32)rec;
    }
}

/* Returns a value: the reference sets r1 before r0 at this site. */
void SceneDialogue_ShowLineF13WithWorkA488(void)
{
    extern s32 Data_0200a488;

    Event_Begin();

    /* r5 holds &Data_0200a488 across the calls; the word is reloaded before
     * the second test. */
    if (Data_0200a488 != 0) {
        Engine_RunRisingObjectSequence(Data_0200a488, 3);
    }

    Party_GiveItem((s32) 0xE6, 0);
    GameFlag_Set((s32) 0xF13);

    if (Data_0200a488 != 0) {
        Engine_ObjectDispatchRelease(Data_0200a488);
    }

    Event_End();
}

void OverlayObject_AdvancePositionByDelta(struct MovingObject_02001bdc *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}

s32 OverlayObject_ApplyValue15(s32 obj)
{
    Object_SetPalette(obj, 15);
    return 0;
}

/*
 * Field scene sequence for overlay resource_39a.  Imports are named by the
 * address their call site computes, not by a location in this image, and
 * their interfaces are left open.  The declarations are old-style because
 * Engine_AudioPlayCue and Engine_MapCopyCellsTo are each reached with two different
 * argument counts.  Func_02009d78 names a loader-relocated call word handed
 * to two imports as a callback, not a runtime address.
 */
void FieldScene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Audio_PlayCue(19);
    Audio_PlayCue(182);
    Event_Begin();
    Func_0200421a();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        ColorBuffer_ApplyTarget((s32)0x204318, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        if (pass == 0) {
            Map_CopyCellsTo(30, 8, 12, 8, step, span);
            Map_CopyCellsTo(30, 57, 19, 57, one, one);
        }
        ColorBuffer_ApplyTarget((s32)0x203108, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Task_Wait(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Func_020040fa((void *)Func_02009d78, (s32)0xc80);
    Task_Wait(40);
    ColorBuffer_ApplyTarget((s32)0x201090, 1);
    ColorBuffer_Interpolate(40);
    Task_Wait(80);
    Func_02004122((void *)Func_02009d78);
    Task_Wait(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    ColorBuffer_ApplyTarget((s32)0x10000, 1);
    ColorBuffer_Interpolate(80);
    /* Same import as in the loop, one argument here. */
    Task_Wait(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    GameFlag_Set((s32)0x820);
    Func_0200420c(230);
    Func_020042e0();
    /* Same import as the first call, no argument register written here. */
    Event_End();
}

void SceneState_SetValue17e1(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DRAGONS_FLAME_ILLUMINATES_PATH_TRUTH, 1);
    Event_End();
}

void SceneDialogue_RunLine17e2(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_SECRET_KI_SHALL_REVEALED_DISCIPLES, 1);
    Event_End();
}

void FieldScene_RunScriptedStep17E3(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_RAYS_LIGHT_GIVE_BIRTH_SHADOWS, 1);
    Event_End();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (GameFlag_IsSet((s32)0x820) != 0) {
        Message_ShowCentered((s32)0x17e5, 1);
    } else {
        Message_ShowCentered((s32)0x17e4, 1);
        if (Func_02004290((s32)0xe6) != -1) {
            u8 *workspace = Data_03001ebc;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Event_End();
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = Data_0200a424[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetAnimation(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetAnimation(o, 2);
    Func_02004328(o, 0x30);
}

/*
 * Pathing step for resource_39a.  r0 holds the popped return address, so
 * nothing is returned, and the seven pool words after the return belong to
 * the owner.  Frame: sp+0 is the goal marker, sp+4 the heading, and
 * sp+8..sp+19 the three-word probe position handed to the stepping imports by
 * address.  The x and z assignment order and the inline stepping wrapper are
 * what reproduce the reference; do not reorder or respell them.
 */
void SceneActor_StepSubjectAlongHeading(void)
{
    extern struct SharedData_02000240 Data_02000240;

    struct Subject_02002094 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = Func_02004498(Data_02000240.selected_subject);

    for (;;) {
        heading = Data_0200a464[(Data_03001ae8 >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Event_Begin();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Func_020043ec((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Func_02004392((s32)0x100000, heading, probe);

        marker = Func_02004406((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Func_02004410((s32)*subject_id, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Func_02004426(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Object_SetAnimation(subject, 2);
        Func_02004416(subject, 48);
        Func_02004444(subject);
        subject->callback = (void *)Func_0200a014;

        goto advance_probe;
continue_probe:
        if (Func_0200446e((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Func_02004480(subject, probe[0], probe[1], probe[2]);
        Func_0200448e(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe_02002094(heading, probe);
        marker = Func_020044c4_b((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Func_020044c2(subject, x, subject->y, z);
        Func_020044d0(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
