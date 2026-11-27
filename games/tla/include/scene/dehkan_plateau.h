#ifndef ALCHEMY_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_SCENE_DEHKAN_PLATEAU_H

/* Dehkan Plateau's tables and scripts, and functions several scenes keep a copy
   of and declare from here (fdPoundSinkTarget, fdActorSetMetatileOpen,
   scFieldEffectSpawn). */

#include "types.h"

struct ACTOR;
struct EFFECT_OPTIONS;

/* Cannon: meeting him at the rope bridge, the chase across the cracks and
   stakes, the fight in his cave and the Pound Cube he drops. */
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
void scDehkanPlateauCrackPillar(Sint32 id, Sint32 standingId, Sint32 flg);
void scDehkanPlateauSetThemisAxeChestStandingPillarMetatiles(void);
void scDehkanPlateauSetTightropesStandingPillarMetatiles(void);

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
void scDehkanPlateauSetBlocksShown(Sint32 show);
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

/* A crack record's halfwords: a crack in a room's floor that Felix can fall
   through, and the hole it leaves in the ceiling of the cave below. */
typedef enum DEHKAN_PLATEAU_CRACK_FIELD {
	CRACK_STARTS_CRACKED, /* nonzero: cracked when the room is first entered */
	CRACK_X,			  /* its map metatile */
	CRACK_Z,
	CRACK_HOLE,	  /* its hole picture, a pair of the room's hole pictures */
	CRACK_HOLE_X, /* the map metatile the hole is drawn at */
	CRACK_HOLE_Z,
	CRACK_FIELDS
} DEHKAN_PLATEAU_CRACK_FIELD;

/* The cracks' pictures and records (each room's crack table). */
extern Uint16 DehkanPlateauPoundCubeCrackPictures[];
extern Uint16 DehkanPlateauPoundCubeCracks[][CRACK_FIELDS];
extern Uint16 DehkanPlateauFullMetalVestChestHolePictures[];
extern Uint16 DehkanPlateauThemisAxeChestHolePictures[];
extern Uint16 DehkanPlateauTightropesHolePictures[];
extern Uint16 DehkanPlateauPoundCubeHolePictures[];

/* Functions and scripts other scenes keep a copy of. */
void fdPoundSinkTarget(Uint16 *list, Sint32 mode, Sint32 cnt);
void fdActorDropAndWait(struct ACTOR *actor);
void syCrumblingPillarPrepare(Sint32 id);
void syCrumblingPillarPlace(Uint16 *list);
void scDustScatter(Sint32 spread);
void scDustBurst(void);
void fdLeaderSetMetatileShapeAt(Sint32 level, Sint32 layer);
void fdActorSetMetatileOpen(Sint32 id, Uint32 mode, Sint32 flg, Sint32 layer);
void fdActorSetMetatileShape(Sint32 level, Sint32 layer, Sint32 owner);
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const struct EFFECT_OPTIONS *options);
extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

#endif
