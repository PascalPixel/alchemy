#ifndef ALCHEMY_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_SCENE_GABOMBA_STATUE_H

/// @file
/// Gabomba Statue's events, scripts and the routines of its clock, cogs,
/// ladders and channel puzzle, and Actor_SetCellOpen and Actors_SetDrawn,
/// which other scenes keep a copy of.

#include "types.h"
#include "resource_ids.h"

struct Actor;

/// The C library's absolute value, which the channel puzzle measures its
/// paths with.
s32 abs(s32);

void GabombaStatue_TurnCogs(s32 direction);

void GabombaStatue_StopClock(void);
s32 GabombaStatue_RunCharges(s32 first, s32 check);

void GabombaStatue_TurnWithCog(void);

void GabombaStatue_RideSlidingCog(void);

void GabombaStatue_RideRoller(void);

void GabombaStatue_HopAcrossEastOrWest(void);

void GabombaStatue_HopAcrossNorthOrSouth(void);

void GabombaStatue_HearTheOffering(void);

void GabombaStatue_ClimbDownBoneArmletChestLadder(void);

void GabombaStatue_ClimbDownElixirChestLadder(void);

void GabombaStatue_ClimbDownSteelNortheastLadder(void);

void GabombaStatue_ClimbDownSteelEastLadder(void);
void GabombaStatue_ClimbDownMouseNorthLadder(void);

void GabombaStatue_ClimbDownMouseWestLadder(void);

void GabombaStatue_ClimbDownCogsLadder(void);

void GabombaStatue_ClimbDownArrowsLadder(void);

void GabombaStatue_PinionsThrowLeaderSouth(void);
void GabombaStatue_CogsTurnBack(void);

void GabombaStatue_PressLeftArrowPanel(void);
void GabombaStatue_ReleaseLeftArrowPanel(void);
void GabombaStatue_PressRightArrowPanel(void);
void GabombaStatue_ReleaseRightArrowPanel(void);

extern s32 gGabombaStatue_FlashCogScript[];
extern s32 gGabombaStatue_TurnToothScript[];
extern u32 gGabombaStatue_JennaFocusScript[];
extern u32 gGabombaStatue_PiersFocusScript[];
extern u32 gGabombaStatue_KradenFocusScript[];
extern u32 gGabombaStatue_KradenPointsScript[];
extern s32 gGabombaStatue_LookNorthAndEastScript[];
extern s32 gGabombaStatue_LookNorthAndWestScript[];

extern u32 gGabombaStatue_PinionReleaseScript[];

extern u32 gGabombaStatue_JennaWatchesScript[];
extern u32 gGabombaStatue_AkafubuComesUpScript[];

void GabombaStatue_RestartClock(void);

void GabombaStatue_HideBoneArmletChestLadderTopBlock(void);
void GabombaStatue_HideElixirChestLadderTopBlocks(void);

s32 GabombaStatue_GetPuzzleSeed(void);
s32 GabombaStatue_SetUpChannelPuzzle(s32 x, s32 z, s32 value, s32 id);
void GabombaStatue_FindBrokenCell(u32 *x_out, u32 *z_out);

extern u32 gGabombaStatue_LookAtChannelScript[];

void GabombaStatue_DrawCogCells(void);

void GabombaStatue_InitializeBoneArmletChest(void);
void GabombaStatue_InitializeElixirChest(void);
void GabombaStatue_InitializeSteel(void);
void GabombaStatue_InitializeCogs(void);
void GabombaStatue_InitializeArrows(void);
void GabombaStatue_InitializePinion(void);
void GabombaStatue_StartClock(s32 mode);
void GabombaStatue_ResetClock(void);
s32 GabombaStatue_LayPath(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length,
                          s32 direction, s32 last, s32 bit);
void GabombaStatue_WritePiece(s32 x, s32 z, s32 kind, s32 state);
s32 GabombaStatue_PickPiece(s32 row, s32 column, s32 direction);
void GabombaStatue_SetBlocksRaised(s32 id, s32 show);

void GabombaStatue_LoadBlockPictures(void);
void GabombaStatue_RedrawChannel(void);
s32 GabombaStatue_LayChannel(s32 value);

void GabombaStatue_CountDownShowingClock(void);
void GabombaStatue_CountDown(void);
void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void GabombaStatue_FadeByDepth(void);

void Actors_SetDrawn(s32 show, s32 first, u32 count);

void GabombaStatue_LandFromFall(void);

void GabombaStatue_ClimbDownLadder(s32 x, s32 z);

s32 GabombaStatue_GetClock(void);
extern u32 gGabombaStatue_CogShapes[];

/// Coils a rope at its first post (Rope_Coil, through the overlay's own import
/// row).
void GabombaStatue_CoilRope(s32 index, s32 firstPost, s32 secondPost);
/// Places a rope between its posts, tied when its flag is set, else coiled
/// (Rope_Place, through the overlay's own import row).
void GabombaStatue_PlaceRope(s32 index, s32 flag, s32 firstPost,
                             s32 secondPost);

#endif
