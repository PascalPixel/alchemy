#ifndef ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void);
const struct SceneExitRegion *KalayTolbiDocks_GetRegions(void);
const u32 *KalayTolbiDocks_GetExits(void);
const struct ActorTemplate *KalayTolbiDocks_GetActorTemplates(void);
const struct MapEvent *KalayTolbiDocks_GetEvents(void);
s32 KalayTolbiDocks_Initialize(void);
extern u16 gKalayTolbiDocks_KalayDoorCells[];
extern u16 gKalayTolbiDocks_TolbiDoorCells[];
extern s32 gKalayTolbiDocks_BodyguardSpinScript[];
extern struct SceneEntrance gKalayTolbiDocks_NoEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_KalayEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_InteriorEntrances[];
extern u32 gKalayTolbiDocks_Exits[];
extern struct ActorTemplate gKalayTolbiDocks_NoActorTemplates[];
extern struct ActorTemplate gKalayTolbiDocks_TolbiActorTemplates[];
extern struct ActorTemplate gKalayTolbiDocks_TolbiActorTemplatesStranded[];
extern struct ActorTemplate gKalayTolbiDocks_KalayActorTemplates[];
extern struct ActorTemplate gKalayTolbiDocks_KalayActorTemplatesShipGone[];
extern struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplates[];
extern struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplatesShipGone[];
extern struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplatesStranded[];
extern struct MapEvent gKalayTolbiDocks_NoEvents[];
extern struct MapEvent gKalayTolbiDocks_KalayEvents[];
extern struct MapEvent gKalayTolbiDocks_KalayEventsShipGone[];
extern struct MapEvent gKalayTolbiDocks_TolbiEvents[];
extern struct MapEvent gKalayTolbiDocks_TolbiEventsStranded[];
extern struct MapEvent gKalayTolbiDocks_InteriorEvents[];
extern struct MapEvent gKalayTolbiDocks_InteriorEventsShipGone[];
extern struct MapEvent gKalayTolbiDocks_InteriorEventsStranded[];

#endif
