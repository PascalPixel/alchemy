#ifndef ALCHEMY_MODULE_FIELD_MAP_H
#define ALCHEMY_MODULE_FIELD_MAP_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "map.h"

struct Actor;

struct ReflectionSprite {
  u8 unk00[2];
  u16 unk02;
  u8 unk04[12];
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unk00[0x28];
  struct ReflectionSprite *sprite;
};

struct WorldTint {
  struct MapWork map;
  u8 unk970[3];
  s8 region;
  s8 tint;
  s8 target;
};

struct WorldMapBlock {
  u8 unk0[2];
  u8 unk2;
  u8 unk3;
};

struct WorldMover {
  u8 unk00[12];
  s32 height;
};

struct ScanlineRow {
  s32 scale;
  s32 depth;
  s32 offsetX;
  s32 offsetY;
  s32 unk10;
};

struct SpriteViewerLayer {
  s16 spriteId;
  s16 animationIndex;
  u8 enabled;
  s8 priority;
  s8 colorMode;
};

struct ViewerMetadata {
  u8 unk00[4];
  u8 drawKind; ///< 20 for a sprite drawn in two parts
};

struct ViewerSprite {
  u8 unk00[0x1a];
  u8 flags;
  u8 unk1b[0x1d];
};

struct ProbeShape {
  u8 unk00[4];
  u8 kind;
};

struct ProbeObjects {
  u8 unk00[40];
  struct ProbeShape *shape;
};

struct UiGridEntry {
  s16 no;
  u16 x;
  u8 value04;
  u8 unk05[7];
  s32 *table0c;
  s32 value10;
  u8 value14;
  u8 value15;
  u8 value16;
  u8 value17;
};

struct BattleCells {
  u8 *actors;
  u8 *unk04;
  u8 *unk08;
  u8 *unk0c;
  u8 *unk10;
  u8 *work;
};

struct Vec2 {
  s32 x;
  s32 y;
};

struct Actor *Func_080eaf28(s32 *position, s32 check_y);

void Func_0200189c(void);

void Func_02003178(void);
void Func_02002fc8(void);
void Func_02002fd0(void);

extern u8 Data_0202d000[];
extern u8 gData_02038000[];
extern u8 gData_0203a000[];
extern u8 gData_0203c000[];
extern u8 gData_0203e000[];
extern u8 gMapCollision[];

void Func_0802d088(s32 region, s32 gradual);

s32 Func_080c8950(void);

extern u8 gData_0201c000[];

void MapAnimation_Update(void);

s32 Func_080c8730(s32 layer, s32 unused, s32 value);

s32 Func_080c8720(s32 layer, s32 unused, s32 value);

extern const u8 Render_DrawMapCode[];
extern u8 Render_DrawMapCodeSize[];

s32 Func_0802e444(s32 spriteId, s32 direction);
void Battle_PlaceActorsByFormationKind(void);
extern char ResourceId_Data012;
extern u8 gData_03001e60[];
extern struct BattleCells gSpriteObjects;

void DisplayBlend_StartScript(u16 *script);
s32 MapGrid_CheckCollisionAt(s32 px, s32 pz, s32 layer, s32 height);
void Func_0802c58c(void);
void DisplayBlend_DisableRunScript(void);
void DisplayBlend_EnableRunScript(void);

#endif
