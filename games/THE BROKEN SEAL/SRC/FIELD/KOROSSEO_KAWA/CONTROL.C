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
#include "MEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
#include "OBJECT_RUNTIME.H"

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

extern u8 Korosseo_GaugeGraphics[];
extern u8 Korosseo_UpdatePathRival[];
extern u8 HexDigits[];
typedef void(*SceneTask)(void);
void Korosseo_DrawGauge(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
void Vector_AddPolarOffset();
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

s32 Object_CheckMovementCollision(struct FieldActor *, Position3 *);
void Script_WaitForEventTimeout(struct FieldActor *);

void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{

    u8 *desc;
    struct FieldActor *rec0;
    struct FieldActor *rec1;
    s32 handle;
    s32 pal;

    desc = Runtime_AllocateBlock(59, 0x7170);
    handle = (s32)Runtime_BumpAllocateAlternatePool(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = Object_GetById(first);
    rec1 = Object_GetById(second);

    if (Engine_GameFlagIsSet(0x109) == 0) {
        rec1->x.fixed =
            (centre << 1) - rec0->x.fixed;
        rec1->z.fixed = rec0->z.fixed;
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Resource_DecodeType01(Korosseo_GaugeGraphics, handle);

    pal = Resource_FindFreeEntry();
    *(u16 *)(desc + 216) = (u16)pal;
    Engine_VramLoad((s16)pal, 512, handle);

    Engine_TaskAddCallback((s32)Korosseo_DrawGauge + 1, 0xc76);

    Runtime_BumpFree(handle);
}

void SceneState_InitControlWhenFlag109Clear(s32 resource)
{
    u8 *work = gKorosseoWork;
    struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

    Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        recorder->mode = 1;
        recorder->mirror = 1;
        recorder->actor = *(u16 *)(work + 224);
        recorder->still = 0;
        recorder->pos = 0;
    }
    ((s32 (*)(void ( *)(void), s32))Engine_TaskAddCallback)(Korosseo_UpdatePathRival, 0xc85);
}
