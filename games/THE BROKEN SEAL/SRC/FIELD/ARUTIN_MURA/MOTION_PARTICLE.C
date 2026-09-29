#include "ARUTIN.H"
extern u8 MsgArutinDefeatedThoseMonstersDidnt[];
extern u8 MsgArutinDidSeeWaterGushingOut[];
extern u8 MsgArutinDoWantWeapons[];
extern u8 MsgArutinGirlFromXianBoughtLot[];
extern u8 MsgArutinGirlFromXianWasAsking[];
extern u8 MsgArutinHowAboutArentImpressedBy[];
extern u8 MsgArutinItsGreatCanSellArmor[];
extern u8 MsgArutinMyStoreSubmergedWantSell[];
extern u8 MsgArutinNoneMyGoodsWereDamaged[];
extern u8 MsgArutinOnesWhoDefeatedWaterBeasts[];
extern u8 MsgArutinThankGoodnessWaterHasReceded[];
extern u8 MsgArutinThereFewBeastsInMine[];
extern u8 MsgArutinThereSmallTempleWestAltin[];
extern u8 MsgArutinTrueFoundAncientRuinsIn[];
extern u8 MsgArutinTryingFindYourWayWest[];
extern u8 MsgArutinWeCantDrinkWaterMonsters[];
extern u8 MsgArutinWeGotLittleDampBut[];
extern u8 MsgArutinWillDoIfMyMerchandise[];
extern u8 MsgArutinYoullHaveFindPassageIn[];
extern u8 MsgArutinYourFirstTimeVisitAltin[];

void FieldScene_RunActorEightPromptDialogue(void)
{
    u8 *work;

    Event_Begin();
    Event_SetMessage((s32)MsgArutinYourFirstTimeVisitAltin);
    /* r1 is set before r0; the argument order is unchanged. */
    Event_OpenMessage(8, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_ShowMessage(8, 0);
    } else {
        work = (u8 *)gEventWork;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Event_AskYesNo(8, 0);
    }

    Event_End();
}

void SceneDialogue_ShowLine1918(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgArutinDidSeeWaterGushingOut);
    Event_AskYesNo(9, 0);
    Event_End();
}

/*
 * One of two branches of the opening setup: a short branch that only moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 of a
 * separately looked-up record on the way.
 */
void FieldScene_RunOpeningAuxiliarySequence(void)
{

    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    Event_Begin();
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage((s32)MsgArutinOnesWhoDefeatedWaterBeasts);
        Event_AskYesNo(14, 0); /* object 14, action 0 */
    } else {
        Actor_SetAnimation(14, 4); /* object 14, action 4 */
        Event_SetMessage((s32)MsgArutinWeCantDrinkWaterMonsters);
        Event_ShowMessageAndWait(14, 0, 10);
        ready_flag = GameFlag_IsSet(0x8ff);
        if (ready_flag == 0) {
            rec18 = (u8 *)Engine_ActorGet(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(Battle_GetWorkObject1e0() + 85) = ready_flag;
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Camera_WaitForMove();
            Event_Wait(120); /* should_wait 120 */
            record = (u8 *)Engine_ActorGet(0);
            Camera_MoveTo(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Camera_WaitForMove();
        }
        Actor_SetAnimationAndWait(14, 4);
    }
    Event_End();
}

void SceneDialogue_RunActor17Message1924(void)
{
    void Event_End(void);

    Event_Begin();
    Event_SetMessage((s32)MsgArutinTrueFoundAncientRuinsIn);
    Event_AskYesNo(17, 0);
    Event_End();
}

void SceneDialogue_RunActor9Message1932(void)
{

    Event_Begin();
    Event_SetMessage((s32)MsgArutinGirlFromXianWasAsking);
    Event_AskYesNo(9, 0);
    Event_End();
}

void SceneDialogue_RunActor10Message18d9(void)
{
    void Event_Begin(void);
    void Event_End(void);

    Event_Begin();
    Event_SetMessage((s32)MsgArutinTryingFindYourWayWest);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor14Message18e1(void)
{

    Event_Begin();
    Event_SetMessage((s32)MsgArutinDefeatedThoseMonstersDidnt);
    Event_AskYesNo(14, 0);
    Event_End();
}

void SceneDialogue_RunActor21Message194a(void)
{

    Event_Begin();
    Event_SetMessage((s32)MsgArutinThereFewBeastsInMine);
    Event_AskYesNo(21, 0);
    Event_End();
}

s32 SceneActor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = (struct Slot02000338 *)Engine_ActorGet(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorFifteenFlagBranch(void)
{
    void Event_SetMessage();
    void Shop_Open();

    if (GameFlag_IsSet(0x242) == 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgArutinDoWantWeapons);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Event_AskYesNo(15, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(19, 15);
        return;
    }

    Event_Begin();
    Event_SetMessage((s32)MsgArutinThankGoodnessWaterHasReceded);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage((s32)MsgArutinYoullHaveFindPassageIn);
    }
    Event_ShowMessage(15, 0);
    Event_End();
}

void FieldScene_RunActorTwentyFlagBranch(void)
{
    void Event_End(void);
    void Event_End(void);
    void Event_SetMessage(s32);

    if (GameFlag_IsSet(0x241) == 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgArutinMyStoreSubmergedWantSell);
        Event_ShowMessage(20, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(20, 17);
        return;
    }

    Event_Begin();
    Event_SetMessage((s32)MsgArutinItsGreatCanSellArmor);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage((s32)MsgArutinHowAboutArentImpressedBy);
    }
    Event_ShowMessage(17, 0);
    Event_End();
}

void FieldScene_RunActorTwentyOneFlagBranch(void)
{
    if (GameFlag_IsSet(0x240) == 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgArutinWillDoIfMyMerchandise);
        Event_ShowMessage(21, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(21, 16);
        return;
    }

    Event_Begin();
    Event_SetMessage((s32)MsgArutinNoneMyGoodsWereDamaged);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage((s32)MsgArutinGirlFromXianBoughtLot);
    }
    Event_ShowMessage(16, 0);
    Event_End();
}

void FieldScene_RunFacingGatedDialogue18(void)
{
    u8 *rec;

    rec = (u8 *)Engine_ActorGet(0);

    if ((u32)((*(u16 *)(rec + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Inn_Open(6, 18);
        return;
    }

    Event_Begin();

    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage((s32)MsgArutinThereSmallTempleWestAltin);
        Event_ShowMessage(18, 0);
    } else {
        Event_SetMessage((s32)MsgArutinWeGotLittleDampBut);
        Event_AskYesNo(18, 0);
    }

    Event_End();
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *work;
    u8 *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = (u8 *)gEventWork;
    Event_Begin();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1(Engine_ActorGet, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = ArutinMura_TriggerTable;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Audio_PlayCue(188);
        Map_CopyCellsTo(42, 33, a, b, 2, 2);
        c = a + 2;
        Map_CopyCellsTo(42, 35, c, b, 2, 2);
        Event_Wait(4);
        Map_CopyCellsTo(40, 33, a, b, 2, 2);
        Map_CopyCellsTo(40, 35, c, b, 2, 2);
        Event_Wait(4);
    } else {
        Audio_PlayCue(158);
        if (idx == 3) {
            Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
        }
        Map_AnimateCells(*(s32 *)(tbl + off), a, b);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x100;
    *(u8 *)((s32)Engine_ActorGet(0) + 85) = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (idx == 6) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
    } else {
        if (idx != 1) {
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
        } else {
            Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
        }
    }
    Event_Wait(10);
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void FieldScene_RunScene3a3SequenceB(void)
{
    u8 *work;

    work = (u8 *)gEventWork;
    Event_Begin();
    *(u8 *)((s32)Engine_ActorGet(0) + 85) = 0;
    Audio_PlayCue(123);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetAnimation(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Audio_PlayCue(229);
                work->active = 0;
                work->delay = 4;
                Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Audio_PlayCue(152);
        work->active = 1;
        Object_SetAnimation(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void FieldScene_RunScene3a3SequenceC(void)
{
    struct SceneMotion *motion;

    motion = (struct SceneMotion *)Value1(Engine_ActorGet, 18);
    motion->timer = 0;
    motion->delay = 0;
    *(s32 *)((u8 *)motion + 72) = 0x6666;
    motion->callback = SceneMotion_UpdateTimedActor;
    Actor_SetSpeed(18, 0x13333, 0x9999);
    Actor_MoveToAndWait(18, 28, 0x1cc);
    Actor_MoveToAndWait(18, 24, 0x1c0);
    Audio_PlayCue(229);
    Actor_Destroy(18);
    Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
    Event_Wait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(40);
    Actor_SetAnimation(18, 1);
}

void SceneState_SetFlag906ByActorNineteenX(void)
{
    struct Actor *p = (struct Actor *)Engine_ActorGet(19);

    if ((p->f08 >> 20) == 22) {
        GameFlag_Set(0x906);
    } else {
        GameFlag_Clear(0x906);
    }
}
