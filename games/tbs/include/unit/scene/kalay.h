#ifndef ALCHEMY_UNIT_MAP_KALAY_H
#define ALCHEMY_UNIT_MAP_KALAY_H

#include "types.h"
#include "script_layer.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Kalay_GetEntrances(void);
const struct SceneExitRegion *Kalay_GetRegions(void);
const u32 *Kalay_GetExits(void);
const struct ActorTemplate *Kalay_GetActorTemplates(void);
const struct Trigger *Kalay_GetTriggers(void);
s32 Kalay_Initialize(void);
extern s32 gKalay_TileStepByFacing[];
extern s32 gKalay_TourGroupWalkScript[];
extern s32 gKalay_TourGroupLeaveScript[];
extern s32 gKalay_TouristFidgetScript[];
extern s32 gKalay_GuideCountScript[];
extern s32 gKalay_GuidePaceScript[];
extern s32 gKalay_HeadGuideStopScript[];
extern s32 gKalay_SecondGuideStopScript[];
extern s32 gKalay_HeadGuideReturnScript[];
extern s32 gKalay_SecondGuideReturnScript[];
extern s32 gKalay_PartyFidgetScript[];
extern s32 gKalay_PartyGatherScript[];
extern s32 gKalay_FrostGrowScript[];
extern struct SceneEntrance gKalay_NoEntrances[];
extern struct SceneEntrance gKalay_TownEntrances[];
extern struct SceneEntrance gKalay_HammetPalaceEntrances[];
extern struct SceneEntrance gKalay_TunnelEntranceEntrances[];
extern struct SceneEntrance gKalay_TunnelCaveEntrances[];
extern struct SceneEntrance gKalay_TunnelPassageEntrances[];
extern struct SceneEntrance gKalay_TunnelEntrances[];
extern u32 gKalay_Exits[];
extern struct ActorTemplate gKalay_NoActorTemplates[];
extern struct ActorTemplate gKalay_TownActorTemplates[];
extern struct ActorTemplate gKalay_TourGroupActorTemplates[];
extern struct ActorTemplate gKalay_HammetPalaceActorTemplates[];
extern struct ActorTemplate gKalay_TunnelEntranceActorTemplates[];
extern struct ActorTemplate gKalay_TunnelActorTemplates[];
extern struct Trigger gKalay_NoTriggers[];
extern struct Trigger gKalay_TownTriggers[];
extern struct Trigger gKalay_HammetPalaceTriggers[];
extern struct Trigger gKalay_TunnelEntranceTriggers[];
extern struct Trigger gKalay_TunnelCaveTriggers[];
extern struct Trigger gKalay_TunnelPassageTriggers[];
extern struct Trigger gKalay_TunnelTriggers[];
extern u16 gKalay_DoorCellSteps[];
extern struct MapGridCellAnimation gKalay_DoorCells[];
extern s32 gKalay_TunnelParticleScript[];

#endif
