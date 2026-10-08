#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
#include "CALL.H"

struct ModeRecord;

extern u16 Korosseo_ModeTaskMode;
extern u16 Korosseo_ModeTaskParam;
extern u16 Korosseo_ModeTaskTimer;
extern s32 Korosseo_ModeTaskScript;
extern u16 Korosseo_ModeMoveTarget;
extern u16 Korosseo_ModeMoveDuration;
extern s32 Korosseo_ModeTaskPosition;
void Korosseo_UpdateModeTask(void);

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

extern u8 HexDigits[];
typedef void(*SceneTask)(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
void Korosseo_LoadPortrait(s32);
s32 AudioCommand_GetStateByte(void);
void Audio_PlayCueFromEventWork(void);
void Korosseo_LoadPortrait();
s32 AudioCommand_GetStateByte();
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);


void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param)
{
    s32 handler;

    Korosseo_ModeTaskMode = (u16)mode;
    Korosseo_ModeTaskParam = (u16)(param << 4);

    Engine_TaskAddCallback((s32)(Korosseo_UpdateModeTask), 0xc80);

    handler = (s32)KorosseoKawa_RoundSpans;
    if (mode == 2) {
        handler = (s32)KorosseoKawa_ModeRecordTwo;
    }
    if (mode == 4) {
        handler = (s32)KorosseoKawa_ModeRecordFour;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&KorosseoKawa_SpanB;
        } else {
            handler = (s32)&KorosseoKawa_SpanA;
        }
    }

    Korosseo_ModeTaskTimer = 0;
    Korosseo_ModeTaskScript = handler;
    Korosseo_ModeMoveTarget = 0;
    Korosseo_ModeMoveDuration = 0;
    Korosseo_ModeTaskPosition = 0;
}

void FieldScene_RunTwoArmSequence(s32 a)
{
    if (a == 0) {
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(30);
        Engine_AudioPlayCue(89);
        Korosseo_LoadPortrait(0);
        SceneData_SelectBlockAndResetCounters(1, 0);
        Engine_EventWait(120);
        Engine_EventEnd();
    } else {
        Engine_AudioPlayCue(247);
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        KorosseoKawa_RoundSpans[15] = a * 60;
        Engine_EventWait(30);
        Engine_AudioPlayCue(a + 90);
        Korosseo_LoadPortrait(a);
        SceneData_SelectBlockAndResetCounters(1, 0);
        Engine_EventWait(120);
        while (AudioCommand_GetStateByte() != 0) {
            Engine_TaskWait(1);
        }
        Engine_AudioPlayCue(0x121);
        Korosseo_LoadPortrait(5);
        SceneData_SelectBlockAndResetCounters(2, 0);
        Engine_AudioPlayCue(236);
        Engine_EventWait(60);
        SceneData_SelectBlockAndResetCounters(2, 1);
        Engine_AudioPlayCue(236);
        Engine_EventWait(60);
        Korosseo_LoadPortrait(6);
        SceneData_SelectBlockAndResetCounters(2, 0);
        Engine_AudioPlayCue(236);
        Engine_EventWait(60);
        Korosseo_LoadPortrait(7);
        SceneData_SelectBlockAndResetCounters(4, 0);
        Engine_AudioPlayCue(237);
        Audio_PlayCueFromEventWork();
        Engine_EventEnd();
        Engine_GameFlagSet(0x123);
    }
}

void FieldScene_RunLateSequence(s32 a0)
{
    void Engine_TaskWait();

    s32 kind;

    Engine_AudioPlayCue(247);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    KorosseoKawa_SpanA.span = a0 * 60;
    KorosseoKawa_SpanB.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(86);
        Korosseo_LoadPortrait(8);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Value2(SceneData_SelectBlockAndResetCounters, 3, 1);
        Engine_EventWait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Engine_EventWait(30);
        Engine_AudioPlayCue(a0 + 90);
        Korosseo_LoadPortrait(4);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Value2(SceneData_SelectBlockAndResetCounters, 3, 0);
        Engine_EventWait(a0 * 60 + 60);
        kind = 8;
    }
    Engine_ActorShowEmote(kind, 0x105, 0);
    while (AudioCommand_GetStateByte()!= 0) {
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(19);
    Engine_EventWait(30);
    Engine_AudioPlayCue(0x121);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}
