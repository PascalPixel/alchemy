#ifndef ALCHEMY_UNIT_SCENE_TOREBI_HEYA_H
#define ALCHEMY_UNIT_SCENE_TOREBI_HEYA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *TorebiHeya_GetEntrances(void);
s32 TorebiHeya_GetDefaultResult(void);
u8 *TorebiHeya_GetExits(void);
s32 TorebiHeya_SelectPlacements(void);
u8 *TorebiHeya_SelectEvents(void);
s32 Scene_InitFacingActors(void);
extern s32 Data_02009040_Data[];
extern struct SceneEntrance Data_02008bb4[];
extern u32 Data_02008dac[];
extern struct ScenePlacement Data_02008e00[];
extern struct ScenePlacement Data_02009040[];
extern struct SceneEvent Data_02009310[];
extern struct SceneEvent Data_02009670[];
extern struct SceneEvent Data_020099d0[];
extern u16 Data_02009dccData[];
extern struct MapCellAnim Data_02009dcc[];

#endif
