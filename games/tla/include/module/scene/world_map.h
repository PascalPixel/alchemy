#ifndef ALCHEMY_MODULE_SCENE_WORLD_MAP_H
#define ALCHEMY_MODULE_SCENE_WORLD_MAP_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

/// FieldSprite's byte 0x11 read as its bits.
struct SpriteCacheBits {
  u8 retainCacheEntry : 1; ///< keeps the VRAM cache entry when it is released
};

struct WorldMapOam {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

void Func_020048f4(void);

void Func_02003654(s32 a, s32 b, s32 c);

void Func_02003bb8(struct Actor *object, s32 mode);

void Func_020049bc(void);
void Func_02003c3c(void);
void Func_02004a1c(void);

void Import_ActorSelectAnimation(struct Actor *object, s32 mode);

void WorldMap_FightBanditAndThieves(s32 entrance);

void WorldMap_DeliverLohoCannon(void);

void WorldMap_MeetEcho(void);

void Func_020029a4(void);

void WorldMap_DriftNorthOfContinent(void);

void Func_02002e00(void);

void WorldMap_TakeTheTiller(void);
void Func_0200310c(void);
void Func_02003490(void);

void Func_02003d44(s32 mode);

void Func_0200009c(void);

void Func_02000100(void);

void Func_02000164(void);

void Func_020001c8(void);

void Func_0200022c(void);

void WorldMap_SetUpFlyingShip(void);

void WorldMap_SetUpShip(void);
void Func_02000918(void);

void WorldMap_RefuseToRunAway(void);

void WorldMap_GazeIntoPond(void);

void Func_02000d1c(void);

void WorldMap_TakeOff(void);

extern s32 gData_02006320[];
extern s32 gData_02006578[];
extern s32 gData_02006584[];

extern u32 gData_02004fc8[];

s32 WorldMap_GetBlockPlace(s32 *position);

extern u16 gData_02006e08;

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void Func_02004634(struct Actor *object);

void Actor_Release(struct Actor *object);

extern u8 Data_02006e20[][32];
extern s16 gData_02006e10;
extern u32 Data_02006e14[3];

void Party_DrainPp(s32 value);
s32 Party_GetPpRatio(void);
void Import_OamAddSpriteAtDepthClamped(u32 *oam, s32 priority);

void WorldMap_WatchVenusLighthouseCollapse(void);

#endif
