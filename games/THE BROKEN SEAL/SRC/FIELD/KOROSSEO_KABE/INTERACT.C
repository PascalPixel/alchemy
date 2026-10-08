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
#include "BATTLE_UNIT.H"
#include "TASK.H"
#include "text/MSG_IDS.H"

#define PartyTalkMenu_Choose ((s32 (*)(s32))PartyTalkMenu_Choose)


void Battle_ResetEffectCounter(void);


extern u8 gGameState[];

void RunPartyCountInteractionCopyA(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = (PartyInteractionRecord *)Object_GetById(actorId);
    x = record->x;
    y = record->y;
    Engine_EventBegin();

    if (Party_CountActiveOwners() <= 1) {
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

void FieldScene_RunSixSteps896To936(void)
{
    GameFlag_SetByte(896, 0);
    GameFlag_SetByte(904, 0);
    GameFlag_SetByte(912, 0);
    GameFlag_SetByte(920, 0);
    GameFlag_SetByte(928, 0);
    GameFlag_SetByte(936, 0);
}

s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Battle_ResetEffectCounter();
    UiWork_PushValueSlot(b, 5);
    v = ((struct GameState *)gGameState)->scene;
    if (v == (s32)&SceneId_KorosseoKawa) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&SceneId_KorosseoKabe) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(id);
    Engine_EventShowMessage(a, 0);
    if (Engine_GameFlagIsSet(b + 512) != 0) {
        return 2;
    }
    if (Engine_GameFlagIsSet(b + 520) != 0) {
        r = PartyTalkMenu_Choose(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Engine_GameFlagSet(b + 520);
    Engine_EventSetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Engine_EventOpenMessage(a, 0);
    return Engine_EventChooseYesNo(0, 0);
}

void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b)
{
    s32 v;
    s32 id;

    UiWork_PushValueSlot(b, 5);
    v = ((struct GameState *)gGameState)->scene;
    if (v == (s32)&SceneId_KorosseoKawa) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&SceneId_KorosseoKabe) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(id + 1);
    Engine_EventShowMessage(a, 0);
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

    rec = Object_GetById(owner);
    p9 = rec->x.part.pixel;
    p11 = rec->z.part.pixel;
    if (mode != 3) {
        count = Party_CountActiveOwners();
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
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Engine_ActorWalkToAndWait(0, lo, hi);
    Engine_ActorFaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimation(obj, 3);
    tail = hi - 32;
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Engine_ActorWalkTo(owner, lo, tail);
    Object_LinkObjectAndSetCallback(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Engine_ActorSetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Party_RemoveActiveOwner(obj);
    GameFlag_Set(base + 512);
    rec = Object_GetById(obj);
    sx = rec->x.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = rec->z.fixed >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

void OverlayObject_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = UnitGet(no);
    s32 i;

    Inventory_AddItem(no, val);

    tbl = ((struct BtlUnit *)tbl)->inventory;
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Inventory_Equip(no, i);
        }
    }
}
