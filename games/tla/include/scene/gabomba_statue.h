#ifndef ALCHEMY_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_SCENE_GABOMBA_STATUE_H

// Gabomba Statue's scripts, scripts and the functions of its clock, cogs, ladders and channel
// puzzle, and fdActorSetMetatileOpen and scActorsSetDrawn, which other scenes keep a copy of.

#include "types.h"
#include "resource_ids.h"

struct ACTOR;

// The C library's absolute value, which the channel puzzle measures its paths with.
Sint32        abs(Sint32);

void          scGabombaStatueTurnCogs(Sint32 direction);

void          scGabombaStatueStopClock(void);
Sint32        scGabombaStatueRunCharges(Sint32 first, Sint32 check);

void          scGabombaStatueTurnWithCog(void);

void          scGabombaStatueRideSlidingCog(void);

void          scGabombaStatueRideRoller(void);

void          scGabombaStatueHopAcrossEastOrWest(void);

void          scGabombaStatueHopAcrossNorthOrSouth(void);

void          scGabombaStatueHearTheOffering(void);

void          scGabombaStatueClimbDownBoneArmletChestLadder(void);

void          scGabombaStatueClimbDownElixirChestLadder(void);

void          scGabombaStatueClimbDownSteelNortheastLadder(void);

void          scGabombaStatueClimbDownSteelEastLadder(void);
void          scGabombaStatueClimbDownMouseNorthLadder(void);

void          scGabombaStatueClimbDownMouseWestLadder(void);

void          scGabombaStatueClimbDownCogsLadder(void);

void          scGabombaStatueClimbDownArrowsLadder(void);

void          scGabombaStatuePinionsThrowLeaderSouth(void);
void          scGabombaStatueCogsTurnBack(void);

void          scGabombaStatuePressLeftArrowPanel(void);
void          scGabombaStatueReleaseLeftArrowPanel(void);
void          scGabombaStatuePressRightArrowPanel(void);
void          scGabombaStatueReleaseRightArrowPanel(void);

extern Sint32 GabombaStatueFlashCogScript[];
extern Sint32 GabombaStatueTurnToothScript[];
extern Uint32 GabombaStatueJennaFocusScript[];
extern Uint32 GabombaStatuePiersFocusScript[];
extern Uint32 GabombaStatueKradenFocusScript[];
extern Uint32 GabombaStatueKradenPointsScript[];
extern Sint32 GabombaStatueLookNorthAndEastScript[];
extern Sint32 GabombaStatueLookNorthAndWestScript[];

extern Uint32 GabombaStatuePinionReleaseScript[];

extern Uint32 GabombaStatueJennaWatchesScript[];
extern Uint32 GabombaStatueAkafubuComesUpScript[];

void          scGabombaStatueRestartClock(void);

void          scGabombaStatueHideBoneArmletChestLadderTopBlock(void);
void          scGabombaStatueHideElixirChestLadderTopBlocks(void);

Sint32        scGabombaStatueGetPuzzleSeed(void);
Sint32        scGabombaStatueSetUpChannelPuzzle(Sint32 x, Sint32 z, Sint32 value, Sint32 id);
void          scGabombaStatueFindBrokenMetatile(Uint32 *x_out, Uint32 *z_out);

extern Uint32 GabombaStatueLookAtChannelScript[];

void          scGabombaStatueDrawCogMetatiles(void);

void          scGabombaStatueInitBoneArmletChest(void);
void          scGabombaStatueInitElixirChest(void);
void          scGabombaStatueInitSteel(void);
void          scGabombaStatueInitCogs(void);
void          scGabombaStatueInitArrows(void);
void          scGabombaStatueInitPinion(void);
void          scGabombaStatueStartClock(Sint32 mode);
void          scGabombaStatueResetClock(void);
Sint32        scGabombaStatueLayPath(Sint32 x0,
                                     Sint32 z0,
                                     Sint32 endX,
                                     Sint32 endZ,
                                     Sint32 length,
                                     Sint32 direction,
                                     Sint32 last,
                                     Sint32 bit);
void          scGabombaStatueWritePiece(Sint32 x, Sint32 z, Sint32 kind, Sint32 state);
Sint32        scGabombaStatuePickPiece(Sint32 row, Sint32 column, Sint32 direction);
void          scGabombaStatueSetBlocksRaised(Sint32 id, Sint32 show);

void          scGabombaStatueLoadBlockPictures(void);
void          scGabombaStatueRedrawChannel(void);
Sint32        scGabombaStatueLayChannel(Sint32 value);

void          scGabombaStatueCountDownShowingClock(void);
void          scGabombaStatueCountDown(void);
void          fdActorSetMetatileOpen(Sint32 id, Uint32 mode, Sint32 flag, Sint32 layer);

void          scGabombaStatueFadeByDepth(void);

void          scActorsSetDrawn(Sint32 show, Sint32 first, Uint32 count);

void          scGabombaStatueLandFromFall(void);

void          scGabombaStatueClimbDownLadder(Sint32 x, Sint32 z);

Sint32        scGabombaStatueGetClock(void);
extern Uint32 GabombaStatueCogShapes[];

// Coils a rope at its first post (fdRopeCoil, through the overlay's own import row).
void          scGabombaStatueCoilRope(Sint32 index, Sint32 firstPost, Sint32 secondPost);
// Places a rope between its posts, tied when its flag is set, else coiled (fdRopePlace, through the
// overlay's own import row).
void scGabombaStatuePlaceRope(Sint32 index, Sint32 flag, Sint32 firstPost, Sint32 secondPost);

#endif
