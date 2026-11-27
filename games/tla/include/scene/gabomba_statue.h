#ifndef ALCHEMY_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_SCENE_GABOMBA_STATUE_H

/* @file */
/* Gabomba Statue's events, scripts and the functions of its clock, cogs, */
/* ladders and channel puzzle, and fdActorSetCellOpen and scActorsSetDrawn, */
/* which other scenes keep a copy of. */

#include "types.h"
#include "resource_ids.h"

struct ACTOR;

/* The C library's absolute value, which the channel puzzle measures its */
/* paths with. */
s32 abs(s32);

void scGabombaStatueTurnCogs(s32 direction);

void scGabombaStatueStopClock(void);
s32 scGabombaStatueRunCharges(s32 first, s32 check);

void scGabombaStatueTurnWithCog(void);

void scGabombaStatueRideSlidingCog(void);

void scGabombaStatueRideRoller(void);

void scGabombaStatueHopAcrossEastOrWest(void);

void scGabombaStatueHopAcrossNorthOrSouth(void);

void scGabombaStatueHearTheOffering(void);

void scGabombaStatueClimbDownBoneArmletChestLadder(void);

void scGabombaStatueClimbDownElixirChestLadder(void);

void scGabombaStatueClimbDownSteelNortheastLadder(void);

void scGabombaStatueClimbDownSteelEastLadder(void);
void scGabombaStatueClimbDownMouseNorthLadder(void);

void scGabombaStatueClimbDownMouseWestLadder(void);

void scGabombaStatueClimbDownCogsLadder(void);

void scGabombaStatueClimbDownArrowsLadder(void);

void scGabombaStatuePinionsThrowLeaderSouth(void);
void scGabombaStatueCogsTurnBack(void);

void scGabombaStatuePressLeftArrowPanel(void);
void scGabombaStatueReleaseLeftArrowPanel(void);
void scGabombaStatuePressRightArrowPanel(void);
void scGabombaStatueReleaseRightArrowPanel(void);

extern s32 GabombaStatue_FlashCogScript[];
extern s32 GabombaStatue_TurnToothScript[];
extern u32 GabombaStatue_JennaFocusScript[];
extern u32 GabombaStatue_PiersFocusScript[];
extern u32 GabombaStatue_KradenFocusScript[];
extern u32 GabombaStatue_KradenPointsScript[];
extern s32 GabombaStatue_LookNorthAndEastScript[];
extern s32 GabombaStatue_LookNorthAndWestScript[];

extern u32 GabombaStatue_PinionReleaseScript[];

extern u32 GabombaStatue_JennaWatchesScript[];
extern u32 GabombaStatue_AkafubuComesUpScript[];

void scGabombaStatueRestartClock(void);

void scGabombaStatueHideBoneArmletChestLadderTopBlock(void);
void scGabombaStatueHideElixirChestLadderTopBlocks(void);

s32 scGabombaStatueGetPuzzleSeed(void);
s32 scGabombaStatueSetUpChannelPuzzle(s32 x, s32 z, s32 value, s32 id);
void scGabombaStatueFindBrokenCell(u32 *x_out, u32 *z_out);

extern u32 GabombaStatue_LookAtChannelScript[];

void scGabombaStatueDrawCogCells(void);

void scGabombaStatueInitBoneArmletChest(void);
void scGabombaStatueInitElixirChest(void);
void scGabombaStatueInitSteel(void);
void scGabombaStatueInitCogs(void);
void scGabombaStatueInitArrows(void);
void scGabombaStatueInitPinion(void);
void scGabombaStatueStartClock(s32 mode);
void scGabombaStatueResetClock(void);
s32 scGabombaStatueLayPath(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length,
						   s32 direction, s32 last, s32 bit);
void scGabombaStatueWritePiece(s32 x, s32 z, s32 kind, s32 state);
s32 scGabombaStatuePickPiece(s32 row, s32 column, s32 direction);
void scGabombaStatueSetBlocksRaised(s32 id, s32 show);

void scGabombaStatueLoadBlockPictures(void);
void scGabombaStatueRedrawChannel(void);
s32 scGabombaStatueLayChannel(s32 value);

void scGabombaStatueCountDownShowingClock(void);
void scGabombaStatueCountDown(void);
void fdActorSetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void scGabombaStatueFadeByDepth(void);

void scActorsSetDrawn(s32 show, s32 first, u32 count);

void scGabombaStatueLandFromFall(void);

void scGabombaStatueClimbDownLadder(s32 x, s32 z);

s32 scGabombaStatueGetClock(void);
extern u32 GabombaStatue_CogShapes[];

/* Coils a rope at its first post (fdRopeCoil, through the overlay's own import
 */
/* row). */
void scGabombaStatueCoilRope(s32 index, s32 firstPost, s32 secondPost);
/* Places a rope between its posts, tied when its flag is set, else coiled */
/* (fdRopePlace, through the overlay's own import row). */
void scGabombaStatuePlaceRope(s32 index, s32 flag, s32 firstPost,
							  s32 secondPost);

#endif
