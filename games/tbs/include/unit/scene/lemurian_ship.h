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
extern struct SceneEvent LemurianShip_PaletteFrames[];
extern s32 LemurianShip_DriftScript[];
extern s16 LemurianShip_WaveSine[256];
extern struct SceneEntrance LemurianShip_SceneTableA[];
extern u32 LemurianShip_SceneTableB[];
extern struct ScenePlacement LemurianShip_SceneTableC[];
extern struct SceneEvent LemurianShip_SceneTableD[];
extern s32 LemurianShip_ShimmerActive;
extern s32 LemurianShip_ShimmerPhase;
extern s32 LemurianShip_Count;
extern s32 LemurianShip_CountTicks;
extern s32 LemurianShip_DriftActive;
extern s32 LemurianShip_SwellActive;
extern s32 LemurianShip_SwellPhase;
extern s32 LemurianShip_SwellLayerSixY;
extern s32 LemurianShip_SwellLayerSevenY;
extern s32 LemurianShip_StoredSlot0;
extern s32 LemurianShip_StoredRecord1;
extern s32 LemurianShip_StoredSlot3;
extern s32 LemurianShip_StoredRecord2;
extern u32 LemurianShip_PaletteStep[];
extern s32 LemurianShip_ActionScriptA[];
extern s32 LemurianShip_ActionScriptB[];
extern s32 LemurianShip_ActionScriptC[];
extern s32 LemurianShip_ActionScriptD[];

#endif
