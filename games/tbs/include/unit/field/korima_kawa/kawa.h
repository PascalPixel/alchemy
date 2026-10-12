#ifndef ALCHEMY_UNIT_FIELD_KORIMA_KAWA_KAWA_H
#define ALCHEMY_UNIT_FIELD_KORIMA_KAWA_KAWA_H

#include "types.h"
#include "field_scene.h"

u8 *MapStagedScene_SelectPrimaryData(void);
s32 MapStagedScene_GetEmptyData(void);
u8 *MapStagedScene_SelectSecondaryData(void);
u8 *MapStagedScene_SelectTertiaryData(void);
u8 *KorimaKawa_GetEvents(void);
s32 KorimaKawa_PrepareWindowTransition(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern struct SceneEntrance gKorimaKawaEntrances[];
extern u32 gKorimaKawaExits[];
extern struct ScenePlacement gKorimaKawaPlacements[];
extern struct SceneEvent gKorimaKawaEvents[];

#endif
