/* NONMATCHING: localized scene source, 2026-10-01.
 * The complete 124-byte getter sets only flag 90a on entrance five.
 * ES/FR/IT also set 87a; their complete getter is 136 bytes.
 */
#include "RAMAKAN.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgShianJiinIDoNotThinkMasterHama);
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "IWRAM_CALL.H"

extern u32 gFrameCount;
void Engine_AudioPlayCue();
s32 Engine_RandomNext();
void Effect_Spawn();

struct EffectParams {
    u8 pad00[8];
    s32 scaleX;
    s32 scaleY;
    u8 pad10[18];
    u16 angle;
    u8 pad24[4];
};

extern const struct SceneEntrance gRamakanSabakuEntrances1[];
extern const struct SceneEntrance gRamakanSabakuEntrances2[];
extern const struct SceneEntrance gRamakanSabakuEntrances3[];
extern const struct SceneEntrance gRamakanSabakuEntrances4[];
extern const struct SceneEntrance gRamakanSabakuEntrancesOther[];

extern const u32 RamakanSabaku_Exits[];

extern const struct ScenePlacement gRamakanSabakuPlacements1[];
extern const struct ScenePlacement gRamakanSabakuPlacements2[];
extern const struct ScenePlacement gRamakanSabakuPlacements3[];
extern const struct ScenePlacement gRamakanSabakuPlacementsOther[];

extern const struct SceneEvent RamakanSabaku_Events[];

void Engine_GameFlagClear();
void Engine_MapCopyCells();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsLayered();
void Engine_ActorSetPosition();
s32 Engine_DisplayScrollStartHBlankDma();

s32 Engine_GameFlagIsSet();
s32 BattleFx_EmitRandomParticle();
s32 DisplayScroll_DisarmHBlankDma();

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        if (gGameState.entrance == 5) {
            GameFlag_Set(0x90a);
        }
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        return gRamakanSabakuPlacements1;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        return gRamakanSabakuPlacements2;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        return gRamakanSabakuPlacements3;
    }
    return gRamakanSabakuPlacementsOther;
}
