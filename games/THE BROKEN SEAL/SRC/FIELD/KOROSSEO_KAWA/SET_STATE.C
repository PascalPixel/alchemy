#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKorosseoMatchAboutBeginPleaseTake[];

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

extern u8 HexDigits[];

struct FieldActor *Object_GetById(s32);
void GameFlag_SetByteFar();
void GameFlag_SetByteFar(s32, s32);
void Korosseo_SelectSoloCompetitor(s32);
typedef void(*SceneTask)(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);

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

/* A countdown word this overlay owns at KorosseoKawa_Countdown: each call decrements
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

void SceneState_SetStateHalfword386To99WhenMatched(void)
{
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
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    struct FieldActor *p = Object_GetById(n);
    s32 i;
    s32 *q;
    s32 base;

    Event_Begin();
    for (i = 8; i <= 66; i++) {
        struct FieldActor *o = Object_GetById(i);

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
    Event_SetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
    Event_ShowMessage(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Event_Wait(20);
    Event_CloseScreen();
    Event_WaitForScreen();
    base = n << 4;
    GameFlag_SetByteFar(base + 880, p->x.fixed >> 20);
    {
        s32 v = p->z.fixed >> 20;

        GameFlag_SetByteFar(base + 888, v);
    }
    n++;
    if (n > 3) {
        Event_RequestExit(10);
        GameFlag_Set(282);
    } else {
        Korosseo_SelectSoloCompetitor(n);
        Event_OpenScreen();
        Event_WaitForScreen();
        *q = 0;
    }
    Event_End();
}
