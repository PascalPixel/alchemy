#ifndef ALCHEMY_UNIT_FIELD_TORETO_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_TORETO_HEYA_HEYA_H

#include "types.h"

struct Effect;
struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct Swirl;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind);
void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void ToretoHeya_HandleFloorSwitch(s32 flag, s32 src_x, s32 src_y, s32 entrance);
void FieldScene_RunStep200(void);
void FieldScene_RunStep201(void);
void FieldScene_RunStep202(void);
void FieldScene_RunStep203(void);
void FieldScene_RunStep204(void);
void FieldScene_RunStep205(void);
void FieldScene_RunStep206(void);
void FieldScene_RunStep207(void);
void FieldScene_RunStep208(void);
void FieldScene_RunStep209(void);
void FieldScene_RunStep20a(void);
void FieldScene_RunStep20b(void);
void FieldScene_RunStep20c(void);
void FieldScene_RunStep20d(void);
void FieldScene_RunStep20e(void);
void FieldScene_RunStep20f(void);
void FieldScene_RunStep210(void);
void FieldScene_RunStep211(void);
void FieldScene_RunStep212(void);
void FieldScene_RunStep213(void);
void FieldScene_RunStep214(void);
void SceneState_ClearStoryVariantWhenIdle(void);
const struct SceneEvent *Scene_GetEvents(void);
void ToretoHeya_RunTableScene(void);
void FieldScene_RunFourActorEncounter(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
s32 ToretoHeya_EnterRoom(void);
void ToretoPalette_CaptureBank(void);
void ToretoPalette_ApplyTint(void);
void ToretoHeya_PlayGesture(s32 gesture);
void ToretoHeya_UpdateSwirlObject(struct Swirl *obj);
void ToretoHeya_SpawnSwirlSparks(void);
void ToretoHeya_ApplyFlaggedMapPatches(void);
void SceneEffect_RegisterPaletteFadeCallback(void);
void ToretoHeya_AdvanceEffectMotion(struct Effect *effect);
void ToretoHeya_RunLandingDustScene(void);

#endif
