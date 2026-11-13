#ifndef ALCHEMY_MODULE_SCENE_GOMA_CAVE_STREAM_H
#define ALCHEMY_MODULE_SCENE_GOMA_CAVE_STREAM_H

#include "types.h"
#include "scene_tables.h"

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
