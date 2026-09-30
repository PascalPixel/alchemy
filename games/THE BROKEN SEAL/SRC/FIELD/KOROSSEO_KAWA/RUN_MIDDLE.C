#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKorosseoDoYourBest[];
extern u8 MsgKorosseoIfKnowWhoWantCheer[];
extern u8 MsgKorosseoRobinWillCheerForWay[];
extern u8 MsgKorosseoUnfortunatelyWeHaveFullHouse[];
extern u8 MsgKorosseoWouldLikeFriendCheerFor[];

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

typedef void(*SceneTask)(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
s32 Party_CountActiveOwnersFar();
s32 Object_GetById();
void Party_RemoveActiveOwner();
void Party_AddActiveOwner();
void UiWork_PushValueSlot();
void GameFlag_SetByte();
void Object_LinkObjectAndSetCallback();
s32 Menu_OpenCharacterSelector();
Rec *Owner_GetState(s32);
void Inventory_AddItem(s32, s32);
void Inventory_EquipFar(s32, s32);

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

    rec = Value1(Object_GetById, owner);
    p9 = rec->x.part.pixel;
    p11 = rec->z.part.pixel;
    if (mode != 3) {
        count = Value0(Party_CountActiveOwnersFar);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Event_SetMessage((s32)MsgKorosseoDoYourBest);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage((s32)MsgKorosseoWouldLikeFriendCheerFor);
            Event_OpenMessage(owner, 0);
            state = Event_ChooseYesNo(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Party_RemoveActiveOwner((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Party_AddActiveOwner((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Menu_OpenCharacterSelector);
            for (i = 0; i < count; i++) {
                Party_RemoveActiveOwner((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Party_AddActiveOwner((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Event_SetMessage((s32)MsgKorosseoIfKnowWhoWantCheer);
    Event_ShowMessage(owner, 0);
    return;
L_main:
    ((void (*)())UiWork_PushValueSlot)(obj, 1);
    Event_SetMessage((s32)MsgKorosseoRobinWillCheerForWay);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Value1(Object_GetById, 0);
    if (record != 0) {
        Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, lo, hi);
    ((void (*)())Engine_ActorFaceEachOther)(obj, 0, 30);
    Actor_SetAnimation(obj, 3);
    tail = hi - 32;
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkTo(owner, lo, tail);
    Object_LinkObjectAndSetCallback(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Actor_SetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Party_RemoveActiveOwner(obj);
    ((void (*)())Engine_GameFlagSet)(base + 512);
    rec = Value1(Object_GetById, obj);
    sx = rec->x.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = rec->z.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

void SceneActor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    Rec *t = Owner_GetState(a);
    s32 i;

    Inventory_AddItem(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->fd8[i] == b) {
            Inventory_EquipFar(a, i);
        }
    }
}
