#ifndef ALCHEMY_MODULE_MAP_MERCURY_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_MAP_MERCURY_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *MercuryLighthouseAerie_GetEntrances(void);
const struct SceneExitRegion *MercuryLighthouseAerie_GetRegions(void);
const u32 *MercuryLighthouseAerie_GetExits(void);
const struct ActorPlacement *MercuryLighthouseAerie_GetActorPlacements(void);
const struct Trigger *MercuryLighthouseAerie_GetTriggers(void);
s32 MercuryLighthouseAerie_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ActorPlacement gMercuryLighthouseAerieActorPlacements[];
extern struct Trigger gMercuryLighthouseAerieTriggers[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#endif
