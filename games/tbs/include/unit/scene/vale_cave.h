#ifndef ALCHEMY_UNIT_MAP_VALE_CAVE_H
#define ALCHEMY_UNIT_MAP_VALE_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *ValeCave_GetEntrances(void);
const struct SceneExitRegion *ValeCave_GetRegions(void);
const u32 *ValeCave_GetExits(void);
const struct ActorTemplate *ValeCave_GetActorTemplates(void);
const struct MapEvent *ValeCave_GetEvents(void);
s32 ValeCave_Initialize(void);
extern s32 gValeCave_ShortEffectScript[];
extern s32 gValeCave_MediumEffectScript[];
extern s32 gValeCave_LongEffectScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gValeCave_EntrancesOther[];
extern struct SceneEntrance gValeCave_MouthEntrances[];
extern struct SceneEntrance gValeCave_KiteEntrances[];
extern struct SceneEntrance gValeCave_LogsEntrances[];
extern u32 gValeCave_Exits[];
extern struct ActorTemplate gValeCave_MouthActorTemplates[];
extern struct ActorTemplate gValeCave_KiteActorTemplates[];
extern struct ActorTemplate gValeCave_LogsActorTemplates[];
extern struct MapEvent gValeCave_EventsOther[];
extern struct MapEvent gValeCave_MouthEvents[];
extern struct MapEvent gValeCave_KiteEvents[];
extern struct MapEvent gValeCave_LogsEvents[];

#endif
