#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
/* FAKEMATCH: calls that cast SceneActor_FindSlotAtTilePosition to another return type keep their original register order. */
s32 *SceneActor_FindSlotAtTilePosition(s32 *arg0);

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

extern u8 LinkedMessage_WouldYouLikeHearDescription;
extern u8 Korosseo_GaugeGraphics[];
extern u8 Korosseo_UpdatePathRival[];
extern u8 HexDigits[];
extern u32 KorosseoKawa_DirectionSteps[];

typedef void(*SceneTask)(void);
void Korosseo_DrawGauge(void);
s32 Resource_GetTableEntryFar(void);
void Resource_DecodeType01(s32, s32);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);

void Vector_AddPolarOffset();

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

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

s32 *SceneActor_FindOccupantAheadOfSubject(void);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

u8 *Runtime_AllocateBlock();            /* allocate a record by (id, size) */


void Resource_DecodeType01();           /* upload image data to a handle */

s32 Resource_FindFreeEntry();            /* next palette slot index */


void Runtime_BumpFree();           /* release a graphics handle */

s32 Object_CheckMovementCollision(struct FieldActor *, Position3 *);  /* terrain probe */

void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);   /* place at (x, y, z) */

void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);   /* place at (x, y, z) */

void Script_WaitForEventTimeout(struct FieldActor *);              /* re-attach the camera */

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
    subject = Object_GetById(handle);

    dir = subject->facing >> 12;

    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = subject->x.fixed + (s32)(step & 0xffff0000);
    pos.y = subject->y.fixed;
    step <<= 16;
    pos.z = subject->z.fixed + (s32)step;

    target = ((struct FieldActor *(*)())SceneActor_FindSlotAtTilePosition)(&pos, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    blocker = ((struct FieldActor *(*)())SceneActor_FindSlotAtTilePosition)(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + 0x100000;      /* 128 << 13 */
    pos.z = target->z.fixed;

    blocker = ((struct FieldActor *(*)())SceneActor_FindSlotAtTilePosition)(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    target->unknown_22 = 2;
    zero = 0;

    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    if (Object_CheckMovementCollision(target, &pos) > 0) {
        return;
    }

    Object_SetMode(subject, 8);
    Engine_TaskWait(15);

    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Object_SetMoveTarget(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->speed = 0x3333;
    subject->acceleration = 0x3333;
    Object_SetMoveTarget(subject, pos.x, pos.y, pos.z);

    Audio_PlayCue(0xee);
    Script_WaitForEventTimeout(target);
    Audio_PlayCue(0x120);                                /* 144 << 1 */

    target->x.fixed = pos.x;
    target->z.fixed = pos.z;
    target->velocity_x = zero;
    target->velocity_z = zero;

    Object_SetMode(subject, 1);
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

    rec = (u8 *)Object_GetById(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;

    pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
    pos[1] = *(s32 *)(rec + 12);
    pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, pos);          /* 128 << 13 */

    hit = ((s32 *(*)())SceneActor_FindSlotAtTilePosition)(pos, rec);
    if (hit == 0) {
        pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
        pos[1] = *(s32 *)(rec + 12);
        pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, pos);      /* 128 << 14 */

        hit = ((s32 *(*)())SceneActor_FindSlotAtTilePosition)(pos, rec);
    }

    return hit;
}
