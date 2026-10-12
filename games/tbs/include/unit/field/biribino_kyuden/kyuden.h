#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_KYUDEN_KYUDEN_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_KYUDEN_KYUDEN_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void Kyuden_AskAboutKolima(void);
void FieldScene_RunStartledGuard(void);
void FieldScene_RunScene38dSequenceA(void);
void SceneDialogue_RunActor16Message1769(void);
void FieldScene_RunActorSeventeenFlaggedDialogue(void);
void SceneDialogue_RunActor15Flag303Scene(void);
void FieldScene_RunActorSeventeenFlagDialogue(void);
void BiribinoKyuden_RunDoorExitScene(void);
void FieldScene_ShowChestEmpty(void);
void FieldScene_ShowChestLocked(void);
void FieldScene_RunChestStep(s32 flag);
void FieldScene_RunStep210ByFlag84e(void);
void FieldScene_RunStep211ByFlag84e(void);
void FieldScene_RunStep212ByFlag84e(void);
void FieldScene_RunStep213ByFlag84e(void);
void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag);
void FieldScene_RunRewardReminder(void);
void FieldScene_RunPalaceFarewell(void);
s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1);
void Kyuden_RunKolimaRequest(void);
void FieldScene_RunPalaceGreeting(void);
void ConfigurePrimarySceneChannels(void);
void ConfigureSecondarySceneChannels(void);
void RunEventScript02(void);
void BiribinoKyuden_RunActorRowScene(void);
extern s32 Kyuden_FacingActions[];
extern s32 Kyuden_PacingActions[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoKyudenPlacementsOther[];
extern struct ScenePlacement gBiribinoKyudenPlacements[];
extern struct SceneEvent gBiribinoKyudenEventsOther[];
extern struct SceneEvent gBiribinoKyudenEvents[];

#endif
