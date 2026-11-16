#ifndef ALCHEMY_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_SCENE_GABOMBA_STATUE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "resource_ids.h"

struct Actor;

void GabombaStatue_TurnCogs(s32 direction);

void GabombaStatue_StopClock(void);
s32 GabombaStatue_RunCharges(s32 first, s32 check);

void GabombaStatue_TurnWithCog(void);

void GabombaStatue_RideSlidingCog(void);

void GabombaStatue_RideRollingCog(void);

void GabombaStatue_HopAcrossEastOrWest(void);

void GabombaStatue_HopAcrossNorthOrSouth(void);

void GabombaStatue_HearTheOffering(void);

void GabombaStatue_StepIntoBoneArmletChestHole(void);

void GabombaStatue_StepIntoElixirChestHole(void);

void GabombaStatue_StepIntoFirstSteelHole(void);

void GabombaStatue_StepIntoThirdSteelHole(void);
void GabombaStatue_StepIntoFirstTurntableHole(void);

void GabombaStatue_StepIntoSecondTurntableHole(void);

void GabombaStatue_StepIntoCogsHole(void);

void GabombaStatue_JumpDownHole(void);

void GabombaStatue_PinionThrowsLeader(void);
void GabombaStatue_CogsTurnBack(void);

void GabombaStatue_LowerArrowA(void);
void GabombaStatue_RedrawArrowA(void);
void GabombaStatue_RaiseArrowB(void);
void GabombaStatue_RedrawArrowB(void);

extern s32 gGabombaStatue_TurntableScript[];
extern s32 gGabombaStatue_LookAroundScript[];
extern u32 gGabombaStatue_JennaFocusScript[];
extern u32 gGabombaStatue_PiersFocusScript[];
extern u32 gGabombaStatue_KradenFocusScript[];
extern u32 gGabombaStatue_KradenPointsScript[];
extern s32 gGabombaStatue_StepBackScript[];
extern s32 gGabombaStatue_GirlsStepBackScript[];

extern u32 gGabombaStatue_PinionReleaseScript[];

extern u32 gGabombaStatue_JennaWatchesScript[];
extern u32 gGabombaStatue_ShebaWatchesScript[];

void GabombaStatue_RestartClock(void);

void GabombaStatue_DimBoneArmletChestActors(void);
void GabombaStatue_DimElixirChestActors(void);

s32 GabombaStatue_GetPuzzleSeed(void);
s32 GabombaStatue_SetUpChannelPuzzle(s32 x, s32 z, s32 value, s32 id);
void GabombaStatue_FindFreeTileCell(u32 *x_out, u32 *z_out);

void Import_ActorRelease(struct Actor *object);

extern u32 gGabombaStatue_LookAtChannelScript[];

void GabombaStatue_DrawCogCells(void);

void GabombaStatue_InitializeBoneArmletChest(void);
void GabombaStatue_InitializeElixirChest(void);
void GabombaStatue_InitializeSteel(void);
void GabombaStatue_InitializeCogs(void);
void GabombaStatue_InitializeArrows(void);
void GabombaStatue_InitializePinion(void);
void GabombaStatue_StartClock();
void GabombaStatue_ResetClock();
s32 GabombaStatue_LayPath(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c,
                          s32 d);
void GabombaStatue_WritePiece(s32 x, s32 z, s32 kind, s32 state);
s32 abs(s32);
s32 GabombaStatue_PickPiece(s32 row, s32 column, s32 direction);
void GabombaStatue_ShowTiles(s32 id, s32 show);

void GabombaStatue_LoadTileGraphics(void);
void GabombaStatue_ClearGrid(void);
s32 GabombaStatue_LayChannel(s32 value);

void GabombaStatue_CountDownShowingClock(void);
void GabombaStatue_CountDown(void);
void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void GabombaStatue_FadeByDepth(void);

void Actors_SetHidden(s32 show, s32 first, u32 count);

void GabombaStatue_LandFromFall(void);

void GabombaStatue_DropIntoHole(s32 x, s32 z);

s32 GabombaStatue_GetClock(void);
extern u32 gGabombaStatue_CogCellPictures[];

void GabombaStatue_CoilRope(s32 index, s32 a, s32 b);
void GabombaStatue_PlaceRope(s32 index, s32 flag, s32 a, s32 b);

#endif
