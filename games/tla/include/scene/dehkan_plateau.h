#ifndef ALCHEMY_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_SCENE_DEHKAN_PLATEAU_H

/* @file */
/* Dehkan Plateau's tables and events, and functions several scenes keep a */
/* copy of and declare from here (fdPoundSinkTarget, fdActorSetCellOpen, */
/* scFieldEffectSpawn). */

#include "types.h"

struct ACTOR;
struct EFFECT_OPTIONS;

/* Cannon: meeting him at the rope bridge, the chase across the cracks and */
/* stakes, the fight in his cave and the Pound Cube he drops. */
void scDehkanPlateauMeetCannon(void);
void scDehkanPlateauCannonLeapsAway(void);
void scDehkanPlateauCannonFleesOverCracks(void);
void scDehkanPlateauCannonPoundsStake(void);
void scDehkanPlateauCannonCracksPillars(void);
void scDehkanPlateauCannonCastsPound(void);
void scDehkanPlateauCannonCastsPoundOnLeader(void);
void scDehkanPlateauCannonPoundsLeaderThrough(void);
void scDehkanPlateauPoundLeaderThrough(void);
void scDehkanPlateauJoltCamera(void);
void scDehkanPlateauCannonCornered(void);
void scDehkanPlateauFightCannon(void);
void scDehkanPlateauCannonDropsPoundCube(void);
void scDehkanPlateauDrawPoundCubeFallLanding(void);
void scDehkanPlateauCreatePoundCube(void);
void scDehkanPlateauTakePoundCube(void);

/* The gray pillars that crack under Felix and crumble at the next step. */
void scDehkanPlateauStepOnFullMetalVestChestPillars(void);
void scDehkanPlateauStepOnThemisAxeChestPillars(void);
void scDehkanPlateauStepOnTightropesPillars(void);
void scDehkanPlateauStepOnCannonPillar(void);
void scDehkanPlateauCrackPillar(s32 id, s32 standingId, s32 flag);
void scDehkanPlateauSetThemisAxeChestStandingPillarCells(void);
void scDehkanPlateauSetTightropesStandingPillarCells(void);

/* The stone and vine pillars Move lifts and Felix pushes into the gaps. */
void scDehkanPlateauDropFullMetalVestChestPillarIntoGap(void);
void scDehkanPlateauLiftThemisAxeChestPillar(void);
void scDehkanPlateauPlaceThemisAxeChestPillar(void);
void scDehkanPlateauLiftTightropesPillar(void);
void scDehkanPlateauPushTightropesSecondPillar(void);
void scDehkanPlateauSetTightropesSecondPillarInPlace(void);
void scDehkanPlateauPushVinePillar(void);
void scDehkanPlateauLiftTightropesVinePillar(void);
void scDehkanPlateauFinishMovingVinePillar(void);

/* The cracks Felix falls through into the caves below, and the ways out. */
void scDehkanPlateauStepOnCrack(void);
void scDehkanPlateauStepOffCrack(void);
void scDehkanPlateauRestoreTightropesCracks(void);
void scDehkanPlateauRestorePoundCubeCracks(void);
void scDehkanPlateauLandFromFall(void);
void scDehkanPlateauEnterLowerPath(void);
void scDehkanPlateauLeaveRoom(void);

/* The unseen blocks that draw Felix behind a cliff's edge. */
void scDehkanPlateauSetBlocksShown(s32 show);
void scDehkanPlateauShowBlocks(void);
void scDehkanPlateauHideBlocks(void);
void scDehkanPlateauHideFullMetalVestChestBlocks(void);
void scDehkanPlateauHideThemisAxeChestBlocks(void);
void scDehkanPlateauHideTightropesBlocks(void);
void scDehkanPlateauHideMintChestBlocks(void);
void scDehkanPlateauHideCannonBlocks(void);

/* Each room's setup as it opens. */
void scDehkanPlateauInitFullMetalVestChest(void);
void scDehkanPlateauInitThemisAxeChest(void);
void scDehkanPlateauInitTightropes(void);
void scDehkanPlateauInitMintChest(void);
void scDehkanPlateauInitPoundCube(void);
void scDehkanPlateauInitCannon(void);
/* Sets up the field engine's ropes for the scene (fdRopeInit). */
void scDehkanPlateauInitRopes(void);

/* A crack record's halfwords: a crack in a room's floor that Felix can fall */
/* through, and the hole it leaves in the ceiling of the cave below. */
typedef enum DEHKAN_PLATEAU_CRACK_FIELD {
	CRACK_STARTS_CRACKED, /* nonzero: cracked when the room is first entered */
	CRACK_X,			  /* its map cell */
	CRACK_Z,
	CRACK_HOLE,	  /* its hole picture, a pair of the room's hole pictures */
	CRACK_HOLE_X, /* the map cell the hole is drawn at */
	CRACK_HOLE_Z,
	CRACK_FIELDS
} DEHKAN_PLATEAU_CRACK_FIELD;

/* The cracks' pictures and records (each room's crack table). */
extern u16 DehkanPlateau_PoundCubeCrackPictures[];
extern u16 DehkanPlateau_PoundCubeCracks[][CRACK_FIELDS];
extern u16 DehkanPlateau_FullMetalVestChestHolePictures[];
extern u16 DehkanPlateau_ThemisAxeChestHolePictures[];
extern u16 DehkanPlateau_TightropesHolePictures[];
extern u16 DehkanPlateau_PoundCubeHolePictures[];

/* Functions and scripts other scenes keep a copy of. */
void fdPoundSinkTarget(u16 *list, s32 mode, s32 count);
void fdActorDropAndWait(struct ACTOR *actor);
void syCrumblingPillarPrepare(s32 id);
void syCrumblingPillarPlace(u16 *list);
void scDustScatter(s32 spread);
void scDustBurst(void);
void fdLeaderSetCellShapeAt(s32 level, s32 layer);
void fdActorSetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);
void fdActorSetCellShape(s32 level, s32 layer, s32 owner);
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const struct EFFECT_OPTIONS *options);
extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];

#endif
