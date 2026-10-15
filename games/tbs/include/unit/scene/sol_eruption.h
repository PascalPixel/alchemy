#ifndef ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
s32 SolEruption_RunHostageSceneOnEntranceTen(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gPlacement_Scripts[];
extern u32 gPlacement_Messages[];
extern struct ScenePlacement gPlacement_Actors[];
extern struct SceneEvent gPlacement_Effects[];
extern u16 gSolEruption_StarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSol_RingDrift[][3];
extern u8 gSol_RingSwing[][3];
extern s8 gSol_RingDirection[][3];
extern s32 gSol_RingOffsetX[];
extern s32 gSol_RingOffsetZ[];

#endif
