#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_STREAM_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_STREAM_H

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
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gGomaCaveStreamEntrances[];
extern struct SceneEntrance gGomaCaveBridgeEntrances[];
extern u32 gGomaCaveStreamAndBridgeExits[];
extern struct ObjectEventTemplate gGomaCaveStreamObjectEventTemplates[];
extern struct ObjectEventTemplate gGomaCaveBridgeObjectEventTemplates[];
extern struct MapEvent gGomaCaveStreamEvents[];
extern struct MapEvent gGomaCaveBridgeEvents[];

#endif
