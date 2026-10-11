#ifndef HEYA_H
#define HEYA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

extern struct SceneEntrance	 gRariberoEntrances[];
extern s16					 gRariberoSanctumRegions[];
extern s16					 gRariberoRegions[];
extern u32					 gRariberoExits[];
extern struct ScenePlacement gRariberoPlacements[];
extern struct ScenePlacement gRariberoHeyaPlacements[];
extern struct ScenePlacement gRariberoHeyaPlacements9a7[];
extern struct SceneEvent	 gRariberoEvents[];
extern struct SceneEvent	 gRariberoEvents9a7[];
extern struct SceneEvent	 gRariberoHeyaEvents[];
extern struct SceneEvent	 gRariberoHeyaEvents9a7[];

void Engine_ResetSceneEffectCounter(void);
void Owner_RefreshActiveRatios();
void FieldScene_RunSecondaryScript();

void FieldScene_RunPrimaryScript(void);

#endif
