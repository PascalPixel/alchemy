#ifndef ALCHEMY_BATTLE_STUBS_H
#define ALCHEMY_BATTLE_STUBS_H

/* @file */
/* The battle module's stub calls: running a battle, its camera and background,
 */
/* and how its units move. */

struct SCR_POINT;

void btDebugBattleRunMenuStub(void);
void btObjectSpawnObjectsForListStub(s16 *unitIds, s32 refreshPoses);
void btCameraSetViewStub(s32 x, s32 depth, s32 y, s32 zoom);
void btCameraSetViewAtPairMidpointStub(s32 firstUnitId, s32 secondUnitId,
									   s32 zoom);
void btBackgroundSetModeStub(s32 mode, s32 unusedResource, s32 fade);
void btBackgroundLoadStub(s32 mode, s32 resourceId, s32 level);
void btBackgroundSetPaletteLevelStub(s32 unused, s32 level);
s32 btSprHasHeavyImpactStub(s32 index);
s32 btUnitGetEffectHeightStub(s32 unitId);
void btMotionApproachTargetStub(s32 unitId, s32 targetId, s32 frames,
								s32 initialVelocityY);
void btMotionApplyKnockbackStub(s32 unitId, s32 variant);
s32 btUnitListIdsStub(s32 sideMask, u16 *unitIds);
/* Returns a unit's battle object, or null for a unit with no place */
/* (btObjectGet); the battle effects find the sprites they move through */
/* it. */
struct BATTLE_OBJECT *btObjectGetStub(s32 unitId);
s32 btRunStub(s32 enemyGroup);
s32 btMotionProjectPosStub(s32 unitId, s32 *projected);
s32 btMotionProjectScaledPosStub(s32 unitId, s32 *projected);
/* Projects the point above a unit, where the target cursor points, onto the */
/* screen. */
s32 btMotionProjectCursorPointStub(s32 unitId, struct SCR_POINT *projected);
s32 btPartyPrepareMembersStub(u16 *unitIds);
void btCameraSetCommandViewStub(s32 verticalOffset);
void *fdActorGetSprStub(struct ACTOR *actor, s32 recordIndex);
void btHighlightSetModesStub(u16 *selectedUnits, s32 mode);
void btPlaybackStartStub(s32 cue);
s32 mnGraphicsScaleRgb555ClampedStub(u16 *source, u16 *destination, s32 scale,
									 s32 count);
s32 btDjinnRecoveryCountDownStub(void);
s32 btObjectGetPaletteStub(s32 id);
s32 btDjinnRecoveryCountDownTwentyStub(void);
s32 btDebugParmSetLoadStub(s32 set);
void btObjectPutStub(void);
UNREAD_RESULT mnBattleMessageWaitForAdvanceAtStub(s32 x, s32 y);
s32 btDjinnCountStandbyStub(u32 unitId, u8 *counts);

#endif
