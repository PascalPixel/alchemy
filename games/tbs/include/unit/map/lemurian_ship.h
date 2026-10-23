#ifndef ALCHEMY_UNIT_MAP_LEMURIAN_SHIP_H
#define ALCHEMY_UNIT_MAP_LEMURIAN_SHIP_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *LemurianShip_GetWarpDests(void);
const struct MapWarpRegion *LemurianShip_GetRegions(void);
const u32 *LemurianShip_GetWarps(void);
const struct ObjectEventTemplate *LemurianShip_GetObjectEventTemplates(void);
const struct MapEvent *LemurianShip_GetEvents(void);
s32 LemurianShip_SetupMap(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
extern u16 gShipGlowPalettes[];
extern s32 gSprayFallScript[];
extern s16 gRippleSine[256];
extern struct MapWarpDest gLemurianShipWarpDests[];
extern u32 gLemurianShipWarps[];
extern struct ObjectEventTemplate gLemurianShipObjectEventTemplates[];
extern struct MapEvent gLemurianShipEvents[];
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
