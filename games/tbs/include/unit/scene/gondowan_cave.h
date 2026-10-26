#ifndef ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gGondowanCaveEntrancesOther[];
extern struct SceneEntrance gGondowanPassageEntrances[];
extern struct SceneEntrance gGondowanCaveRiverEntrances[];
extern struct SceneEntrance gGondowanCaveEntrances[];
extern u32 gGondowanCaveExits[];
extern struct ObjectEventTemplate gGondowanCaveObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gGondowanPassageObjectEventTemplates[];
extern struct ObjectEventTemplate
    gGondowanPassageObjectEventTemplatesWithIodem[];
extern struct ObjectEventTemplate gGondowanCaveRiverObjectEventTemplates[];
extern struct ObjectEventTemplate gGondowanCaveObjectEventTemplates[];
extern struct MapEvent gGondowanCaveEventsOther[];
extern struct MapEvent gGondowanPassageEvents[];
extern struct MapEvent gGondowanPassageEventsWithIodem[];
extern struct MapEvent gGondowanCaveRiverEvents[];
extern struct MapEvent gGondowanCaveEvents[];

#endif
