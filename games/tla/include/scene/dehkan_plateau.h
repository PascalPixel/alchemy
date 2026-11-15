#ifndef ALCHEMY_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_SCENE_DEHKAN_PLATEAU_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

void DehkanPlateau_CannonLeapsAway(void);

void Pound_SinkTarget(u16 *list, s32 mode, s32 count);

void DehkanPlateau_SetActorsHidden(s32 show);

void Actor_DropAndWait(struct Actor *object);
void DehkanPlateau_SetRoom3SecondPillarInPlace(void);

void Import_ActorRelease(struct Actor *object);

void DehkanPlateau_PoundLeaderThrough(void);
void DehkanPlateau_CannonCornered(void);

extern u16 gDehkanPlateau_PoundCubeCrackPictures[];
extern u16 gDehkanPlateau_PoundCubeCracks[][6];

void CrumblingPillar_Prepare(s32 id);

void Rubble_Spray(s32 spread);

void Rubble_Burst(void);

void Leader_SetCellShapeAt(s32 a, s32 b);

extern u16 gDehkanPlateau_FullMetalVestChestHolePictures[];
extern u16 gDehkanPlateau_ThemisAxeChestHolePictures[];
extern u16 gDehkanPlateau_Room3HolePictures[];
extern u16 gDehkanPlateau_PoundCubeHolePictures[];

void DehkanPlateau_JoltCamera(void);
void DehkanPlateau_CannonCastsPound(void);

void DehkanPlateau_FightCannon(void);

void DehkanPlateau_StepOnFullMetalVestChestPillars(void);
void DehkanPlateau_StepOnThemisAxeChestPillars(void);
void DehkanPlateau_StepOnRoom3Pillars(void);
void DehkanPlateau_StepOnCannonPillar(void);
void DehkanPlateau_HideActors(void);

void DehkanPlateau_ShowActors(void);

void DehkanPlateau_DropFullMetalVestChestPillarIntoGap(void);

void DehkanPlateau_LiftThemisAxeChestPillar(void);

void DehkanPlateau_PlaceThemisAxeChestPillar(void);
void DehkanPlateau_LiftRoom3Pillar(void);

void DehkanPlateau_PushRoom3SecondPillar(void);

void DehkanPlateau_StepOffCrack(void);

void DehkanPlateau_StepOnCrack(void);
void DehkanPlateau_PushRaisedPillar(void);
void DehkanPlateau_LiftRoom3RaisedPillar(void);
void DehkanPlateau_FinishMovingRaisedPillar(void);
void DehkanPlateau_CannonFleesOverCracks(void);
void DehkanPlateau_CannonPoundsPillar(void);

void DehkanPlateau_CannonCrumblesPillars(void);
void DehkanPlateau_CannonPoundsLeaderThrough(void);
void DehkanPlateau_MeetCannon(void);
void DehkanPlateau_TakePoundCube(void);
void DehkanPlateau_LeaveRoom(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void Actor_SetCellShape(s32 level, s32 layer, s32 owner);

void DehkanPlateau_HideFullMetalVestChestActors(void);

struct Actor;
struct EffectOptions;

struct AnimationFacing {
  u8 unk00[9];
  u8 low : 2;
  u8 facing : 2;
  u8 high : 4;
};

void DehkanPlateau_EnterConeCell(void);

void DehkanPlateau_CannonCastsPoundOnLeader(void);

void DehkanPlateau_CrumblePillar();
void DehkanPlateau_InitializeFullMetalVestChest(void);

void DehkanPlateau_InitializeThemisAxeChest(void);

void DehkanPlateau_InitializeRoom3(void);
void DehkanPlateau_InitializeRoom4(void);
void DehkanPlateau_InitializePoundCube(void);
void DehkanPlateau_InitializeCannon(void);

void CrumblingPillars_Place(u16 *list);

void DehkanPlateau_SetThemisAxeChestStandingPillarCells(void);
void DehkanPlateau_HideThemisAxeChestActors(void);

void DehkanPlateau_SetRoom3StandingPillarCells(void);
void DehkanPlateau_RestoreRoom3Cracks(void);
void DehkanPlateau_HideRoom3Actors(void);

void DehkanPlateau_InitializeRopes(void);

void DehkanPlateau_HideRoom4Actors(void);

void DehkanPlateau_LandFromFall(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void DehkanPlateau_RestorePoundCubeCracks(void);

void DehkanPlateau_CannonDropsPoundCube(void);
void DehkanPlateau_DrawPoundCubeFallLanding(void);
void DehkanPlateau_CreatePoundCube(void);

void DehkanPlateau_HideCannonActors(void);

extern s32 gDehkanPlateau_CrackX;

#endif
