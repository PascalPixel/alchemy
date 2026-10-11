#ifndef ALCHEMY_UNIT_DEBUG_ITEM_LEVEL_LEVEL_H
#define ALCHEMY_UNIT_DEBUG_ITEM_LEVEL_LEVEL_H

#include "types.h"
#include "field_scene.h"

u8 *ItemLevel_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ItemLevel_GetExits(void);
u8 *ItemLevel_GetPlacements(void);
void FieldScene_RunActor13Mode102Step(void);
void FieldScene_RunActor13Mode105Step(void);
u8 *ItemLevel_GetEvents(void);
void SceneState_AddToRecordCount(s32 id, s32 amount);
void Scene_AddToListedRecordCounts(s32 amount);
void FieldScene_RunCountAdjustPanel(void);
void ItemLevel_SelectItem(void);
void ItemLevel_SelectAbility(void);
void SceneState_SetWorkWords1c0And1c8(void);
s32 FieldScene_RunEntrySetup(void);
void ItemLevel_RunMotionTest(void);
void FieldScene_DrawThreeCaptionWindow(void);
void SceneState_SetRecordFlag53(void);
int SceneState_GetFarResult100c(void);
int SceneState_GetFarResult1020(void);
extern char gItemLevelLvLabel[4];
extern char gItemLevelItemPrompt[12];
extern char gItemLevelItemHelp[24];
extern char gItemLevelItemFull[20];
extern char gItemLevelPsyPrompt[8];
extern char gItemLevelPsyHelp[12];
extern char gItemLevelGlyphsUpper[28];
extern char gItemLevelGlyphsLower[28];
extern char gItemLevelGlyphsMarks[12];
extern struct SceneEntrance gItemLevelEntrances[];
extern u32 gItemLevelExits[];
extern struct ScenePlacement gItemLevelPlacements[];
extern struct SceneEvent gItemLevelEvents[];

#endif
