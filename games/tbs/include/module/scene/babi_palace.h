#ifndef ALCHEMY_MODULE_MAP_BABI_PALACE_H
#define ALCHEMY_MODULE_MAP_BABI_PALACE_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *BabiPalace_GetRegions(void);
const u32 *BabiPalace_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct SceneEntrance gBabiPalace_Entrances[];
extern struct SceneEntrance gBabiPalace_OutsideEntrances[];
extern struct SceneExitRegion gBabiPalace_Regions[];
extern u32 gBabiPalace_Exits[];
extern struct ActorPlacement gBabiPalace_OutsideActorPlacements[];
extern struct ActorPlacement gBabiPalace_ActorPlacementsBabiMissing[];
extern struct ActorPlacement gBabiPalace_ActorPlacementsFinals[];
extern struct ActorPlacement gBabiPalace_ActorPlacementsAfterColosso[];
extern struct Trigger gBabiPalace_TriggersBabiMissing[];
extern struct Trigger gBabiPalace_TriggersFinals[];
extern struct Trigger gBabiPalace_TriggersAfterColosso[];
extern struct Trigger gBabiPalace_OutsideTriggersBabiMissing[];
extern struct Trigger gBabiPalace_OutsideTriggersFinals[];
extern struct Trigger gBabiPalace_OutsideTriggersAfterColosso[];

#endif
