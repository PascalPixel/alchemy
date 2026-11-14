#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

void Leader_KnockBack(void);
void Actor_LaunchAhead(struct Actor *object);

void MarsLighthouseStarRoom_ShootFireballs(void);

void FlameThrowers_Puff(void);

void Party_TakeFallDamage(void);

void Actor_RollAndCrumble(struct Actor *actor);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *object);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void MarsLighthouseStarRoom_BeginBlazeOnStatue(void);

void MarsLighthouseStarRoom_BlazeStatue(void);

void MarsLighthouseStarRoom_WestFireballCracksIce(void);

void MarsLighthouseStarRoom_EastFireballCracksIce(void);

void MarsLighthouseStarRoom_WakeFlameDragons(void);

void MarsLighthouseStarRoom_HoldWarmHand(void);

void MarsLighthouseStarRoom_TakeMarsStar(void);

void MarsLighthouseStarRoom_UseMarsStar(void);

void MarsLighthouseStarRoom_EndBlazeOnStatue(void);

#endif
