#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

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

void Func_02001c74();

s32 MapCell_Swap(u32 layer, s32 x, s32 y, s32 value);

void MarsLighthouseEntrance_DrawRoom3CrackedSheet(void);

void MarsLighthouseEntrance_ShootFireballAtRoom5IceSheet(void);

void Teleport_StartShimmer(void);
void Teleport_StopShimmer(void);

void MarsLighthouseEntrance_ShootFireballAtRoom3IceSheet(void);

void MarsLighthouseEntrance_ShootFireballThroughIce(void);

void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell);

void MarsLighthouseEntrance_FlingIceCone(struct Actor *object);

void Actor_Attach(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

void Leader_SlideOnIceFromEdge(void);

s32 MarsLighthouseEntrance_SlideIfRiding(void);

void Leader_SlideOnIceAhead(void);

void MarsLighthouseEntrance_DrawHoleMask(void);

s32 Ride_IsActive(void);

void Leader_SetSlideStopped(s32 value);

void MarsLighthouseEntrance_ScatterIceChips(s32 value);

void MarsLighthouseEntrance_ShootRoom6LowerFireball(void);
void MarsLighthouseEntrance_ShootRoom4Fireball(void);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);

s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void Func_02001e60();
void Func_02001ea8();
void Func_02001ef0();

void Actor_RollAndCrumble(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern u16 *Data_02006894;
extern u16 *Data_02006880;

void Func_020048e8(void);

extern u16 gPaletteCycle_Flash;
extern u16 gPaletteCycle_Countdown;
extern u16 gPaletteCycle_Step;
extern u16 gPaletteCycle_Period;

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void MarsLighthouseEntrance_CrackRoom3IceSheet(void);

void MarsLighthouseEntrance_CrackRoom5IceSheet(void);

void MarsLighthouseEntrance_MeltIceInPath(void);

void MarsLighthouseEntrance_ClearFireballRow(void);

void MarsLighthouseEntrance_StartPoundIceCone(void);

void MarsLighthouseEntrance_CheckIce(void);

void MarsLighthouseEntrance_Teleport(void);

void MarsLighthouseEntrance_BlazeRoom3Statue(void);

void MarsLighthouseEntrance_BlockRetreatAtChest(void);

void MarsLighthouseEntrance_TeleportFromRoom4(void);

void MarsLighthouseEntrance_BlazeRoom5WestStatue(void);

void MarsLighthouseEntrance_WatchFireballCrackRoom5Ice(void);

void MarsLighthouseEntrance_BlazeRoom5EastStatue(void);

void MarsLighthouseEntrance_WatchFireballMeltIce(void);

void MarsLighthouseEntrance_TeleportFromRoom5(void);

void MarsLighthouseEntrance_HideTeleportCircle(void);

void MarsLighthouseEntrance_TeleportFromRoom6West(void);

void MarsLighthouseEntrance_TeleportFromRoom6East(void);

void MarsLighthouseEntrance_WakeDragonFace(void);

void MarsLighthouseEntrance_SlideFromEdge(void);

int MarsLighthouseEntrance_SlideOnIce(void);
void Leader_SlideOnIce(void);

void Leader_StopIfPending(void);

void MarsLighthouseEntrance_RevealTeleportCircle(void);

#endif
