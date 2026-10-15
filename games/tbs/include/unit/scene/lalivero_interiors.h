#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H

#include "types.h"
#include "field_scene.h"

u8                          *LaliveroInteriors_GetEntrances(void);
u8                          *LaliveroInteriors_GetRegions(void);
u8                          *LaliveroInteriors_GetExits(void);
u8                          *LaliveroInteriors_GetPlacements(void);
u8                          *LaliveroInteriors_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gLaliveroPoseAction[];
extern struct SceneEntrance  gLaliveroEntrances[];
extern s16                   gLaliveroSanctumRegions[];
extern s16                   gLaliveroRegions[];
extern u32                   gLaliveroExits[];
extern struct ScenePlacement gLaliveroPlacements[];
extern struct ScenePlacement gLaliveroInteriorsPlacements[];
extern struct ScenePlacement gLaliveroInteriorsPlacements9a7[];
extern struct SceneEvent     gLaliveroEvents[];
extern struct SceneEvent     gLaliveroEvents9a7[];
extern struct SceneEvent     gLaliveroInteriorsEvents[];

#endif
