#ifndef ALCHEMY_UNIT_SCENE_FUCHIN_TEMPLE_H
#define ALCHEMY_UNIT_SCENE_FUCHIN_TEMPLE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gFuchinTemple_JumpingStudentScript[];
extern s32 gFuchinTemple_HiddenSpriteScript[];
extern u16 gFuchinTemple_CellStepsA[];
extern u16 gFuchinTemple_CellStepsB[];
extern u16 gFuchinTemple_CellStepsC[];
extern struct SceneEntrance gFuchinTempleGroundsEntrances[];
extern struct SceneEntrance gFuchinTempleEntrancesOther[];
extern u32 gFuchinTempleExits[];
extern struct ScenePlacement gFuchinTempleGroundsPlacements[];
extern struct ScenePlacement gFuchinTemplePlacements[];
extern struct ScenePlacement gFuchinTemplePlacementsFromLamaTemple[];
extern struct SceneEvent gFuchinTempleGroundsEvents[];
extern struct SceneEvent gFuchinTempleEvents[];
extern struct SceneEvent gFuchinTempleEventsFromLamaTemple[];
extern s32 gFuchinTemple_ShotScript[];

#endif
