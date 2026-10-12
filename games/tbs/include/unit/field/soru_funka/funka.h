#ifndef ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H
#define ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
s32 SoruFunka_RunHostageSceneOnEntranceTen(void);
extern s32 Funka_ArcOriginsHead[];
extern s32 Funka_ArcOrigins[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern u16 SoruFunka_StarCells[];
extern s32 Funka_EmberScript[];
extern u8 Soru_RingDrift[][3];
extern u8 Soru_RingSwing[][3];
extern s8 Soru_RingDirection[][3];
extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];

#endif
