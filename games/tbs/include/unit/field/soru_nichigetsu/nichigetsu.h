#ifndef ALCHEMY_UNIT_FIELD_SORU_NICHIGETSU_NICHIGETSU_H
#define ALCHEMY_UNIT_FIELD_SORU_NICHIGETSU_NICHIGETSU_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunScene37aSequenceA(void);
void FieldScene_RunScene37aSequenceB(void);
void FieldScene_RunScene37aSequenceC(void);
void FieldScene_RunScene37aSequenceD(void);
void ClearSolShindenBackdrop(void);
void SetStatueLightGroup1(void);
void SetStatueLightGroup2(void);
void SetStatueLightGroup3(void);
void SetStatueLightGroup4(void);
void FieldScene_RunScene37aSequenceF(void);
void Scene_EnterInnerSanctum(void);
void UpdateStatueTrapActor(void);
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);
void FieldScene_PrepareStatueTransition(void);
void Scene_SpringStatueTrap(void);
void FieldScene_RunClosingSequence(void);
void FieldScene_RunFlaggedSequence(void);
void Scene_ChangeLunaPictureToSol(void);
void FieldScene_RunActorPositionTransition(void);
void FieldScene_RunSanctumRiseAndShrink(void);
s32 SoruNichigetsu_RestoreEntryState(void);
s32 CheckAllStatueLights(void);
void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);
void SoruNichigetsu_RunLightScene(void);
void Sukureta_Talk(void);
extern struct SceneEntrance gSoruNichigetsuEntrances[];
extern u32 gSoruNichigetsuExits[];
extern struct ScenePlacement gSoruNichigetsuPlacements[];
extern struct SceneEvent gSoruNichigetsuEvents[];

#endif
