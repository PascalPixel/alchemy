#ifndef ALCHEMY_BATTLE_STUBS_H
#define ALCHEMY_BATTLE_STUBS_H

/* The battle module's stub calls: running a battle, its camera and background,
   and how its units move. */

struct SCREEN_POINT;

void btDebugBattleRunMenuStub(void);
void btObjectSpawnObjectsForListStub(Sint16 *unitIds, Sint32 refreshPoses);
void btCameraSetViewStub(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
void btCameraSetViewAtPairMidpointStub(
	Sint32 firstUnitId, Sint32 secondUnitId, Sint32 zoom);
void   btBackgroundSetModeStub(Sint32 mode, Sint32 unusedResource, Sint32 fade);
void   btBackgroundLoadStub(Sint32 mode, Sint32 resourceId, Sint32 level);
void   btBackgroundSetPaletteLevelStub(Sint32 unused, Sint32 level);
Sint32 btSpriteHasHeavyImpactStub(Sint32 index);
Sint32 btUnitGetEffectHeightStub(Sint32 unitId);
void   btMotionApproachTargetStub(
	  Sint32 unitId, Sint32 targetId, Sint32 ticks, Sint32 initVelocityY);
void   btMotionApplyKnockbackStub(Sint32 unitId, Sint32 variant);
Sint32 btUnitListIdsStub(Sint32 sideMask, Uint16 *unitIds);
/* Returns a unit's battle object, or null for a unit with no place
   (btObjectGet); the battle effects find the sprites they move through it. */
struct BATTLE_OBJECT *btObjectGetStub(Sint32 unitId);
Sint32				  btRunStub(Sint32 enemyGroup);
Sint32				  btMotionProjectPosStub(Sint32 unitId, Sint32 *projected);
Sint32 btMotionProjectScaledPosStub(Sint32 unitId, Sint32 *projected);
/* Projects the point above a unit, where the target cursor points, onto the
   screen. */
Sint32 btMotionProjectCursorPointStub(
	Sint32 unitId, struct SCREEN_POINT *projected);
Sint32 btPartyPrepareMembersStub(Uint16 *unitIds);
void   btCameraSetCommandViewStub(Sint32 verticalOffset);
void  *btActorGetSpriteStub(struct ACTOR *actor, Sint32 recordIndex);
void   btHighlightSetModesStub(Uint16 *selectedUnits, Sint32 mode);
void   btPlaybackStartStub(Sint32 sound);
Sint32 btGraphicsScaleRGB555ClampedStub(
	Uint16 *src, Uint16 *dest, Sint32 scale, Sint32 count);
Sint32		  btDjinnRecoveryCountDownStub(void);
Sint32		  btObjectGetPaletteStub(Sint32 id);
Sint32		  btDjinnRecoveryCountDownTwentyStub(void);
Sint32		  btDebugParamSetLoadStub(Sint32 set);
void		  btObjectPutStub(void);
UNREAD_RESULT btMessageWaitForAdvanceAtStub(Sint32 x, Sint32 y);
Sint32		  btDjinnCountStandbyStub(Uint32 unitId, Uint8 *counts);

#endif
