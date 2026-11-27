#ifndef ALCHEMY_BATTLE_STUBS_H
#define ALCHEMY_BATTLE_STUBS_H

/* The stub calls into the battle module, as the other modules' code calls
   them. */

#include "fixed_math.h"
#include "types.h"

struct TARGET_POINT;
/* Returns the height above a unit where its battle effects play, in 16.16
   pixels. */
FIXED btUnitGetEffectHeightStub(Sint32 unitId);
/* Loads a battle background, its palette darkened by level sixtieths. */
void btBackgroundLoadStub(Sint32 mode, Sint32 resource, Sint32 level);
/* Points the battle camera at (x, depth, y) with a zoom in percent. */
void btCameraSetViewStub(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
/* Darkens the battle background's palette by level sixtieths; 0 restores it. */
void btBackgroundSetPaletteLevelStub(Sint32 unused, Sint32 level);
/* Returns sprite recordIndex of a unit's motion object, or null. */
void *btActorGetSprStub(void *object, Sint32 recordIndex);
/* Projects the point above a unit, where the target cursor points, onto the
   screen. */
Sint32 btMotionProjectCursorPointStub(
	Sint32 unitId, struct TARGET_POINT *screen);
/* Readies the sprites of the listed units and destroys every other unit's. */
void btActorSpawnObjectsForListStub(Sint16 *list, Sint32 refresh);
/* Destroys a unit's sprites, its status icon and shadow with them. */
void btObjectDestroySprsStub(Sint32 unitId);
/* Readies every unit's sprites, destroying those of units no longer in the
   battle, and sets each unit's pose again. */
void btObjectPutStub(void);
/* Knocks a unit back as variant (BATTLE_KNOCKBACK) says. */
void btMotionApplyKnockbackStub(Sint32 unitId, Sint32 variant);

/* Plays the action on its targets as the blow lands, with sound as the hit's
   sound (0 keeps the one set). */
void btPlaybackStartStub(Sint32 snd);
/* Writes count colours of source to destination scaled by scale / 256. */
void btColBufferScaleStub(
	void *source, void *destination, Sint32 scale, Sint32 count);
/* Switches the battle background's display mode, darkened by fade. */
void btBackgroundSetModeStub(Sint32 mode, Sint32 background, Sint32 fade);
/* Replaces the first enemy with the Doom Dragon's next form after formation. */
void btChangeEnemyFormStub(Sint32 formation);
/* Returns the palette of a unit's battle object. */
Sint32 btMotionGetSlotPaletteStub(Sint32 unitId);
/* Lists the party's reserve members into owners and returns how many there
   are. */
Sint32 btPartyPrepareReserveOwnersStub(Uint16 *owners);
/* As btUnitListIds, listing only the units whose HP is not 0. */
Sint32 btUnitListLivingIdsStub(Sint32 sideMask, Uint16 *unitIds);
/* Works out which summons the party can call. */
void btSummonUpdateAvailabilityStub(void);
/* Turns the battle camera to the view the commands are chosen in, raised by
   verticalOffset pixels. */
void btCameraSetCommandViewStub(Sint32 verticalOffset);

/* Lists the units on the sides in sideMask into unitIds, or only counts them
   when it is NULL, and returns how many. */
Sint32 btUnitListIdsStub(Sint32 sideMask, Uint16 *unitIds);
/* Lists the party members who fight into owners when it is not NULL, and
   returns how many. */
Sint32 btPartyPrepareMembersStub(Uint16 *owners);

#endif
