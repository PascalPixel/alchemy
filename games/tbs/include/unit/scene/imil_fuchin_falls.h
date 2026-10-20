#ifndef ALCHEMY_UNIT_SCENE_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_UNIT_SCENE_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "field_scene.h"
#include "field_effect.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

extern s32 gImilFuchinFalls_TrackLeader;
const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void);
const struct SceneRegion *ImilFuchinFalls_GetRegions(void);
const u32 *ImilFuchinFalls_GetExits(void);
const struct ScenePlacement *ImilFuchinFalls_GetPlacements(void);
const struct SceneEvent *ImilFuchinFalls_GetEvents(void);
s32 ImilFuchinFalls_Initialize(void);
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra);
extern s16 gImilFuchinFalls_KeyHeadings[];
extern s16 gImilFuchinFalls_SlideHeadings[];
extern struct FieldActor *gImilFuchinFalls_DragonsEye;
extern struct SceneEntrance gImilFuchinFalls_NoEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[];
extern struct SceneEntrance gImilFuchinFalls_FirstRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_SecondRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ThirdRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_FourthRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[];
extern u32 gImilFuchinFalls_Exits[];
extern struct ScenePlacement gImilFuchinFalls_NoPlacements[];
extern struct ScenePlacement gImilFuchinFalls_ImilFallsCavePlacements[];
extern struct ScenePlacement gImilFuchinFalls_FirstRoomPlacements[];
extern struct ScenePlacement gImilFuchinFalls_SecondRoomPlacements[];
extern struct ScenePlacement gImilFuchinFalls_ThirdRoomPlacements[];
extern struct ScenePlacement gImilFuchinFalls_FourthRoomPlacements[];
extern struct ScenePlacement gImilFuchinFalls_DragonRoomPlacements[];
extern struct SceneEvent gImilFuchinFalls_NoEvents[];
extern struct SceneEvent gImilFuchinFalls_ImilFallsCaveEvents[];
extern struct SceneEvent gImilFuchinFalls_FirstRoomEvents[];
extern struct SceneEvent gImilFuchinFalls_SecondRoomEvents[];
extern struct SceneEvent gImilFuchinFalls_ThirdRoomEvents[];
extern struct SceneEvent gImilFuchinFalls_FourthRoomEvents[];
extern struct SceneEvent gImilFuchinFalls_KiRoomEvents[];
extern struct SceneEvent gImilFuchinFalls_DragonRoomEvents[];
extern s32 gImilFuchinFalls_DustScript0[];
extern s32 gImilFuchinFalls_DustScript1[];
extern s32 gImilFuchinFalls_DustScript2[];

#endif
