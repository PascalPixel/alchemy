#ifndef ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *LemurianShip_GetEntrances(void);
const struct SceneRegion *LemurianShip_GetRegions(void);
const u32 *LemurianShip_GetExits(void);
const struct ScenePlacement *LemurianShip_GetPlacements(void);
const struct SceneEvent *LemurianShip_GetEvents(void);
s32 LemurianShip_SetupScene(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
extern struct SceneEvent gShipGlowPalettes[];
extern s32 gSprayFallScript[];
extern s16 gRippleSine[256];
extern struct SceneEntrance gLemurianShipEntrances[];
extern u32 gLemurianShipExits[];
extern struct ScenePlacement gLemurianShipPlacements[];
extern struct SceneEvent gLemurianShipEvents[];
extern s32 gOrbGlowActive;
extern s32 gOrbGlowPhase;
extern s32 gOrbGlowStrength;
extern s32 gOrbGlowFadeFrames;
extern s32 gSprayActive;
extern s32 gSwellActive;
extern s32 gSwellPhase;
extern s32 gShipHullY;
extern s32 gShipDeckY;
extern s32 gLeaderDeckY;
extern s32 gGaretDeckY;
extern s32 gMiaDeckY;
extern s32 gIvanDeckY;
extern u32 gShipGlowStep[];
extern s32 gLeaderBoardsShip[];
extern s32 gGaretBoardsShip[];
extern s32 gIvanBoardsShip[];
extern s32 gMiaBoardsShip[];

#endif
