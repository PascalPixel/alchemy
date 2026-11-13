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

void Func_02004b18(struct Actor *object, s32 mode);

void Func_020012bc(s32 entrance);

void Func_02001c5c(void);

void Func_02001e34(void);

void Func_020029a4(void);

void Func_02002ab0(void);

void Func_02002e00(void);

void Func_02002e78(void);
void Func_0200310c(void);
void Func_02003490(void);

void Func_02003d44(s32 mode);

void Func_0200009c(void);

void Func_02000100(void);

void Func_02000164(void);

void Func_020001c8(void);

void Func_0200022c(void);

void Func_02000458(void);

void Func_0200050c(void);
void Func_02000918(void);

void Func_02000994(void);

void Func_02000be0(void);

void Func_02000d1c(void);

void Func_02003144(void);

extern s32 gData_02006320[];
extern s32 gData_02006578[];
extern s32 gData_02006584[];

extern u32 gData_02004fc8[];

s32 Func_02004b90(s32 *position);

extern u16 gData_02006e08;

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void Func_02004634(struct Actor *object);

void Func_02004b38(struct Actor *object);

extern u8 Data_02006e20[][32];
extern s16 gData_02006e10;
extern u32 Data_02006e14[3];

void Func_02004c08(s32 value);
s32 Func_02004c10(void);
void Func_02004ad0(u32 *oam, s32 priority);

#endif
