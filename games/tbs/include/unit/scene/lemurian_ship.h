#ifndef ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_UNIT_SCENE_LEMURIAN_SHIP_H

#include "types.h"
#include "field_scene.h"

u8 *BabiFune_GetEntrances(void);
s32 BabiFune_GetDefaultResult(void);
u8 *BabiFune_GetExits(void);
u8 *BabiFune_GetPlacements(void);
u8 *BabiFune_GetEvents(void);
s32 BabiFune_SetupScene(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
extern struct SceneEvent BabiFune_PaletteFrames[];
extern s32 BabiFune_DriftScript[];
extern s16 BabiFune_WaveSine[256];
extern struct SceneEntrance BabiFune_SceneTableA[];
extern u32 BabiFune_SceneTableB[];
extern struct ScenePlacement BabiFune_SceneTableC[];
extern struct SceneEvent BabiFune_SceneTableD[];
extern s32 BabiFune_ShimmerActive;
extern s32 BabiFune_ShimmerPhase;
extern s32 BabiFune_Count;
extern s32 BabiFune_CountTicks;
extern s32 BabiFune_DriftActive;
extern s32 BabiFune_SwellActive;
extern s32 BabiFune_SwellPhase;
extern s32 BabiFune_SwellLayerSixY;
extern s32 BabiFune_SwellLayerSevenY;
extern s32 BabiFune_StoredSlot0;
extern s32 BabiFune_StoredRecord1;
extern s32 BabiFune_StoredSlot3;
extern s32 BabiFune_StoredRecord2;
extern u32 BabiFune_PaletteStep[];
extern s32 BabiFune_ActionScriptA[];
extern s32 BabiFune_ActionScriptB[];
extern s32 BabiFune_ActionScriptC[];
extern s32 BabiFune_ActionScriptD[];

#endif
