#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
#include "OBJECT_RUNTIME.H"

extern u16 Korosseo_MarkerX;
extern u16 Korosseo_MarkerY;
extern u16 Korosseo_MarkerPriority;
extern u16 Korosseo_MarkerBlink;
extern u16 Korosseo_MarkerSteps;
extern u16 Korosseo_MarkerEndX;
extern u16 Korosseo_MarkerEndY;
extern u16 Korosseo_MarkerStartX;
extern u16 Korosseo_MarkerStartY;
extern u16 Korosseo_MarkerStep;
void Korosseo_UpdateMarker(void);

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

struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern s16 Korosseo_MarkerSlot;
extern u8 HexDigits[];
typedef void(*SceneTask)(void);

PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
void ObjectDispatch_InitFromTable6(struct FieldActor *);
void Script_WaitForEventTimeout(struct FieldActor *);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

void SceneState_StoreParamsAndInitTable(u32 x, u32 y, u32 style)
{
    KorosseoKawa_EnsureMarkerSlot();

    Korosseo_MarkerX = (u16)x;
    Korosseo_MarkerY = (u16)y;
    Korosseo_MarkerPriority = (u16)(style & 3);
    Korosseo_MarkerBlink = 0;
    Korosseo_MarkerSteps = 0;

    Engine_TaskAddCallback((s32)(Korosseo_UpdateMarker), 0xc80);
}

void SceneState_InitTableWordsAndLoad3200(u32 x, u32 y, u32 duration)
{
    Korosseo_MarkerEndX = (u16)x;
    Korosseo_MarkerEndY = (u16)y;
    Korosseo_MarkerStartX = Korosseo_MarkerX;
    Korosseo_MarkerStartY = Korosseo_MarkerY;
    Korosseo_MarkerSteps = (u16)duration;
    Korosseo_MarkerStep = 0;

    Engine_TaskAddCallback((s32)(Korosseo_UpdateMarker), 0xc80);
}

void KorosseoKawa_ReleaseMarker(void)
{

    Engine_TaskRemoveCallback(Korosseo_UpdateMarker);
    Resource_ResetEntry(Korosseo_MarkerSlot);
    Korosseo_MarkerSlot = -1;
}

void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c)
{
    struct FieldActor *o = Engine_ActorLookup(a);

    if (o != 0) {
        s32 v = 0x20000;
        s32 z = 0;

        o->speed = v;
        o->acceleration = v >> 1;
        o->paused = z;
        ObjectDispatch_InitFromTable6(o);
        Object_SetMode(o, 5);
        Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
    }
}

void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
    struct FieldActor *o = Engine_ActorLookup(a);

    if (o != 0) {
        s32 v = 0x14000;
        s32 z = 0;

        o->speed = v;
        o->acceleration = v >> 1;
        o->paused = z;
        ObjectDispatch_InitFromTable6(o);
        Object_SetMode(o, 5);
        Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
        Script_WaitForEventTimeout(o);
        Object_SetMode(o, 1);
    }
}
