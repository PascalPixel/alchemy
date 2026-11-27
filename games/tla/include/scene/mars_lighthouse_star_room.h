#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H

/* @file */
/* Mars Lighthouse's star room: its torches, fireballs and the Mars Star, and */
/* the knock-back, flame and crack functions it shares with the rooms that keep
 */
/* copies of them. */

#include "types.h"
#include "field/cell_target.h"

struct ACTOR;
struct CELL_TARGET;
struct CELL_TARGET_ENTRY;

s32 fdLeaderKnockBack(void);
void fdActorLaunchAhead(struct ACTOR *actor);

void scMarsLighthouseStarRoomShootFireballs(void);

void scFlameThrowersPuff(void);

void gmPartyTakeFallDamage(void);

void scActorRollAndCrumble(struct ACTOR *actor);

void fdLeaderStopIfPending(void);
void fdActorSteerByPad(struct ACTOR *actor);

void scCrackedTileCrack(CELL_TARGET *table, CELL_TARGET_ENTRY *entry);

void scMarsLighthouseStarRoomStartBlazeOnEastTorch(void);

void scMarsLighthouseStarRoomBlazeWestTorch(void);

void scMarsLighthouseStarRoomWestFireballCracksIce(void);

void scMarsLighthouseStarRoomEastFireballCracksIce(void);

void scMarsLighthouseStarRoomWakeFlameDragons(void);

void scMarsLighthouseStarRoomHoldWarmHand(void);

void scMarsLighthouseStarRoomTakeMarsStar(void);

void scMarsLighthouseStarRoomUseMarsStar(void);

void scMarsLighthouseStarRoomEndBlazeOnWestTorch(void);

#endif
