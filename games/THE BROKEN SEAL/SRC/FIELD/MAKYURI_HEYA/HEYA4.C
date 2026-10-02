#include "GLOBAL_CELLS.H"
#include "EDITION.H"
#include "PROBE.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"

#include "MAKYURI.H"
#include "CALL.H"
#include "MAKYURI_HEYA.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

TEXT_MESSAGE_ENUM(MsgFieldDoorTightlyLocked);
TEXT_MESSAGE_ENUM(MsgImiruHermesHealingWaterFountain);
extern u8 MsgImiruSomebodyHere[];
TEXT_MESSAGE_ENUM(MsgImiruStatueBlocksEntrance);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaFountainFlowsWithWater);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaFountainSeemsDry);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaRobinGot);


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
void MakyuriHeya_OpenStoneDoor(s32 mode);
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
extern const s32 MakyuriHeya_SparkBurstScript[];
void WaitFrames(s32 frames);
void MakyuriHeya_FadePaletteToBlack();
void SceneEffect_RotatePaletteEntries97To103(void);
void BattleFx_PlayQueuedSound();
void MakyuriHeya_SinkActorWithSparks();
void BattleFx_RunRisingObjectSequence();

void Engine_MapCopyCellsTo(s32 sx, s32 sy, s32 dx, s32 dy, s32 w, s32 h);
void Battle_WaitMode0(s32 frames);
extern u8 MsgMakyuriHonorsGoddessRainbows[];
void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Makyuri_CyclePalette(void);
void SceneActor_UseActorNinePositionWithYOffset(void);
void MakyuriHeya_RideLift(void);
void BattleFx_StartFadeOverlay(s32 value);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void DisplayBlend_EnableRunScript(void);
void DisplayBlend_DisableRunScript(void);
void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);
void Engine_MapRenderWaitForValues(void);
void FieldScene_RedrawActorFootprint(s32 actor);
void FieldScene_RunSupplementalSequenceOne(s32 mode);
void MakyuriHeya_ArriveWithSparks(void);
void Engine_ColorBufferInterpolate(s32 frames);

extern u8 MsgMakyuriSavedMeAgain[];
extern u8 MsgMakyuriDontHideTruth[];
extern u8 MsgMakyuriThoughtSo[];
void Event_PrepareObjectAndApplyValue();
s32 Djinn_AddToOwner();
void Djinn_Activate();
void Owner_RecalculateStats();

/* The map layer the lift is drawn on, scrolled by its vertical offset. */
struct LiftLayer {
    u8 unknown_00[12];
    s32 offset;
    u8 unknown_10[12];
    s32 unknown_1c;
};

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
                Battle_WaitMode0(1);
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

    Engine_EventBegin();
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
            MakyuriHeya_OpenStoneDoor(2);
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
                MakyuriHeya_OpenStoneDoor(1);
                Engine_ActorStartAction(9);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3120000, 223);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3320000, 223);
                Actor_SetDestination(9, 0x348, 0x2e8);
                Battle_WaitMode0(5);
                Audio_PlayCue(189);
                ObjectMotion_CommitCurrentPositionAndActivate(9);
                Battle_WaitMode0(40);
                GameFlag_Set(0x877);
                ColorBuffer_ApplySource(0x10000, 0);
                *(s32 *)&(*(struct GameState **)&gEventWork)->scene = 0x100;
                Engine_EventCloseScreen();
                Engine_EventWaitForScreen();
                Engine_EventRequestExit(15);
                goto end;
            }
            SceneEffect_SpawnParticleRowsByMode(2);
            SceneData_ApplyTableA2c5AndReturnZero();
            MakyuriHeya_OpenStoneDoor(1);
            goto wait;
        } else if (column == 53) {
            GameFlag_Set(0x31b);
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            MakyuriHeya_StartPillarPush();
            SceneEffect_SpawnParticleRowsByMode(0);
        wait:
            Battle_WaitMode0(60);
        } else {
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            GameFlag_Clear(0x31b);
            MakyuriHeya_StartPillarPush();
            if (start == 47)
                SceneEffect_SpawnParticleRowsByMode(2);
            else if (start == 48)
                SceneEffect_SpawnParticleRowsByMode(1);
            MakyuriHeya_OpenStoneDoor(0);
            Battle_WaitMode0(60);
        }
        break;
    }
end:
    Engine_EventEnd();
}

void FieldScene_RunFourStepSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    MakyuriHeya_SyncBlockFlags();
    Engine_EventEnd();
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

    Engine_EventBegin();
    col = Object_GetById(20)->x.fixed / 0x100000;
    if (col == 28) {
        GameFlag_Set(840);
        {
            s32 a = 31;
            s32 b = 20;

            Map_CopyCellAttributes(29, 20, 1, 1, a, b);
        }
    }
    Engine_EventEnd();
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

    Engine_EventBegin();
    Actor_SetSpeed(3, FX16_0_8, FX16_0_4);
    Actor_SetSpeed(0, FX16_0_8, FX16_0_4);
    Engine_EventSetMessage((s32)MsgImiruSomebodyHere);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_WalkToAndWait(3, 0x348, 0x288);
    Actor_ShowEmote(3, 0x100, 60);
    Actor_FaceDirection(3, FX16_0_5, 20);
    Object_SetModeById(3, 16);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 1);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    Battle_WaitMode0(20);
    Actor_FaceDirection(3, FX16_0_25, 20);
    Event_OpenMessage(3, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_ShowMessageAndWait(3, 0, 20);
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 4);
        Event_ShowMessageAndWait(3, 0, 20);
    }
    Battle_WaitMode0(20);
    Actor_FaceDirection(3, FX16_0_75, 20);
    Camera_SetSpeed(FX16_0_8, FX16_0_1);
    Camera_MoveTo(0x3480000, -1, 0x2780000, 1);
    Actor_WalkToAndWait(3, 0x348, 0x278);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(20);
    Engine_ActorRunRepeatedMotion(3, 2);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Engine_GameFlagSet(0x870);
    Engine_EventEnd();
}

void SceneDialogue_RunActor3TimedLine(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimationAndWait(3, 4);
    Battle_WaitMode0(20);
    Engine_EventSetMessage(MsgImiruStatueBlocksEntrance);
    Event_ShowMessageAndWait(3, 0, 20);
    Engine_EventEnd();
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
        WaitFrames(2);
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
        Engine_ActorRunRepeatedMotion(1, 1);
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
        Engine_ActorSetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorWalkTo(3, 0x348, 0x290);
        Battle_WaitMode0(5);
        Engine_ActorFaceDirection(2, 0x5000, 0);
        Battle_WaitMode0(10);
        Engine_ActorFaceDirection(0, 0x4000, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Battle_WaitMode0(10);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorFaceDirection, 3, 0xd000, 20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorRunRepeatedMotion(1, 1);
        Battle_WaitMode0(20);
        Actor_FaceDirection(1, 0xc000, 20);
        Engine_EventOpenMessage(1, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
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
        Battle_WaitMode0(30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorWalkToAndWait, 3, 0x348, 0x278);
        Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
        Battle_WaitMode0(30);
        MakyuriHeya_CastPsynergyAtActor11();
        Battle_WaitMode0(50);
        Audio_PlayCue(131);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x207e9f, 0);
        Engine_ColorBufferInterpolate(10);
        WaitFrames(1);
        Audio_PlayCue(220);
        WaitFrames(40);
        Engine_ColorBufferApplyTarget(0x10000, 0);
        Engine_ColorBufferInterpolate(60);
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
        Object_SetModeById(1, 2);
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
        WaitFrames(5);
    } while (done != 7);
}

/* The actor sinks away in a burst of sparks over 48 frames. */
void MakyuriHeya_SinkActorWithSparks(s32 actor)
{
    struct EffectOptions params;
    struct EffectOptions *p;
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
    p->priority = 1;
    p->palette = 5;
    p->type = 0x11e;
    p->script = MakyuriHeya_SparkBurstScript;
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

    Engine_EventBegin();
    Battle_WaitMode0(20);
    MakyuriHeya_FadePaletteToBlack();
    Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
    Call6(Engine_MapCopyCellsTo, 45, 77, 45, 73, 9, 4);
    Battle_WaitMode0(30);
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
        Battle_WaitMode0(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Battle_WaitMode0(40);
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
        Battle_WaitMode0(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Battle_WaitMode0(60);
    Audio_PlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Battle_WaitMode0(60);
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
            Battle_WaitMode0(2);
        } while ((u32)particle <= 31);
        phase = (phase + 1);
    } while ((u32)phase <= 3);
    Audio_PlayCue(0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    BattleFx_PlayQueuedSound();
    Battle_WaitMode0(30);
    if (GameFlag_IsSet(0x881) != 0) {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_WalkToAndWait(0, 0x338, 232);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Engine_ActorJump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Battle_WaitMode0(10);
        Object_SetModeById(0, 18);
        MakyuriHeya_SinkActorWithSparks(0);
        Battle_WaitMode0(60);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x10005, 0);
        Engine_ColorBufferInterpolate(120);
        Battle_WaitMode0(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        Engine_ColorBufferInterpolate(60);
        Battle_WaitMode0(60);
        Engine_EventRequestExit(9);
        Engine_EventEnd();
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
        Battle_WaitMode0(60);
        Actor_FaceDirection(3, 0x2000, 20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Battle_WaitMode0(40);
        Actor_FaceDirection(3, 0xc000, 20);
        Actor_WalkToAndWait(3, 0x318, 200);
        MakyuriHeya_SinkActorWithSparks(3);
        Battle_WaitMode0(20);
        Engine_ActorRunRepeatedMotion(0, 2);
        Battle_WaitMode0(30);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_SetPosition(1, 0x3180000, 0xe80000);
        Actor_SetPosition(2, 0x3180000, 0xe80000);
        Actor_WalkTo(1, 0x330, 224);
        Actor_WalkToAndWait(2, 0x300, 224);
        ObjectMotion_CommitCurrentPositionAndActivate(1);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_ShowEmote(0, 0x102, 0);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Actor_FaceDirection(1, 0x6000, 0);
        Actor_FaceDirection(2, 0x2000, 20);
        Engine_ActorRunRepeatedMotion(0, 1);
        Battle_WaitMode0(60);
        Actor_SetSpeed(0, 0x8000, 0x4000);
        Actor_WalkToAndWait(0, 0x318, 224);
        Actor_FaceDirection(1, 0x8000, 0);
        Actor_FaceDirection(2, 0, 0);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Engine_ActorSetAnimationAndWait(0, 3);
        Battle_WaitMode0(20);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Engine_ActorJump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Battle_WaitMode0(10);
        Object_SetModeById(0, 18);
        Actor_ShowEmote(1, 0x100, 0);
        Actor_ShowEmote(2, 0x100, 0);
        Engine_ActorStartRepeatedMotion(1, 2);
        Engine_ActorStartRepeatedMotion(2, 2);
        MakyuriHeya_SinkActorWithSparks(0);
        Battle_WaitMode0(60);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Engine_ActorFaceEachOther(1, 2, 20);
        Object_SetModeById(1, 3);
        Engine_ActorSetAnimationAndWait(2, 3);
        Battle_WaitMode0(40);
        Actor_WalkToAndWait(1, 0x318, 216);
        Actor_FaceDirection(2, 0xe000, 0);
        Actor_WalkToAndWait(1, 0x318, 200);
        Battle_WaitMode0(30);
        MakyuriHeya_SinkActorWithSparks(1);
        Actor_WalkToAndWait(2, 0x318, 216);
        Actor_WalkToAndWait(2, 0x318, 200);
        Battle_WaitMode0(30);
        MakyuriHeya_SinkActorWithSparks(2);
        ColorBuffer_ApplySource(0x10000, 0);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplyTarget(0x10005, 0);
        Engine_ColorBufferInterpolate(120);
        Battle_WaitMode0(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        Engine_ColorBufferInterpolate(60);
        Battle_WaitMode0(60);
        Engine_EventEnd();
        Engine_EventRequestExit(8);
    }
}

void FieldScene_RunScriptedSteps0And1576(void)
{
    Engine_EventBegin();
    Object_SetModeById(0, 1);
    Engine_MessageShowCentered(MsgImiruHermesHealingWaterFountain, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedSteps0And953(void)
{
    Engine_EventBegin();
    Object_SetModeById(0, 1);
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
    Engine_EventEnd();
}

void FieldScene_RunFlag881Dialogue(void)
{

    Engine_EventBegin();
    Object_SetModeById(0, 1);
    if (GameFlag_IsSet(0x881) == 0)
        Engine_MessageShowCentered(MsgMakyuriHeyaFountainSeemsDry, 1);
    else
        Engine_MessageShowCentered(MsgMakyuriHeyaFountainFlowsWithWater, 1);
    if (PartyInventory_FindOwner(0xb9) != -1) {
        s16 *slot = (s16 *)gEventWork + 185;
        s32 one = 1;

        *slot = one;
    }
    Engine_EventEnd();
}

void FieldScene_RunActor184Sequence(void)
{
    Engine_EventBegin();
    Audio_PlayCue(0x53);
    Engine_ItemShowFound(ITEM_HERMES_WATER, 3);
    SceneState_SetRecordTableValue(0xb9, 0xb8);
    UiWork_PushValueSlot(PartyInventory_FindOwner(0xb8), 1);
    UiWork_PushValueSlot(0xb8, 2);
    Engine_MessageShowCentered(MsgMakyuriHeyaRobinGot, 1);
    GameFlag_Set(512);
    Engine_EventEnd();
}

/* The rising sequence goes through Call3, which sets r0 and r1 before
 * negating the third argument, as the game does; a direct call negates it
 * first. */

/* In the first room the leader walks to the stair, turns, and sinks out of
 * sight through the opened floor before the party leaves by exit 8; in the
 * others the leader only rises. */
void FieldScene_RunScene39cSequenceB(void)
{
    Engine_EventBegin();
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya1) {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d8, 0x258);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
        Camera_MoveTo(0x1d00000, -1, 0x2900000, 1);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
        OverlayObject_PrepareSpawnedObject(Object_GetById(ACTOR_PARTY_LEADER)->x.fixed, 0, 0x2be0000, 223);
        Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
        *(s32 *)&Object_GetById(ACTOR_PARTY_LEADER)->unknown_44[4] = 0x8000;
        Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
        Battle_WaitMode0(60);
        Engine_EventRequestExit(8);
    } else {
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
    }
    Engine_EventEnd();
}

void MakyuriHeya_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 done;

    actor = Actor_Get(0);
    done = GameFlag_IsSet(0x109);
    if (done == 0) {
        Engine_EventBegin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) - 0x100000);
        Actor_SetChildValue(0, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(0), 0);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriHeya_TrailSparks;
        Actor_SetSpeed(0, 0x6666, 0x3333);
        Engine_ActorWalkByAndWait(0, 0, 8);
        Actor_SetChildValue(0, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(0), 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Engine_EventEnd();
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
                Battle_WaitMode0(1);
            }
        }
        Map_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Map_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}

/* Open the three-step stair: each step throws dust along both of its edges. */
void MakyuriHeya_OpenThreeStepStair(void)
{
    struct EffectOptions params;
    struct EffectOptions *p;
    u32 i;
    u32 j;
    s32 z;

    Engine_MapCopyCellsTo(76, 61, 74, 38, 1, 1);
    p = &params;
    p->palette = 5;
    p->start_scale_x = 0x8000;
    p->start_scale_y = 0x8000;
    for (i = 0; i <= 2; i++) {
        z = -0x20000;
        for (j = 1; j <= 7; j++) {
            if (j & 1) {
                if (j & 2) {
                    Effect_Spawn((105 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0, z - (i << 19) + 0x22e0000, 0, 0, -0x4000, 0x90000, p);
                } else {
                    Effect_Spawn((((i << 2) + j) << 17) + 0xb70000, 0, (0x26c - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0x4000, 0, 0, 0x90000, p);
                }
                Battle_WaitMode0(1);
            }
            z += -0x20000;
        }
        Engine_MapCopyCellsTo(71, 59, 70, 34 - i, 1, 1);
        Engine_MapCopyCellsTo(71, 59, i + 75, 38, 1, 1);
    }
}

void SceneEffect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Actor_Get(0);
    s32 m = gFrameCount & 3;

    if (m == 0) {
        buf[1] = 7;
        if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + ((((u32)Engine_RandomNext() << 2) >> 16) << 16);
            s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

            Effect_Spawn(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = gFrameCount & 3;

    if (m == 0) {
        buf[1] = 7;
        if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

            Effect_Spawn(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + ((((u32)Engine_RandomNext() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + ((((u32)Engine_RandomNext() << 3) >> 16) << 16) + 0xfffc0000;

        Effect_Spawn(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}

void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Audio_PlayCue(0xd8);
    i = 0;
    do {
        if ((i & 1) != 0) {
            desc[1] = 7;
            if ((i & 2) != 0)
                desc[1] = 5;
            desc[2] = 0x9999;
            desc[3] = 0x9999;
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            Vector_AddPolarOffset((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);
                s32 b = z + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);

                Effect_Spawn(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        WaitFrames(2);
        i++;
    } while (i <= 11);
}

/* The lighthouse rooms' scene start, entry veneer 0: sets the retreat to the
 * fourth room, the blend and the room lights, starts the palette cycle when
 * flag 0x875 is set, then per room and entrance restores the pushed
 * pillars, lifts and cells from the story flags. */

/* FAKEMATCH: the do/while blocks keep the blend constants out of the shared
 * pool. */

/* Mercury Lighthouse rooms: set the blend and lights, then per area and entrance restore the pushed pillars, lifts and cells from the story flags. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;
    u32 i;
    s32 flag;
    s32 x;
    s32 set;

    Engine_GameFlagSet(0x111);
    gGameState.retreat_entrance = 11;
    gGameState.retreat_scene = (s32)&SceneId_MakyuriHeya4;
    gEventWork->start_transition = 0x204;
    /* FAKEMATCH: the do/while and the held value keep the blend constant in a register ahead of its address load. */
    do {
        s32 blend = 0x3f40;
        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    {
        s32 alpha = 0x1010;
        *(volatile u16 *)0x04000052 = alpha;
    }
    Makyuri_SpawnLightObjects(21, (s32)gSceneState);
    BattleFx_StartFadeOverlay(0);
    if (Engine_GameFlagIsSet(0x875))
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
    else
        Makyuri_ClearPalette();
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya1) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
            if (Engine_GameFlagIsSet(0x875))
                Call6((void (*)())Map_CopyCellAttributeRect, 66, 5, 27, 23, 2, 5);
            break;
        case 5:
        case 6:
            BattleFx_SetQueuedSoundAndPlay(170);
            break;
        case 15:
            SetEffectRecordMode(Object_GetById(0), 1);
        case 4:
            if (Engine_GameFlagIsSet(0x876)) {
                WaitFrames(1);
                Call3((void (*)())Engine_ActorSetPosition, 9, 168 << 18, 128 << 16);
                Call3((void (*)())Engine_ActorSetPosition, 10, 176 << 18, 192 << 16);
                Call3((void (*)())Engine_ActorSetPosition, 11, 162 << 18, 240 << 16);
            } else if (!Engine_GameFlagIsSet(0x109)) {
                Engine_GameFlagClear(0x310);
                Engine_GameFlagClear(0x311);
                Engine_GameFlagClear(0x312);
                Engine_GameFlagClear(0x313);
            }
            SetEffectRecordMode(Object_GetById(9), 1);
            SetEffectRecordMode(Object_GetById(10), 1);
            SetEffectRecordMode(Object_GetById(11), 1);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            SetEffectRecordMode(Object_GetById(12), 1);
            if (Engine_GameFlagIsSet(0x310)) {
                Engine_MapCopyCellsTo(119, 9, 109, 11, 1, 1);
                if (Engine_GameFlagIsSet(0x311))
                    Engine_MapCopyCellsTo(118, 9, 104, 13, 1, 1);
                Engine_MapRedraw();
                WaitFrames(1);
            }
            break;
        case 9:
        case 10:
            if (!Engine_GameFlagIsSet(0x873)) {
                Call3((void (*)())Engine_ActorSetPosition, 3, 174 << 18, 158 << 18);
                Engine_ActorFaceDirection(3, 0, 0);
            } else {
                Call3((void (*)())Engine_ActorSetPosition, 8, 194 << 18, 158 << 18);
                Call6((void (*)())Map_CopyCellAttributeRect, 110, 39, 5, 1, 46, 39);
            }
            break;
        }
    }
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya2) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
            SetEffectRecordMode(Object_GetById(8), 1);
            SetEffectRecordMode(Object_GetById(9), 1);
            if (Engine_GameFlagIsSet(0x302)) {
                WaitFrames(1);
                Audio_PlayCue(211);
                Call3((void (*)())Engine_ActorSetPosition, 8, 184 << 16, 132 << 18);
                Map_CopyCellAttributeRect(11, 31, 1, 4, 9, 31);
                Map_CopyCellAttributeRect(7, 30, 1, 4, 11, 31);
                Engine_MapCopyCellsTo(74, 58, 70, 32, 1, 2);
                Engine_MapCopyCellsTo(74, 59, 70, 34, 1, 1);
                Engine_MapCopyCellsTo(76, 60, 74, 38, 3, 1);
                Engine_MapCopyCellsTo(77, 60, 76, 38, 2, 1);
                Engine_MapCopyCellsTo(75, 58, 86, 41, 1, 3);
                Engine_MapCopyCellsTo(75, 59, 86, 43, 1, 2);
                Engine_MapCopyCellsTo(76, 59, 80, 49, 2, 1);
                Engine_MapCopyCellsTo(77, 59, 82, 49, 2, 1);
            }
            break;
        case 3:
        case 4:
            Engine_MapRedraw();
            WaitFrames(1);
            if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256)) {
                Engine_MapCopyCellsTo(5, 2, 5, 11, 1, 1);
                Engine_MapCopyCellsTo(9, 1, 9, 7, 1, 2);
            }
            if (Engine_GameFlagIsSet(0x874)) {
                Engine_ActorSetPosition(11, 176 << 15, 216 << 16);
                Object_GetById(11)->y.fixed += -0x20000;
                Object_GetById(11)->target_y = Object_GetById(11)->y.fixed;
                Engine_MapCopyCellsTo(9, 1, 9, 7, 1, 2);
                Engine_MapCopyCellsTo(5, 2, 5, 11, 1, 1);
                Call6((void (*)())Map_CopyCellAttributeRect, 9, 5, 1, 1, 9, 10);
            }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            Engine_TaskAddCallback(MakyuriHeya_UpdateLeaderEffectTarget, 0xc80);
#endif
            break;
        case 7:
        case 8:
        case 9:
            SetEffectRecordMode(Object_GetById(10), 1);
            if (Engine_GameFlagIsSet(0x306)) {
                FieldScene_RunSupplementalSequenceOne(0);
                Map_CopyCellAttributeRect(42, 41, 4, 1, 42, 39);
                Map_CopyCellAttributeRect(42, 40, 4, 1, 42, 41);
                Call3((void (*)())Engine_ActorSetPosition, 10, 176 << 18, 160 << 18);
            }
            break;
        }
    }
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya3) {
        switch (gGameState.entrance) {
        case 4:
        case 5:
        case 6:
            SetEffectRecordMode(Object_GetById(15), 1);
            SetEffectRecordMode(Object_GetById(16), 1);
            SetEffectRecordMode(Object_GetById(17), 1);
            SetEffectRecordMode(Object_GetById(18), 1);
            SetEffectRecordMode(Object_GetById(19), 1);
            SetEffectRecordMode(Object_GetById(0), 1);
            i = 0;
            x = 158 << 18;
            flag = 0x330;
            for (; i <= 3; i++) {
                if (Engine_GameFlagIsSet(flag))
                    Engine_ActorSetPosition(i + 15, x, 176 << 15);
                else if (Engine_GameFlagIsSet(flag + 1))
                    Engine_ActorSetPosition(i + 15, x + (128 << 14), 176 << 15);
                x += 128 << 15;
                flag += 2;
            }
            if (Engine_GameFlagIsSet(0x338)) {
                Call3((void (*)())Engine_ActorSetPosition, 19, 230 << 18, 176 << 15);
                Call6((void (*)())Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
            } else if (Engine_GameFlagIsSet(0x339)) {
                Call3((void (*)())Engine_ActorSetPosition, 19, 238 << 18, 176 << 15);
                Call6((void (*)())Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
            }
            break;
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x348)) {
                Call3((void (*)())Engine_ActorSetPosition, 20, 228 << 17, 164 << 17);
                Call6((void (*)())Map_CopyCellAttributeRect, 29, 20, 1, 1, 31, 20);
            }
            break;
        case 3:
        case 13:
            MakyuriHeya_ArriveWithSparks();
        case 1:
        case 2:
        case 12:
            BattleFx_SetQueuedSoundAndPlay(170);
            break;
        case 7:
        case 8:
        case 9:
            if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256)) {
                Object_GetById(0)->y.fixed = -0x20000;
                Object_GetById(0)->target_y = Object_GetById(0)->y.fixed;
                Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
                Engine_MapCopyCellsTo(10, 28, 10, 18, 1, 2);
            }
            if (Engine_GameFlagIsSet(0x878)) {
                Engine_ActorSetPosition(8, 168 << 16, 188 << 17);
                Object_GetById(8)->y.fixed += -0x20000;
                Object_GetById(8)->target_y = Object_GetById(8)->y.fixed;
                Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
                Engine_MapCopyCellsTo(10, 28, 10, 18, 1, 2);
                Call6((void (*)())Map_CopyCellAttributeRect, 10, 16, 1, 1, 10, 19);
                Engine_MapRedraw();
            }
            break;
        case 16:
            WaitFrames(1);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            if (Engine_GameFlagIsSet(0x318))
                Call3((void (*)())Engine_ActorSetPosition, 10, 204 << 18, 152 << 18);
            if (Engine_GameFlagIsSet(0x31a))
                Call3(Engine_ActorSetPosition, 11, 194 << 18, 144 << 18);
            else if (Engine_GameFlagIsSet(0x319))
                Call3(Engine_ActorSetPosition, 11, 190 << 18, 144 << 18);
            else if (Engine_GameFlagIsSet(0x31b))
                Call3(Engine_ActorSetPosition, 11, 214 << 18, 144 << 18);
#else
            Call3((void (*)())Engine_ActorSetPosition, 10, 204 << 18, 152 << 18);
            Engine_ActorSetPosition(11, 194 << 18, 144 << 18);
#endif
            SetEffectRecordMode(Object_GetById(0), 1);
            SceneEffect_SpawnParticleRowsByMode(0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            if (Engine_GameFlagIsSet(0x319)) {
                MakyuriHeya_OpenStoneDoor(2);
                if (Object_GetById(9)->z.fixed >> 20 == 44)
                    Engine_TaskAddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
            } else if (Engine_GameFlagIsSet(0x31a)) {
                MakyuriHeya_OpenStoneDoor(1);
            } else if (!Engine_GameFlagIsSet(0x31b)) {
                MakyuriHeya_OpenStoneDoor(0);
            }
#else
            MakyuriHeya_OpenStoneDoor(1);
#endif
        case 14:
            if (gGameState.entrance == 14)
                Audio_PlayCue(211);
            SetEffectRecordMode(Object_GetById(9), 1);
            Engine_ActorSetSpritePriority(10, 2);
            Object_GetById(10)->unknown_22 = 2;
            SetEffectRecordMode(Object_GetById(11), 1);
            SetEffectRecordMode(Object_GetById(12), 1);
            SetEffectRecordMode(Object_GetById(13), 1);
            SetEffectRecordMode(Object_GetById(14), 1);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            if (gGameState.entrance != 14)
                break;
            if (!Engine_GameFlagIsSet(0x109)) {
                Engine_GameFlagClear(0x318);
                Engine_GameFlagClear(0x319);
                Engine_GameFlagClear(0x31a);
                Engine_GameFlagClear(0x31b);
                break;
            }
            SceneEffect_SpawnParticleRowsByMode(0);
            if (Engine_GameFlagIsSet(0x319)) {
                MakyuriHeya_OpenStoneDoor(2);
                if (Object_GetById(9)->z.fixed >> 20 == 44)
                    Engine_TaskAddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
            } else if (Engine_GameFlagIsSet(0x31a)) {
                MakyuriHeya_OpenStoneDoor(1);
            } else if (!Engine_GameFlagIsSet(0x31b)) {
                MakyuriHeya_OpenStoneDoor(0);
            }
            break;
        case 15:
            Engine_EventBegin();
            SetEffectRecordMode(Object_GetById(8), 1);
            Engine_ActorSetChildValue(0, 15);
            Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
            Object_GetById(8)->y.fixed = 144 << 16;
            Object_GetById(8)->motion_flags = 0;
            *(s32 *)&Object_GetById(8)->unknown_44[0] = 0;
            *(s32 *)&Object_GetById(8)->unknown_44[4] = 0x4ccc;
            gEventWork->start_transition = 0x100;
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Object_GetById(8)->motion_flags = 3;
            Audio_PlayCue(189);
            Battle_WaitMode0(32);
            Audio_PlayCue(188);
            SetEffectRecordMode(Object_GetById(8), 2);
            Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 192 << 10, 192 << 10, 128 << 9);
            Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapRenderWaitForValues();
            Battle_WaitMode0(60);
            Engine_EventRequestExit(16);
            Engine_EventEnd();
            break;
        }
    }
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya4) {
        switch (gGameState.entrance) {
        case 10:
            Engine_TaskAddCallback(SceneEffect_RotatePaletteEntries97To103, 0xc80);
            if (Engine_GameFlagIsSet(0x109))
                break;
            MakyuriHeya_ArriveWithSparks();
            BattleFx_SetQueuedSoundAndPlay(170);
            Call2((void (*)())Engine_ColorBufferApplySource, 128 << 9, 0);
            Engine_ColorBufferApplyTarget(0x10003, 1);
            Engine_ColorBufferInterpolate(30);
            Engine_EventWaitForScreen();
            Object_SetModeById(0, 1);
            Battle_WaitMode0(30);
            UiText_ShowCenteredMessage((s32)MsgMakyuriHonorsGoddessRainbows, 0, 0);
            Engine_ColorBufferApplyTarget(128 << 9, 0);
            Engine_ColorBufferInterpolate(30);
            break;
        case 15:
            Engine_ActorSetPosition(3, 0, 0);
            DisplayBlend_EnableRunScript();
            /* FAKEMATCH: the do/while keeps this zero from being shared, as a pool halfword, with the later zero stores. */
            do { s32 z = 0; *(volatile u16 *)0x04000050 = z; } while (0);
            Object_GetById(9)->scale_x = -0x10000;
            Engine_ActorSetSpritePriority(14, 1);
            Engine_ActorSetSpritePriority(15, 1);
            Engine_ActorSetSpritePriority(16, 1);
            set = Engine_GameFlagIsSet(0x109);
            if (set) {
                Call6((void (*)())Map_CopyCellAttributeRect, 104, 34, 5, 4, 40, 34);
                Call6((void (*)())Engine_MapCopyCellsTo, 45, 91, 40, 91, 5, 4);
                break;
            }
            Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
            Call6((void (*)())Map_CopyCellAttributeRect, 116, 36, 3, 4, 52, 36);
            Object_GetById(10)->motion_flags = set;
            SetEffectRecordMode(Object_GetById(10), 1);
            Engine_EventBegin();
            Call4((void (*)())Engine_CameraMoveTo, -1, -1, -1, 0);
            Engine_EventGetViewCenter()->motion_flags = set;
            Engine_ActorSetSpritePriority(0, 1);
            Engine_ActorSetSpritePriority(13, 1);
            MakyuriHeya_RideLift();
            DisplayBlend_DisableRunScript();
            Engine_EventEnd();
            break;
        case 12:
            Battle_WaitMode0(1);
            SetEffectRecordMode(Object_GetById(0), 1);
        case 11:
            Call6((void (*)())Map_CopyCellAttributeRect, 104, 34, 5, 4, 40, 34);
            Engine_MapCopyCellsTo(45, 91, 40, 91, 5, 4);
            if (!Engine_GameFlagIsSet(0x881)) {
                Engine_MapCopyCellsTo(30, 45, 50, 45, 5, 6);
                Engine_MapCopyCellsTo(50, 105, 50, 109, 5, 3);
                Engine_MapRedraw();
                WaitFrames(1);
            } else {
                Engine_ActorSetSpritePriority(14, 1);
                Engine_ActorSetSpritePriority(15, 1);
                Engine_ActorSetSpritePriority(16, 1);
            }
            Object_GetById(9)->scale_x = -0x10000;
            if (!Engine_GameFlagIsSet(0x82b)) {
                Engine_ActorSetPosition(3, 0, 0);
                Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
                Engine_ActorSetSpritePriority(10, 1);
                Call6((void (*)())Map_CopyCellAttributeRect, 116, 37, 3, 3, 52, 37);
                Call6((void (*)())Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
                Engine_TaskAddCallback(SceneEffect_RotatePaletteEntries40To47, 0xc80);
                break;
            }
            if (!Engine_GameFlagIsSet(0x871)) {
                if (!Engine_GameFlagIsSet(0x870)) {
                    Engine_ActorFaceDirection(3, 0, 0);
                    Object_SetModeById(3, 16);
                } else {
                    Call3((void (*)())Engine_ActorSetPosition, 3, 210 << 18, 158 << 18);
                    Call3((void (*)())Engine_ActorFaceDirection, 3, 192 << 8, 0);
                }
                SetEffectRecordMode(Object_GetById(3), 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
                Engine_TaskAddCallback(SceneEffect_RotatePaletteEntries40To47, 0xc80);
            } else {
                Engine_ActorSetPosition(3, 0, 0);
                Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
                Call6((void (*)())Map_CopyCellAttributeRect, 116, 36, 3, 4, 52, 36);
                Object_GetById(10)->collision_flags = 254;
                Battle_WaitMode0(1);
            }
            Object_GetById(10)->motion_flags = 0;
            SetEffectRecordMode(Object_GetById(10), 1);
            break;
        }
    }
    return 0;
}

/* Mercury Lighthouse: the girl the party saved asks for the truth until
 * it is told, then joins with her Djinni. */
void MakyuriHeya_RunPartyScene(void)
{
    s32 base;
    s32 position;
    s32 event;
    u8 *actor;

    position = *(s32 *)((u8 *)Actor_Get(8) + 8) / 0x100000;
    if (position != 48) {
        return;
    }
    Engine_EventBegin();
    base = (s32)MsgMakyuriSavedMeAgain;
    Engine_EventSetMessage(base);
    Battle_WaitMode0(20);
    Engine_ActorRunRepeatedMotion(3, 1);
    Actor_FaceDirection(0, 32768, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Engine_ActorSetAnimationAndWait(3, 3);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Battle_WaitMode0(20);
    Battle_WaitMode0(60);
    Object_SetModeById(3, 16);
    Battle_WaitMode0(50);
    Object_SetModeById(3, 1);
    Engine_EventOpenMessage(3, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Battle_WaitMode0(20);
        Engine_ActorRunRepeatedMotion(3, 2);
        Battle_WaitMode0(20);
        Event_ShowMessageAndWait(3, 0, 20);
        Engine_ActorSetAnimationAndWait(3, 4);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Engine_EventOpenMessage(3, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Battle_WaitMode0(20);
            ((s32 (*)())Engine_ActorSetAnimationAndWait)(3, 4);
            Battle_WaitMode0(20);
            event = base + 5;
            for (;;) {
                Engine_EventSetMessage(event);
                Engine_EventOpenMessage(3, 0);
                if (Engine_EventChooseYesNo(0, 0) != 1) {
                    break;
                }
                Battle_WaitMode0(20);
                ((s32 (*)())Engine_ActorSetAnimationAndWait)(3, 4);
                Battle_WaitMode0(20);
                event = (s32)MsgMakyuriDontHideTruth;
            }
        }
    }
    Engine_EventSetMessage((s32)MsgMakyuriThoughtSo);
    Actor_SetSpeed(3, 52428, 26214);
    Engine_ActorWalkToAndWait(3, 728, 632);
    Battle_WaitMode0(20);
    Engine_EventShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 16);
    Engine_EventShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 1);
    Engine_ActorFaceActor(3, 0, 20);
    Engine_ActorSetAnimationAndWait(3, 4);
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_ShowEmote(3, 261, 90);
    Engine_ActorSetAnimationAndWait(3, 3);
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Event_PrepareObjectAndApplyValue(3, 1);
    Engine_GameFlagSet(68);
    Djinn_AddToOwner(3, 1, 0);
    Djinn_Activate(3, 1, 0);
    Owner_RecalculateStats(3);
    Object_SetModeById(3, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(3);
    Engine_ActorSetPosition(3, 0, 0);
    Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
    Engine_GameFlagSet(2163);
    Engine_EventEnd();
}

/* The lift rises into the room with the leader and actor 13 on it, easing to
 * a stop; the leader hops off onto the landing, and the lift sinks back down
 * the shaft, speeding up, before the landing's cells close over it. */
void MakyuriHeya_RideLift(void)
{
    struct LiftLayer *layer;
    s32 speed;

    layer = (struct LiftLayer *)(((u8 *)gMapWork[0]) + 356);
    speed = 0x9c28;
    layer->offset = 0x04890000;
    layer->unknown_1c = 0;
    Actor_Get(0)->motion_flags = 0;
    Actor_Get(0)->z.fixed += -0x890000;
    Actor_Get(0)->target_z = Actor_Get(0)->z.fixed;
    Actor_Get(13)->motion_flags = 0;
    Actor_SetPosition(13, 170 << 18, 220 << 17);
    Actor_Get(13)->z.fixed += -0x890000;
    Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
    Engine_MapRedraw();
    WaitFrames(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_WaitMode0(60);
    DisplayBlend_EnableRunScript();
    Audio_PlayCue(223);
    for (;;) {
        layer->offset -= speed;
        Actor_Get(0)->z.fixed += speed;
        Actor_Get(0)->target_z = Actor_Get(0)->z.fixed;
        Actor_Get(13)->z.fixed += speed;
        Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
        if (layer->offset > 0x04000000) {
            if ((gFrameCount & 15) == 0 && speed > 0xccb)
                speed += -0x560;
            WaitFrames(1);
            continue;
        }
        break;
    }
    layer->offset = 0x04000000;
    Engine_MapRedraw();
    WaitFrames(2);
    Actor_Get(0)->motion_flags = 3;
    Actor_Get(13)->z.fixed = 220 << 17;
    Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
    Battle_WaitMode0(30);
    Actor_WalkToAndWait(0, 704, 584);
    *(s32 *)Actor_Get(0)->unknown_44 = 0;
    Object_SetModeById(0, 6);
    Battle_WaitMode0(6);
    Object_SetModeById(0, 7);
    Actor_Get(0)->speed = 0x30000;
    Actor_Get(0)->acceleration = 0x20000;
    Audio_PlayCue(152);
    Actor_Get(0)->velocity_y = 0x40000;
    Engine_ActorSetSpriteFlags(Actor_Get(0), 0);
    Actor_SetDestination(0, 736, 584);
    ObjectMotion_CommitCurrentPositionAndActivate(0);
    Engine_ActorSetSpriteFlags(Actor_Get(0), 1);
    *(s32 *)Actor_Get(0)->unknown_44 = 0x4000;
    Object_SetModeById(0, 6);
    Battle_WaitMode0(6);
    Actor_FaceDirection(0, 0x8000, 30);
    Audio_PlayCue(223);
    for (;;) {
        layer->offset += speed;
        Actor_Get(13)->z.fixed -= speed;
        Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
        if (layer->offset >= 0x04890000)
            break;
        if ((gFrameCount & 7) == 0 && speed <= 0xcccc)
            speed += 0x1999;
        WaitFrames(1);
    }
    layer->offset = 0x04000000;
    Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
    Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
    Engine_MapRedraw();
    WaitFrames(2);
    Actor_SetPosition(13, 0, 0);
    Battle_WaitMode0(30);
    Actor_Get(0)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
}

void MakyuriHeya_CastPsynergyAtActor11(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)gEffectWork;
    Actor_SetPosition(11, 0x3480000, 0x2580000);
    Engine_PsynergyBegin(93, 1);
    Engine_PsynergySetTarget(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
}

void SceneState_SetRecordTableValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->tbl[index] = value;
        }
    }
}
