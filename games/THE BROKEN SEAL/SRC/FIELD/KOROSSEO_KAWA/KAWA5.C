#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"

extern const s32 KorosseoKawa_ApproachScript[];


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


/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 LinkedMessage_WouldYouLikeHearDescription;
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
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void Object_SetMoveTarget();
s32 SceneActor_ApplyValueAndMatchingSlots();
void UiWork_PushValueSlot();
void ObjectDispatch_WaitForValue16();

extern u8 MsgKorosseoRobinGotItem[];

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

/* veneer to Object_SetPosition */

/* local thunk to Func_020020e8, site A */

/* local thunk to Func_020020e8, site B */

/* veneer to UiText_DrawQuantity, site A */

/* veneer to UiText_DrawQuantity, site B */

/* shared veneer, selector refresh + 0x96a */

/* veneer to Func_08009148 */
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

/* The river stage marks its start in the scene state's first halfword: 9
   once the stage is ready. */
void SceneState_SetHalfword1000To9(void)
{
    s16 *ready = (s16 *)gSceneState;
    /* FAKEMATCH: a word temporary; a halfword constant stored into the byte
       buffer is loaded from the literal pool instead. */
    s32 nine = 9;

    *ready = nine;
}

/* Wait a frame at a time until the stage is ready. */
void SceneState_WaitUntilWord1000IsNine(void)
{
    s16 *ready = (s16 *)gSceneState;

    while (*ready != 9) {
        Engine_TaskWait(1);
    }
}

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

/* veneer to Object_SetPosition */

/* local thunk to Func_020020e8, site A */

/* local thunk to Func_020020e8, site B */

/* veneer to UiText_DrawQuantity, site A */

/* veneer to UiText_DrawQuantity, site B */

/* shared veneer, selector refresh + 0x96a */

/* veneer to Func_08009148 */
void SceneEffect_SpawnKind285AtRandomChance(struct FieldActor *a)
{
    s32 t[3];
    u32 n;

    if (a->velocity_y >= -255 && a->velocity_y <= 255) {
        a->motion_flags = 0;
    }
    n = Engine_RandomNext();
    if (n * 100 >> 16 <= 9) {
        struct FieldActor *o;
        s32 u;
        s32 w;

        t[0] = a->x.fixed;
        t[1] = a->y.fixed;
        t[2] = a->z.fixed;
        u = Engine_RandomNext();
        w = Engine_RandomNext();
        Vector_AddPolarOffset(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Object_CreateFar(285, x, y, z);
        }
        if (o != 0) {
            o->motion_flags = 0;
            Engine_ActorSetSpriteFlags(o, 0);
            Engine_ObjectSetScript(o, (s32)KorosseoKawa_ScriptA);
            Object_SetMode(o, 1);
            Object_SetMode(o, 0);
        }
    }
}

s32 SceneActor_PlaceLinkedActorAbove(struct FieldActor *a)
{
    struct FieldActor *o = Object_GetById((s16)a->unknown_64);

    Object_SetMoveTarget(o, a->x.fixed, a->y.fixed + 0x240000, a->z.fixed);
    o->motion_flags = 0;
    Engine_ObjectSetScript(o, (s32)KorosseoKawa_ScriptB);
    Engine_AudioPlayCue(83);
    a->unknown_64 = 0;
    return 0;
}

s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 Data_02000240[];

    u8 *work = gKorosseoWork;
    u8 *shared;
    struct FieldActor *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Engine_GameFlagIsSet(0x211);

    shared = Data_02000240;
    rec = Object_GetById(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < rec->x.fixed) {
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

    waitp = (s16 *)&rec->unknown_64;
    *waitp = *cuep;
    rec->acceleration = 0x4000;
    rec->speed = 0x10000;

    Object_SetMoveTarget(rec, x, 0, z);
    Engine_GameFlagSet(0x211);
    Engine_ObjectSetScript(rec, (s32)KorosseoKawa_ApproachScript);

    while (*waitp != 0) {
        Engine_TaskWait(1);
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
    Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(rec);

    return flag;
}
