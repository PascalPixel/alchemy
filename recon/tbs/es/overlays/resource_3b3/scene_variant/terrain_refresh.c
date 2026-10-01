/* NONMATCHING: localized scene source, 2026-10-01.
 * The complete 94-byte scene lacks the European leader-height refresh.
 * ES/FR/IT query terrain height before the probe; their function is 148 bytes.
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

void FieldScene_RunScene3b3_02001fd4(void)
{
    Engine_EventBegin();
    if (FieldScene_RunScene3b3SequenceD() == 0) {
        *((u8 *)Object_GetById(0) + 85) &= 254;
        *((u8 *)Object_GetById(0) + 35) &= 254;
        StagedActor_AdvancePair();
        TakaraHashira_UpdatePillarActors();
        {
            u8 bits = 1;
            u8 *flags = (u8 *)Actor_Get(ACTOR_PARTY_LEADER) + 85;
            u8 value = *flags;

            value |= bits;
            *flags = value;
            flags = (u8 *)Actor_Get(ACTOR_PARTY_LEADER) + 35;
            bits |= *flags;
            *flags = bits;
        }
    }
    Engine_EventEnd();
}
