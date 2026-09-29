#include "YAMA.H"
extern u8 MsgFieldFlippedSwitch[];

/* A scene number the main image supplies as a link-time value. */
extern u8 Value_00000051;

void SceneState_SetByte22bTo3AndSend51(void)
{
    extern u8 Data_02000240[];

    Data_02000240[0x22b] = 3;
    Party_SetFields1ceAnd1d0((s32)&Value_00000051, 99);
    BattleFx_SetWeightedResult(53, 2);
}

void SceneActor_UpdateSlot10ByTileX(void)
{
    s32 *a = Actor_Get(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Map_CopyCellAttributes(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Map_CopyCellAttributes(0, 0, 1, 1, t, y);
        } else {
            Map_CopyCellAttributes(0, 0, 1, 1, x, y);
        }
        Actor_SetSpriteFlags(a, 0);
        ((u8 *)a)[0x55] = 0;
        Map_Redraw();
        Task_Wait(1);
    }
}

/*
 * Presentation setup in resource_3a4: clear a record byte, adjust two
 * handle flag bits, run two presentation primitives, then stamp a fixed
 * rate into the record.
 */

/* Declared without prototypes -- call sites vary in argument shape. */

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */
void ActorPresentation_SetCellAndLowerActorEight(void)
{
    extern u32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(8);
    s0 = 9;
    s1 = 13;
    Map_CopyCellAttributes(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndSetActor9Byte55(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 17;
    s1 = 13;
    Map_CopyCellAttributes(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Actor_Get(9);
    s0 = 26;
    Map_CopyCellAttributes(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 25;
    s1 = 13;
    Map_CopyCellAttributes(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndLowerActor9(void)
{
    extern s32 Data_03001e40;

    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Actor_Get(9);
    s0 = 43;
    s1 = 41;
    Map_CopyCellAttributes(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Actor_SetSpriteFlags(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(11);
    s0 = 17;
    s1 = 10;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_AdjustSlot12AndSetFlag204(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(12);
    s0 = 26;
    s1 = 15;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x204);
}

void SceneState_SetDispcntBit9ByThreshold(void)
{
    extern u16 ArutinYama_RiseTimer;

    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Random_Next() * 100) >> 16 >= ArutinYama_RiseTimer) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)&gMapWork;
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
        if (i == 8) {
            record = Value1(Engine_ActorGet, 8);
            *(s32 *)(record + 24) = 0x1999;
            record = Actor_Get(8);
            *(s32 *)(record + 28) = 0x1999;
            Actor_SetPosition(8, 0x980000, 0xd80000);
            Actor_EnableActionCallback(8, ArutinYama_EarlyActorScript);
        }
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    GameFlag_Set(0x8fd);
}

void FieldScene_RunScene3a4SequenceH(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)&gMapWork;
    Map_CopyCells(93, 41, 16, 4, 77, 28);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCells(77, 41, 16, 4, 77, 28);
    GameFlag_Set(0x8fe);
}

void FieldScene_RunScene3a4SequenceI(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)&gMapWork;
    Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
    Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
    Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
    GameFlag_Set(0x907);
}

void FieldScene_RunScene3a4SequenceB(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
        GameFlag_Clear(0x323);
    } else {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
        GameFlag_Set(0x323);
    }
}

void FieldScene_RunValue1528Scene(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceB();
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceA(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
        GameFlag_Clear(0x325);
    } else {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
        GameFlag_Set(0x325);
    }
}

void FieldScene_RunLine1528Sequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceA();
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceC(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
        GameFlag_Clear(0x326);
    } else {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
        GameFlag_Set(0x326);
    }
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceD(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x327) != 0) {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
        GameFlag_Clear(0x327);
    } else {
        Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
        GameFlag_Set(0x327);
    }
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void SceneActor_SetActor10Byte23To3(void)
{
    extern u32 Data_03001e40;

    ((u8 *)Engine_ActorGet(10))[0x23] = 3;
}

void SceneActor_SetActor10Byte23To1(void)
{
    extern u32 Data_03001e40;

    ((u8 *)Engine_ActorGet(10))[0x23] = 1;
}

void FieldScene_RunScene3a4_02000c9c(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    Audio_PlayCue(185);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    *(u8 *)((u8 *)Engine_ActorGet(10) + 90) &= 254;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x190, 0x1a8);
    Actor_SetDestination(10, 0x198, 0x1a8);
    Actor_WaitForMove(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    SceneActor_UpdateSlot10ByTileX();
    Event_End();
}
