#include "GROUP_DEPARTURE.H"

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

s32 OverlayObject_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Random_Next();
        counter = IwramUnsignedRemainder(Random_Next(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state)
{
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += -0x1000;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16)(IwramUnsignedRemainder(Random_Next(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}

u8 *SceneData_GetTableD0E4(void)
{
    return HaidiaArashi_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return HaidiaArashi_SceneTable1;
}

u8 *SceneData_GetTableD2B8(void)
{
    return HaidiaArashi_SceneTable2;
}

u8 *SceneData_GetTableD558(void)
{
    return HaidiaArashi_SceneTable3;
}

void SceneState_SetFlag210AndConfigureRegion40_84(void)
{
    s32 a;
    s32 b;

    GameFlag_Set(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 84, 7, 4, a, b);
}

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{

    s32 a;
    s32 b;

    GameFlag_Clear(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{

    u8 *state;

    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Event_RequestExit(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(HaidiaArashi_CellSteps0, 45, 11);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1A4);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(11);
}

void SceneState_SetValue123Mode1(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(1);
}

void SceneState_ApplyValues123And3(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(3);
}

void SceneState_SetValue123Mode4(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(4);
}

void FieldScene_RunStep7BAndCheckFlags841And842(void)
{
    Audio_PlayCue(0x7B);
    if (GameFlag_IsSet(0x841) != 0
        && GameFlag_IsSet(0x842) == 0) {
        FieldScene_ConfigureActorTwentyTwoScene();
    }
    SceneState_SetWork1c0AndRunObject(2);
}

void FieldScene_SetupDescriptorD78a(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 54, 32);
    Engine_ActorWalkTo(0, 0x196, 0x2d7);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(5);
}

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x206) == 0) {
        Audio_PlayCue(158);
        Map_AnimateCells(HaidiaArashi_CellSteps2, 45, 39);
    }
    if (GameFlag_IsSet(0x835) == 0) {
        record = GameFlag_IsSet(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        FieldScene_RunScene372SequenceA();
        GameFlag_Set(0x206);
    } else {
        L_020002b4:;
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
        Event_Wait(3);
        SceneState_SetWork1c0AndRunObject(6);
    }
}

void FieldScene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (GameFlag_IsSet(0x205) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(HaidiaArashi_CellSteps1, 50, 44);
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 49, 69);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x146, 0x466);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps3, 52, 76);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 74);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 73);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(HaidiaArashi_CellSteps2, 38, 72);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 0x49e);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(13);
}

s32 FieldScene_RunFlagGatedActorSetup(void)
{
    s32 m;
    s32 t;
    s32 m2;
    s32 h;
    s32 k;
    s32 w1 = 0x14E0000;
    s32 w2 = 0x3A40000;
    s32 w3 = 0xE00000;
    s32 w4 = 0x3680000;
    s32 w5 = 0x400000;
    s32 w6 = 0x1B00000;
    s32 w7 = 0x720000;
    s32 w8 = 0xC00000;
    s32 w9 = 0x2000;
    s32 w10 = 0xE30000;
    s32 w11 = 0x4000;
    s32 w12 = 0xF70000;
    s32 w13 = 0x4000;
    s32 w14 = 0xF30000;
    s32 w15 = 0x1900000;
    s32 w16 = 0x1A80000;
    s32 w17 = 0x190;
    s32 w18 = 0x1A8;
    s32 w19 = 0x1A80000;
    s32 w20 = 0x1A8;
    s32 b1 = 0x4BE0000;
    s32 b2 = 0x4BE0000;
    s32 b3 = 0x4BE0000;
    s32 w21 = 0xA50000;
    s32 w22 = 0xA50000;
    s32 p1 = 0x2BF0000;
    s32 p2 = 0x47B0000;
    s32 p3 = 0x14D0000;
    s32 p4 = 0x4FD0000;
    s32 p5 = 0x2630000;
    s32 p6 = 0x2730000;
    s32 p7 = 0x2730000;
    s32 c1 = 0x26B;
    s32 c2 = 0x101;
    s32 c3 = 0x26B;

    BattleFx_SetQueuedSoundAndPlay(0xAA);
    Actor_SetPosition(23, 0, 0);
    if (GameFlag_IsSet(0x109) != 0) {
        GameFlag_Clear(0x205);
        GameFlag_Clear(0x206);
    }
    if (GameFlag_IsSet(0x830) != 0) {
        Actor_SetPosition(11, w1, w2);
        HaidiaArashi_SetStormCellAttributes();
    }
    if (GameFlag_IsSet(0x831) != 0) {
        Actor_SetPosition(12, w3, w4);
        ActorPresentation_SetEightSceneCells();
    }
    if (GameFlag_IsSet(0x832) != 0) {
        Actor_SetPosition(13, w5, p1);
        SceneState_ApplyFourRects();
    }
    if (GameFlag_IsSet(0x833) != 0) {
        Actor_SetPosition(14, w6, p2);
        FieldScene_DrawFiveTileBlocks();
    }
    {
        u8 *q;
        q = Actor_Get(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Actor_Get(12);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(13);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(14);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(15);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(16);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(17);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (GameFlag_IsSet(0x837) != 0) {
        Actor_SetPosition(22, 0, 0);
    }
    {
        u8 *q;
        q = Actor_Get(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (GameFlag_IsSet(FLAG_BOULDER_FELL) != 0) {
        Actor_SetPosition(19, w7, p3);
    } else {
        Actor_EnableActionCallback(19, HaidiaArashi_ActorNineteenScript);
    }
    if (GameFlag_IsSet(0x841) != 0) {
        s32 h2;
        u8 *tbl;
        FieldScene_BuildPlacementGrid();
        Actor_SetPosition(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = HaidiaArashi_ActorEightScript;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Actor_EnableActionCallback(9, tbl);
        }
        Actor_SetPosition(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(26);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Actor_EnableActionCallback(26, tbl);
        }
        Actor_SetPosition(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(22);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Actor_EnableActionCallback(22, tbl);
        }
        Actor_SetPosition(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(8);
            *(u16 *)(o + 6) = h2;
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Actor_EnableActionCallback(8, tbl);
        }
        Actor_SetAnimation(8, 6);
        {
            u8 *r;
            r = Actor_Get(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Actor_Get(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Scheduler_AddOrUpdateCallback(OverlayObject_CopyRecordField1ToSlots22And8, 0xC80);
        Actor_SetPosition(24, 0, 0);
        Actor_SetPosition(25, 0, 0);
        Actor_SetPosition(23, 0, 0);
        Actor_SetPosition(19, 0, 0);
        if (GameFlag_IsSet(0x842) != 0) {
            Actor_SetPosition(22, 0, 0);
        }
    } else if (GameFlag_IsSet(0x83a) != 0) {
        u8 *tbl;
        Actor_SetPosition(10, w8, b1);
        Actor_FaceDirection(10, w9, 0);
        Actor_SetAnimation(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(10);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = HaidiaArashi_ActorEightScript;
        Actor_EnableActionCallback(10, tbl);
        Actor_SetPosition(24, w10, b2);
        Actor_FaceDirection(24, w11, 0);
        Actor_SetAnimation(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(24);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(24, tbl);
        Actor_SetPosition(25, w12, b3);
        Actor_FaceDirection(25, w13, 0);
        Actor_SetAnimation(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(25);
            v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(25, tbl);
        Actor_SetPosition(23, w14, p4);
        Actor_FaceDirection(23, 0xC000, 0);
        Actor_SetSpriteFlags(Actor_Get(23), 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    } else {
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    }
    {
        s16 *table = (s16 *)&gGameState;
        if (table[225] != 15 || GameFlag_IsSet(0x87b) != 0) {
            Effect_SoundAndFlash();
            BattleFx_StartTwelveFrameBlend();
        }
    }
    if (GameFlag_IsSet(0x210) != 0) {
        SceneState_SetFlag210AndConfigureRegion40_84();
    }
    GameFlag_Set(0x834);
    k = 46;
    Map_CopyCellAttributes(29, 24, 1, 2, 26, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 27, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 28, k);
    k = 20;
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x58);
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Actor_Get(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Actor_SetSpriteFlags(o, 0);
    }
    Task_Wait(1);
    if (GameFlag_IsSet(0x87b) == 0) {
        s16 *table = (s16 *)&gGameState;
        if (table[225] == 15) {
            Scene_DoraSendsRobinToThePlaza();
            return 0;
        }
    }
    Actor_SetAnimation(23, 7);
    if (GameFlag_IsSet(0x837) == 0) {
        Event_Begin();
        Actor_SetAttachedEffect(22, c2);
        Actor_SetPosition(22, w15, p5);
        Actor_SetPosition(21, w16, p6);
        Actor_WalkTo(22, w17, c1);
        Actor_WalkToAndWait(21, w18, 0x26B);
        Actor_SetAnimation(21, 2);
        Actor_SetAnimation(22, 5);
        Event_End();
    } else {
        Event_Begin();
        Actor_SetPosition(21, w19, p7);
        Actor_WalkToAndWait(21, w20, c3);
        Actor_SetAnimation(21, 3);
        Event_End();
    }
    {
        u8 *state = (u8 *)gEventWork;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Event_OpenScreen();
    Event_WaitForScreen();
    BattleFx_SetBlock30Values128One();
    return 0;
}
