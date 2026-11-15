#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct MapCell;
struct Position;

struct MarsLighthouseEntranceAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[6];
  u8 value10;
  u8 flag11 : 1;
  u16 angle;
};

struct MarsLighthouseEntranceRideHead {
  s32 counter;
  s16 active;
  u8 unk06[6];
  u16 value0c;
  s16 value0e;
  u8 unk10[0x60];
  struct Actor *handle;
};

void Leader_KnockBack(void);

void Ride_SpawnForward();

s32 MapCell_Swap(u32 layer, s32 x, s32 y, s32 value);

void MarsLighthouseEntrance_DrawLapisCrackedSheet(void);

void MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet(void);

void Teleport_StartShimmer(void);
void Teleport_StopShimmer(void);

void MarsLighthouseEntrance_ShootFireballAtLapisIceSheet(void);

void MarsLighthouseEntrance_ShootFireballThroughIce(void);

void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell);

void MarsLighthouseEntrance_FlingIceCone(struct Actor *object);

s32 Actor_Attach(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

void Leader_SlideOnIceFromEdge(void);

s32 MarsLighthouseEntrance_SlideIfRiding(void);

void Leader_SlideOnIceAhead(void);

void MarsLighthouseEntrance_DrawHoleMask(void);

s32 Ride_IsActive(void);

void Leader_SetSlideStopped(s32 value);

void MarsLighthouseEntrance_ScatterIceChips(s32 value);

void MarsLighthouseEntrance_ShootValkyrieLowerFireball(void);
void MarsLighthouseEntrance_ShootBalmFireball(void);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);

s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void Ride_SpawnBackward();
void Ride_SpawnLandingForward();
void Ride_SpawnLandingBackward();

void Actor_RollAndCrumble(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern u16 *gPaletteCycle_Colors;
extern u16 *gPaletteCycle_Order;

void Teleport_ArriveWhirling(void);

extern u16 gPaletteCycle_Flash;
extern u16 gPaletteCycle_Countdown;
extern u16 gPaletteCycle_Step;
extern u16 gPaletteCycle_Period;

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void MarsLighthouseEntrance_CrackLapisIceSheet(void);

void MarsLighthouseEntrance_CrackHiddenCircleIceSheet(void);

void MarsLighthouseEntrance_MeltIceInPath(void);

void MarsLighthouseEntrance_ClearFireballRow(void);

void MarsLighthouseEntrance_StartPoundIceCone(s32 mode, s32 index);

void MarsLighthouseEntrance_CheckIce(void);

void MarsLighthouseEntrance_Teleport(void);

void MarsLighthouseEntrance_BlazeLapisStatue(void);

void MarsLighthouseEntrance_BlockRetreatAtChest(void);

void MarsLighthouseEntrance_TeleportFromBalm(void);

void MarsLighthouseEntrance_BlazeHiddenCircleWestStatue(void);

void MarsLighthouseEntrance_WatchFireballCrackHiddenCircleIce(void);

void MarsLighthouseEntrance_BlazeHiddenCircleEastStatue(void);

void MarsLighthouseEntrance_WatchFireballMeltIce(void);

void MarsLighthouseEntrance_TeleportFromHiddenCircle(void);

void MarsLighthouseEntrance_HideTeleportCircle(void);

void MarsLighthouseEntrance_TeleportFromValkyrieWest(void);

void MarsLighthouseEntrance_TeleportFromValkyrieEast(void);

void MarsLighthouseEntrance_WakeDragonFace(void);

void MarsLighthouseEntrance_SlideFromEdge(void);

int MarsLighthouseEntrance_SlideOnIce(void);
void Leader_SlideOnIce(void);

void Leader_StopIfPending(void);

void MarsLighthouseEntrance_RevealTeleportCircle(void);

#endif
