/* Scene tables, layouts and supplemental sequences. */
#include "BABI.H"
s32 SceneActor_CopyActor8PositionWhenAtRow10();

void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (SceneActor_MoveActorZeroToTarget(target)!= 0) {
        SceneState_ApplyTwoRectsAndRunThree();
    }
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 */
void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    GameFlag_Set(0x200);
    if (GameFlag_IsSet(0x201) != 0) {
        ((u8* (*)())Object_GetById)(14)[98] = 0;
        ((u8* (*)())Object_GetById)(14)[89] &= (u8)0xf7;
    } else {
        ((u8* (*)())Object_GetById)(14)[98] = 1;
        flags = Actor_Get(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    GameFlag_Set(0x201);
    if (GameFlag_IsSet(0x200) != 0) {
        ((u8* (*)())Object_GetById)(14)[98] = 0;
        ((u8* (*)())Object_GetById)(14)[89] &= (u8)0xf7;
    } else {
        ((u8* (*)())Object_GetById)(14)[98] = 1;
        p = Actor_Get(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void SceneState_ApplyFlag970(void)
{
    GameFlag_Set(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor_02001510 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        SceneActor_ApplyPointLeftOfActorZero();
    }
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct Actor_02000cc0 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        SceneActor_PassActorZeroOffsetPoint();
    }
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Event_Begin();
    actor = (struct FieldActor *)Value1(Object_GetById, 18);
    if ((actor->x.fixed >> 20) == 46) {
        Event_Wait(30);
        rec2 = OverlayObject_CreateAndInitialize(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        Actor_Get(18)->motion_flags = 0;
        Audio_PlayCue(185);
        for (i = 0; i < 16; i++) {
            Task_Wait(3);
            Actor_Get(18)->y.fixed -= 0x10000;
            rec7 = Value0(Engine_RandomNext);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Value0(Engine_RandomNext);
            ((void (*)())Effect_Spawn)(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Event_Wait(30);
        Actor_Get(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(18, 3);
        Engine_ObjectDispatchRelease(rec2);
        Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Audio_PlayCue(188);
        Map_CopyCellsTo(58, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Event_Wait(20);
        Audio_PlayCue(188);
        Map_CopyCellsTo(59, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        MapRender_WaitForValues();
        Event_Wait(10);
        GameFlag_Set(0x971);
    }
    Event_End();
}

void FieldScene_RunFourCallSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunSupplementalSequenceTwo();
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Actor_SetAnimation(20, 1);
    Actor_SetChildValue(20, 0);
    Actor_SetAnimation(20, 2);
    flags = ((u8* (*)())Object_GetById)(20) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x200);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Engine_EventBegin();
    actor = (struct FieldActor *)Value1(Object_GetById, 19);
    if ((actor->x.fixed >> 20) == 48) {
        if (GameFlag_IsSet(0x202) != 0) {
            Event_Wait(30);
            rec2 = OverlayObject_CreateAndInitialize(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            Actor_Get(19)->motion_flags = 0;
            Audio_PlayCue(185);
            for (i = 0; i < 16; i++) {
                Task_Wait(3);
                Actor_Get(19)->y.fixed -= 0x10000;
                rec7 = Value0(Engine_RandomNext);
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Value0(Engine_RandomNext);
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                ((void (*)())Effect_Spawn)(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
            Event_Wait(30);
            Actor_Get(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Actor_SetAnimation(19, 3);
            Engine_ObjectDispatchRelease(rec2);
            Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
            Actor_SetPosition(21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Audio_PlayCue(188);
            Map_CopyCellsTo(58, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(20);
            Audio_PlayCue(188);
            Map_CopyCellsTo(59, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            MapRender_WaitForValues();
            Event_Wait(10);
            GameFlag_Set(0x972);
        }
    }
    Event_End();
}

void FieldScene_RunFourStepSequenceA(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunSupplementalSequenceOne();
}

void FieldScene_SetActor19TableB3B8(void)
{
    Engine_ActorEnableActionCallback(19, 0x0200B3B8);
}

void SceneState_SetValue202ThenCall(void)
{
    GameFlag_Set(0x202);
    FieldScene_RunSupplementalSequenceOne();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Actor_SetAnimation(21, 1);
    Actor_SetChildValue(21, 0);
    Actor_SetAnimation(21, 2);
    flags = ((u8* (*)())Object_GetById)(21) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    if (GameFlag_IsSet(0x982) != 0 || GameFlag_IsSet(0x983) != 0) {
        Message_ShowCentered(MSG_STATUE_SPEAKS_ROBIN_SOUL_YE_2, 1);
    } else {
        Message_ShowCentered(MSG_STATUE_SPEAKS_ROBIN_SOUL_YE, 1);
    }
    Event_End();
}

void FieldScene_RunTwoStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    Event_End();
}

void FieldScene_RunFourStepSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunTwoStepSequence();
}

void SceneActor_InstallSlotNineHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *owner;

    Actor_EnableActionCallback(8, 0x0200B3B8);
    GameFlag_Set(0x203);
    owner = Actor_Get(9);
    *(s32 *)(owner + 108) = (s32)SceneActor_CopyActor8PositionWhenAtRow10;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is SceneActor_SetFlagBitByRelativeDepth. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *desc;
    s32 col;
    s32 row;

    Event_Begin();
    Actor_SetSpritePriority(9, 1);
    Actor_SetAnimation(9, 1);
    Actor_SetChildValue(9, 0);
    Actor_SetAnimation(9, 2);

    {
        u8 *flag = ((u8* (*)())Object_GetById)(9) + 35;
        *flag &= (u8)0xfd;
    }

    GameFlag_Set(0x204);

    col = ((Slot_02001a10* (*)())Object_GetById)(9)->col;
    row = ((Slot_02001a10* (*)())Object_GetById)(9)->row >> 20;
    Map_CopyCellAttributes(26, 8, 1, 1, col >> 20, row);

    desc = Actor_Get(9);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    desc = Actor_Get(8);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    ((u8* (*)())Engine_EventEnd)(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Actor_Get(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}
