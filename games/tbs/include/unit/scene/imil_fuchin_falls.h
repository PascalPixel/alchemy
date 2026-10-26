#ifndef ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

extern s32 gImilFuchinFalls_TrackLeader;
const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void);
const struct SceneExitRegion *ImilFuchinFalls_GetRegions(void);
const u32 *ImilFuchinFalls_GetExits(void);
const struct ActorTemplate *ImilFuchinFalls_GetActorTemplates(void);
const struct MapEvent *ImilFuchinFalls_GetEvents(void);
s32 ImilFuchinFalls_Initialize(void);
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra);
extern s16 gImilFuchinFalls_KeyHeadings[];
extern s16 gImilFuchinFalls_SlideHeadings[];
extern struct Actor *gImilFuchinFalls_DragonsEye;
extern struct SceneEntrance gImilFuchinFalls_NoEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[];
extern struct SceneEntrance gImilFuchinFalls_EntranceEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ZephyrRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonsEyeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ArcticBladeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[];
extern u32 gImilFuchinFalls_Exits[];
extern struct ActorTemplate gImilFuchinFalls_NoActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_ImilFallsCaveActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_EntranceActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_ZephyrRoomActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_DragonsEyeRoomActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_ArcticBladeRoomActorTemplates[];
extern struct ActorTemplate gImilFuchinFalls_DragonRoomActorTemplates[];
extern struct MapEvent gImilFuchinFalls_NoEvents[];
extern struct MapEvent gImilFuchinFalls_ImilFallsCaveEvents[];
extern struct MapEvent gImilFuchinFalls_EntranceEvents[];
extern struct MapEvent gImilFuchinFalls_ZephyrRoomEvents[];
extern struct MapEvent gImilFuchinFalls_DragonsEyeRoomEvents[];
extern struct MapEvent gImilFuchinFalls_ArcticBladeRoomEvents[];
extern struct MapEvent gImilFuchinFalls_KiRoomEvents[];
extern struct MapEvent gImilFuchinFalls_DragonRoomEvents[];
extern s32 gImilFuchinFalls_ShortDustScript[];
extern s32 gImilFuchinFalls_MediumDustScript[];
extern s32 gImilFuchinFalls_LongDustScript[];

#endif
