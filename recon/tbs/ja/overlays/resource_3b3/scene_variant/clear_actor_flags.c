/* NONMATCHING: localized scene source, 2026-10-01.
 * The complete 736-byte setup retains localized actor-eight/nine flag clears.
 * Japanese omits those clears; its complete setup is 716 bytes.
 */
#include "HASHIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "FIELD_SCENE.H"

void FieldScene_RunScene3b3_02001fd4(void);
s32 FieldScene_RunScene3b3SequenceD(void);
void FieldEffect_UpdateGridPlacement(void);

void Engine_ObjectSetPosition();
void Engine_ActorWaitForMove();
void Engine_MapCopyCellAttributes();

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_MapCopyCellsTo();
s32 Engine_TaskAddCallback();
void Engine_ActorSetAnimation();
void ObjectMotion_WaitForAnimationChange();
void Engine_GameFlagClear();
void Engine_ActorSetChildValue();
void Engine_ActorEnableActionCallback();
void Engine_EventEnd();
void SceneEffect_SpawnRandomizedParticle();

s32 StagedActor_FillGridAttributeRectangle();
void Engine_AudioPlayCue();

struct Probe {
    s32 word[6];
};

void StagedActor_PlaceAtObjectTenCell();
void StagedActor_AdvancePair();
void TakaraHashira_DropActorTen();

extern const struct SceneEvent gTakaraHashiraEvents1[];
extern const struct SceneEvent gTakaraHashiraEvents2[];
extern const struct SceneEvent gTakaraHashiraEvents3[];
extern const struct SceneEvent gTakaraHashiraEvents4[];
extern const struct SceneEvent gTakaraHashiraEvents5[];
extern const struct SceneEvent gTakaraHashiraEventsOther[];

extern u8 TakaraHashira_PillarSlots[];
void SceneActor_ApplyPlacementQueryAndTag();
void BattleFx_StartFadeOverlay();
void SceneState_ClearWord24AndObjectByte62();
void OverlayObject_SetCallbackAndMode2();
void SceneActor_CheckActors8To11NearSlotZero(void);
void TakaraHashira_PrepLoweredActor();
void FieldScene_RunScene3b3_0200263c();

s32 TakaraHashira_SetupArea(void)
{
    u32 i;
    u8 *record;
    s32 v1;
    s32 v2;
    s32 base5_8;
    s32 v5;
    s32 v0;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraHashira2) {
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
    } else {
        if (gGameState.scene == (s32)&SceneId_TakaraHashira3) {
            Call6(Engine_MapCopyCells, 32, 0, 64, 32, 0, 64);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            FieldScene_RedrawActorFootprint(12);
            FieldScene_RedrawActorFootprint(13);
            FieldScene_RedrawActorFootprint(14);
            FieldScene_RedrawActorFootprint(15);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            if (Engine_GameFlagIsSet(0x200) == 0) {
                goto L_020029fa;
            }
            Call6(Engine_MapCopyCellsTo, 79, 34, 84, 24, 1, 2);
            Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
            Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
            SceneActor_ApplyPlacementQueryAndTag(9);
            SceneActor_ApplyPlacementQueryAndTag(10);
            SceneActor_ApplyPlacementQueryAndTag(11);
            SceneActor_ApplyPlacementQueryAndTag(12);
            SceneActor_ApplyPlacementQueryAndTag(13);
            SceneActor_ApplyPlacementQueryAndTag(14);
            SceneActor_ApplyPlacementQueryAndTag(15);
            Call6(Engine_MapCopyCellAttributes, 24, 3, 1, 1, 24, 8);
            goto L_020029fa;
        } else {
            if (gGameState.scene != (s32)&SceneId_TakaraHashira4) {
                goto L_02002922;
            }
            OverlayObject_CreateConfiguredObject(0x2480000, 0, 0xc80000, 223);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                *((u8 *)Object_GetById(0) + 98) = 1;
            }
            BattleFx_StartFadeOverlay(0);
            if (*((u8 *)Object_GetById(0) + 98) == 0) {
                SceneState_ClearWord24AndObjectByte62();
            }
            OverlayObject_SetCallbackAndMode2(8);
            OverlayObject_SetCallbackAndMode2(9);
            OverlayObject_SetCallbackAndMode2(10);
            OverlayObject_SetCallbackAndMode2(11);
            {
                s32 *entry = (s32 *)TakaraHashira_PillarSlots;

                for (v1 = 0; (u32)v1 <= 3; v1++) {
                    entry[0] = 0;
                    entry[1] = 0;
                    entry[2] = 0;
                    entry[4] = v1 + 0x200;
                    entry += 5;
                }
            }
            TakaraHashira_UpdatePillarActors();
            Engine_EventWait(1);
            ((void (*)())Engine_TaskAddCallback)((s32)SceneActor_CheckActors8To11NearSlotZero, 0xc80);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            for (base5_8 = 8; (u32)base5_8 <= 11; base5_8++) {
                record = (u8 *)Object_GetById(base5_8);
                v2 = *(s32 *)(record + 8) >> 20;
                if (v2 == 37) {
                    v0 = *(s32 *)(record + 16) >> 20;
                    if (v0 == 9) {
                        Engine_MapCopyCellAttributes(27, 8, 1, 1, v2, v0);
                        break;
                    }
                }
            }
        }
        goto L_020029fa;
        L_02002922:;
        if (gGameState.scene == (s32)&SceneId_TakaraHashira5) {
            Engine_ActorSetAnimation(10, 2);
            Engine_ActorSetChildValue(10, 6);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            v5 = 0;
            *((u8 *)Object_GetById(8) + 85) = v5;
            *((u8 *)Object_GetById(9) + 85) = v5;
            TakaraHashira_DropActorTen();
            TakaraHashira_PrepLoweredActor(11);
            TakaraHashira_PrepLoweredActor(12);
            TakaraHashira_PrepLoweredActor(13);
            FieldScene_RunScene3b3_0200263c(11);
            FieldScene_RunScene3b3_0200263c(12);
            FieldScene_RunScene3b3_0200263c(13);
            record = (u8 *)Object_GetById(13);
            *(s32 *)((s32)record + 108) = v5;
            TakaraHashira_PrepLoweredActor(14);
            {
                u8 *record = (u8 *)Object_GetById(14);
                /* FAKEMATCH: retain the flag read before its merge. */
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 8);
            }
            if (Engine_GameFlagIsSet(0x202) == 0) {
                v5 = 192;
                record = (u8 *)Object_GetById(13);
                *(s32 *)((s32)record + 24) = (v5 << 9);
                record = (u8 *)Object_GetById(13);
                *(s32 *)((s32)record + 28) = (v5 << 9);
                record = (u8 *)Object_GetById(13);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                record = (u8 *)Object_GetById(14);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                Call6(Engine_MapCopyCellAttributes, 26, 12, 1, 1, 22, 16);
            }
        }
    }
    L_020029fa:;
    return 0;
}
