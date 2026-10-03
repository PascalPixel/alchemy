/* Robin's look at the course, and clearing the saved actor positions. */
#include "LOG_ROLLING.H"

extern u8 MsgKorosseoRobinDidGetGoodLook[];
extern u8 MsgKorosseoWaitShouldntDecideWhereBest[];

extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];
void Battle_ResetEffectCounter(void);
void UiText_DrawQuantity(s32 value, s32 digits);
s32 PartyTalkMenu_Choose(s32 menu);

extern u8 MsgKorosseoDoYourBest[];
extern u8 MsgKorosseoIfKnowWhoWantCheer[];
extern u8 MsgKorosseoRobinWillCheerForWay[];
extern u8 MsgKorosseoUnfortunatelyWeHaveFullHouse[];
extern u8 MsgKorosseoWouldLikeFriendCheerFor[];

void RunPartyCountInteractionCopyB(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = (PartyInteractionRecord *)Object_GetById(actorId);
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

void ColossoLogRollingStage_ClearSavedActorPositions(void)
{
    extern void GameFlag_SetByte(s32, s32);

    GameFlag_SetByte(896, 0);
    GameFlag_SetByte(904, 0);
    GameFlag_SetByte(912, 0);
    GameFlag_SetByte(920, 0);
    GameFlag_SetByte(928, 0);
    GameFlag_SetByte(936, 0);
}

/* The finals' stage announcer: the line naming this match depends on which
 * of the three Colosso stages the party is on, and the first time a stage's
 * flag is met the announcer offers to describe it. */
s32 ColossoLogRollingStage_RunStateInteraction(s32 actor, s32 flags)
{
    s32 scene;
    s32 msg;
    s32 result;

    Battle_ResetEffectCounter();
    UiText_DrawQuantity(flags, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        msg = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        msg = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        msg = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(msg);
    Engine_EventShowMessage(actor, 0);
    if (Engine_GameFlagIsSet(flags + 512) != 0) {
        return 2;
    }
    if (Engine_GameFlagIsSet(flags + 520) != 0) {
        result = PartyTalkMenu_Choose(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Engine_GameFlagSet(flags + 520);
    Engine_EventSetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Engine_EventOpenMessage(actor, 0);
    return Engine_EventChooseYesNo(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor, s32 flags)
{
    s32 scene;
    s32 msg;

    UiText_DrawQuantity(flags, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        msg = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        msg = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        msg = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Engine_EventSetMessage(msg + 1);
    Engine_EventShowMessage(actor, 0);
}

/* Choosing a friend to cheer, and equipping a prize item. */
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern s32 GetPartyMemberCount();
    extern void Party_RemoveActiveOwner();
    extern void Party_AddActiveOwner();
    extern void GameFlag_SetByte();
    extern s32 Menu_OpenCharacterSelector();
    extern void Object_LinkObjectAndSetCallback();

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
        count = GetPartyMemberCount();
        for (i = 0; i < count; i++) {
            s32 at = 504 + i;

            buf[i] = ((u8 *)&gGameState)[at];
        }
        if (count <= 1) {
            Engine_EventSetMessage((s32)MsgKorosseoDoYourBest);
            Engine_EventShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Engine_EventSetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
            Engine_EventShowMessage(owner, 0);
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
    Engine_EventShowMessage(owner, 0);
    return;
L_main:
    UiText_DrawQuantity(obj, 1);
    Engine_EventSetMessage((s32)MsgKorosseoRobinWillCheerForWay);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(obj, *(s32 *)(record + 8), *(s32 *)(record + 16));
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
    sx = *(s32 *)(rec + 8) >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = *(s32 *)(rec + 16) >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    extern u8 *Owner_GetState();
    extern s32 Inventory_AddItem();
    extern void Inventory_Equip();

    u8 *record;
    s32 slot;

    record = Owner_GetState(handle);
    Inventory_AddItem(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Inventory_Equip(handle, slot);
        }
    }
}
