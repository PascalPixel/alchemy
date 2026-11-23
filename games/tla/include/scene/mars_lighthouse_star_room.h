#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H

/// @file
/// Mars Lighthouse's star room: its torches, fireballs and the Mars Star, and
/// the knock-back, flame and crack routines it shares with the rooms that keep
/// copies of them.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

s32 Leader_KnockBack(void);
void Actor_LaunchAhead(struct Actor *actor);

void MarsLighthouseStarRoom_ShootFireballs(void);

void FlameThrowers_Puff(void);

void Party_TakeFallDamage(void);

void Actor_RollAndCrumble(struct Actor *actor);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void MarsLighthouseStarRoom_StartBlazeOnEastTorch(void);

void MarsLighthouseStarRoom_BlazeWestTorch(void);

void MarsLighthouseStarRoom_WestFireballCracksIce(void);

void MarsLighthouseStarRoom_EastFireballCracksIce(void);

void MarsLighthouseStarRoom_WakeFlameDragons(void);

void MarsLighthouseStarRoom_HoldWarmHand(void);

void MarsLighthouseStarRoom_TakeMarsStar(void);

void MarsLighthouseStarRoom_UseMarsStar(void);

void MarsLighthouseStarRoom_EndBlazeOnWestTorch(void);

#endif
