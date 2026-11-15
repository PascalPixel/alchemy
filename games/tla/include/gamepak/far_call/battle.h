#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// The far calls into the battle bank, as the other banks' code calls them.

#include "types.h"

struct TargetPoint;
/// Returns the height above a unit where its battle effects play.
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
/// Places the battle camera at a position and zoom for a transition.
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
/// Sets the palette brightness.
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
void *GetMotionRecordFar(void *object, s32 index);
/// Projects the point above unit id onto the screen, into projected.
s32 BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                               struct TargetPoint *point);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
/// Releases all graphics the object owns and marks it as having none.
void ReleaseBattleObjectRecordsFar(s32 actor);
/// Readies every unit's battle graphics, releasing those of departed units, and
/// resets everyone's display mode and motion.
void BattleObject_PutFar(void);
/// Sends a battle object back by its variant's distance and speed, with a hop
/// when it stands on the ground or for the strong variants (5 and up).
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

void BattlePlayback_StartFar(s32 phase);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void BattleBackground_SetModeFar(s32 mode, s32 background, s32 value);
/// Replaces enemy 128 with the next form of the three-part battle of enemy
/// groups 78 to 80.
void Battle_ChangeEnemyFormFar(s32 kind);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
/// As BattleUnit_ListIds, listing only the actors whose HP is not 0.
s32 BattleUnit_ListLivingIdsFar(s32, s32);
/// Works out which summons the party can call.
void BattleSummon_UpdateAvailabilityFar(void);
/// Turns the battle camera to the view the commands are chosen in.
void BattleCamera_SetCommandViewFar(s32 mode);

#endif
