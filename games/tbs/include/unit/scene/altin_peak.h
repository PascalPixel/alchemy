#ifndef ALCHEMY_UNIT_MAP_ALTIN_PEAK_H
#define ALCHEMY_UNIT_MAP_ALTIN_PEAK_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s8 gAltinPeak_PaletteScript[];
extern s32 gAltinPeak_FaceLeaderScript[];
extern s32 gAltinPeak_PuddleGrowScript[];
extern s32 gAltinPeak_BoulderPath[];
extern s32 gAltinPeak_LeaderFleePath[];
extern s32 gAltinPeak_DebrisScript[];
extern s32 gAltinPeak_GaretScript[];
extern s32 gAltinPeak_IvanScript[];
extern s32 gAltinPeak_MiaScript[];
extern s32 gAltinPeak_StatueSinkScript[];
extern s32 gAltinPeak_RejoinScript[];
extern struct SceneEntrance gAltinPeak_EntrancesOther[];
extern struct SceneEntrance gAltinPeak_FirstWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_RailTunnelEntrances[];
extern struct SceneEntrance gAltinPeak_SecondWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_SouthEntranceEntrances[];
extern struct SceneEntrance gAltinPeak_ThirdWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_PoolEntrances[];
extern struct SceneEntrance gAltinPeak_SpritzEntrances[];
extern struct SceneEntrance gAltinPeak_JunctionEntrances[];
extern struct SceneEntrance gAltinPeak_FallingRocksEntrances[];
extern struct SceneEntrance gAltinPeak_HydrosStatueEntrances[];
extern struct SceneEntrance gAltinPeak_WestExitEntrances[];
extern struct SceneExitRegion gAltinPeak_FallingRocksRegions[];
extern struct SceneExitRegion gAltinPeak_HydrosStatueRegions[];
extern u32 gAltinPeak_Exits[];
extern struct ActorTemplate gAltinPeak_ActorTemplatesOther[];
extern struct ActorTemplate gAltinPeak_FirstWaterBeastActorTemplates[];
extern struct ActorTemplate gAltinPeak_SecondWaterBeastActorTemplates[];
extern struct ActorTemplate gAltinPeak_ThirdWaterBeastActorTemplates[];
extern struct ActorTemplate gAltinPeak_PoolActorTemplates[];
extern struct ActorTemplate gAltinPeak_SpritzActorTemplates[];
extern struct ActorTemplate gAltinPeak_JunctionActorTemplates[];
extern struct ActorTemplate gAltinPeak_FallingRocksActorTemplates[];
extern struct ActorTemplate gAltinPeak_HydrosStatueActorTemplates[];
extern struct ActorTemplate gAltinPeak_WestExitActorTemplates[];
extern struct Trigger gAltinPeak_TriggersOther[];
extern struct Trigger gAltinPeak_FirstWaterBeastTriggers[];
extern struct Trigger gAltinPeak_FirstWaterBeastTriggersDrained[];
extern struct Trigger gAltinPeak_RailTunnelTriggers[];
extern struct Trigger gAltinPeak_SecondWaterBeastTriggers[];
extern struct Trigger gAltinPeak_SouthEntranceTriggers[];
extern struct Trigger gAltinPeak_ThirdWaterBeastTriggers[];
extern struct Trigger gAltinPeak_PoolTriggers[];
extern struct Trigger gAltinPeak_SpritzTriggers[];
extern struct Trigger gAltinPeak_JunctionTriggers[];
extern struct Trigger gAltinPeak_FallingRocksTriggers[];
extern struct Trigger gAltinPeak_HydrosStatueTriggers[];
extern struct Trigger gAltinPeak_WestExitTriggers[];
extern s32 gAltinPeak_RumbleFrames;
extern u32 gAltinPeak_RollRadii[];

#endif
