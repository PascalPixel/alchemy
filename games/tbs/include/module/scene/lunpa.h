#ifndef ALCHEMY_MODULE_SCENE_LUNPA_H
#define ALCHEMY_MODULE_SCENE_LUNPA_H

#include "types.h"
#include "scene_tables.h"

extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gLunpaEntrances[];
extern struct SceneEntrance gGateEntrances[];
extern u32 gLunpaExits[];
extern u32 gGateExits[];
extern struct ActorPlacement gLunpaActorPlacements[];
extern struct ActorPlacement gGateActorPlacements[];
extern struct Trigger gLunpaSealedTriggers[];
extern struct Trigger gLunpaReopenedTriggers[];
extern struct Trigger gGateTriggers[];
extern u32 gLunpaUnusedWord;

#endif
