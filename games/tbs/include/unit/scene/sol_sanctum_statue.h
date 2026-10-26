#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_STATUE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_STATUE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *SolSanctumStatue_GetEntrances(void);
const struct SceneExitRegion *SolSanctumStatue_GetRegions(void);
const u32 *SolSanctumStatue_GetExits(void);
const struct ActorTemplate *SolSanctumStatue_GetActorTemplates(void);
const struct Trigger *SolSanctumStatue_GetTriggers(void);
s32 SolSanctumStatue_Initialize(void);
extern struct SceneEntrance gSolSanctumStatue_Entrances[];
extern u32 gSolSanctumStatue_Exits[];
extern struct ActorTemplate gSolSanctumStatue_ActorTemplates[];
extern struct Trigger gSolSanctumStatue_Triggers[];
extern s32 gSolSanctumStatue_ScrollPhase;

#endif
