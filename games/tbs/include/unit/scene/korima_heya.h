#ifndef ALCHEMY_UNIT_SCENE_KORIMA_HEYA_H
#define ALCHEMY_UNIT_SCENE_KORIMA_HEYA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
u8 *SceneData_GetPreparedRecords(void);
u8 *SceneData_GetFourthTable(void);
s32 FieldScene_SetupEntryLayoutsBySelector(void);
extern struct SceneEntrance EntryLayout_PrimaryTable[];
extern u32 EntryLayout_SecondaryTable[];
extern struct ScenePlacement EntryLayout_Records[];
extern struct SceneEvent EntryLayout_FourthTable[];

#endif
