#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *MercuryLighthouseAerie_GetEntrances(void);
const struct SceneExitRegion *MercuryLighthouseAerie_GetRegions(void);
const u32 *MercuryLighthouseAerie_GetExits(void);
const struct ActorTemplate *MercuryLighthouseAerie_GetActorTemplates(void);
const struct Trigger *MercuryLighthouseAerie_GetTriggers(void);
s32 MercuryLighthouseAerie_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ActorTemplate gMercuryLighthouseAerieActorTemplates[];
extern struct Trigger gMercuryLighthouseAerieTriggers[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#endif
