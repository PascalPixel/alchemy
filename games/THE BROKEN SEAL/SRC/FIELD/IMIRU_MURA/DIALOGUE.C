#include "IMIRU.H"

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_EVEN_IF_MIA_HEALS_US);
    } else {
        Event_SetMessage(MSG_BRRRRR_CHOO_IM_FREEZING_MIA);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Event_ShowMessage(mode, val);
    }
    Event_End();
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_CANT_UNDERSTAND_WHY_ANY_ONE);
    } else {
        Event_SetMessage(MSG_MIA_SHOULD_HERE_BY_NOW);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActor9Line(void)
{
    Event_Begin();
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 10);
    Event_SetMessage(MSG_HI_NEW_IN_IMIL);
    Event_AskYesNo(9, 0);
    Event_End();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Engine_EventAskYesNo's
 * unused s32 return is what fixes that call's argument order.
 */
void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x881) != 0) {
        Event_SetMessage(MSG_ONE_TWO_THREE_FOUR_2);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_AskYesNo(10, 0);
        Call_02002630(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    } else {
        Event_SetMessage(MSG_ONE_TWO_THREE_FOUR);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_ShowMessage(10, 0);
        Call_02002684(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    }

    Event_End();
}

/* Picks one of three scripted call sequences depending on two condition
 * checks (codes 2177 and 2091), each acting on actor 9 and/or actor 8. */
void FieldScene_RunSupplementalSequenceOne(void)
{

    void *actor9_record;
    void *unused_actor9_record;
    void *actor8_record;

    if (GameFlag_IsSet(2177) != 0) {
        Event_Begin();
        unused_actor9_record = Value3(Engine_ActorFaceActor, 9, 0, 0);
        Value1(Engine_EventWait, 10);
        Value1(Engine_EventSetMessage, 5700);
        Event_AskYesNo(9, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(2091) != 0) {
            Event_Begin();
            Actor_SetAnimation(9, 7);
            Value3(Engine_MapAnimateCells, (s32)ImiruMura_CellStepsA, 10, 69);
            Value1(Engine_EventSetMessage, 5484);
            Event_ShowMessage(9, 0);
            Actor_SetAnimation(9, 8);
            Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
            Event_End();
        } else {
            Event_Begin();
            actor9_record = Value1(Engine_ActorGet, 9);
            ((struct SceneRecord *)actor9_record)->field_0x64 = 10;
            Value2(Engine_ActorEnableActionCallback, 9, (s32)ImiruMura_ActorScriptA);
            Value1(Engine_EventSetMessage, 5428);
            Value2(Engine_EventShowMessage, 9, 0);
            Value1(Engine_ActorStop, 8);
            Actor_ShowEmote(8, 256, 40);
            Actor_FaceDirection(8, 53248, 10);
            Actor_StartRepeatedMotion(8, 2);
            Event_ShowMessageAndWait(8, 0, 20);
            Value2(Engine_ActorEnableActionCallback, 0, (s32)ImiruMura_ActorScriptC);
            Actor_SetSpeed(8, 104857, 52428);
            Value2(Object_SetActionCallbackAndRefreshById, 8, (s32)ImiruMura_ActorScriptB);
            Value1(Engine_EventWait, 40);
            Actor_Jump(8, 2, 0);
            Actor_StartRepeatedMotion(8, 2);
            Value2(Engine_ActorSetAttachedEffect, 8, 258);
            Value1(Engine_EventWait, 60);
            Event_ShowMessageAndWait(8, 0, 10);
            Actor_FaceDirection(8, 12288, 20);
            Actor_StartRepeatedMotion(8, 2);
            Value2(Engine_EventShowMessage, 8, 0);
            actor8_record = Value1(Engine_ActorGet, 8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set(0x82c);
            Event_End();
        }
    }
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_WANT_WEAPONS);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActor18Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_WANT_SEE_RESTAURANT_MENU);
    Event_AskYesNo(18, 0);
    Event_End();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_EVERYONE_COUNTS_ON_MIA_THATS);
        Event_ShowMessage(20, 0);
    } else {
        Event_SetMessage(MSG_HAVE_VISITED_OLD_COUPLE_WHO);
        Event_AskYesNo(20, 0);
        GameFlag_Set(0x82a);
        GameFlag_Set(0x82c);
    }
    Event_End();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_WE_HAVE_DO_WHATEVER_WE);
    } else {
        Event_SetMessage(MSG_MIA_WAS_SAYING_SHE_HAS);
    }
    Event_ShowMessage(20, 0);
    Event_End();
}

void FieldScene_RunScene399_020005dc(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_HAPPENED_IN_LIGHTHOUSE_NORTHEAST);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_End();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x82b) != 0) {
        Event_SetMessage(MSG_MIA_CLAN_ONCE_LIVED_HERE);
    } else if (GameFlag_IsSet(0x82c) != 0) {
        Event_SetMessage(MSG_HES_ALWAYS_EXAGGERATING_THINGS_BUT);
    } else {
        Event_SetMessage(MSG_MIA_RUNNING_AROUND_TOWN_CARING);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunSingleStep(void)
{
    FieldScene_RunSupplementalSequenceOne();
}

void SceneDialogue_ShowLine156E(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MIA_GOOD_GIRL_WISH_HAD);
    Event_ShowMessage(10, 0);
    Event_End();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_FEEL_LIKE_GROWN_UP_WHEN);
    } else {
        Event_SetMessage(MSG_THESE_FOLK_OKAY_THEY_DONT);
    }
    Event_ShowMessage(19, 0);
    Event_End();
}
