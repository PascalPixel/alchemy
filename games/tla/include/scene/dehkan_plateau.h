#ifndef ALCHEMY_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_SCENE_DEHKAN_PLATEAU_H

/// @file
/// Dehkan Plateau's tables and events, and routines several scenes keep a
/// copy of and declare from here (Pound_SinkTarget, Actor_SetCellOpen,
/// FieldEffect_Spawn).

#include "types.h"

struct Actor;
struct EffectOptions;

/// Cannon: meeting him at the rope bridge, the chase across the cracks and
/// stakes, the fight in his cave and the Pound Cube he drops.
void DehkanPlateau_MeetCannon(void);
void DehkanPlateau_CannonLeapsAway(void);
void DehkanPlateau_CannonFleesOverCracks(void);
void DehkanPlateau_CannonPoundsStake(void);
void DehkanPlateau_CannonCracksPillars(void);
void DehkanPlateau_CannonCastsPound(void);
void DehkanPlateau_CannonCastsPoundOnLeader(void);
void DehkanPlateau_CannonPoundsLeaderThrough(void);
void DehkanPlateau_PoundLeaderThrough(void);
void DehkanPlateau_JoltCamera(void);
void DehkanPlateau_CannonCornered(void);
void DehkanPlateau_FightCannon(void);
void DehkanPlateau_CannonDropsPoundCube(void);
void DehkanPlateau_DrawPoundCubeFallLanding(void);
void DehkanPlateau_CreatePoundCube(void);
void DehkanPlateau_TakePoundCube(void);

/// The gray pillars that crack under Felix and crumble at the next step.
void DehkanPlateau_StepOnFullMetalVestChestPillars(void);
void DehkanPlateau_StepOnThemisAxeChestPillars(void);
void DehkanPlateau_StepOnTightropesPillars(void);
void DehkanPlateau_StepOnCannonPillar(void);
void DehkanPlateau_CrackPillar(s32 id, s32 standingId, s32 flag);
void DehkanPlateau_SetThemisAxeChestStandingPillarCells(void);
void DehkanPlateau_SetTightropesStandingPillarCells(void);

/// The stone and vine pillars Move lifts and Felix pushes into the gaps.
void DehkanPlateau_DropFullMetalVestChestPillarIntoGap(void);
void DehkanPlateau_LiftThemisAxeChestPillar(void);
void DehkanPlateau_PlaceThemisAxeChestPillar(void);
void DehkanPlateau_LiftTightropesPillar(void);
void DehkanPlateau_PushTightropesSecondPillar(void);
void DehkanPlateau_SetTightropesSecondPillarInPlace(void);
void DehkanPlateau_PushVinePillar(void);
void DehkanPlateau_LiftTightropesVinePillar(void);
void DehkanPlateau_FinishMovingVinePillar(void);

/// The cracks Felix falls through into the caves below, and the ways out.
void DehkanPlateau_StepOnCrack(void);
void DehkanPlateau_StepOffCrack(void);
void DehkanPlateau_RestoreTightropesCracks(void);
void DehkanPlateau_RestorePoundCubeCracks(void);
void DehkanPlateau_LandFromFall(void);
void DehkanPlateau_EnterLowerPath(void);
void DehkanPlateau_LeaveRoom(void);

/// The unseen blocks that draw Felix behind a cliff's edge.
void DehkanPlateau_SetBlocksShown(s32 show);
void DehkanPlateau_ShowBlocks(void);
void DehkanPlateau_HideBlocks(void);
void DehkanPlateau_HideFullMetalVestChestBlocks(void);
void DehkanPlateau_HideThemisAxeChestBlocks(void);
void DehkanPlateau_HideTightropesBlocks(void);
void DehkanPlateau_HideMintChestBlocks(void);
void DehkanPlateau_HideCannonBlocks(void);

/// Each room's setup as it opens.
void DehkanPlateau_InitializeFullMetalVestChest(void);
void DehkanPlateau_InitializeThemisAxeChest(void);
void DehkanPlateau_InitializeTightropes(void);
void DehkanPlateau_InitializeMintChest(void);
void DehkanPlateau_InitializePoundCube(void);
void DehkanPlateau_InitializeCannon(void);
/// Sets up the field's ropes for the scene (Rope_Initialize).
void DehkanPlateau_InitializeRopes(void);

/// The cracks' pictures and records (each room's crack table).
extern u16 gDehkanPlateau_PoundCubeCrackPictures[];
extern u16 gDehkanPlateau_PoundCubeCracks[][6];
extern u16 gDehkanPlateau_FullMetalVestChestHolePictures[];
extern u16 gDehkanPlateau_ThemisAxeChestHolePictures[];
extern u16 gDehkanPlateau_TightropesHolePictures[];
extern u16 gDehkanPlateau_PoundCubeHolePictures[];

/// Routines and scripts other scenes keep a copy of.
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Actor_DropAndWait(struct Actor *actor);
void CrumblingPillar_Prepare(s32 id);
void CrumblingPillar_Place(u16 *list);
void Dust_Scatter(s32 spread);
void Dust_Burst(void);
void Leader_SetCellShapeAt(s32 level, s32 layer);
void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);
void Actor_SetCellShape(s32 level, s32 layer, s32 owner);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

#endif
