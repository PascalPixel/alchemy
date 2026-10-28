#ifndef ALCHEMY_MODULE_MAP_GOMA_CAVE_ENTRANCE_H
#define ALCHEMY_MODULE_MAP_GOMA_CAVE_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gGomaCaveEntranceTallIvyScript[];
extern struct SceneEntrance gGomaCaveEntranceEntrances[];
extern u32 gGomaCaveEntranceExits[];
extern struct ActorPlacement gGomaCaveEntranceActorPlacements[];
extern struct Trigger gGomaCaveEntranceTriggers[];

#endif
