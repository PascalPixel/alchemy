#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *MercuryLighthouseAerie_GetEntrances(void);
const struct SceneRegion *MercuryLighthouseAerie_GetRegions(void);
const u32 *MercuryLighthouseAerie_GetExits(void);
const struct ScenePlacement *MercuryLighthouseAerie_GetPlacements(void);
const struct SceneEvent *MercuryLighthouseAerie_GetEvents(void);
s32 MercuryLighthouseAerie_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ScenePlacement gMercuryLighthouseAeriePlacements[];
extern struct SceneEvent gMercuryLighthouseAerieEvents[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#endif
