#ifndef ALCHEMY_MODULE_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_MODULE_SCENE_GABOMBA_STATUE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Sprite;

struct Unk02004d48 {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u16 unk66;
};

void GabombaStatue_TurnCogs(s32 value);

void GabombaStatue_StopClock(void);
s32 GabombaStatue_RunCharges(s32 a, s32 b);

void GabombaStatue_TurnWithCog(void);

void GabombaStatue_RideSlidingCog(void);

void GabombaStatue_RideRollingCog(void);

void GabombaStatue_HopAcrossEastOrWest(void);

void GabombaStatue_HopAcrossNorthOrSouth(void);

void GabombaStatue_HearTheOffering(void);

void GabombaStatue_StepIntoRoom1Hole(void);

void GabombaStatue_StepIntoRoom2Hole(void);

void GabombaStatue_StepIntoFirstRoom3Hole(void);

void GabombaStatue_StepIntoThirdRoom3Hole(void);
void GabombaStatue_StepIntoFirstRoom4Hole(void);

void GabombaStatue_StepIntoSecondRoom4Hole(void);

void GabombaStatue_StepIntoRoom5Hole(void);

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

extern u32 gGabombaStatue_Room2GuardScript[];

extern u32 gGabombaStatue_JennaWatchesScript[];
extern u32 gGabombaStatue_ShebaWatchesScript[];

void GabombaStatue_RestartClock(void);

void GabombaStatue_DimRoom1Actors(void);
void GabombaStatue_DimRoom2Actors(void);

s32 GabombaStatue_GetPuzzleSeed(void);
s32 GabombaStatue_SetUpChannelPuzzle(s32 x, s32 z, s32 value, s32 id);
void GabombaStatue_FindFreeTileCell(u32 *x_out, u32 *z_out);

void Import_ActorRelease(struct Actor *object);

extern u32 gGabombaStatue_LookAtChannelScript[];

void GabombaStatue_DrawCogCells(void);

void GabombaStatue_InitializeRoom1(void);
void GabombaStatue_InitializeRoom2(void);
void GabombaStatue_InitializeRoom3(void);
void GabombaStatue_InitializeRoom5(void);
void GabombaStatue_InitializeRoom6(void);
void GabombaStatue_InitializeRoom7(void);
void GabombaStatue_StartClock();
void GabombaStatue_ResetClock();
s32 GabombaStatue_LayPath(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c,
                          s32 d);
void GabombaStatue_WritePiece(s32 col, s32 row, s32 a, s32 b);
s32 abs(s32);
s32 GabombaStatue_PickPiece(s32 from, s32 to, s32 kind);
void GabombaStatue_ShowTiles(s32 id, s32 value);

void GabombaStatue_LoadTileGraphics(void);
void GabombaStatue_ClearGrid(void);
s32 GabombaStatue_LayChannel(s32 value);

void Import_ActorStartIdle();

void GabombaStatue_CountDownShowingClock(void);
void GabombaStatue_CountDown(void);
extern char ResourceId_ClockDigits;
void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void GabombaStatue_FadeByDepth(void);

void Actors_SetHidden(s32 show, s32 first, u32 count);

void GabombaStatue_LandFromFall(void);

void GabombaStatue_DropIntoHole(s32 message, s32 value);

s32 GabombaStatue_GetClock(void);
extern u32 gGabombaStatue_CogCellPictures[];

void GabombaStatue_CoilRope(s32 index, s32 a, s32 b);
void GabombaStatue_PlaceRope(s32 index, s32 flag, s32 a, s32 b);

#endif
