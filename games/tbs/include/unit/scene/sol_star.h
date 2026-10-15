#ifndef ALCHEMY_UNIT_SCENE_SOL_STAR_H
#define ALCHEMY_UNIT_SCENE_SOL_STAR_H

#include "types.h"
#include "field_scene.h"

s32 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
s32 *SceneData_GetMessageTable(void);
s32 *SceneData_GetActorTable(void);
s32 *SceneData_GetEffectTable(void);
s32 SolStar_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gSolStar_AngleScript[];
extern s32 gSolStar_PresentItemScript[];
extern struct SceneEntrance gPlacement_Scripts[];
extern u32 gPlacement_Messages[];
extern struct ScenePlacement gPlacement_Actors[];
extern u16 gSolStar_StarCells[];
extern u8 gSol_RingDrift[][3];
extern u8 gSol_RingSwing[][3];
extern s8 gSol_RingDirection[][3];
extern s32 gSol_RingOffsetX[];
extern s32 gSol_RingOffsetZ[];

#endif
