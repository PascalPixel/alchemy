#include "PROBE.H"
#include "TYPES.H"
#include "MAKYURI.H"
#include "CALL.H"
#include "MAKYURI_HEYA.H"
#include "FIELD_SCENE.H"

extern const u8 MakyuriHeya_PushScriptA[];
extern const u8 MakyuriHeya_PushScriptB[];
extern const u8 MakyuriHeya_PushScriptC[];
extern const u8 MakyuriHeya_PushScriptD[];

struct ColumnProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct ColumnProbe *probe);
void SceneActor_MoveAndRedraw(struct ColumnProbe probe);
s32 MakyuriHeya_StartPillarPush(void);
void SceneEffect_SpawnParticleRowsByMode(s32 mode);
void FieldScene_RunPrimarySequence(s32 mode);
s32 SceneData_ApplyTableA2c5AndReturnZero(void);
void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth);
void Engine_ActorStartAction(s32 actor);

void Engine_EventBegin();
void Engine_GameFlagClear();
void Engine_EventEnd();

extern u8 MsgMakyuriDidThat[];
void SetEffectRecordMode();
void MakyuriHeya_FadePaletteToWhite();
void MakyuriHeya_CastPsynergyAtActor11(void);
void SceneEffect_RotatePaletteEntries40To47(void);
extern const u16 MakyuriHeya_ColumnCells[];

void Engine_ActorSetChildValue(s32 actor, s32 value);
void Audio_PlayCue(s32 cue);
void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
extern const u8 MakyuriHeya_SparkBurstScript[];
void WaitFrames(s32 frames);

struct EffectParams {
    s32 count;
    s32 kind;
    u8 pad08[16];
    u16 tile;
    u16 pad1a;
    s32 script;
    u8 pad20[8];
};

void MakyuriHeya_FadePaletteToBlack();
void SceneEffect_RotatePaletteEntries97To103(void);
void BattleFx_PlayQueuedSound();
void MakyuriHeya_SinkActorWithSparks();

void BattleFx_RunRisingObjectSequence();

void SceneEffect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Map_CopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Map_CopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Map_CopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Audio_PlayCue(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Effect_Spawn((0x319 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Effect_Spawn((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Engine_RandomNext)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Effect_Spawn(0x3380000 - (i << 17) - (j << 19), 0,
                                  ((((u32)Engine_RandomNext() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Event_Wait(1);
            }
        }
        if (mode == 0)
            Map_CopyCellsTo(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Map_CopyCellsTo(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Map_CopyCellsTo(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}

/* Mercury Lighthouse pillar push: pan the camera over the room and, by the pillar's cell and the story flags, start its push script; returns 1 when the last push begins. */
s32 MakyuriHeya_StartPillarPush(void)
{
    s32 cell_x;
    s32 cell_z;

    cell_x = Object_GetById(9)->x.fixed / 0x100000;
    cell_z = Object_GetById(9)->z.fixed / 0x100000;
    Engine_CameraSetSpeed(0x50000, 0xa000);
    Engine_CameraMoveTo(0x3300000, -1, 0x2c80000, 1);
    Engine_CameraWaitForMove();
    if (!Engine_GameFlagIsSet(0x877)) {
        if (cell_x == 50 && Engine_GameFlagIsSet(0x319)) {
            Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptA);
        } else if (cell_x == 49) {
            if (cell_z == 44 && !Engine_GameFlagIsSet(0x319) && !Engine_GameFlagIsSet(0x31a) && !Engine_GameFlagIsSet(0x31b)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptB);
            } else if (cell_z == 44 && Engine_GameFlagIsSet(0x319)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptC);
            } else if (cell_z == 46 && Engine_GameFlagIsSet(0x31a)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptD);
                Battle_WaitMode0(30);
                return 1;
            }
        }
    }
    Battle_WaitMode0(30);
    return 0;
}

void SceneActor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Actor_Get(9);
    u32 v = Random_Next();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    SceneEffect_SpawnRandomEveryFourFrames(p[2], b, c);
}

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Engine_TaskAddCallback((s32)SceneActor_UseActorNinePositionWithYOffset, n);
    return 0;
}

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Engine_TaskRemoveCallback((s32)SceneActor_UseActorNinePositionWithYOffset);
    return 0;
}

/* Mercury Lighthouse: after actor 10 is pushed, set flag 0x318 when it stops
 * in row 38. After actor 11 is pushed, the column it stops in selects which
 * of flags 0x319..0x31b is set; column 48, once every column is lit, plays
 * the beam and leaves through exit 15. */
void MakyuriHeya_RunColumnProbeScene(void)
{
    struct ColumnProbe probe;
    s32 column;
    s32 start;

    Event_Begin();
    if (StagedActor_FindClearPosition(&probe) == 0)
        goto end;
    switch (probe.word[1]) {
    case 10:
        SceneActor_MoveAndRedraw(probe);
        if (probe.word[4] >> 20 == 38)
            GameFlag_Set(0x318);
        else
            GameFlag_Clear(0x318);
        break;
    case 11:
        start = ((s32 *)Actor_Get(11))[2] >> 20;
        SceneActor_MoveAndRedraw(probe);
        column = probe.word[2] >> 20;
        if (column == 47) {
            GameFlag_Set(0x319);
            GameFlag_Clear(0x31a);
            GameFlag_Clear(0x31b);
            MakyuriHeya_StartPillarPush();
            if (start == 54)
                SceneEffect_SpawnParticleRowsByMode(0);
            else if (start == 48)
                SceneEffect_SpawnParticleRowsByMode(1);
            FieldScene_RunPrimarySequence(2);
            goto wait;
        } else if (column == 48) {
            GameFlag_Set(0x31a);
            GameFlag_Clear(0x31b);
            GameFlag_Clear(0x319);
            if (MakyuriHeya_StartPillarPush() != 0) {
                s32 x;

                SceneEffect_SpawnParticleRowsByMode(2);
                x = 210 << 18;
                SceneData_ApplyTableA2c5AndReturnZero();
                FieldScene_RunPrimarySequence(1);
                Engine_ActorStartAction(9);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3120000, 223);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3320000, 223);
                Actor_SetDestination(9, 0x348, 0x2e8);
                Event_Wait(5);
                Audio_PlayCue(189);
                Actor_WaitForMove(9);
                Event_Wait(40);
                GameFlag_Set(0x877);
                ColorBuffer_ApplySource(0x10000, 0);
                *(s32 *)&(*(struct GameState **)&gEventWork)->scene = 0x100;
                Event_CloseScreen();
                Event_WaitForScreen();
                Event_RequestExit(15);
                goto end;
            }
            SceneEffect_SpawnParticleRowsByMode(2);
            SceneData_ApplyTableA2c5AndReturnZero();
            FieldScene_RunPrimarySequence(1);
            goto wait;
        } else if (column == 53) {
            GameFlag_Set(0x31b);
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            MakyuriHeya_StartPillarPush();
            SceneEffect_SpawnParticleRowsByMode(0);
        wait:
            Event_Wait(60);
        } else {
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            GameFlag_Clear(0x31b);
            MakyuriHeya_StartPillarPush();
            if (start == 47)
                SceneEffect_SpawnParticleRowsByMode(2);
            else if (start == 48)
                SceneEffect_SpawnParticleRowsByMode(1);
            FieldScene_RunPrimarySequence(0);
            Event_Wait(60);
        }
        break;
    }
end:
    Event_End();
}

void FieldScene_RunFourStepSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    MakyuriHeya_SyncBlockFlags();
    Event_End();
}

void MakyuriHeya_SyncBlockFlags(void)
{
    s32 i;
    s32 flag;
    s32 x;

    Engine_EventBegin();
    for (i = 0, flag = 0x330; i <= 3; i++, flag += 2) {
        x = *(s32 *)((s32)Object_GetById(i + 15) + 8) / 0x100000;
        if (x == (i << 2) + 39) {
            Engine_GameFlagSet(flag);
            Engine_GameFlagClear(flag + 1);
        } else if (x == (i << 2) + 41) {
            Engine_GameFlagSet(flag + 1);
            Engine_GameFlagClear(flag);
        } else {
            Engine_GameFlagClear(flag);
            Engine_GameFlagClear(flag + 1);
        }
    }
    x = *(s32 *)((s32)Object_GetById(19) + 8) / 0x100000;
    if (x == 57) {
        Engine_GameFlagSet(0x338);
        Engine_GameFlagClear(0x339);
        Call6(Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
    } else if (x == 59) {
        Engine_GameFlagSet(0x339);
        Engine_GameFlagClear(0x338);
        Call6(Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
    } else {
        Engine_GameFlagClear(0x338);
        Engine_GameFlagClear(0x339);
        Call6(Map_CopyCellAttributeRect, 53, 11, 1, 1, 58, 7);
    }
    Engine_EventEnd();
}

void SceneState_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Event_Begin();
    col = Object_GetById(20)->x.fixed / 0x100000;
    if (col == 28) {
        GameFlag_Set(840);
        {
            s32 a = 31;
            s32 b = 20;

            Map_CopyCellAttributes(29, 20, 1, 1, a, b);
        }
    }
    Event_End();
}

void SceneEffect_RotatePaletteEntries40To47(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)&gFrameCount & 7) != 0) {
        return;
    }

    dst = (u16 *)0x05000050;
    front = *dst;
    index = 0;
    *(u16 *)0x0500005e = front;

    src = (u16 *)0x05000052;
    while (index <= 6) {
        *dst++ = *src++;
        index++;
    }
}

void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(3, FX16_0_8, FX16_0_4);
    Actor_SetSpeed(0, FX16_0_8, FX16_0_4);
    Event_SetMessage(MSG_SOMEBODY_HERE);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_WalkToAndWait(3, 0x348, 0x288);
    Actor_ShowEmote(3, 0x100, 60);
    Actor_FaceDirection(3, FX16_0_5, 20);
    Actor_SetAnimation(3, 16);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAnimation(3, 1);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    Event_Wait(20);
    Actor_FaceDirection(3, FX16_0_25, 20);
    Event_OpenMessage(3, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_ShowMessageAndWait(3, 0, 20);
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
        Event_Wait(20);
        Actor_SetAnimationAndWait(3, 4);
        Event_ShowMessageAndWait(3, 0, 20);
    }
    Event_Wait(20);
    Actor_FaceDirection(3, FX16_0_75, 20);
    Camera_SetSpeed(FX16_0_8, FX16_0_1);
    Camera_MoveTo(0x3480000, -1, 0x2780000, 1);
    Actor_WalkToAndWait(3, 0x348, 0x278);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(10);
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    ((void (*)())Engine_GameFlagSet)(0x870);
    Event_End();
}

void SceneDialogue_RunActor3TimedLine(void)
{
    Event_Begin();
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_STATUE_BLOCKING_ENTRANCE);
    Event_ShowMessageAndWait(3, 0, 20);
    Event_End();
}

/* Fade palette entries 40 to 47 to white one step every two frames. */
void MakyuriHeya_FadePaletteToWhite(void)
{
    volatile u16 *pal;
    u32 i;
    u32 done;
    s32 r;
    s32 g;
    s32 b;

    do {
        pal = (volatile u16 *)0x05000050;
        done = 0;
        for (i = 0; i <= 7; i++) {
            r = *pal & 31;
            g = (u16)(*pal >> 5) & 31;
            b = (u16)(*pal >> 10) & 31;
            if (r == 31 && g == 31 && b == 31) {
                done++;
            } else {
                if (r <= 30)
                    r++;
                if (g <= 30)
                    g++;
                if (b <= 30)
                    b++;
                *pal = (b << 10) | (g << 5) | r;
            }
            pal++;
        }
        Task_Wait(2);
    } while (done <= 7);
}

/* When actor 10 stands in tile column 51, plays the scripted actor
 * choreography; each of its two query branches advances the scene step counter
 * once. The closing call runs on every path. */
void FieldScene_RunColumnChoreography(void)
{
    s32 record;
    s32 pos;

    record = Object_GetById(10);
    pos = *(s32 *)(record + 8);
    if (pos < 0) pos += 0xfffff;
    pos >>= 20;
    Engine_EventBegin();
    if (pos != 51) {
    } else {
        ObjectMotion_SetSpeedParameters(3, 0xcccc, 0x6666);
        Battle_WaitMode0(20);
        Engine_ActorRunRepeatedMotion(3, 2);
        Battle_WaitMode0(20);
        Call3(Engine_ActorFaceDirection, 3, 0xd000, 0);
        Engine_ActorFaceDirection(0, 0x5000, 10);
        Engine_EventSetMessage((s32)MsgMakyuriDidThat);
        Engine_EventOpenMessage(3, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(3, 3);
            Engine_EventShowMessageAndWait(3, 0, 20);
        } else {
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(3, 4);
            Event_ShowMessageAndWait(3, 0, 20);
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        }
        Engine_ActorShowEmote(0, 0x100, 60);
        record = Object_GetById(1);
        SetEffectRecordMode(record, 1);
        record = Object_GetById(2);
        SetEffectRecordMode(record, 1);
        Call3(ObjectMotion_SetSpeedParameters, 1, 0xcccc, 0x6666);
        Call3(ObjectMotion_SetSpeedParameters, 2, 0xcccc, 0x6666);
        Call3(Engine_ActorSetPosition, 1, 0x3680000, 0x2580000);
        Call3(Engine_ActorSetPosition, 2, 0x3680000, 0x2580000);
        Call3(Engine_ActorWalkTo, 2, 0x378, 0x278);
        Actor_WalkToAndWait(1, 0x370, 0x268);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(2);
        Actor_FaceDirection(2, 0x8000, 0);
        Actor_RunRepeatedMotion(1, 1);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(1, 0, 20);
        Call3(Engine_ActorShowEmote, 3, 0x101, 60);
        Event_ShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xb000, 20);
        Object_SetModeById(1, 3);
        Engine_ActorSetAnimationAndWait(2, 3);
        Battle_WaitMode0(30);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0x8000, 20);
        Engine_EventShowMessageAndWait(2, 0, 20);
        Actor_SetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorWalkTo(3, 0x348, 0x290);
        Event_Wait(5);
        Engine_ActorFaceDirection(2, 0x5000, 0);
        Battle_WaitMode0(10);
        Engine_ActorFaceDirection(0, 0x4000, 0);
        ((void (*)())ObjectMotion_CommitCurrentPositionAndActivate)(3);
        Battle_WaitMode0(10);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorFaceDirection, 3, 0xd000, 20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Actor_RunRepeatedMotion(1, 1);
        Battle_WaitMode0(20);
        Actor_FaceDirection(1, 0xc000, 20);
        Engine_EventOpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(1, 3);
            Engine_EventShowMessageAndWait(1, 0, 20);
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        } else {
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(1, 4);
            Engine_EventShowMessageAndWait(1, 0, 20);
        }
        Object_SetModeById(3, 16);
        Event_Wait(30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorWalkToAndWait, 3, 0x348, 0x278);
        Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
        Battle_WaitMode0(30);
        MakyuriHeya_CastPsynergyAtActor11();
        Event_Wait(50);
        Audio_PlayCue(131);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x207e9f, 0);
        Engine_ColorBufferInterpolate(10);
        Task_Wait(1);
        Audio_PlayCue(220);
        WaitFrames(40);
        Engine_ColorBufferApplyTarget(0x10000, 0);
        ((void (*)())Engine_ColorBufferInterpolate)(60);
        WaitFrames(60);
        Audio_PlayCue(209);
        MakyuriHeya_FadePaletteToWhite();
        Call6(Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
        Engine_MapAnimateCells((s32)MakyuriHeya_ColumnCells, 116, 35);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries40To47);
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Call3(ObjectMotion_SetSpeedParameters, 3, 0x30000, 0x18000);
        Engine_ActorJump(3, 4, 0);
        Call3(Engine_ActorWalkTo, 3, 0x348, 0x258);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Call6(Map_CopyCellAttributeRect, 116, 36, 3, 4, 52, 36);
        OverlayObject_PrepareSpawnedObject(0x3480000, 0x380000, 0x2600000, 223);
        Call3(ObjectMotion_SetSpeedParameters, 3, 0xcccc, 0x6666);
        Call3(Engine_ActorWalkToAndWait, 3, 0x348, 0x230);
        Engine_ActorSetPosition(3, 0, 0);
        Battle_WaitMode0(20);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
        Engine_ActorFaceDirection(2, 0xb000, 0);
        Battle_WaitMode0(10);
        Object_SetModeById(0, 3);
        Object_SetModeById(1, 3);
        Engine_ActorSetAnimationAndWait(2, 3);
        Battle_WaitMode0(20);
        Actor_SetAnimation(1, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Object_SetModeById(2, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Actor_SetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(1);
        Engine_ActorSetPosition(1, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(2);
        Engine_ActorSetPosition(2, 0, 0);
        Engine_GameFlagSet(0x871);
    }
    Engine_EventEnd();
}

void SceneEffect_RotatePaletteEntries97To103(void)
{
    unsigned int i;
    u16 *dst;
    u16 *src;
    u32 first;

    if ((*(volatile u32 *)&gFrameCount & 7) != 0) {
        return;
    }

    dst = (u16 *)0x050000c2;
    first = *dst;
    i = 0;
    *(u16 *)0x050000ce = first;

    src = (u16 *)0x050000c4;
    while (i <= 5) {
        *dst++ = *src++;
        i++;
    }
}

/* Fade palette entries 97 to 103 to black one step every five frames. */
void MakyuriHeya_FadePaletteToBlack(void)
{
    volatile u16 *pal;
    u32 i;
    s32 done;
    s32 r;
    s32 g;
    s32 b;

    do {
        pal = (volatile u16 *)0x050000c2;
        done = 0;
        for (i = 0; i <= 6; i++) {
            r = *pal & 31;
            g = (u16)(*pal >> 5) & 31;
            b = (u16)(*pal >> 10) & 31;
            if (r > 0)
                r--;
            if (g > 0)
                g--;
            if (b > 0)
                b--;
            *pal = (b << 10) | (g << 5) | r;
            if (*pal == 0)
                done++;
            pal++;
        }
        Task_Wait(5);
    } while (done != 7);
}

/* The actor sinks away in a burst of sparks over 48 frames. */
void MakyuriHeya_SinkActorWithSparks(s32 actor)
{
    struct EffectParams params;
    struct EffectParams *p;
    u8 *rec;
    u32 i;
    s32 x;
    s32 y;
    s32 speed;

    rec = (u8 *)Object_GetById(actor);
    rec[85] = 0;
    Engine_ActorSetSpriteFlags(rec, 0);
    Call2(Engine_ActorSetChildValue, actor, 0x100);
    Audio_PlayCue(221);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11e;
    p->script = (s32)MakyuriHeya_SparkBurstScript;
    for (i = 0; i <= 47; i++) {
        if (i <= 31)
            SceneEffect_SpawnWithRandomOffset(*(s32 *)(rec + 8), *(s32 *)(rec + 12), *(s32 *)(rec + 16));
        if (i & 1) {
            Audio_PlayCue(246);
            x = *(s32 *)(rec + 8) + ((((u32)Engine_RandomNext() * 24) >> 16) << 16) + -0xc0000;
            y = *(s32 *)(rec + 12) + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000;
            speed = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + 0x8000;
            Effect_Spawn(x, y, *(s32 *)(rec + 16), 0, speed, 0, 0x330000, p);
        }
        *(s32 *)(rec + 12) += i * 0x1999;
        *(s32 *)(rec + 60) = *(s32 *)(rec + 12);
        WaitFrames(2);
    }
}

/* Raises three randomized particle fields, then stages the actors according
 * to whether the lighthouse event flag has already been set. */
void FieldScene_RunRandomEffectActorSequence(void)
{
    struct ConfiguredEffectOptions *effect;
    s32 row_offset;
    u32 value;
    s32 zero;
    s32 phase;
    s32 particle;
    s32 x;
    s32 velocity_x;
    struct ConfiguredEffectOptions options;

    Event_Begin();
    Event_Wait(20);
    MakyuriHeya_FadePaletteToBlack();
    Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
    Call6(Engine_MapCopyCellsTo, 45, 77, 45, 73, 9, 4);
    Event_Wait(30);
    effect = &options;
    effect->mode_bits = 1;
    effect->mode = 5;
    effect->kind = 0x11e;
    effect->callback_arg = (s32)MakyuriHeya_SparkBurstScript;
    zero = 0;
    phase = zero;
    do {
        u32 x, z;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Engine_RandomNext();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Engine_RandomNext();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x880000;
        Effect_Spawn(x, 0, z, 0, 0, 0, 0x330001, effect);
        ((void (*)())Battle_WaitMode0)(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Event_Wait(40);
    zero = 0;
    phase = zero;
    do {
        u32 x, z;
        s32 speed;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Engine_RandomNext();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Engine_RandomNext();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x980000;
        value = Engine_RandomNext();
        speed = -((value * 10 >> 16) * 0x3333) - 0x3333;
        Effect_Spawn(x, 0, z, 0, 0, speed, 0x330001, effect);
        Event_Wait(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Event_Wait(60);
    Audio_PlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Event_Wait(60);
    effect->mode = 7;
    effect->accum18 = 0xb333;
    effect->accum1c = 0xb333;
    effect->target30 = 0x13333;
    effect->target34 = 0x13333;
    zero = 0;
    phase = zero;
    do {
        s32 speed;

        Call6(Engine_MapCopyCellsTo, 59, (12 - phase), 48, (12 - phase), 3, 1);
        particle = 0;
        row_offset = (phase << 4);
        do {
            value = Engine_RandomNext();
            x = ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x3000000);
            velocity_x = (0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16)) - 0x6664;
            speed = 0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16);
            Effect_Spawn(x, 0, ((s32)(-((u32)particle >> 1) - row_offset) << 16) + 0xc00000, velocity_x, 0, speed, 0xd0001, effect);
            particle = (particle + 1);
            Event_Wait(2);
        } while ((u32)particle <= 31);
        phase = (phase + 1);
    } while ((u32)phase <= 3);
    Audio_PlayCue(0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    MapRender_WaitForValues();
    BattleFx_PlayQueuedSound();
    Event_Wait(30);
    if (GameFlag_IsSet(0x881) != 0) {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_WalkToAndWait(0, 0x338, 232);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Actor_Jump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Event_Wait(10);
        Actor_SetAnimation(0, 18);
        MakyuriHeya_SinkActorWithSparks(0);
        Event_Wait(60);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x10005, 0);
        ColorBuffer_Interpolate(120);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(60);
        Event_RequestExit(9);
        Event_End();
    } else {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_SetSpeed(1, 0xcccc, 0x6666);
        Actor_SetSpeed(2, 0xcccc, 0x6666);
        Actor_SetSpeed(3, 0xcccc, 0x6666);
        Actor_WalkToAndWait(0, 0x338, 240);
        Actor_FaceDirection(0, 0xa000, 20);
        Actor_SetPosition(3, 0x3380000, 0xf00000);
        Actor_WalkToAndWait(3, 0x318, 232);
        Actor_FaceDirection(3, 0xc000, 0);
        Event_Wait(60);
        Actor_FaceDirection(3, 0x2000, 20);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(40);
        Actor_FaceDirection(3, 0xc000, 20);
        Actor_WalkToAndWait(3, 0x318, 200);
        MakyuriHeya_SinkActorWithSparks(3);
        Event_Wait(20);
        Actor_RunRepeatedMotion(0, 2);
        Event_Wait(30);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_SetPosition(1, 0x3180000, 0xe80000);
        Actor_SetPosition(2, 0x3180000, 0xe80000);
        Actor_WalkTo(1, 0x330, 224);
        Actor_WalkToAndWait(2, 0x300, 224);
        Actor_WaitForMove(1);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_ShowEmote(0, 0x102, 0);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Actor_FaceDirection(1, 0x6000, 0);
        Actor_FaceDirection(2, 0x2000, 20);
        Actor_RunRepeatedMotion(0, 1);
        Event_Wait(60);
        Actor_SetSpeed(0, 0x8000, 0x4000);
        Actor_WalkToAndWait(0, 0x318, 224);
        Actor_FaceDirection(1, 0x8000, 0);
        Actor_FaceDirection(2, 0, 0);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Actor_Jump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Event_Wait(10);
        Actor_SetAnimation(0, 18);
        Actor_ShowEmote(1, 0x100, 0);
        Actor_ShowEmote(2, 0x100, 0);
        Actor_StartRepeatedMotion(1, 2);
        Actor_StartRepeatedMotion(2, 2);
        MakyuriHeya_SinkActorWithSparks(0);
        Event_Wait(60);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Actor_FaceEachOther(1, 2, 20);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(40);
        Actor_WalkToAndWait(1, 0x318, 216);
        Actor_FaceDirection(2, 0xe000, 0);
        Actor_WalkToAndWait(1, 0x318, 200);
        Event_Wait(30);
        MakyuriHeya_SinkActorWithSparks(1);
        Actor_WalkToAndWait(2, 0x318, 216);
        Actor_WalkToAndWait(2, 0x318, 200);
        Event_Wait(30);
        MakyuriHeya_SinkActorWithSparks(2);
        ColorBuffer_ApplySource(0x10000, 0);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplyTarget(0x10005, 0);
        ColorBuffer_Interpolate(120);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(60);
        Event_End();
        Event_RequestExit(8);
    }
}

void FieldScene_RunScriptedSteps0And1576(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_FOUNTAIN_HEALING_WATER_HERMES_BRINGS, 1);
    Event_End();
}

void FieldScene_RunScriptedSteps0And953(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_DOOR_TIGHTLY_LOCKED, 1);
    Event_End();
}

void FieldScene_RunFlag881Dialogue(void)
{

    Event_Begin();
    Actor_SetAnimation(0, 1);
    if (GameFlag_IsSet(0x881) == 0)
        Message_ShowCentered(MSG_FOUNTAIN_SEEMS_DRY, 1);
    else
        Message_ShowCentered(MSG_FOUNTAIN_FLOWING_WITH_WATER, 1);
    if (PartyInventory_FindOwner(0xb9) != -1) {
        s16 *slot = (s16 *)gEventWork + 185;
        s32 one = 1;

        *slot = one;
    }
    Event_End();
}

void FieldScene_RunActor184Sequence(void)
{
    Event_Begin();
    Audio_PlayCue(0x53);
    Item_ShowFound(ITEM_HERMES_WATER, 3);
    SceneState_SetRecordTableValue(0xb9, 0xb8);
    UiWork_PushValueSlot(PartyInventory_FindOwner(0xb8), 1);
    UiWork_PushValueSlot(0xb8, 2);
    Message_ShowCentered(MSG_ROBIN_GOT, 1);
    GameFlag_Set(512);
    Event_End();
}

/* The rising sequence goes through Call3, which sets r0 and r1 before
 * negating the third argument, as the game does; a direct call negates it
 * first. */

/* In the first room the leader walks to the stair, turns, and sinks out of
 * sight through the opened floor before the party leaves by exit 8; in the
 * others the leader only rises. */
void FieldScene_RunScene39cSequenceB(void)
{
    Event_Begin();
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya1) {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d8, 0x258);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
        Camera_MoveTo(0x1d00000, -1, 0x2900000, 1);
        Actor_SetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
        OverlayObject_PrepareSpawnedObject(Object_GetById(ACTOR_PARTY_LEADER)->x.fixed, 0, 0x2be0000, 223);
        Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
        *(s32 *)&Object_GetById(ACTOR_PARTY_LEADER)->unknown_44[4] = 0x8000;
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
        Event_Wait(60);
        Event_RequestExit(8);
    } else {
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
    }
    Event_End();
}

void MakyuriHeya_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 done;

    actor = Actor_Get(0);
    done = GameFlag_IsSet(0x109);
    if (done == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) - 0x100000);
        Actor_SetChildValue(0, 15);
        Actor_SetSpriteFlags(Actor_Get(0), 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriHeya_TrailSparks;
        Actor_SetSpeed(0, 0x6666, 0x3333);
        Engine_ActorWalkByAndWait(0, 0, 8);
        Actor_SetChildValue(0, 0);
        Actor_SetSpriteFlags(Actor_Get(0), 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Event_End();
    }
}

void SceneEffect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Map_CopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

                Effect_Spawn(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Event_Wait(1);
            }
        }
        Map_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Map_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}
