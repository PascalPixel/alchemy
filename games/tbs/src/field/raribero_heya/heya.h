#ifndef HEYA_H
#define HEYA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

extern u8 gRariberoEntrances[];
extern u8 gRariberoSanctumRegions[];
extern u8 gRariberoRegions[];
extern u8 gRariberoExits[];
extern u8 gRariberoPlacements[];
extern u8 gRariberoHeyaPlacements[];
extern u8 gRariberoHeyaPlacements9a7[];
extern u8 gRariberoEvents[];
extern u8 gRariberoEvents9a7[];
extern u8 gRariberoHeyaEvents[];
extern u8 gRariberoHeyaEvents9a7[];

void Engine_ResetSceneEffectCounter(void);
void Owner_RefreshActiveRatios();
void FieldScene_RunSecondaryScript();

void FieldScene_RunPrimaryScript(void);

#endif
