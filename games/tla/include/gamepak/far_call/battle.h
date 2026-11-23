#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// The far calls into the battle bank, as the other banks' code calls them.

#include "fixed_math.h"
#include "types.h"

struct TargetPoint;
/// Returns the height above a unit where its battle effects play, in 16.16
/// pixels.
FIXED BattleUnit_GetEffectHeightFar(s32 unitId);
/// Loads a battle background, its palette darkened by level sixtieths.
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
/// Points the battle camera at (x, depth, y) with a zoom in percent.
void BattleCamera_SetViewFar(s32 x, s32 depth, s32 y, s32 zoom);
/// Darkens the battle background's palette by level sixtieths; 0 restores it.
void BattleBackground_SetPaletteLevelFar(s32 unused, s32 level);
/// Returns sprite recordIndex of a unit's motion object, or null.
void *GetMotionRecordFar(void *object, s32 recordIndex);
/// Projects the point above a unit, where the target cursor points, onto the
/// screen.
s32 BattleMotion_ProjectConditionalPositionFar(s32 unitId,
                                               struct TargetPoint *screen);
/// Readies the sprites of the listed units and destroys every other unit's.
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 refresh);
/// Destroys a unit's sprites, its status icon and shadow with them.
void ReleaseBattleObjectRecordsFar(s32 unitId);
/// Readies every unit's sprites, destroying those of units no longer in the
/// battle, and sets each unit's pose again.
void BattleObject_PutFar(void);
/// Knocks a unit back as variant (enum BattleKnockback) says.
void BattleMotion_ApplyKnockbackFar(s32 unitId, s32 variant);

/// Plays the action on its targets as the blow lands, with cue as the hit's
/// sound (0 keeps the one set).
void BattlePlayback_StartFar(s32 cue);
/// Writes count colours of source to destination scaled by scale / 256.
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
/// Switches the battle background's display mode, darkened by fade.
void BattleBackground_SetModeFar(s32 mode, s32 background, s32 fade);
/// Replaces the first enemy with the Doom Dragon's next form after formation.
void Battle_ChangeEnemyFormFar(s32 formation);
/// Returns the palette of a unit's battle object.
s32 BattleMotion_GetSlotPaletteFar(s32 unitId);
/// Lists the party's reserve members into owners and returns how many there
/// are.
s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
/// As BattleUnit_ListIds, listing only the units whose HP is not 0.
s32 BattleUnit_ListLivingIdsFar(s32 sideMask, u16 *unitIds);
/// Works out which summons the party can call.
void BattleSummon_UpdateAvailabilityFar(void);
/// Turns the battle camera to the view the commands are chosen in, raised by
/// verticalOffset pixels.
void BattleCamera_SetCommandViewFar(s32 verticalOffset);

#endif
