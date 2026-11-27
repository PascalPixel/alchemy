#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

/* @file */
/* Mars Lighthouse's entrance: its events of fireballs, ice and teleport */
/* circles, and the teleport and cracked-tile functions other scenes keep a */
/* copy of and declare from here. */

#include "types.h"
#include "field/cell_target.h"

struct ACTOR;
struct CELL_TARGET;
struct CELL_TARGET_ENTRY;
struct MAP_CELL;
struct POSITION;

/* The leader is thrown back by the flames that struck them */
/* (shared/leader_knock_back.c). */
s32 fdLeaderKnockBack(void);

void scMarsLighthouseEntranceDrawLapisCrackedSheet(void);

void scMarsLighthouseEntranceShootFireballAtHiddenCircleIceSheet(void);

void scTeleportStartShimmer(void);
void scTeleportStopShimmer(void);

void scMarsLighthouseEntranceShootFireballAtLapisIceSheet(void);

void scMarsLighthouseEntranceShootFireballThroughIce(void);

void scMarsLighthouseEntranceFallThroughCrack(const struct MAP_CELL *cell);

void scMarsLighthouseEntranceFlingIceCone(struct ACTOR *actor);

s32 scActorAttach(s32 torchId, s32 sparkleId, s32 fireballId, s32 facing,
				  const void *path);

void scLeaderSlideOnIceFromEdge(void);

s32 scMarsLighthouseEntranceRideFireballIfHit(void);

void fdLeaderSlideOnIceAhead(void);

void scMarsLighthouseEntranceDrawHoleMask(void);

s32 scFlameThrowersHasHitLeader(void);

void scLeaderSetSlideStopped(s32 value);

void scMarsLighthouseEntranceScatterIceChips(s32 value);

void scMarsLighthouseEntranceShootValkyrieLowerFireball(void);
void scMarsLighthouseEntranceShootBalmFireball(void);

s32 fdActorIsTouching(struct ACTOR *a, struct ACTOR *b);

s32 fdActorIsBeside(struct ACTOR *a, struct ACTOR *b);
/* A flame thrower's flame sent forward, backward, east or west from */
/* `position` at `speed` (shared/flame_thrower_send_*.c); `offScreen` keeps its
 */
/* sprite's picture loaded out of sight. */
/* @{ */
void scFlameThrowerSendForward(s32 *position, s32 speed, s32 offScreen);
void scFlameThrowerSendBackward(s32 *position, s32 speed, s32 offScreen);
void scFlameThrowerSendEast(s32 *position, s32 speed, s32 offScreen);
void scFlameThrowerSendWest(s32 *position, s32 speed, s32 offScreen);
/* @} */

void scActorRollAndCrumble(struct ACTOR *actor);

void scCrackedTileCrack(CELL_TARGET *table, CELL_TARGET_ENTRY *entry);

void scTeleportArriveWhirling(void);

s32 syListCountEntries(u16 *list);
void fdPaletteCycleFade(void);
void fdActorSetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void scMarsLighthouseEntranceCrackLapisIceSheet(void);

void scMarsLighthouseEntranceCrackHiddenCircleIceSheet(void);

void scMarsLighthouseEntranceMeltIceInPath(void);

void scMarsLighthouseEntranceClearFireballRow(void);

void scMarsLighthouseEntranceStartPoundIceCone(s32 mode, s32 index);

void scMarsLighthouseEntranceCheckIce(void);

void scMarsLighthouseEntranceTeleportFromLapisNorthCircle(void);

void scMarsLighthouseEntranceBlazeLapisTorch(void);

void scMarsLighthouseEntranceBlockRetreatAtChest(void);

void scMarsLighthouseEntranceTeleportFromBalm(void);

void scMarsLighthouseEntranceBlazeHiddenCircleWestTorch(void);

void scMarsLighthouseEntranceWatchFireballCrackHiddenCircleIce(void);

void scMarsLighthouseEntranceBlazeHiddenCircleEastTorch(void);

void scMarsLighthouseEntranceWatchFireballMeltIce(void);

void scMarsLighthouseEntranceTeleportFromHiddenCircle(void);

void scMarsLighthouseEntranceHideTeleportCircle(void);

void scMarsLighthouseEntranceTeleportFromValkyrieNorth(void);

void scMarsLighthouseEntranceTeleportFromValkyrieSouth(void);

void scMarsLighthouseEntranceWakeDragonFace(void);

void scMarsLighthouseEntranceSlideFromEdge(void);

int scMarsLighthouseEntranceRideFireball(void);
void fdLeaderSlideOnIce(void);

void fdLeaderStopIfPending(void);

void scMarsLighthouseEntranceRevealTeleportCircle(void);

#endif
