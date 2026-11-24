#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

/// @file
/// Mars Lighthouse's entrance: its events of fireballs, ice and teleport
/// circles, and the teleport and cracked-tile routines other scenes keep a
/// copy of and declare from here.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct MapCell;
struct Position;

/// The leader is thrown back by the flames that struck them
/// (shared/leader_knock_back.c).
s32 Leader_KnockBack(void);

void FlameThrower_SendForward();

void MarsLighthouseEntrance_DrawLapisCrackedSheet(void);

void MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet(void);

void Teleport_StartShimmer(void);
void Teleport_StopShimmer(void);

void MarsLighthouseEntrance_ShootFireballAtLapisIceSheet(void);

void MarsLighthouseEntrance_ShootFireballThroughIce(void);

void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell);

void MarsLighthouseEntrance_FlingIceCone(struct Actor *actor);

s32 Actor_Attach(s32 torchId, s32 sparkleId, s32 fireballId, s32 facing,
                 const void *path);

void Leader_SlideOnIceFromEdge(void);

s32 MarsLighthouseEntrance_RideFireballIfHit(void);

void Leader_SlideOnIceAhead(void);

void MarsLighthouseEntrance_DrawHoleMask(void);

s32 FlameThrowers_HasHitLeader(void);

void Leader_SetSlideStopped(s32 value);

void MarsLighthouseEntrance_ScatterIceChips(s32 value);

void MarsLighthouseEntrance_ShootValkyrieLowerFireball(void);
void MarsLighthouseEntrance_ShootBalmFireball(void);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);

s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void FlameThrower_SendBackward();
void FlameThrower_SendEast();
void FlameThrower_SendWest();

void Actor_RollAndCrumble(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void Teleport_ArriveWhirling(void);

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void MarsLighthouseEntrance_CrackLapisIceSheet(void);

void MarsLighthouseEntrance_CrackHiddenCircleIceSheet(void);

void MarsLighthouseEntrance_MeltIceInPath(void);

void MarsLighthouseEntrance_ClearFireballRow(void);

void MarsLighthouseEntrance_StartPoundIceCone(s32 mode, s32 index);

void MarsLighthouseEntrance_CheckIce(void);

void MarsLighthouseEntrance_TeleportFromLapisNorthCircle(void);

void MarsLighthouseEntrance_BlazeLapisTorch(void);

void MarsLighthouseEntrance_BlockRetreatAtChest(void);

void MarsLighthouseEntrance_TeleportFromBalm(void);

void MarsLighthouseEntrance_BlazeHiddenCircleWestTorch(void);

void MarsLighthouseEntrance_WatchFireballCrackHiddenCircleIce(void);

void MarsLighthouseEntrance_BlazeHiddenCircleEastTorch(void);

void MarsLighthouseEntrance_WatchFireballMeltIce(void);

void MarsLighthouseEntrance_TeleportFromHiddenCircle(void);

void MarsLighthouseEntrance_HideTeleportCircle(void);

void MarsLighthouseEntrance_TeleportFromValkyrieNorth(void);

void MarsLighthouseEntrance_TeleportFromValkyrieSouth(void);

void MarsLighthouseEntrance_WakeDragonFace(void);

void MarsLighthouseEntrance_SlideFromEdge(void);

int MarsLighthouseEntrance_RideFireball(void);
void Leader_SlideOnIce(void);

void Leader_StopIfPending(void);

void MarsLighthouseEntrance_RevealTeleportCircle(void);

#endif
