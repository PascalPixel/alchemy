#include "MURA.H"

void FieldScene_RunScriptedStep1472(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_AREA_OFF_LIMITS_THOSE_WITHOUT, 1);
    Event_End();
}

void FieldScene_RunScriptedStep146E(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_MCCOYS_HIDDEN_WAREHOUSE_DO_NOT, 1);
    Event_End();
}

void SceneDialogue_RunLine1470(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_THERE_TREE_LOOKS_LIKE_PERSON, 1);
    Event_End();
}

void FieldScene_RunScene38b_02000240(void)
{
    Event_Begin();
    Event_SetMessage(MSG_ITS_TREE_BUT_ALMOST_LOOKS);
    if (GameFlag_IsSet(0x301) != 0) {
        bump_step_020001ec(1);
    }
    Event_ShowMessage(9, 0);
    GameFlag_Set(0x301);
    Event_End();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_SEE_TREE_AT_ENTRANCE);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HAVE_TRIED_HEADING_SOUTHEAST_FROM);
    Event_AskYesNo(14, 0);
    Event_End();
}

void SceneDialogue_ShowLine16BF(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WAS_TURNED_INTO_TREE_FOR);
    Event_AskYesNo(21, 0);
    Event_End();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_CURSE_WAS_BROKEN_THANKS_EFFORTS);
    Event_AskYesNo(16, 0);
    Event_End();
}

void SceneDialogue_ShowLine16CC(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HAVE_EVER_BEEN_VILLAGE_IMIL);
    Event_AskYesNo(18, 0);
    Event_End();
}

void FieldScene_RunEarlySequence(void)
{

    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 **)&gEventWork;
    Event_Begin();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Value1(Engine_ActorGet, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Audio_PlayCue(188);
    } else {
        Call1((void (*)())Engine_AudioPlayCue, 158);
    }
    off = v5 << 2;
    tbl = (u8 *)Mura_DoorCellOrigins;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)Mura_DoorCellSteps;
    Value3(Engine_MapAnimateCells, *(s32 *)(tbl2 + off), a1, a2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *((u8 *)Engine_ActorGet(0) + 85) = 0;
    *(s32 *)((*(u8 **)&gEventWork + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    if (v5 == 4) {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    } else {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    }
    Event_Wait(16);
    Event_RequestExit(v5 + 3);
    Event_End();
}

void FieldScene_RunScene38bSequenceC(void)
{
    struct FieldActor *rec;
    struct FieldActor *rec7;

    rec = (struct FieldActor *)Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec7 = (struct FieldActor *)Value1(Engine_ActorGet, 11);
    if ((rec7->x.fixed >> 20) == 6) {
        Event_Begin();
        Actor_SetSpritePriority(11, 1);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Event_Wait(20);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
        Actor_SetSpeed(11, 0x3333, 0x1999);
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
        rec7->motion_flags = 0;
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 111, 196);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
        Event_Wait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 121, 190);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
        Event_Wait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 132, 186);
        rec->scale_x = 0x10000;
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
        Actor_SetSpritePriority(11, 2);
        RunSceneTransitionEffect(0, 11);
        Task_Wait(10);
        Event_SetMessage(MSG_THANK_SAVED_ME_FROM_BEING);
        Event_ShowMessage(11, 0);
        Psynergy_Cancel();
        Task_Wait(10);
        GameFlag_Set(0x848);
        Event_End();
    }
}

void FieldScene_CallHelper170c(void)
{
    Leader_CheckAhead();
}

void FieldScene_RunScene38b_02000584(void)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 villager_actions;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (GameFlag_IsSet(0x845) == 0) {
    } else {
        if (GameFlag_IsSet(0x848) == 0) {
        } else {
            Event_Begin();
            Camera_SetSpeed(0x26666, 0x4ccc);
            Camera_MoveTo(0x1070000, -1, 0xad0000, 1);
            Camera_WaitForMove();
            record = Actor_Get(12);
            if (record->x.fixed > rec7->x.fixed) {
                Actor_FaceDirection(13, 0x5000, 20);
                Actor_ShowEmote(13, 0x100, 20);
                Event_SetMessage(MSG_YOURE_GUY);
                Event_ShowMessageAndWait(13, 0, 10);
                Actor_ShowEmote(12, 0x100, 0);
            } else {
                Actor_FaceDirection(12, 0x3000, 20);
                Actor_ShowEmote(12, 0x100, 20);
                Event_SetMessage(MSG_YOURE_GUY);
                Event_ShowMessageAndWait(12, 0, 10);
                Actor_ShowEmote(13, 0x100, 0);
            }
            Actor_ShowEmote(14, 0x100, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 0);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x10c, 184);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
            Actor_RunRepeatedMotion(13, 2);
            Event_ShowMessageAndWait(13, 0, 10);
            Actor_FaceDirection(13, 0, 0);
            Actor_FaceDirection(14, 0x3000, 20);
            Actor_FaceDirection(12, 0x8000, 20);
            Actor_SetAnimationAndWait(12, 3);
            Actor_SetAttachedEffect(14, 0x102);
            Event_Wait(40);
            Actor_FaceDirection(14, 0x3000, 10);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 10);
            Actor_RunRepeatedMotion(14, 1);
            Event_ShowMessageAndWait(14, 0, 10);
            Actor_SetAnimation(12, 3);
            Actor_SetAnimationAndWait(13, 3);
            Event_Wait(20);
            Event_ShowMessage(14, 0);
            Actor_SetSpeed(14, 0x9999, 0x4ccc);
            *((u8 *)Engine_ActorGet(14) + 90) &= 254;
            Actor_WalkToAndWait(14, 0x10a, 172);
            Event_Wait(1);
            *((u8 *)Engine_ActorGet(14) + 90) |= 1;
            Event_Wait(10);
            Actor_SetAnimationAndWait(14, 3);
            Event_ShowMessageAndWait(14, 0, 10);
            Message_ShowCentered(MSG_JILL_GAVE_ROBIN_SPECIAL_GIFT, 1);
            bump_step(1);
            Item_ShowFound(ITEM_HARD_NUT, 3);
            Party_GiveItem(ITEM_HARD_NUT, 0);
            Actor_SetAnimationAndWait(14, 3);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_SetSpeed(14, 0x10000, 0x8000);
            *((u8 *)Engine_ActorGet(14) + 90) &= 254;
            Actor_WalkToAndWait(14, 0x106, 156);
            Event_Wait(1);
            {
                u8 *record = Actor_Get(14);
                u8 value = record[90] | 1;

                record[90] = value;
            }
            Event_Wait(20);
            Actor_RunRepeatedMotion(12, 2);
            Event_ShowMessageAndWait(12, 0, 10);
            Actor_SetAnimation(12, 3);
            Actor_SetAnimation(13, 3);
            Actor_SetAnimationAndWait(14, 3);
            villager_actions = (s32)Mura_VillagerActions;
            Call3(Object_SetTargetAndCallback, 12, 0x10000, villager_actions);
            Call3(Object_SetTargetAndCallback, 13, 0x10000, villager_actions);
            Call3(Object_SetTargetAndCallback, 14, 0x10000, villager_actions);
            GameFlag_Set(0x849);
            Event_End();
        }
    }
}
