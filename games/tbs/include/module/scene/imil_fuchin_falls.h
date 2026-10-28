#ifndef ALCHEMY_MODULE_MAP_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_MODULE_MAP_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "scene_tables.h"
#include "field_effect.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

extern s32 gImilFuchinFalls_TrackLeader;
const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void);
const struct SceneExitRegion *ImilFuchinFalls_GetRegions(void);
const u32 *ImilFuchinFalls_GetExits(void);
const struct ActorPlacement *ImilFuchinFalls_GetActorPlacements(void);
const struct Trigger *ImilFuchinFalls_GetTriggers(void);
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
extern struct ActorPlacement gImilFuchinFalls_NoActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ImilFallsCaveActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_EntranceActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ZephyrRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_DragonsEyeRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ArcticBladeRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_DragonRoomActorPlacements[];
extern struct Trigger gImilFuchinFalls_NoTriggers[];
extern struct Trigger gImilFuchinFalls_ImilFallsCaveTriggers[];
extern struct Trigger gImilFuchinFalls_EntranceTriggers[];
extern struct Trigger gImilFuchinFalls_ZephyrRoomTriggers[];
extern struct Trigger gImilFuchinFalls_DragonsEyeRoomTriggers[];
extern struct Trigger gImilFuchinFalls_ArcticBladeRoomTriggers[];
extern struct Trigger gImilFuchinFalls_KiRoomTriggers[];
extern struct Trigger gImilFuchinFalls_DragonRoomTriggers[];
extern s32 gImilFuchinFalls_ShortDustScript[];
extern s32 gImilFuchinFalls_MediumDustScript[];
extern s32 gImilFuchinFalls_LongDustScript[];

#endif
