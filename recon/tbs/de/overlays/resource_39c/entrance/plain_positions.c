/* NONMATCHING: localized room entry before call and callback refinements, 2026-10-01.
 * The complete 3444-byte initializer compiles with the approved TBS flags.
 * 10 instruction bytes differ: direct position calls schedule the
 * actor argument after the shifts. Production preserves
 * the measured Call3 spelling and registers the later callback on both paths.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PROBE.H"
#include "TYPES.H"
#include "MAKYURI.H"
#include "CALL.H"
#include "MAKYURI_HEYA.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

void MakyuriHeya_UpdateLeaderEffectTarget(void);

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
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                Engine_TaskAddCallback(MakyuriHeya_UpdateLeaderEffectTarget, 0xc80);
#endif
            }
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
#if !defined(TBS_EDITION_EN) && !defined(TBS_EDITION_JA)
            if (Engine_GameFlagIsSet(0x318))
                Call3((void (*)())Engine_ActorSetPosition, 10, 204 << 18, 152 << 18);
            if (Engine_GameFlagIsSet(0x31a))
                Engine_ActorSetPosition(11, 194 << 18, 144 << 18);
            else if (Engine_GameFlagIsSet(0x319))
                Engine_ActorSetPosition(11, 190 << 18, 144 << 18);
            else if (Engine_GameFlagIsSet(0x31b))
                Engine_ActorSetPosition(11, 214 << 18, 144 << 18);
#else
            Call3((void (*)())Engine_ActorSetPosition, 10, 204 << 18, 152 << 18);
            Engine_ActorSetPosition(11, 194 << 18, 144 << 18);
#endif
            SetEffectRecordMode(Object_GetById(0), 1);
            SceneEffect_SpawnParticleRowsByMode(0);
#if !defined(TBS_EDITION_EN) && !defined(TBS_EDITION_JA)
            if (Engine_GameFlagIsSet(0x319)) {
                FieldScene_RunPrimarySequence(2);
                if (Object_GetById(9)->z.fixed >> 20 == 44)
                    Engine_TaskAddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
            } else if (Engine_GameFlagIsSet(0x31a)) {
                FieldScene_RunPrimarySequence(1);
            } else if (!Engine_GameFlagIsSet(0x31b)) {
                FieldScene_RunPrimarySequence(0);
            }
#else
            FieldScene_RunPrimarySequence(1);
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
                FieldScene_RunPrimarySequence(2);
                if (Object_GetById(9)->z.fixed >> 20 == 44)
                    Engine_TaskAddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
            } else if (Engine_GameFlagIsSet(0x31a)) {
                FieldScene_RunPrimarySequence(1);
            } else if (!Engine_GameFlagIsSet(0x31b)) {
                FieldScene_RunPrimarySequence(0);
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
