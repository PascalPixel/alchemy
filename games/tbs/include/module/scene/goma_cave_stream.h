#ifndef ALCHEMY_MODULE_MAP_GOMA_CAVE_STREAM_H
#define ALCHEMY_MODULE_MAP_GOMA_CAVE_STREAM_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

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
extern struct SceneEntrance gGomaCaveStreamEntrances[];
extern struct SceneEntrance gGomaCaveBridgeEntrances[];
extern u32 gGomaCaveStreamAndBridgeExits[];
extern struct ActorPlacement gGomaCaveStreamActorPlacements[];
extern struct ActorPlacement gGomaCaveBridgeActorPlacements[];
extern struct Trigger gGomaCaveStreamTriggers[];
extern struct Trigger gGomaCaveBridgeTriggers[];

#endif
