#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKorosseoRobinGotItem[];
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
struct FieldActor *Object_GetById();

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

extern u8 KorosseoKawa_ScriptB[];
extern u8 HexDigits[];
extern u8 KorosseoKawa_ScriptA[];

typedef void(*SceneTask)(void);

void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);
void Vector_AddPolarOffset(s32, s32, s32 *);
struct FieldActor *Object_CreateFar(s32, s32, s32, s32);

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

void Object_SetMoveTarget();          /* veneer to Object_SetPosition */

s32 SceneActor_ApplyValueAndMatchingSlots();           /* local thunk to Func_020020e8, site A */

s32 SceneActor_ApplyValueAndMatchingSlots();           /* local thunk to Func_020020e8, site B */

void UiWork_PushValueSlot();          /* veneer to UiText_DrawQuantity, site A */

void UiWork_PushValueSlot();          /* veneer to UiText_DrawQuantity, site B */

void UiWork_PushValueSlot();          /* shared veneer, selector refresh + 0x96a */

void ObjectDispatch_WaitForValue16();          /* veneer to Func_08009148 */

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
        Vector_AddPolarOffset(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Object_CreateFar(285, x, y, z);
        }
        if (o != 0) {
            o->motion_flags = 0;
            Actor_SetSpriteFlags(o, 0);
            Object_SetScript(o, (s32)KorosseoKawa_ScriptA);
            Object_SetAnimation(o, 1);
            Object_SetAnimation(o, 0);
        }
    }
}

s32 SceneActor_PlaceLinkedActorAbove(struct FieldActor *a)
{
    struct FieldActor *o = Object_GetById((s16)a->unknown_64);

    Object_SetMoveTarget(o, a->x.fixed, a->y.fixed + 0x240000, a->z.fixed);
    o->motion_flags = 0;
    Object_SetScript(o, (s32)KorosseoKawa_ScriptB);
    Audio_PlayCue(83);
    a->unknown_64 = 0;
    return 0;
}

s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 Data_02000240[];

    u8 *work = gKorosseoWork;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    rec = ((u8 *(*)())Object_GetById)(*(s32 *)(shared + 500));

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

    Object_SetMoveTarget(rec, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        SceneActor_ApplyValueAndMatchingSlots(0, handle_a);
        UiWork_PushValueSlot(handle_a, 2);
    } else {
        SceneActor_ApplyValueAndMatchingSlots(0, handle_b);
        UiWork_PushValueSlot(handle_b, 2);
    }

    shared = Data_02000240;
    UiWork_PushValueSlot(*(s32 *)(shared + 500), 1);
    Message_ShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(rec);

    return flag;
}
