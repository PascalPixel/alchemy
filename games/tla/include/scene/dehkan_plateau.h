#ifndef ALCHEMY_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_SCENE_DEHKAN_PLATEAU_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

void DehkanPlateau_CannonLeapsAway(void);

void Pound_SinkTarget(u16 *list, s32 mode, s32 count);

void DehkanPlateau_SetBlocksShown(s32 show);

void Actor_DropAndWait(struct Actor *actor);
void DehkanPlateau_SetTightropesSecondPillarInPlace(void);

void DehkanPlateau_PoundLeaderThrough(void);
void DehkanPlateau_CannonCornered(void);

extern u16 gDehkanPlateau_PoundCubeCrackPictures[];
extern u16 gDehkanPlateau_PoundCubeCracks[][6];

void CrumblingPillar_Prepare(s32 id);

void Dust_Scatter(s32 spread);

void Dust_Burst(void);

void Leader_SetCellShapeAt(s32 a, s32 b);

extern u16 gDehkanPlateau_FullMetalVestChestHolePictures[];
extern u16 gDehkanPlateau_ThemisAxeChestHolePictures[];
extern u16 gDehkanPlateau_TightropesHolePictures[];
extern u16 gDehkanPlateau_PoundCubeHolePictures[];

void DehkanPlateau_JoltCamera(void);
void DehkanPlateau_CannonCastsPound(void);

void DehkanPlateau_FightCannon(void);

void DehkanPlateau_StepOnFullMetalVestChestPillars(void);
void DehkanPlateau_StepOnThemisAxeChestPillars(void);
void DehkanPlateau_StepOnTightropesPillars(void);
void DehkanPlateau_StepOnCannonPillar(void);
void DehkanPlateau_ShowBlocks(void);

void DehkanPlateau_HideBlocks(void);

void DehkanPlateau_DropFullMetalVestChestPillarIntoGap(void);

void DehkanPlateau_LiftThemisAxeChestPillar(void);

void DehkanPlateau_PlaceThemisAxeChestPillar(void);
void DehkanPlateau_LiftTightropesPillar(void);

void DehkanPlateau_PushTightropesSecondPillar(void);

void DehkanPlateau_StepOffCrack(void);

void DehkanPlateau_StepOnCrack(void);
void DehkanPlateau_PushVinePillar(void);
void DehkanPlateau_LiftTightropesVinePillar(void);
void DehkanPlateau_FinishMovingVinePillar(void);
void DehkanPlateau_CannonFleesOverCracks(void);
void DehkanPlateau_CannonPoundsStake(void);

void DehkanPlateau_CannonCracksPillars(void);
void DehkanPlateau_CannonPoundsLeaderThrough(void);
void DehkanPlateau_MeetCannon(void);
void DehkanPlateau_TakePoundCube(void);
void DehkanPlateau_LeaveRoom(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void Actor_SetCellShape(s32 level, s32 layer, s32 owner);

void DehkanPlateau_HideFullMetalVestChestBlocks(void);

struct Actor;
struct EffectOptions;

void DehkanPlateau_EnterLowerPath(void);

void DehkanPlateau_CannonCastsPoundOnLeader(void);

void DehkanPlateau_CrackPillar();
void DehkanPlateau_InitializeFullMetalVestChest(void);

void DehkanPlateau_InitializeThemisAxeChest(void);

void DehkanPlateau_InitializeTightropes(void);
void DehkanPlateau_InitializeMintChest(void);
void DehkanPlateau_InitializePoundCube(void);
void DehkanPlateau_InitializeCannon(void);

void CrumblingPillar_Place(u16 *list);

void DehkanPlateau_SetThemisAxeChestStandingPillarCells(void);
void DehkanPlateau_HideThemisAxeChestBlocks(void);

void DehkanPlateau_SetTightropesStandingPillarCells(void);
void DehkanPlateau_RestoreTightropesCracks(void);
void DehkanPlateau_HideTightropesBlocks(void);

/// Sets up the field's ropes for the scene (Rope_Initialize).
void DehkanPlateau_InitializeRopes(void);

void DehkanPlateau_HideMintChestBlocks(void);

void DehkanPlateau_LandFromFall(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void DehkanPlateau_RestorePoundCubeCracks(void);

void DehkanPlateau_CannonDropsPoundCube(void);
void DehkanPlateau_DrawPoundCubeFallLanding(void);
void DehkanPlateau_CreatePoundCube(void);

void DehkanPlateau_HideCannonBlocks(void);

#endif
