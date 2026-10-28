#ifndef ALCHEMY_UNIT_MAP_LEMURIAN_SHIP_H
#define ALCHEMY_UNIT_MAP_LEMURIAN_SHIP_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *LemurianShip_GetEntrances(void);
const struct SceneExitRegion *LemurianShip_GetRegions(void);
const u32 *LemurianShip_GetExits(void);
const struct ActorTemplate *LemurianShip_GetActorTemplates(void);
const struct Trigger *LemurianShip_GetTriggers(void);
s32 LemurianShip_SetupMap(void);
void LineScroll_ArmDma(void);
void LineScroll_BuildPage(void);
extern u16 gShipGlowPalettes[];
extern s32 gSprayFallScript[];
extern s16 gRippleSine[256];
extern struct SceneEntrance gLemurianShipEntrances[];
extern u32 gLemurianShipExits[];
extern struct ActorTemplate gLemurianShipActorTemplates[];
extern struct Trigger gLemurianShipTriggers[];
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
