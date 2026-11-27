#ifndef ALCHEMY_BATTLE_STUBS_H
#define ALCHEMY_BATTLE_STUBS_H

/* @file */
/* The stub calls into the battle module, as the other modules' code calls */
/* them. */

#include "fixed_math.h"
#include "types.h"

struct TARGET_POINT;
/* Returns the height above a unit where its battle effects play, in 16.16 */
/* pixels. */
FIXED btUnitGetEffectHeightStub(s32 unitId);
/* Loads a battle background, its palette darkened by level sixtieths. */
void btBackgroundLoadStub(s32 mode, s32 resource, s32 level);
/* Points the battle camera at (x, depth, y) with a zoom in percent. */
void btCameraSetViewStub(s32 x, s32 depth, s32 y, s32 zoom);
/* Darkens the battle background's palette by level sixtieths; 0 restores it. */
void btBackgroundSetPaletteLevelStub(s32 unused, s32 level);
/* Returns sprite recordIndex of a unit's motion object, or null. */
void *fdActorGetSprStub(void *object, s32 recordIndex);
/* Projects the point above a unit, where the target cursor points, onto the */
/* screen. */
s32 btMotionProjectCursorPointStub(s32 unitId, struct TARGET_POINT *screen);
/* Readies the sprites of the listed units and destroys every other unit's. */
void btActorSpawnObjectsForListStub(s16 *list, s32 refresh);
/* Destroys a unit's sprites, its status icon and shadow with them. */
void btObjectDestroySprsStub(s32 unitId);
/* Readies every unit's sprites, destroying those of units no longer in the */
/* battle, and sets each unit's pose again. */
void btObjectPutStub(void);
/* Knocks a unit back as variant (BATTLE_KNOCKBACK) says. */
void btMotionApplyKnockbackStub(s32 unitId, s32 variant);

/* Plays the action on its targets as the blow lands, with cue as the hit's */
/* sound (0 keeps the one set). */
void btPlaybackStartStub(s32 cue);
/* Writes count colours of source to destination scaled by scale / 256. */
void syColBufferScaleStub(void *source, void *destination, s32 scale,
						  s32 count);
/* Switches the battle background's display mode, darkened by fade. */
void btBackgroundSetModeStub(s32 mode, s32 background, s32 fade);
/* Replaces the first enemy with the Doom Dragon's next form after formation. */
void btChangeEnemyFormStub(s32 formation);
/* Returns the palette of a unit's battle object. */
s32 btMotionGetSlotPaletteStub(s32 unitId);
/* Lists the party's reserve members into owners and returns how many there */
/* are. */
s32 btPartyPrepareReserveOwnersStub(u16 *owners);
/* As btUnitListIds, listing only the units whose HP is not 0. */
s32 btUnitListLivingIdsStub(s32 sideMask, u16 *unitIds);
/* Works out which summons the party can call. */
void btSummonUpdateAvailabilityStub(void);
/* Turns the battle camera to the view the commands are chosen in, raised by */
/* verticalOffset pixels. */
void btCameraSetCommandViewStub(s32 verticalOffset);

/* Lists the units on the sides in sideMask into unitIds, or only counts them */
/* when it is NULL, and returns how many. */
s32 btUnitListIdsStub(s32 sideMask, u16 *unitIds);
/* Lists the party members who fight into owners when it is not NULL, and */
/* returns how many. */
s32 btPartyPrepareMembersStub(u16 *owners);

#endif
