#ifndef ALCHEMY_UNIT_FIELD_SUHARA_SABAKU_SABAKU_H
#define ALCHEMY_UNIT_FIELD_SUHARA_SABAKU_SABAKU_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
void SceneState_SendRequest15With45(void);
s32 SuharaSabaku_ClearActorSpriteAndCollisionFlags(u8 *obj);
s32 SuharaSabaku_ToggleActorActive(u8 *obj);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SuharaSabaku_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void SuharaSabaku_SyncSelectedActorProgress(void);
void FieldScene_RunMiddleAuxiliarySequence(s32 a0);
void FieldScene_RunActor8Step(void);
void FieldScene_RunActor9Step(void);
void FieldScene_RunActor10Step(void);
void FieldScene_RunActor11Step(void);
void FieldScene_RunActor12Step(void);
void SuharaSabaku_RestoreActorScaleAndBlend(s32 a0);
void FieldScene_RunLateActor8Step(void);
void FieldScene_RunLateActor9Step(void);
void FieldScene_RunLateActor10Step(void);
void FieldScene_RunLateActor11Step(void);
void FieldScene_RunLateActor12Step(void);
void FieldScene_RunActorThirteenRestoration(void);
s32 SuharaSabaku_FollowLeaderWithSparks(struct FieldActor *actor);
void PlaceActorTwelveWhenFlagClear(void);
void SuharaSabaku_EnableEffectChildOption(void);
void SuharaSabaku_MeetActor(s32 a0, s32 actor);
void SuharaSabaku_DropAndLeave(void);
void EncounterPalette_Pulse(void);
s32 FieldScene_RunOpeningAuxiliarySequence(void);
s32 FieldScene_RunScene3c0SequenceA(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 SuharaSabaku_RunSceneScript(void);
void SuharaSabaku_ApplyVisitFlagBlend(void);
void SuharaSabaku_ApplyAltarFlagBlend(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gSuharaSabakuPlacements1_Data[];
extern s32 gSuharaSabakuPlacements2_1[];
extern struct SceneEntrance gSuharaSabakuEntrancesOther[];
extern struct SceneEntrance gSuharaSabakuEntrances1[];
extern struct SceneEntrance gSuharaSabakuEntrances2[];
extern struct SceneEntrance gSuharaSabakuEntrances3[];
extern u32 gSuharaSabakuExits[];
extern struct ScenePlacement gSuharaSabakuPlacementsOther[];
extern struct ScenePlacement gSuharaSabakuPlacements1[];
extern struct ScenePlacement gSuharaSabakuPlacements2[];
extern struct ScenePlacement gSuharaSabakuPlacements3[];
extern s32 gSuharaSabakuActor12Action[];
extern struct SceneEvent gSuharaSabakuEventsOther[];
extern struct SceneEvent gSuharaSabakuEvents3[];

#endif
