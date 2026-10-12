#ifndef ALCHEMY_UNIT_FIELD_TORETO_EDA_EDA_H
#define ALCHEMY_UNIT_FIELD_TORETO_EDA_EDA_H

#include "types.h"
#include "field_scene.h"

u8 *ToretoEda_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ToretoEda_GetExits(void);
u8 *ToretoEda_GetPlacements(void);
u8 *ToretoEda_GetEvents(void);
s32 ToretoEda_InitializeBackdropTransition(void);
extern struct SceneEntrance ToretoEda_SceneTable0[];
extern u32 ToretoEda_SceneTable1[];
extern struct ScenePlacement ToretoEda_SceneTable2[];
extern struct SceneEvent ToretoEda_SceneTable3[];
extern u16 ToretoEda_CellSteps[];

#endif
