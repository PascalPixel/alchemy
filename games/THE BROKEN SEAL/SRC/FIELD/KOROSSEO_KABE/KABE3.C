#include "TASK.H"

extern u8 MsgKorosseoRobinDidGetGoodLook[];
extern u8 MsgKorosseoWaitShouldntDecideWhereBest[];

extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];
void Battle_ResetEffectCounter(void);
s32 PartyTalkMenu_Choose(s32 mode);

extern u8 MsgKorosseoDoYourBest[];
extern u8 MsgKorosseoIfKnowWhoWantCheer[];
extern u8 MsgKorosseoRobinWillCheerForWay[];
extern u8 MsgKorosseoUnfortunatelyWeHaveFullHouse[];
extern u8 MsgKorosseoWouldLikeFriendCheerFor[];

/* The game state's cells, read here as bytes. */
extern u8 gCell[];

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

/*
 * Six steps, each clearing one game-state byte -- 896 through 936, eight
 * apart.  It reads no incoming argument, so it takes none.
 */
void FieldScene_RunSixSteps896To936(void)
{
    GameFlag_SetByte(896, 0);
    GameFlag_SetByte(904, 0);
    GameFlag_SetByte(912, 0);
    GameFlag_SetByte(920, 0);
    GameFlag_SetByte(928, 0);
    GameFlag_SetByte(936, 0);
}

/* A finals competitor names the match by the stage the party stands on, the
 * river, the wall or the log-rolling stage; the first time, the competitor
 * offers to describe it. Returns the choice, or 2 and 3 for a party that has
 * heard it before. */
s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Battle_ResetEffectCounter();
    UiWork_PushValueSlot(b, 5);
    v = gGameState.scene;
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

/* The competitor's follow-up line for the stage's match. */
void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b)
{
    s32 v;
    s32 id;

    UiWork_PushValueSlot(b, 5);
    v = gGameState.scene;
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
    s32 rec;
    s32 record;
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
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Party_CountActiveOwners();
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            Engine_EventSetMessage((s32)MsgKorosseoDoYourBest);
            Engine_EventShowMessage(owner, 0);
            return;
        }
        if (Engine_GameFlagIsSet(base + 512) != 0) {
            Engine_EventSetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
            Engine_EventShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Engine_TaskWait(6);
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoWouldLikeFriendCheerFor);
            Engine_EventOpenMessage(owner, 0);
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
    Engine_EventShowMessage(owner, 0);
    return;
L_main:
    UiWork_PushValueSlot(obj, 1);
    Engine_EventSetMessage((s32)MsgKorosseoRobinWillCheerForWay);
    Engine_EventShowMessage(owner, 0);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Engine_ActorSetSpeed(obj, 0x10000, 0x8000);
    Engine_ActorSetSpeed(owner, 0x10000, 0x8000);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(obj, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    hi = p11 + 16;
    Engine_ActorWalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Engine_ActorWalkToAndWait(0, lo, hi);
    Engine_ActorFaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimation(obj, 3);
    tail = hi - 32;
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorWalkToAndWait(owner, p9, tail);
    Engine_ActorWalkTo(owner, lo, tail);
    Object_LinkObjectAndSetCallback(0, obj);
    Engine_ActorWalkToAndWait(obj, p9, tail);
    Engine_ActorSetAnimation(owner, 1);
    Engine_ActorFaceDirection(owner, 0x8000, 0);
    Engine_ActorWalkToAndWait(obj, p9, p11 - 48);
    Engine_ActorWalkToAndWait(owner, p9, tail);
    Engine_ActorWalkToAndWait(owner, p9, p11);
    Party_RemoveActiveOwner(obj);
    Engine_GameFlagSet(base + 512);
    rec = Object_GetById(obj);
    sx = *(s32 *)(rec + 8) >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = *(s32 *)(rec + 16) >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

/*
* Look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
* with each matching index.  The callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */
void OverlayObject_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = Owner_GetState(no);
    s32 i;

    Inventory_AddItem(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Inventory_Equip(no, i);
        }
    }
}
