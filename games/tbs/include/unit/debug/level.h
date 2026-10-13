#ifndef ALCHEMY_UNIT_DEBUG_LEVEL_H
#define ALCHEMY_UNIT_DEBUG_LEVEL_H

#include "types.h"
#include "field_scene.h"

u8 *ItemLevel_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ItemLevel_GetExits(void);
u8 *ItemLevel_GetPlacements(void);
u8 *ItemLevel_GetEvents(void);
s32 FieldScene_RunEntrySetup(void);
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
