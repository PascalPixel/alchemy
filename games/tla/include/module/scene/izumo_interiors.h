#ifndef ALCHEMY_MODULE_SCENE_IZUMO_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_IZUMO_INTERIORS_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gIzumoInteriors_ActorPlacements[];
extern struct ActorPlacement gIzumoInteriors_SusasHouseActorPlacements[];
extern struct ActorPlacement
    gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain[];
extern struct Trigger gIzumoInteriors_Triggers[];
extern struct Trigger gIzumoInteriors_SusasHouseTriggers[];
extern struct Trigger gIzumoInteriors_TriggersOnceTheSerpentIsSlain[];

void Inn_CheckIn(s32 slot, s32 actor);

void IzumoInteriors_RecognizeTheHeroes(s32 mode);

void Func_02001d8c(void);
extern s32 gData_02002324[];
extern s32 gData_020024f0[];
extern s32 gData_020026bc[];

#endif
