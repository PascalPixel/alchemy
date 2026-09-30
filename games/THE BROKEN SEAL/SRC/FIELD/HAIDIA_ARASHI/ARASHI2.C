#include "GROUP_DEPARTURE.H"
#include "CALL.H"
#include "TYPES.H"

extern u8 MsgHaidiaNoBrother[];
void HaidiaArashi_RunRiverSearch(void);

extern u8 MsgHaidiaDontSupposeTwo[];
extern u8 MsgHaidiaGoLookNorth[];
extern u8 HaidiaArashi_ActorNineScriptA[];
extern u8 HaidiaArashi_ActorNineScriptB[];
extern u8 HaidiaArashi_ActorNineScriptC[];
extern u8 HaidiaArashi_ActorNineScriptD[];
extern u8 HaidiaArashi_ActorTwentySixScriptA[];
extern u8 HaidiaArashi_ActorTwentySixScriptB[];
extern u8 HaidiaArashi_ActorTwentySixScriptC[];
extern u8 HaidiaArashi_CellSteps4[];
extern u8 HaidiaArashi_CellSteps5[];

extern u8 MsgHaidiaOh[];
extern u8 MsgHaidiaTwoDontEnough[];

extern u8 MsgHaidiaMomDadBack[];
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_ActorEnableActionCallback();

/* Actor 9's action table for the run through the storm. */
extern u8 HaidiaArashi_StormRunActions[];
void Engine_EventWait();
void Engine_CameraWaitForMove();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorFaceEachOther();
void Engine_ActorSetAnimation();
void Scene_RunActorGroupDepartureSequence();
void Engine_EventEnd();

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

/* The storm night at the river, until flag 0x83a is set: the villagers are
 * placed along the bank, actor 26 cries out for her brother, the leader and
 * actor 22 come down to look, actor 23 is swept away, and the river search
 * follows. */
void FieldScene_RunFlagGatedActorSequence(void)
{
    u8 *tbl;

    if (Value1(Engine_GameFlagIsSet, 0x83a) != 0) {
        return;
    }
    Engine_EventBegin();
    Call3(Engine_ActorSetPosition, 10, 0xC00000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 10, 0x2000, 0);
    Engine_ActorSetAnimation(10, 5);
    {
        u8 *o;
        s32 v;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(10);
        v = IwramUnsignedRemainder(Engine_RandomNext(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(10, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 9, 0xC00000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Call3(Engine_ActorSetPosition, 24, 0xE30000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 24, 0x4000, 0);
    Engine_ActorSetAnimation(24, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(24);
        v = IwramUnsignedRemainder(Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(24, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 25, 0xFA0000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 25, 0x4000, 0);
    Engine_ActorSetAnimation(25, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(25);
        v = IwramUnsignedRemainder(Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Engine_ActorEnableActionCallback(25, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 26, 0xE30000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 26, 0x3000, 0);
    Call3(Engine_ActorSetPosition, 23, 0xF30000, 0x4FD0000);
    Call3(Engine_ActorFaceDirection, 23, 0xC000, 0);
    Engine_ActorSetSpriteFlags(((s32 (*)())Engine_ActorGet)(23), 0);
    Engine_TaskWait(3);
    Engine_EventSetMessage((s32)MsgHaidiaNoBrother);
    Engine_EventShowMessage(0x201a, 0);
    Call3(Engine_ActorShowEmote, ACTOR_PARTY_LEADER, 0x100, 20);
    Call3(Engine_ActorWalkToAndWait, ACTOR_PARTY_LEADER, 150, 0x446);
    {
        u8 *p;
        p = (u8 *)((s32 (*)())Engine_ActorGet)(ACTOR_PARTY_LEADER);
        if (p != 0) {
            Engine_ActorSetPosition(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Call3(Engine_ActorWalkToAndWait, 22, 132, 0x446);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, ACTOR_PARTY_LEADER, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 22, 0x4000, 20);
    Call2(Engine_CameraSetSpeed, 0x40000, 0x8000);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorRunRepeatedMotion(23, 3);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 9, 0x3000, 10);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_AudioPlayCue(134);
    Engine_ActorJump(23, 4, 0);
    Engine_ActorSetAnimation(23, 6);
    Engine_EventWait(10);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_ActorSetAnimation(10, 1);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(10);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorSetAnimation(24, 1);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(24);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorSetAnimation(25, 1);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(25);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorStartRepeatedMotion(24, 2);
    Engine_ActorStartRepeatedMotion(25, 2);
    Engine_ActorRunRepeatedMotion(26, 2);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Call2(Engine_ActorSetAttachedEffect, 26, 0x102);
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(26, 2);
    Engine_ActorStartRepeatedMotion(26, 3);
    Engine_EventShowMessage(26, 0);
    Engine_ActorJump(25, 2, 0);
    Call3(Engine_ActorSetDestination, 25, 234, 0x4B5);
    Engine_ActorJump(26, 2, 0);
    Call3(Engine_ActorSetDestination, 26, 227, 0x4B1);
    Engine_EventWait(90);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetPosition(23, 0xF30000, 0x4FD0000);
    Engine_TaskWait(1);
    Engine_AudioPlayCue(106);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(23);
        *(s32 *)(o + 0x28) = 0x20000;
    }
    Engine_EventWait(6);
    Engine_ActorSetAnimation(23, 7);
    Engine_EventWait(20);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Engine_CameraMoveTo(0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 24, 0x105, 40);
    Engine_ActorFaceEachOther(24, 10, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Call2(Engine_EventShowMessage, 0x800A, 0);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(25);
        o[0x5A] &= 0xFE;
    }
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(26);
        o[0x5A] &= 0xFE;
    }
    Call3(Engine_ActorSetSpeed, 25, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetSpeed, 26, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetDestination, 25, 247, 0x4BA);
    Call3(Engine_ActorMoveToAndWait, 26, 227, 0x4A5);
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(25);
        o[0x5A] |= 1;
    }
    {
        u8 *o;
        o = (u8 *)((s32 (*)())Engine_ActorGet)(26);
        o += 0x5A;
        /* FAKEMATCH: the set bit first, so the or writes into the register
           that holds it, as the ROM's does. */
        {
            u8 set = 1 | *o;
            *o = set;
        }
    }
    Engine_ActorFaceDirection(26, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Engine_ActorSetAnimation(24, 4);
    Engine_EventShowMessageAndWait(0x8018, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 20);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_ActorSetAnimation(10, 4);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 10, 0x105, 60);
    Call3(Engine_ActorShowEmote, 9, 0x106, 20);
    Call3(Engine_ActorFaceDirection, 9, 0x8000, 40);
    Call3(Engine_ActorFaceDirection, 9, 0xC000, 20);
    Engine_ActorFaceDirection(9, 0, 30);
    Call3(Engine_ActorFaceDirection, 9, 0x4000, 10);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x9000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Engine_ActorSetAnimation(9, 4);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorShowEmote, 10, 0x105, 0);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 25, 0x105, 0);
    Call3(Engine_ActorShowEmote, 26, 0x105, 40);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_ActorFaceEachOther(24, 25, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_ActorFaceDirection(10, 0, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_ActorFaceEachOther(10, 9, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventShowMessageAndWait(0x800A, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Call3(Engine_ActorFaceDirection, 24, 0xD000, 10);
    Engine_ActorStartRepeatedMotion(24, 1);
    Engine_EventShowMessageAndWait(24, 0, 10);
    Engine_ActorFaceDirection(10, 0, 0);
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_ActorRunRepeatedMotion(26, 1);
    Call3(Engine_ActorFaceDirection, 26, 0x2000, 20);
    Call3(Engine_ActorFaceDirection, 25, 0xA000, 20);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventShowMessageAndWait(25, 0, 10);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventShowMessageAndWait(9, 0, 10);
    HaidiaArashi_RunRiverSearch();
    Engine_GameFlagSet(0x83A);
    Engine_EventEnd();
}

/* The storm night by the river: while the cells shake, actors 9 and 26
 * run their scripts, actors 10, 24 and 25 each get a random count of 60
 * to 149 and actor 8's script, actor 9 says she will look north and
 * sends the others to the plaza, and asks the party twice for help before
 * actor 22 follows the leader out. */
void HaidiaArashi_RunRiverSearch(void)
{
    s32 entry;
    s32 record;
    s32 script;
    s32 north;
    s32 suppose;

    Actor_FaceDirection(26, 0x3000, 0);
    Actor_FaceDirection(24, 0xd000, 0);
    Actor_FaceDirection(25, 0xb000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(10, 0xd000, 20);
    Actor_SetAnimation(26, 3);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimation(25, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(25, 3);
    Event_Wait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x860000, -1, 0x4ab0000, 1);
    Actor_SetSpeed(26, 0x19999, 0xcccc);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptA);
    Object_SetActionCallbackAndRefreshById(9, (s32)HaidiaArashi_ActorNineScriptA);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps2, 38, 72);
    Event_Wait(10);
    Actor_WalkToAndWait(9, 149, 0x497);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(25, 250, 0x4be);
    BattleFx_PlayQueuedSound();
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(24, 0x3000, 0);
    Actor_FaceDirection(25, 0x3000, 0);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(24, 6);
    Actor_SetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = (s32)Actor_Get(10);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    entry = (s32)Actor_Get(24);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    entry = (s32)Actor_Get(25);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    script = (s32)HaidiaArashi_ActorEightScript;
    Actor_EnableActionCallback(10, (const u8 *)script);
    Actor_EnableActionCallback(24, (const u8 *)script);
    Actor_EnableActionCallback(25, (const u8 *)script);
    Object_RefreshSelectorById(26);
    Event_Wait(10);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps5, 38, 72);
    Event_Wait(30);
    BattleFx_PlayQueuedSound();
    Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps1, 35, 73);
    Event_Wait(20);
    BattleFx_PlayQueuedSound();
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptB);
    Event_Wait(20);
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptB);
    Event_Wait(40);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps4, 35, 73);
    Object_RefreshSelectorById(26);
    BattleFx_PlayQueuedSound();
    Event_Wait(40);
    north = (s32)MsgHaidiaGoLookNorth;
    Event_SetMessage(north);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessageAndWait(0x201a, 0, 40);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(30);
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptC);
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptC);
    Event_Wait(40);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
    Object_RefreshSelectorById(9);
    Actor_FaceDirection(9, 0, 0);
    Actor_ShowEmote(9, 0x100, 40);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(22, 0x8000, 10);
    Actor_WalkToAndWait(9, 105, 0x43e);
    Actor_RunRepeatedMotion(9, 2);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the 0xe9b
     * table to the corresponding follow-up call. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((north + 4));
    } else {
        Actor_RunRepeatedMotion(9, 2);
        Event_SetMessage((north + 5));
    }
    Event_ShowMessage(0x8009, 0);
    Actor_FaceDirection(22, 0x8000, 40);
    Actor_ShowEmote(9, 0x100, 30);
    suppose = (s32)MsgHaidiaDontSupposeTwo;
    Event_SetMessage(suppose);
    Event_OpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * 0xea1 table and runs its own follow-up sequence. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((suppose + 1));
        Event_ShowMessageAndWait(0x8009, 0, 30);
        Actor_FaceDirection(22, 0x8000, 20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(22, 3);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(40);
    } else {
        Actor_ShowEmote(9, 0x105, 90);
        Actor_ShowEmote(9, 0x103, 40);
        Actor_SetAnimation(9, 4);
        Event_SetMessage((suppose + 2));
        Event_ShowMessage(0x8009, 0);
    }
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptD);
    Event_Wait(90);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_SetAnimation(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = (s32)Actor_Get(0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    u8 *tbl;

    Event_Begin();
    Actor_SetAnimation(10, 1);
    Event_Wait(10);
    Actor_FaceEachOther(10, ACTOR_PARTY_LEADER, 20);
    if (GameFlag_IsSet(0x30d) != 0) {
        Event_SetMessage((s32)MsgHaidiaTwoDontEnough);
        Event_ShowMessageAndWait(10, 0, 10);
    } else {
        Event_SetMessage((s32)MsgHaidiaOh);
        Actor_StartRepeatedMotion(10, 1);
        Event_ShowMessageAndWait(10, 0, 10);
        Actor_StartRepeatedMotion(10, 2);
        Event_ShowMessageAndWait(10, 0, 10);
    }
    Actor_FaceDirection(10, v2000, 20);
    Actor_SetAnimation(10, 5);
    Event_Wait(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_Get(10);
        v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(rec + 0x64) = v;
        Actor_EnableActionCallback(10, tbl);
    }
    Event_Wait(20);
    GameFlag_Set(0x30d);
    Event_End();
}

void HaidiaArashi_RunScene02DEC(void)
{
    u32 i;
    u8 *rec;
    u8 *record;
    s32 v5;
    u8 *p4;

    if (Engine_GameFlagIsSet(0x840) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x841) != 0) {
        } else {
            Engine_EventBegin();
            Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 22, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 26, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
            Call3(Engine_ActorWalkToAndWait, 0, 217, 0x557);
            record = (s32)Engine_ActorGet(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(22, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 22, 235, 0x557);
            Call3(Engine_ActorFaceDirection, 22, 0xb000, 0);
            record = (s32)Engine_ActorGet(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(26, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 26, 199, 0x557);
            Call3(Engine_ActorFaceDirection, 26, 0xd000, 0);
            Call3(Engine_ActorSetPosition, 25, 0xf70000, 0x4ba0000);
            Engine_ActorFaceDirection(25, 0x6000, 0);
            record = (s32)Engine_ActorGet(8);
            p4 = *(s32 *)((s32)record + 80);
            ((struct Flags35 *)record)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 1;
            rec = (s32)Engine_ActorGet(0);
            p4 = *(s32 *)((s32)rec + 80);
            ((struct Flags35 *)rec)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 2;
            record = (s32)Engine_ActorGet(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(8, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 8, 221, 0x569);
            Call3(Engine_ActorFaceDirection, 8, 0xb000, 60);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventSetMessage((s32)MsgHaidiaMomDadBack);
            Engine_EventShowMessageAndWait(26, 0, 40);
            Call3(Engine_ActorSetPosition, 9, 0x650000, 0x4ad0000);
            Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
            Engine_EventShowMessageAndWait(0x1009, 0, 10);
            Call3(Engine_ActorFaceDirection, 26, 0xa000, 0);
            Engine_CameraSetSpeed(0x13333, 0x2666);
            Call4(Engine_CameraMoveTo, 0x650000, -1, 0x4ad0000, 1);
            Call3(Engine_ActorSetSpeed, 9, 0x16666, 0xb333);
            Engine_ActorEnableActionCallback(9, (s32)HaidiaArashi_StormRunActions);
            Engine_EventWait(60);
            Engine_CameraSetSpeed(0x9999, 0x1333);
            Call4(Engine_CameraMoveTo, 0xbb0000, -1, 0x5300000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(40);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 20);
            Engine_ActorRunRepeatedMotion(9, 2);
            Engine_EventShowMessageAndWait(0x4009, 0, 20);
            Engine_CameraSetSpeed(0x20000, 0x4000);
            Call4(Engine_CameraMoveTo, 0xdd0000, -1, 0x5690000, 1);
            Engine_ActorFaceActor(0, 8, 0);
            Engine_ActorFaceActor(22, 8, 0);
            Call3(Engine_ActorFaceDirection, 26, 0x3000, 80);
            Engine_CameraMoveTo(0xb60000, -1, 0x5500000, 1);
            Call3(Engine_ActorWalkToAndWait, 8, 182, 0x568);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_EventWait(30);
            Engine_ActorSetAnimationAndWait(8, 3);
            Engine_EventWait(10);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 10);
            Call3(Engine_ActorFaceDirection, 9, 0xe000, 40);
            Call3(Engine_ActorFaceDirection, 9, 0x3000, 20);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorFaceEachOther(26, 8, 0);
            Engine_ActorFaceEachOther(22, 0, 0);
            Engine_EventWait(40);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_ActorRunRepeatedMotion(9, 2);
            Engine_EventWait(20);
            Engine_EventShowMessageAndWait(9, 0, 10);
            Engine_ActorSetAnimation(0, 3);
            Engine_ActorSetAnimation(26, 3);
            Engine_ActorSetAnimation(22, 3);
            v5 = 1;
            Engine_ActorSetAnimationAndWait(8, 3);
            rec[35] |= v5;
            record = (s32)Engine_ActorGet(8);
            record[35] |= v5;
            Scene_RunActorGroupDepartureSequence();
            Engine_GameFlagSet(0x841);
            Engine_EventEnd();
        }
    }
}
