#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"

enum CoordinatorMessage {
    MSG_ROBIN_GOT = 0x96a,
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_ROBIN_WILL_CHEER_FOR_WAY = 0x207f,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_OPERATOR_BRIDGE_WILL_ALSO_CHEER = 0x2094,
    MSG_THEY_CALL_BROKEN_BRIDGE = 0x2095,
    MSG_LOGS_KEY_CLEARING_STAGE = 0x2098,
    MSG_PLACE_NORMALLY_CALLED_LUMBER_WATER = 0x2099,
    MSG_SITE_FIRST_FINALS_BATTLE = 0x20cb,
    MSG_ASK_ATTENDANTS_FOR_EXPLANATIONS_STAGES = 0x20d4,
    MSG_WARRIORS_ENTER_FINALS_WITHOUT_ANY = 0x20d5,
    MSG_ROBIN_YOURE_CONTESTANT_IN_FINALS = 0x20e1,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8
};

#define FieldScene_RunSceneFourCoordinator Func_020016ec
#define GetPartyInteractionRecord Func_0200593a
#define GetPartyMemberCount Func_0200590a
#define HexDigits Data_0200bfd0

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 Data_0200c194[];
extern u8 Data_0200c1dc[];
extern u8 Data_0200c1f4[];
extern u8 Data_0200a1b9[];
extern u8 Data_0200be4e[];
extern u8 Data_0200c5aa[];
extern u16 Data_0200c790;
extern u16 Data_0200c764;
extern u16 Data_0200c79c;
extern u8 *Data_0200c7a0;
extern u16 Data_0200c7f8;
extern u16 Data_0200c76c;
extern u32 Data_0200c770;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern s16 Data_0200c6a6;
extern u8 Data_0200abed[];
extern u16 Data_0200c7f4;
extern u16 Data_0200c780;
extern u16 Data_0200c758;
extern u16 Data_0200c774;
extern u16 Data_0200c78c;
extern u16 Data_0200c760;
extern u16 Data_0200c800;
extern u16 Data_0200c7a4;
extern u16 Data_0200c7bc;
extern u16 Data_0200c750;
extern u8 *Data_03001f3c;
extern u8 LinkedMessage_WouldYouLikeHearDescription;
extern u8 Data_0200c420[];
extern u8 Data_0200bf14[];
extern u8 Data_0200b1c1[];
extern volatile u32 Data_03001ae8;
extern u8 Data_0200c008[];
extern u8 Data_00000000[];
extern u8 Data_02002090[];
extern u8 Data_0200cbfc[];
extern u8 Data_0200cc28[];
extern u8 Data_0200cca4[];
extern u8 Data_0200bfd0[];
extern u8 Data_0200bfe4[];
extern u32 Data_0200c154[];

void Func_020061d0(u8 *, s32);
void Func_02003c7e();
void Func_02003c90();
void Func_02003cc0();
void Func_02003cd2();
void Func_02003cf0();
void Func_02003d02();
void Func_02003eb0();
void Func_02003ed2();
void Func_02003ee0();
s32 Func_02003f3c();
s32 Func_02003f96();
void Func_02004618();
s32 Func_02004a7e();
s32 Func_02004b28_a();
s32 Func_02004dec();
s32 Func_02004e0c();
s32 Func_02004e2c();
void Func_02003152();
s32 Func_02003f6c();
void Func_0200418e();
struct FieldActor *Func_02005708(s32);
struct FieldActor *Func_02005716(s32);
void Func_02005780_a();
void Func_02005770(s32, s32);
void Func_0200343a(s32);
typedef void(*SceneTask)(void);
void Func_02003c5a(SceneTask);
s32 Func_02003c78(s32, s32);
void Func_02005a4a(s32, s32);
s16 Func_02006816(void);
void Func_0200686c(u8 *, s32);
void Func_020068c8(u8 *, s32);
void Func_02006906(u8 *);
void Func_02006960(s16);
void Func_02005b3c(void);
void Func_02005994(s32, s32);
s32 Func_020059f6(s32);
struct FieldActor *Func_02003e7c(s32);
s32 Func_02003db8(s32, s32, s32);
struct FieldActor *Func_02003ebe(s32);
void Func_02003e84(s32, s32);
void Func_0200b3a0(void);
s32 Func_020073ee(void);
void Func_020073be(s32, s32);
void Func_02007392(s32, s32);
struct FieldActor *Func_02004016(s32);
struct FieldActor *Func_0200401e(s32);
void Func_02003f7e(struct FieldActor *, s32, s32, s32);
void Func_02003faa(struct FieldActor *, s32, s32, s32);
void Func_02003fdc(struct FieldActor *);
struct FieldActor *Func_020040f4(s32);
struct FieldActor *Func_02004124(s32);
struct FieldActor *Func_02004158(s32);
struct FieldActor *Func_0200417e(s32);
struct FieldActor *Func_020041d0(s32);
struct FieldActor *Func_020046cc_a(void);
s32 Func_02005508(s32);
s32 Func_02005512(s32);
s32 Func_02005530(s32);
s32 Func_0200553a(s32);
s32 Func_02005554(s32);
s32 Func_0200555e(s32);
struct FieldActor *Func_02006d24(s16);
void Func_02006c56(struct FieldActor *, s32, s32, s32);
void Func_0200325e();
s32 Func_02003474();
void Func_02004080();
void Func_0200432e();
PartyInteractionRecord *Func_0200593a(void);
s32 Func_0200590a(void);
void Func_0200599a(s32, s32);
void Func_020059a4(s32, s32);
void Func_020059ae(s32, s32);
void Func_020059b8(s32, s32);
void Func_020059c2(s32, s32);
void Func_020059cc_a(s32, s32);
s32 Func_02005b24();
s32 Func_02005b4e();
void Func_02005bb0();
void Func_02005bc4();
void Func_02005bc4_a();
void Func_02005bee();
void Func_02005c00();
s32 Func_02005c9a();
void Func_02005d10();
void Func_02005d1e_a();
void Func_02005d2c();
s32 Func_02005d52();
void Func_02005e2c();
s32 Func_02005d3e();
Rec *Func_02005d34(s32);
void Func_02005d46(s32, s32);
void Func_02005d62(s32, s32);
void Func_02004880(s32);
void Func_020048c4(s32);
s32 Func_020065ba(void);
void Func_020048ee(s32);
void Func_0200491c(s32);
void Func_02004936(s32);
void Func_020065fc(void);
void Func_020049aa();
void Func_020049d6();
void Func_02006a56();
void Func_02006a5e();
void Func_02006a76();
void Func_02006aa6();
u8 *Func_02006b14();
void Func_02006bba();
void Func_02006be4();
void Func_02006bec();
s32 Func_020066d8();
void Func_02006c5c();
void Func_02006c62();
void Func_02006c6a();
void Func_02006c80();
void Func_02006cb2();
u8 *Func_02006cde();
void Func_02006cea();
void Func_02006d00();
void Func_02006d36();
void Func_02006d42();
void Func_02006db2();
struct FieldActor *Func_02006868(void);
void Func_0200677e(struct FieldActor *);
struct FieldActor *Func_02006c20(s32);
void Func_02006a3c(struct FieldActor *);
void Func_02006a60(struct FieldActor *, s32, s32, s32);
void Func_02006a6e(struct FieldActor *);
struct FieldActor *Func_02006be0(s32);
void Func_020069fc(struct FieldActor *);
void Func_02006a20(struct FieldActor *, s32, s32, s32);
void Func_02006b3e(s32, s32, s32 *);
struct FieldActor *Func_02006bd2(s32, s32, s32, s32);
struct FieldActor *Func_020071f6(Position3 *, struct FieldActor *);
struct FieldActor *Func_02007220(Position3 *, struct FieldActor *);
struct FieldActor *Func_0200724c(Position3 *, struct FieldActor *);
u8 *Func_0200772c();
void Func_020075de();
s32 *Func_02007366();
void Func_02007610();
s32 *Func_02007398();

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* A countdown word this overlay owns at Data_0200c41c: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

/* FAKEMATCH: Calls through these inline helpers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

s32 *SceneActor_FindOccupantAheadOfSubject(void);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

u8 *Func_020072c6();            /* allocate a record by (id, size) */

s32 Func_020072e0();            /* reserve a graphics handle */

u8 *Func_02007478();            /* scene record for an actor selector */

u8 *Func_02007480();            /* scene record for an actor selector */

void Func_02007356();           /* upload image data to a handle */

s32 Func_0200737a();            /* next palette slot index */

void Func_02007326();           /* install a per-frame task (callback, rate) */

void Func_02007374();           /* release a graphics handle */

u8 *Func_02006d88();           /* veneer to Scene_GetRecord */

void Func_02006d02();          /* veneer to Object_SetPosition */

s32 Func_02005242();           /* local thunk to Func_020020e8, site A */

s32 Func_02005254();           /* local thunk to Func_020020e8, site B */

void Func_02006d82();          /* veneer to UiText_DrawQuantity, site A */

void Func_02006d94();          /* veneer to UiText_DrawQuantity, site B */

void Func_02006da4();          /* shared veneer, selector refresh + 0x96a */

void Func_02006d62();          /* veneer to Func_08009148 */

struct FieldActor *Func_020075cc();   /* scene record for a subject handle */

s32 Func_020075e6(struct FieldActor *, Position3 *);  /* terrain probe */

void Func_020075da(struct FieldActor *, s32, s32, s32);   /* place at (x, y, z) */

void Func_020075ea(struct FieldActor *, s32, s32, s32);   /* place at (x, y, z) */

void Func_020075fe(struct FieldActor *);              /* re-attach the camera */

/* Contiguous unnamed leaf-owner run for resource_3ba. */
u8 *SceneData_GetTableC194(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c194;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTablec1dc(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c1dc;
}

u8 *SceneData_GetTablec1f4(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c1f4;
}

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern s32 Data_0200c41c;

    switch ((u32)Data_0200c41c) {
    case 66:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Actor_SetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Call6(Func_02003c7e, 92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003c90, 92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Map_CopyCellAttributes(50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Actor_SetAnimation(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Actor_SetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Call6(Func_02003cf0, 92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003d02, 92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Actor_SetAnimation(16, 12); /* object 16, action 12 */
        Map_CopyCellAttributes(50, 24, 6, 1, 50, 12); /* main:080091c0 */
        Data_0200c41c = 120;
        break;
    }
    Data_0200c41c = Data_0200c41c - 1;
}

void SceneState_ResetCounterAndStartTask(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    SceneTask task;

    Data_0200c41c = 0;
    task = (SceneTask) 0x0200804D;
    Func_02003c5a(task);
    task();
}

void SceneState_SetMode66AndPassOpeningSequence(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    s32 value = 66;
    s32 *mode = (s32 *)0x0200C41C;

    *mode = value;
    Func_02003c78(0x0200804D, 0xC80);
}

void SceneState_WaitUntilWordC41cIs22(void)
{
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    s32 i;

    Task_Wait(10);
    i = 0;
    if (Data_0200c41c != 22) {
        do {
            Task_Wait(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (Data_0200c41c != 22);
    }
}

void SceneState_ApplyRectsForActorsNineAndTen(void)
{
    extern Ctl Data_02001000;

    struct FieldActor *actor;
    s32 blocked;

    {
        s32 x = 23;
        s32 y = 12;

        Map_CopyCellAttributes(27, 13, 3, 1, x, y);
    }
    actor = Func_02003e7c(9);
    blocked = Func_02003db8(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && blocked == 0) {
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        actor->motion_flags = 0;
        {
            s32 x = actor->x.fixed >> 20;
            s32 y = actor->z.fixed >> 20;

            Map_CopyCellAttributes(14, 13, 1, 1, x, y);
        }
    }
    actor = Func_02003ebe(10);
    {
        s32 x = actor->x.fixed >> 20;

        Func_02003e84(784, x);
    }
    {
        s32 x = actor->x.fixed >> 20;
        s32 y = actor->z.fixed >> 20;

        Map_CopyCellAttributes(14, 13, 1, 1, x, y);
    }
}

void FieldScene_RunTwoCallSequence(void)
{
    extern Ctl Data_02001000;

    StagedActor_PushActorAhead();
    SceneState_ApplyRectsForActorsNineAndTen();
}

void SceneState_ApplyRectAndSend303(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    {
        s32 x = 47;
        s32 y = 12;

        Map_CopyCellAttributes(47, 24, 1, 1, x, y);
    }
    GameFlag_Set(0x303);
}

void FieldScene_RunScene3ba_02000270(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    GameFlag_Set(0x301);
    rec7 = Value1(Func_02003f3c, 13);
    Event_Begin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
    Object_SetAnimation((s32)rec7, 3);
    Camera_WaitForMove();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003eb0, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = Value1(Func_02003f96, 14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003ed2, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    Func_02003ee0((s32)rec7);
    Event_Wait(45);
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    Event_End();
}

void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 x;
    s32 z;

    leader = Func_02004016(gGameState.selected_actor);
    actor = Func_0200401e(actor_id);
    Event_Begin();
    {
        x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        leader->speed = 0x10000;
        leader->acceleration = 0x8000;
        Func_02003f7e(leader, x, leader->y.fixed, z);
    }
    Object_SetAnimation(leader, 27);
    {
        x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        actor->speed = 0x10000;
        actor->acceleration = 0x8000;
        Func_02003faa(actor, x, actor->y.fixed, z);
    }
    if (dx < 0 || dz < 0) {
        Object_SetAnimation(actor, 4);
    } else {
        Object_SetAnimation(actor, 3);
    }
    Audio_PlayCue(226);
    Func_02003fdc(leader);
    Object_SetAnimation(actor, 2);
    Audio_PlayCue(288);
    Event_End();
}

void SceneActor_ShiftActorSeventeenByLeaderRow(void)
{
    struct FieldActor *actor;
    s32 row;
    s32 offset;

    actor = Func_020040f4(gGameState.selected_actor);
    row = actor->z.fixed >> 20;
    offset = -48;
    if (row <= 8) {
        offset = 48;
    }
    Map_CopyCellAttributes(67, 8, 3, 1, 64, row);
    StagedActor_PlacePairAtOffsetAndRun(17, 0, offset);
    actor = Func_02004124(17);
    row = actor->z.fixed >> 20;
    Map_CopyCellAttributes(64, 24, 3, 1, 64, row);
}

void SceneActor_ShiftActorEighteenByInputAndLeaderColumn(void)
{
    struct FieldActor *actor;
    s32 column;
    s32 offset;
    s32 direction;

    actor = Func_02004158(gGameState.selected_actor);
    column = actor->x.fixed >> 20;
    if ((Data_03001ae8 & 32) != 0) {
        direction = -1;
    }
    if ((Data_03001ae8 & 16) != 0) {
        direction = 1;
    }
    actor = Func_0200417e(17);
    offset = actor->z.fixed >> 20;
    if (column == 63) {
        if (offset == 11) {
            return;
        }
        offset = 160;
    } else if (column == 67) {
        if (offset == 11 && direction == -1) {
            return;
        }
        offset = 96;
    } else {
        if (offset == 11) {
            offset = 96;
        } else {
            offset = 160;
        }
        offset = -offset;
    }
    Map_CopyCellAttributes(72, 9, 1, 3, column, 9);
    StagedActor_PlacePairAtOffsetAndRun(18, offset, 0);
    actor = Func_020041d0(18);
    column = actor->x.fixed >> 20;
    Map_CopyCellAttributes(63, 25, 1, 3, column, 9);
}

void SceneState_ApplyValue768(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    GameFlag_Set(768);
}

void FieldScene_RunBranchedStep(void)
{
    extern Ctl Data_02001000;

    if (SceneActor_FindOccupantAheadOfSubject() == 0) {
        Leader_CheckAhead();
    } else {
        FieldScene_RunTwoCallSequence();
    }
}

u8 *SceneData_GetTablec420(void)
{
    extern Ctl Data_02001000;

    return Data_0200c420;
}

void FieldScene_RunScene3ba_02000974(s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    u32 i;
    s32 record;

    Actor_Destroy(24);
    Actor_Destroy(25);
    Func_02004618(1);
    Event_Begin();
    Actor_SetPosition(8, 0x5280000, 0xc00000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5080000, 0xc00000);
    Actor_FaceActor(8, 0x4000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
    if (a0 < 0) {
        Actor_SetAnimation(8, 10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 35);
    } else {
        Actor_SetAnimation(8, 8);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Task_Wait(1);
    Camera_MoveTo(0x5180000, 0, 0x800000, 0);
    FieldScene_RunLateSequence(a0);
    Event_End();
}

void SceneActor_MarkObjectAtTiles94To95(void)
{
    struct FieldActor *o;
    s32 x;
    s32 y;

    o = Func_020046cc_a();
    if (o != 0) {
        x = o->x.fixed >> 19;
        y = o->z.fixed >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->unknown_22 = 1;
        }
    }
}

void FieldScene_RunCommandSequence(s32 a0)
{
    struct FieldActor *actor;
    s32 x;
    s32 z;

    actor = (struct FieldActor *)Func_02004a7e();
    x = actor->x.part.pixel;
    z = actor->z.part.pixel;
    Event_Begin();
    Actor_SetSpeed(a0, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorSetPosition, 0, x << 16, (z << 16) - 0x300000);
    Actor_SetPosition(ACTOR_GERALD, (x << 16) - 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_IVAN, (x << 16) + 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_MIA, x << 16, (z << 16) - 0x200000);
    Actor_SetPosition(a0, x << 16, (z << 16) - 0x500000);
    actor = (struct FieldActor *)Func_02004b28_a(0);
    actor->facing = 0xc000;
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_SetMessage(MSG_SITE_FIRST_FINALS_BATTLE);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_StartRepeatedMotion(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(a0, ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(a0, 0x102, 60);
    if (Event_AskYesNo(a0, 0) == 0) {
        do {
            Event_SetMessage(MSG_WARRIORS_ENTER_FINALS_WITHOUT_ANY);
            Actor_SetAnimation(ACTOR_IVAN, 3);
            Event_Wait(2);
            Actor_SetAnimation(ACTOR_GERALD, 3);
            Event_Wait(2);
            Actor_SetAnimation(ACTOR_MIA, 3);
            Event_Wait(1);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimationAndWait(a0, 3);
            Event_ShowMessage(a0, 0);
            Actor_FaceDirection(a0, 0xa000, 0);
            Event_Wait(20);
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x1380000, -1, 0x680000, 1);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x18000, 0x3000);
            Camera_MoveTo(0x3080000, -1, 0x680000, 1);
            Event_ShowMessage(a0, 0);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4d80000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Actor_FaceActor(a0, 0x6000, 0);
            Event_ShowMessage(a0, 0);
            Camera_MoveTo(0x5180000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Actor_FaceActor(a0, ACTOR_PARTY_LEADER, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_RunRepeatedMotion(a0, 2);
        } while (Event_AskYesNo(a0, 0) != 0);
        Actor_RunRepeatedMotion(a0, 2);
        Event_SetMessage(MSG_ASK_ATTENDANTS_FOR_EXPLANATIONS_STAGES);
        Event_ShowMessage(a0, 0);
    }
    Event_SetMessage(MSG_ROBIN_YOURE_CONTESTANT_IN_FINALS);
    Actor_RunRepeatedMotion(a0, 2);
    Event_ShowMessage(a0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(1);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Event_Wait(2);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Event_Wait(1);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(6);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Value1(Func_02004dec, 0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Value1(Func_02004e0c, 0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Value1(Func_02004e2c, 0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WalkToAndWait(a0, x - 16, z - 64);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WalkToAndWait(a0, x - 16, z - 16);
    Actor_WalkToAndWait(a0, x, z);
    Actor_FaceDirection(a0, 0xc000, 10);
    Event_End();
}

void FieldScene_RunScene3ba_020015e0(s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    u32 i;
    s32 rec8;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003152();
    } else {
        Event_Begin();
        rec8 = Value2(SceneDialogue_RunFlagGatedPromptInteraction, a0, 3);
        if (rec8 == 0) {
            Event_SetMessage(MSG_THEY_CALL_BROKEN_BRIDGE);
            SceneState_ResetCounterAndStartTask();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3480000, -1, 0xd80000, 1);
            ((void (*)())Engine_CameraWaitForMove)();
            Event_ShowMessage(a0, 0);
            SceneState_SetMode66AndPassOpeningSequence();
            Event_Wait(60);
            Event_ShowMessage(a0, 0);
            Value3(Func_02003f6c, 0, 0x2e0, 200);
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            SceneState_WaitUntilWordC41cIs22();
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x330, 200);
            Event_Wait(30);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
            Event_ShowMessage(a0, 0);
            Func_0200418e(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            SceneState_SendIdBySceneId(a0, 3);
        } else {
            if (rec8 == 1) {
                Event_SetMessage(MSG_OPERATOR_BRIDGE_WILL_ALSO_CHEER);
                Event_ShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec8, a0, 3);
        Event_End();
    }
}

void Func_020016ec(s32 scene)
{
    extern s16 Data_02000240[];

    s32 path;

    if (gGameState.entrance == 2) {
        Func_0200325e();
        return;
    }
    Event_Begin();
    path = Func_02003474(scene, 4);
    if (path == 0) {
        Event_SetMessage(MSG_PLACE_NORMALLY_CALLED_LUMBER_WATER);
        Camera_SetSpeed(196608, 24576);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Camera_WaitForMove();
        Event_ShowMessage(scene, 0);
        Value3(SceneState_StoreParamsAndInitTable, 120, 72, 0);
        Event_ShowMessage(scene, 0);
        SceneState_ReleaseTableAndResetC6a6();
        Event_Wait(15);
        Value3(Func_02004080, 0, 984, 200);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
        Event_ShowMessage(scene, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 262, 60);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 98304, 49152);
        OverlayObject_PlaceWithScale14000(0, 1000, 192);
        OverlayObject_PlaceWithScale14000(0, 1000, 176);
        Call3(OverlayObject_PlaceWithScale14000, 0, 1016, 168);
        Event_Wait(15);
        Value3(StagedActor_PlacePairAtOffsetAndRun, 18, 160, 0);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Event_Wait(10);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_ShowMessage(scene, 0);
        Func_0200432e(0);
        Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
        Actor_SetPosition(18, 66584576, 11010048);
        SceneState_SendIdBySceneId(scene, 4);
    } else if (path == 1) {
        Event_SetMessage(MSG_LOGS_KEY_CLEARING_STAGE);
        Event_ShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Event_End();
}

void SceneActor_PlaceSlots1To3FromWork(void)
{
    {
        s32 x = Func_02005508(896);
        s32 y = Func_02005512(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = Func_02005530(912);
        s32 y = Func_0200553a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = Func_02005554(928);
        s32 y = Func_0200555e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}

void SceneState_SetStateHalfword386To99WhenMatched(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 sel = gGameState.selected_actor;

    if (sel != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == sel
        && GameFlag_IsSet(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 val = 99;

        *p = val;
    }
}

void FieldScene_RunNearestActor165Scene(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    struct FieldActor *p = Func_02005708(n);
    s32 i;
    s32 *q;
    s32 base;

    Event_Begin();
    for (i = 8; i <= 66; i++) {
        struct FieldActor *o = Func_02005716(i);

        if (o != 0 && o->active == 1 && *STAGED_ACTOR_PROBE_DETAILS(o)->unknown_28 == 165) {
            s32 dx = (p->x.fixed - o->x.fixed) / 65536;
            s32 dy = (p->z.fixed - o->z.fixed) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Event_SetMessage(MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE);
    Event_ShowMessage(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Event_Wait(20);
    Event_CloseScreen();
    Event_WaitForScreen();
    base = n << 4;
    Func_02005770(base + 880, p->x.fixed >> 20);
    {
        s32 v = p->z.fixed >> 20;

        Func_02005780_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Event_RequestExit(10);
        GameFlag_Set(282);
    } else {
        Func_0200343a(n);
        Event_OpenScreen();
        Event_WaitForScreen();
        *q = 0;
    }
    Event_End();
}

void RunPartyCountInteraction(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    Event_Begin();

    if (GetPartyMemberCount() <= 1) {
        Event_SetMessage(MSG_ROBIN_DID_GET_GOOD_LOOK);
        if (Event_AskYesNo(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            Actor_WalkTo(actorId, x, y + 0x40);
            Event_Wait(15);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, x, y + 0x20);
            Event_CloseScreen();
            Event_WaitForScreen();
            Event_RequestExit(11);
        }
    } else {
        Event_SetMessage(MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST);
        Event_ShowMessage(actorId, 0);
    }

    Event_End();
}

void FieldScene_RunSixSteps380To3A8(void)
{
    Func_0200599a(896, 0);
    Func_020059a4(904, 0);
    Func_020059ae(912, 0);
    Func_020059b8(920, 0);
    Func_020059c2(928, 0);
    Func_020059cc_a(936, 0);
}

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b)
{
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    s32 v;
    s32 id;
    s32 r;

    Func_02005b3c();
    Func_02005994(b, 5);
    v = gGameState.scene;
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Event_SetMessage(id);
    Event_ShowMessage(a, 0);
    if (GameFlag_IsSet(b + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(b + 520) != 0) {
        r = Func_020059f6(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    GameFlag_Set(b + 520);
    Event_SetMessage((s32)&LinkedMessage_WouldYouLikeHearDescription);
    Event_OpenMessage(a, 0);
    return Event_ChooseYesNo(0, 0);
}

void SceneState_SendIdBySceneId(s32 a, s32 b)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    s32 v;
    s32 id;

    Func_02005a4a(b, 5);
    v = gGameState.scene;
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Event_SetMessage(id + 1);
    Event_ShowMessage(a, 0);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern u8 Data_02000240[];

    struct FieldActor *rec;
    struct FieldActor *record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1(Func_02005b4e, owner);
    p9 = rec->x.part.pixel;
    p11 = rec->z.part.pixel;
    if (mode != 3) {
        count = Value0(Func_02005b24);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Event_SetMessage(MSG_DO_YOUR_BEST);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage(MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage(MSG_WOULD_LIKE_FRIEND_CHEER_FOR);
            Event_OpenMessage(owner, 0);
            state = Event_ChooseYesNo(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02005bb0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02005bc4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02005d3e);
            for (i = 0; i < count; i++) {
                Func_02005bee((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02005c00((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Event_SetMessage(MSG_IF_KNOW_WHO_WANT_CHEER);
    Event_ShowMessage(owner, 0);
    return;
L_main:
    ((void (*)())Func_02005bc4_a)(obj, 1);
    Event_SetMessage(MSG_ROBIN_WILL_CHEER_FOR_WAY);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Value1(Func_02005c9a, 0);
    if (record != 0) {
        Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Value3(Engine_ActorWalkToAndWait, 0, lo, hi);
    ((void (*)())Engine_ActorFaceEachOther)(obj, 0, 30);
    Actor_SetAnimation(obj, 3);
    tail = hi - 32;
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Value3(Engine_ActorWalkTo, owner, lo, tail);
    Func_02005e2c(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Actor_SetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Func_02005d10(obj);
    ((void (*)())Engine_GameFlagSet)(base + 512);
    rec = Value1(Func_02005d52, obj);
    sx = rec->x.fixed >> 20;
    Func_02005d1e_a((obj << 4) + 880, sx);
    sy = rec->z.fixed >> 20;
    Func_02005d2c((obj << 4) + 888, sy);
}

void SceneActor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    Rec *t = Func_02005d34(a);
    s32 i;

    Func_02005d46(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->fd8[i] == b) {
            Func_02005d62(a, i);
        }
    }
}

void SceneData_SelectBlockAndResetCounters(s32 a, s32 b)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    u8 *p;

    Data_0200c790 = a;
    Data_0200c764 = b << 4;
    Func_020061d0(Data_0200a1b9, 3200);
    p = Data_0200c57e;
    if (a == 2) {
        p = Data_0200be4e;
    }
    if (a == 4) {
        p = Data_0200c5aa;
    }
    if (a == 3) {
        if (b != 0) {
            p = Data_0200be76;
        } else {
            p = Data_0200c628;
        }
    }
    Data_0200c79c = 0;
    Data_0200c7a0 = p;
    Data_0200c7f8 = 0;
    Data_0200c76c = 0;
    Data_0200c770 = 0;
}

void FieldScene_RunTwoArmSequence(s32 a)
{
    extern u16 Data_0200c57e[];

    if (a == 0) {
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Audio_PlayCue(89);
        Func_02004880(0);
        SceneData_SelectBlockAndResetCounters(1, 0);
        Event_Wait(120);
        Event_End();
    } else {
        Audio_PlayCue(247);
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Data_0200c57e[15] = a * 60;
        Event_Wait(30);
        Audio_PlayCue(a + 90);
        Func_020048c4(a);
        SceneData_SelectBlockAndResetCounters(1, 0);
        Event_Wait(120);
        while (Func_020065ba() != 0) {
            Task_Wait(1);
        }
        Audio_PlayCue(0x121);
        Func_020048ee(5);
        SceneData_SelectBlockAndResetCounters(2, 0);
        Audio_PlayCue(236);
        Event_Wait(60);
        SceneData_SelectBlockAndResetCounters(2, 1);
        Audio_PlayCue(236);
        Event_Wait(60);
        Func_0200491c(6);
        SceneData_SelectBlockAndResetCounters(2, 0);
        Audio_PlayCue(236);
        Event_Wait(60);
        Func_02004936(7);
        SceneData_SelectBlockAndResetCounters(4, 0);
        Audio_PlayCue(237);
        Func_020065fc();
        Event_End();
        GameFlag_Set(0x123);
    }
}

void FieldScene_RunLateSequence(s32 a0)
{
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    void Func_02002ba8();
    void Func_02002d84();
    void Task_Wait();
    void Func_02006d62();

    s32 kind;

    Audio_PlayCue(247);
    Event_OpenScreen();
    Event_WaitForScreen();
    Data_0200c628.span = a0 * 60;
    Data_0200be76.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Event_Wait(30);
        Audio_PlayCue(86);
        Func_020049aa(8);
        Value2(SceneData_SelectBlockAndResetCounters, 3, 1);
        Event_Wait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Event_Wait(30);
        Audio_PlayCue(a0 + 90);
        Func_020049d6(4);
        Value2(SceneData_SelectBlockAndResetCounters, 3, 0);
        Event_Wait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (Value0(Func_020066d8)!= 0) {
        Task_Wait(1);
    }
    Audio_PlayCue(19);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Event_CloseScreen();
    Event_WaitForScreen();
}

void OverlayObject_ResetMotionFields(void)
{
    struct FieldActor *o = Func_02006868();

    Func_0200677e(o);
    o->velocity_x = 0;
    o->velocity_z = 0;
    o->target_x = ACTOR_NO_TARGET;
    o->target_z = ACTOR_NO_TARGET;
}

void SceneState_InitHalfwordC6a6Once(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    if (Data_0200c6a6 == -1) {
        Data_0200c6a6 = Func_02006816();
    }
}

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    SceneState_InitHalfwordC6a6Once();
    Data_0200c7f4 = a;
    Data_0200c780 = b;
    Data_0200c758 = c & 3;
    Data_0200c774 = 0;
    Data_0200c78c = 0;
    {
        s32 n = 3200;

        Func_0200686c(Data_0200abed, n);
    }
}

void SceneState_InitTableWordsAndLoad3200(s32 a, s32 b, s32 c)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    Data_0200c760 = a;
    Data_0200c800 = b;
    Data_0200c7a4 = Data_0200c7f4;
    Data_0200c7bc = Data_0200c780;
    Data_0200c78c = c;
    Data_0200c750 = 0;
    {
        s32 n = 3200;

        Func_020068c8(Data_0200abed, n);
    }
}

void SceneState_ReleaseTableAndResetC6a6(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    Func_02006906(Data_0200abed);
    Func_02006960(Data_0200c6a6);
    Data_0200c6a6 = -1;
}

void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c)
{
    struct FieldActor *o = Func_02006be0(a);

    if (o != 0) {
        s32 v = 0x20000;
        s32 z = 0;

        o->speed = v;
        o->acceleration = v >> 1;
        o->unknown_5b = z;
        Func_020069fc(o);
        Object_SetAnimation(o, 5);
        Func_02006a20(o, b << 16, o->y.fixed, c << 16);
    }
}

void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
    struct FieldActor *o = Func_02006c20(a);

    if (o != 0) {
        s32 v = 0x14000;
        s32 z = 0;

        o->speed = v;
        o->acceleration = v >> 1;
        o->unknown_5b = z;
        Func_02006a3c(o);
        Object_SetAnimation(o, 5);
        Func_02006a60(o, b << 16, o->y.fixed, c << 16);
        Func_02006a6e(o);
        Object_SetAnimation(o, 1);
    }
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

void Resource3ba_NoOpCallback(void)
{
}

void SceneState_SetHalfword1000To9(void)
{
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    u16 *p = &Data_02001000;
    s32 v = 9;

    *p = v;
}

void SceneState_WaitUntilWord1000IsNine(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    s16 *p = &Data_02001000;

    while (*p != 9) {
        Task_Wait(1);
    }
}

void SceneEffect_SpawnKind285AtRandomChance(struct FieldActor *a)
{
    s32 t[3];
    u32 n;

    if (a->velocity_y >= -255 && a->velocity_y <= 255) {
        a->motion_flags = 0;
    }
    n = Random_Next();
    if (n * 100 >> 16 <= 9) {
        struct FieldActor *o;
        s32 u;
        s32 w;

        t[0] = a->x.fixed;
        t[1] = a->y.fixed;
        t[2] = a->z.fixed;
        u = Random_Next();
        w = Random_Next();
        Func_02006b3e(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Func_02006bd2(285, x, y, z);
        }
        if (o != 0) {
            o->motion_flags = 0;
            Actor_SetSpriteFlags(o, 0);
            Object_SetScript(o, (s32)Data_0200bfe4);
            Object_SetAnimation(o, 1);
            Object_SetAnimation(o, 0);
        }
    }
}

s32 SceneActor_PlaceLinkedActorAbove(struct FieldActor *a)
{
    struct FieldActor *o = Func_02006d24((s16)a->unknown_64);

    Func_02006c56(o, a->x.fixed, a->y.fixed + 0x240000, a->z.fixed);
    o->motion_flags = 0;
    Object_SetScript(o, (s32)Data_0200c008);
    Audio_PlayCue(83);
    a->unknown_64 = 0;
    return 0;
}

s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 Data_02000240[];

    u8 *work = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    rec = Func_02006d88(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < *(s32 *)(rec + 8)) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cuep = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cuep = (u16 *)(work + 226);
    }

    waitp = (s16 *)(rec + 100);
    *waitp = *cuep;
    *(s32 *)(rec + 52) = 0x4000;
    *(s32 *)(rec + 48) = 0x10000;

    Func_02006d02(rec, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        Func_02005242(0, handle_a);
        Func_02006d82(handle_a, 2);
    } else {
        Func_02005254(0, handle_b);
        Func_02006d94(handle_b, 2);
    }

    shared = Data_02000240;
    Func_02006da4(*(s32 *)(shared + 500), 1);
    Message_ShowCentered(MSG_ROBIN_GOT, 3);
    Func_02006d62(rec);

    return flag;
}

/*
 * Seven arguments: four in registers, three from the caller's stack. The
 * 232-byte owner includes the six-word literal pool the body branches over.
 * The palette index stored at +216 is passed on sign-extended from sixteen
 * bits, so the narrowing is deliberate. Descriptor layout is asserted only
 * for the fields written here, and the actor records are touched at +8 and
 * +16 only on the flag-clear path.
 */
void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    extern Ctl Data_02001000;

    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = Func_020072c6(59, 0x7170);
    handle = Func_020072e0(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = Func_02007478(first);
    rec1 = Func_02007480(second);

    if (GameFlag_IsSet(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Func_02007356(Data_0200bf14, handle);

    pal = Func_0200737a();
    *(u16 *)(desc + 216) = (u16)pal;
    Vram_Load((s16)pal, 512, handle);

    Func_02007326((s32)Func_0200b3a0 + 1, 0xc76);

    Func_02007374(handle);
}

void SceneState_InitControlWhenFlag109Clear(void)
{
    extern Ctl Data_02001000;

    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020073be(Func_020073ee(), (s32)(state + 240));
    if (GameFlag_IsSet(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_02007392((s32)Data_0200b1c1, e);
    }
}

void SceneState_SetStateHalfword220(s32 a)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;

    u8 *p = Data_03001f3c;

    *(s16 *)(p + 220) = a;
}

s32 *SceneActor_FindSlotAtTilePosition(s32 *arg0)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] >> 20) == (p[3] >> 20)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * The push interaction: probe the cell one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. The 360-byte
 * owner includes its four-word literal pool, which ends where the next owner
 * begins. Record fields are asserted only where written -- facing at +6,
 * position at +8/+12/+16, state at +0x22, occupancy flag bit 0 at +0x59 -- and
 * the terrain probe is tested signed, so only a positive code refuses.
 */
void StagedActor_PushActorAhead(void)
{
    extern s16 Data_02000240[];

    struct FieldActor *subject;
    struct FieldActor *target;
    struct FieldActor *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)Data_02000240 + (idx << 1));
    subject = Func_020075cc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c154[dir];
    pos.x = subject->x.fixed + (s32)(step & 0xffff0000);
    pos.y = subject->y.fixed;
    step <<= 16;
    pos.z = subject->z.fixed + (s32)step;

    target = Func_020071f6(&pos, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200c154[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    blocker = Func_02007220(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + 0x100000;      /* 128 << 13 */
    pos.z = target->z.fixed;

    blocker = Func_0200724c(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    target->unknown_22 = 2;
    zero = 0;

    step = Data_0200c154[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    if (Func_020075e6(target, &pos) > 0) {
        return;
    }

    Object_SetAnimation(subject, 8);
    Task_Wait(15);

    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Func_020075da(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->speed = 0x3333;
    subject->acceleration = 0x3333;
    Func_020075ea(subject, pos.x, pos.y, pos.z);

    Audio_PlayCue(0xee);
    Func_020075fe(target);
    Audio_PlayCue(0x120);                                /* 144 << 1 */

    target->x.fixed = pos.x;
    target->z.fixed = pos.z;
    target->velocity_x = zero;
    target->velocity_z = zero;

    Object_SetAnimation(subject, 1);
}

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *SceneActor_FindOccupantAheadOfSubject(void)
{
    extern s16 Data_02000240[];

    u8 *rec;
    s32 facing;
    s32 pos[3];
    s32 *hit;

    rec = Func_0200772c(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;

    pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
    pos[1] = *(s32 *)(rec + 12);
    pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
    Func_020075de(0x100000, facing, pos);          /* 128 << 13 */

    hit = Func_02007366(pos, rec);
    if (hit == 0) {
        pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
        pos[1] = *(s32 *)(rec + 12);
        pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
        Func_02007610(0x200000, facing, pos);      /* 128 << 14 */

        hit = Func_02007398(pos, rec);
    }

    return hit;
}
