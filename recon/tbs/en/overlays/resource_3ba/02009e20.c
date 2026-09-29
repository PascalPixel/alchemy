/* Draft of SceneState_SendIdBySceneId, resource_3ba at 0x02009e20 (split from FIELD/KOROSSEO_KAWA/COORDINATOR.C).
 * Remaining difference: it loads constants through address-derived symbols (Value_/Data_0000/LinkedMessage_ names) that no link defines, so the overlay keeps its listing rows.
 * Twins: resource_3bb:0x0200a0b8 and resource_3bc:0x0200ab50 hold the same
 * 92 bytes with identical pools (scene numbers 0x8f and 0x90, messages
 * 0x2076 and 0x2078, plus 0x207a), so this one draft serves all three once
 * link-time numbers exist. */
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
extern u8 gIoWriteQueue[];
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
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
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

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
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
