#ifndef ALCHEMY_MODULE_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_MODULE_SCENE_DEHKAN_PLATEAU_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

void DehkanPlateau_CannonLeapsAway(void);

void Pillars_Pound(u16 *list, s32 mode, s32 count);

void DehkanPlateau_SetActorsHidden(s32 show);

void Actor_DropAndWait(struct Actor *object);
void DehkanPlateau_SetRoom3SecondPillarInPlace(void);

void Import_ActorRelease(struct Actor *object);

void DehkanPlateau_PoundLeaderThrough(void);
void DehkanPlateau_CannonCornered(void);

extern u16 gDehkanPlateau_Room5CrackPictures[];
extern u16 gDehkanPlateau_Room5Cracks[][6];

void CrumblingPillar_Prepare(s32 id);

void Rubble_Spray(s32 spread);

void Rubble_Burst(void);

void Leader_SetCellShapeAt(s32 a, s32 b);

extern u16 gDehkanPlateau_Room1HolePictures[];
extern u16 gDehkanPlateau_Room2HolePictures[];
extern u16 gDehkanPlateau_Room3HolePictures[];
extern u16 gDehkanPlateau_Room5HolePictures[];
extern u16 gData_02004068[];
extern u16 gData_02004110[];
extern u16 gData_02004170[];

void DehkanPlateau_JoltCamera(void);
void DehkanPlateau_CannonCastsPound(void);

void DehkanPlateau_FightCannon(void);

void DehkanPlateau_StepOnRoom1Pillars(void);
void DehkanPlateau_StepOnRoom2Pillars(void);
void DehkanPlateau_StepOnRoom3Pillars(void);
void DehkanPlateau_StepOnRoom6Pillar(void);
void DehkanPlateau_HideActors(void);

void DehkanPlateau_ShowActors(void);

void DehkanPlateau_DropRoom1PillarIntoGap(void);

void DehkanPlateau_LiftRoom2Pillar(void);

void DehkanPlateau_PlaceRoom2Pillar(void);
void DehkanPlateau_LiftRoom3Pillar(void);

void DehkanPlateau_PushRoom3SecondPillar(void);

void Func_020017f4(void);

void DehkanPlateau_StepOnCrack(void);
void Scene_RunIfHeld(void);
void DehkanPlateau_LiftRoom3RaisedPillar(void);
void DehkanPlateau_FinishMovingRaisedPillar(void);
void DehkanPlateau_CannonFleesOverCracks(void);
void DehkanPlateau_CannonPoundsPillar(void);

void DehkanPlateau_CannonCrumblesPillars(void);
void DehkanPlateau_CannonPoundsLeaderThrough(void);
void DehkanPlateau_MeetCannon(void);
void DehkanPlateau_TakePoundCube(void);
void DehkanPlateau_LeaveRoom(void);

extern u32 gData_02003e60[];
extern u32 gData_02003e9c[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void Actor_SetCellShape(s32 level, s32 layer, s32 owner);

void DehkanPlateau_HideRoom1Actors(void);

struct Actor;
struct EffectOptions;

struct AnimationFacing {
  u8 unk00[9];
  u8 low : 2;
  u8 facing : 2;
  u8 high : 4;
};

void Func_02002140(void);

void DehkanPlateau_CannonCastsPoundOnLeader(void);

void DehkanPlateau_CrumblePillar();
void DehkanPlateau_InitializeRoom1(void);

void DehkanPlateau_InitializeRoom2(void);

void DehkanPlateau_InitializeRoom3(void);
void DehkanPlateau_InitializeRoom4(void);
void DehkanPlateau_InitializeRoom5(void);
void DehkanPlateau_InitializeRoom6(void);

void CrumblingPillars_Place(u16 *list);

void Func_020012d8(void);
void DehkanPlateau_HideRoom2Actors(void);

void Func_020013b0(void);
void DehkanPlateau_RestoreRoom3Cracks(void);
void DehkanPlateau_HideRoom3Actors(void);

void Func_02003d90(void);

s32 Func_02003da0(s32 a, s32 b, s32 c);

void Func_02003da8(s32 a, s32 b, s32 c, s32 d);

void DehkanPlateau_HideRoom4Actors(void);

void Func_02003764(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void DehkanPlateau_RestoreRoom5Cracks(void);

void DehkanPlateau_CannonDropsPoundCube(void);
void Func_020035ec(void);
void Func_02003a30(void);

void DehkanPlateau_HideRoom6Actors(void);

void Func_02003de8(void);
s32 Func_02003db0(struct Actor *object);
extern s32 gDehkanPlateau_CrackX;

#endif
