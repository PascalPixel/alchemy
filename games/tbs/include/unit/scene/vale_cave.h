#ifndef ALCHEMY_UNIT_MAP_VALE_CAVE_H
#define ALCHEMY_UNIT_MAP_VALE_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *ValeCave_GetEntrances(void);
const struct SceneExitRegion *ValeCave_GetRegions(void);
const u32 *ValeCave_GetExits(void);
const struct ActorPlacement *ValeCave_GetActorPlacements(void);
const struct Trigger *ValeCave_GetTriggers(void);
s32 ValeCave_Initialize(void);
extern s32 gValeCave_ShortEffectScript[];
extern s32 gValeCave_MediumEffectScript[];
extern s32 gValeCave_LongEffectScript[];
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gValeCave_EntrancesOther[];
extern struct SceneEntrance gValeCave_MouthEntrances[];
extern struct SceneEntrance gValeCave_KiteEntrances[];
extern struct SceneEntrance gValeCave_LogsEntrances[];
extern u32 gValeCave_Exits[];
extern struct ActorPlacement gValeCave_MouthActorPlacements[];
extern struct ActorPlacement gValeCave_KiteActorPlacements[];
extern struct ActorPlacement gValeCave_LogsActorPlacements[];
extern struct Trigger gValeCave_TriggersOther[];
extern struct Trigger gValeCave_MouthTriggers[];
extern struct Trigger gValeCave_KiteTriggers[];
extern struct Trigger gValeCave_LogsTriggers[];

#endif
