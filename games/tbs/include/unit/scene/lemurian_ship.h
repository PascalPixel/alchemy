#ifndef ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H

#include "types.h"
#include "field_scene.h"

u8 *LemurianShip_GetEntrances(void);
s32 LemurianShip_GetDefaultResult(void);
u8 *LemurianShip_GetExits(void);
u8 *LemurianShip_GetPlacements(void);
u8 *LemurianShip_GetEvents(void);
s32 LemurianShip_SetupScene(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
extern struct SceneEvent gLemurianShip_PaletteFrames[];
extern s32 gLemurianShip_DriftScript[];
extern s16 gLemurianShip_WaveSine[256];
extern struct SceneEntrance gLemurianShip_SceneTableA[];
extern u32 gLemurianShip_SceneTableB[];
extern struct ScenePlacement gLemurianShip_SceneTableC[];
extern struct SceneEvent gLemurianShip_SceneTableD[];
extern s32 gLemurianShip_ShimmerActive;
extern s32 gLemurianShip_ShimmerPhase;
extern s32 gLemurianShip_Count;
extern s32 gLemurianShip_CountTicks;
extern s32 gLemurianShip_DriftActive;
extern s32 gLemurianShip_SwellActive;
extern s32 gLemurianShip_SwellPhase;
extern s32 gLemurianShip_SwellLayerSixY;
extern s32 gLemurianShip_SwellLayerSevenY;
extern s32 gLemurianShip_StoredSlot0;
extern s32 gLemurianShip_StoredRecord1;
extern s32 gLemurianShip_StoredSlot3;
extern s32 gLemurianShip_StoredRecord2;
extern u32 gLemurianShip_PaletteStep[];
extern s32 gLemurianShip_ActionScriptA[];
extern s32 gLemurianShip_ActionScriptB[];
extern s32 gLemurianShip_ActionScriptC[];
extern s32 gLemurianShip_ActionScriptD[];

#endif
