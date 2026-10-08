#define OWN_gGameState
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
#include "BATTLE_UNIT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
#include "text/MSG_IDS.H"

extern u8 gGameState[];


#define PartyTalkMenu_Choose ((s32 (*)(s32))PartyTalkMenu_Choose)

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
s32 GetPartyMemberCount(void);
void GameFlag_SetByte(s32, s32);
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

static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void Battle_ResetEffectCounter(void);
void UiWork_PushValueSlot(s32 value, s32 digits);

void Party_RemoveActiveOwner();
void Party_AddActiveOwner();
void UiWork_PushValueSlot();
void GameFlag_SetByte();
void Object_LinkObjectAndSetCallback();
s32 Menu_OpenCharacterSelector();
void Inventory_AddItem(s32, s32);

void RunPartyCountInteraction(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = ((PartyInteractionRecord * (*)(void))Object_GetById)();
    x = record->x;
    y = record->y;
    Engine_EventBegin();

    if (GetPartyMemberCount() <= 1) {
        Engine_EventSetMessage((s32)MsgKorosseoRobinDidGetGoodLook);
        if (Engine_EventAskYesNo(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            Engine_ActorWalkTo(actorId, x, y + 0x40);
            Engine_EventWait(15);
            Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, x, y);
            Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, x, y + 0x20);
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
            Engine_EventRequestExit(11);
        }
    } else {
        Engine_EventSetMessage((s32)MsgKorosseoWaitShouldntDecideWhereBest);
        Engine_EventShowMessage(actorId, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunSixSteps380To3A8(void)
{
    GameFlag_SetByte(896, 0);
    GameFlag_SetByte(904, 0);
    GameFlag_SetByte(912, 0);
    GameFlag_SetByte(920, 0);
    GameFlag_SetByte(928, 0);
    GameFlag_SetByte(936, 0);
}

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor)
{
    s32 scene;
    s32 message;
    s32 choice;

    Battle_ResetEffectCounter();
    UiWork_PushValueSlot(competitor, 5);
    scene = ((struct GameState *)gGameState)->scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        message = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        message = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        message = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(message);
    Engine_EventShowMessage(speaker, 0);
    if (Engine_GameFlagIsSet(competitor + 512) != 0) {
        return 2;
    }
    if (Engine_GameFlagIsSet(competitor + 520) != 0) {
        choice = PartyTalkMenu_Choose(0);
        if (choice == 1) {
            return 2;
        }
        if (choice == 2 || choice == -1) {
            return 3;
        }
        return choice;
    }
    Engine_GameFlagSet(competitor + 520);
    Engine_EventSetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Engine_EventOpenMessage(speaker, 0);
    return Engine_EventChooseYesNo(0, 0);
}

void SceneState_SendIdBySceneId(s32 speaker, s32 competitor)
{
    s32 scene;
    s32 message;

    UiWork_PushValueSlot(competitor, 5);
    scene = ((struct GameState *)gGameState)->scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        message = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        message = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        message = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(message + 1);
    Engine_EventShowMessage(speaker, 0);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{

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

    rec = (s32)Object_GetById(owner);
    p9 = rec->x.part.pixel;
    p11 = rec->z.part.pixel;
    if (mode != 3) {
        count = GetPartyMemberCount();
        for (i = 0; i < count; i++) {
            buf[i] = gGameState[504 + i];
        }
        if (count <= 1) {
            Engine_EventSetMessage((s32)MsgKorosseoDoYourBest);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Engine_EventSetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Engine_TaskWait(6);
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoWouldLikeFriendCheerFor);
            Event_OpenMessage(owner, 0);
            state = Engine_EventChooseYesNo(0, 0);
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
            obj = Menu_OpenCharacterSelector();
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
    Engine_EventSetMessage((s32)MsgKorosseoIfKnowWhoWantCheer);
    Event_ShowMessage(owner, 0);
    return;
L_main:
    UiWork_PushValueSlot(obj, 1);
    Engine_EventSetMessage((s32)MsgKorosseoRobinWillCheerForWay);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, lo, hi);
    Engine_ActorFaceEachOther(obj, 0, 30);
    Engine_ActorSetAnimation(obj, 3);
    tail = hi - 32;
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkTo(owner, lo, tail);
    Object_LinkObjectAndSetCallback(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Engine_ActorSetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Party_RemoveActiveOwner(obj);
    Engine_GameFlagSet(base + 512);
    rec = (s32)Object_GetById(obj);
    sx = rec->x.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = rec->z.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

void SceneActor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    struct BtlUnit *t = UnitGet(a);
    s32 i;

    Inventory_AddItem(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->inventory[i] == b) {
            Inventory_Equip(a, i);
        }
    }
}
