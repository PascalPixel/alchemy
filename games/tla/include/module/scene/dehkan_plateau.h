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

/// A shape grid byte seen as a bit-field, so its store takes the struct's
/// alias set (see the FAKEMATCH tag in Func_02000d78).
struct ShapeBits {
  u32 shape : 8;
};

void DehkanPlateau_CannonLeapsAway(void);

void Pillars_Pound(const void *data, s32 mode, s32 timer);

void DehkanPlateau_SetActorsHidden(s32 show);

void Actor_DropAndWait(struct Actor *object);
void Func_0200172c(void);

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

void Func_020012c8(void);
void Func_02001368(void);
void Func_020013fc(void);
void Func_02001444(void);
void DehkanPlateau_HideActors(void);

void DehkanPlateau_ShowActors(void);

void Func_020015e0(void);

void Func_02001630(void);

void Func_02001640(void);
void Func_020016a0(void);

void Func_020017a0(void);

void Func_020017f4(void);

void DehkanPlateau_StepOnCrack(void);
void Scene_RunIfHeld(void);
void Func_02002114(void);
void Func_02002124(void);
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

void CrumblingPillars_Place(const void *data);

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
void Func_02003ba0(struct Actor *object, s32 mode);
s32 Func_02003db0(struct Actor *object);
extern s32 gDehkanPlateau_CrackX;

#endif
