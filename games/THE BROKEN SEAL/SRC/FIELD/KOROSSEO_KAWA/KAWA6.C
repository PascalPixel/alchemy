#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"


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

u8 *Runtime_AllocateBlock();
s32 Runtime_BumpAllocateAlternatePool();
s32 Resource_FindFreeEntry();
void Runtime_BumpFree();
s32 Object_CheckMovementCollision(struct FieldActor *, Position3 *);
void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);
void Script_WaitForEventTimeout(struct FieldActor *);

/* The rival's path recorder in the scene state, as COMMON/KOROSSEO/PATH_RIVAL.C
   reads it. */
struct PathRecorder {
    s16 mode;
    s16 mirror;
    s16 actor;
    u16 pos;
    s16 still;
};

void *Resource_GetTableEntry(s32 resource);

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

/* allocate a record by (id, size) */

/* reserve a graphics handle */

/* upload image data to a handle */

/* next palette slot index */

/* install a per-frame task (callback, rate) */

/* release a graphics handle */

/* terrain probe */

/* place at (x, y, z) */

/* place at (x, y, z) */

/* re-attach the camera */

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

    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = Runtime_AllocateBlock(59, 0x7170);
    handle = Runtime_BumpAllocateAlternatePool(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = (u8 *)Object_GetById(first);
    rec1 = (u8 *)Object_GetById(second);

    if (GameFlag_IsSet(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Resource_DecodeType01(Korosseo_GaugeGraphics, handle);

    pal = Resource_FindFreeEntry();
    *(u16 *)(desc + 216) = (u16)pal;
    Engine_VramLoad((s16)pal, 512, handle);

    Scheduler_AddOrUpdateCallback((s32)Korosseo_DrawGauge + 1, 0xc76);

    Runtime_BumpFree(handle);
}

/* Decode the rival's recorded course into the stage work and, until flag
   0x109 is set, start the recorder replaying it for the actor the work
   names; then schedule the rival's update. */
void SceneState_InitControlWhenFlag109Clear(s32 resource)
{
    u8 *work = gKorosseoWork;
    struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

    Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
    if (GameFlag_IsSet(0x109) == 0) {
        recorder->mode = 1;
        recorder->mirror = 1;
        recorder->actor = *(u16 *)(work + 224);
        recorder->still = 0;
        recorder->pos = 0;
    }
    ((s32 (*)(void ( *)(void), s32))Scheduler_AddOrUpdateCallback)(Korosseo_UpdatePathRival, 0xc85);
}
