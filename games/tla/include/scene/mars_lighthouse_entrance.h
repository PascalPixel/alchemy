#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

/* Mars Lighthouse's entrance: its scripts of fireballs, ice and teleport
   circles, and the teleport and cracked-tile functions other scenes keep a copy
   of and declare from here. */

#include "types.h"
#include "field/metatile_target.h"

struct ACTOR;
struct METATILE_TARGET;
struct METATILE_TARGET_ENTRY;
struct MAP_METATILE;
struct POSITION;

/* The leader is thrown back by the flames that struck them
   (shared/leader_knock_back.c). */
Sint32 fdLeaderKnockBack(void);

void scMarsLighthouseEntranceDrawLapisCrackedSheet(void);

void scMarsLighthouseEntranceShootFireballAtHiddenCircleIceSheet(void);

void scTeleportStartShimmer(void);
void scTeleportStopShimmer(void);

void scMarsLighthouseEntranceShootFireballAtLapisIceSheet(void);

void scMarsLighthouseEntranceShootFireballThroughIce(void);

void scMarsLighthouseEntranceFallThroughCrack(
	const struct MAP_METATILE *metatile);

void scMarsLighthouseEntranceFlingIceCone(struct ACTOR *actor);

Sint32 scActorAttach(Sint32 torchId, Sint32 sparkleId, Sint32 fireballId,
	Sint32 facing, const void *path);

void scLeaderSlideOnIceFromEdge(void);

Bool scMarsLighthouseEntranceRideFireballIfHit(void);

void fdLeaderSlideOnIceAhead(void);

void scMarsLighthouseEntranceDrawHoleMask(void);

Sint32 scFlameThrowersHasHitLeader(void);

void scLeaderSetSlideStopped(Sint32 value);

void scMarsLighthouseEntranceScatterIceChips(Sint32 value);

void scMarsLighthouseEntranceShootValkyrieLowerFireball(void);
void scMarsLighthouseEntranceShootBalmFireball(void);

Bool fdActorIsTouching(struct ACTOR *a, struct ACTOR *b);

Bool fdActorIsBeside(struct ACTOR *a, struct ACTOR *b);
/* A flame thrower's flame sent forward, backward, east or west from `position`
   at `speed` (shared/flame_thrower_send_*.c); `offScreen` keeps its sprite's
   picture loaded out of sight. */
void scFlameThrowerSendForward(
	Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendBackward(
	Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendEast(Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendWest(Sint32 *position, Sint32 speed, Sint32 offScreen);

void scActorRollAndCrumble(struct ACTOR *actor);

void scCrackedTileCrack(METATILE_TARGET *table, METATILE_TARGET_ENTRY *entry);

void scTeleportArriveWhirling(void);

Sint32 syListCountEntries(Uint16 *list);
void   fdPaletteCycleFade(void);
void fdActorSetMetatileOpen(Sint32 id, Uint32 kind, Sint32 flag, Sint32 layer);

void scMarsLighthouseEntranceCrackLapisIceSheet(void);

void scMarsLighthouseEntranceCrackHiddenCircleIceSheet(void);

void scMarsLighthouseEntranceMeltIceInPath(void);

void scMarsLighthouseEntranceClearFireballRow(void);

void scMarsLighthouseEntranceStartPoundIceCone(Sint32 mode, Sint32 index);

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

int	 scMarsLighthouseEntranceRideFireball(void);
void fdLeaderSlideOnIce(void);

void fdLeaderStopIfPending(void);

void scMarsLighthouseEntranceRevealTeleportCircle(void);

#endif
