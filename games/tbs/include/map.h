#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field11.h"
#include "types.h"
#include "layout_guard.h"

struct MapCell
{
	u16 metatile_and_flags;
	u8	collision_code;
	u8	attribute_b;
};

struct MapLayer
{
	struct MapCell *cells;
	u8				unknown04[0x2c];
};

struct BlendScriptState
{
	u16 *script;
	u16 *cursor;
	u16	 delay;
	u16	 paused;
};

struct MapAnimation
{
	const u16 *start;
	const u16 *cursor;
	u16		   timer;
	u16		   paused;
};

struct MapState
{
	u8						unknown000[0x14];
	u16						flags;
	u8						wide_tiles;
	u8						unknown_17;
	struct MapAnimation		anim[16];
	struct BlendScriptState blend;
	u8						unknown_e4[0x1f];
	u8						blend_control;
	u8						unknown_104[0x2c];
	struct MapLayer			layers[4];
};

LAYOUT_SIZE_GUARD(MapAnimation_Size, struct MapAnimation, 12);
LAYOUT_OFFSET_GUARD(MapState_Flags, struct MapState, flags, 0x14);
LAYOUT_OFFSET_GUARD(MapState_WideTiles, struct MapState, wide_tiles, 0x16);
LAYOUT_OFFSET_GUARD(MapState_Animation, struct MapState, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapState_Blend, struct MapState, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapState_Cells, struct MapState, layers, 0x130);

struct WorldPosition
{
	s32 x;
	s32 height;
	s32 y;
};

extern u32 gMapBlocks[];

extern u8 gMapLayerData[];

extern u8 gMapCollision[];

extern u8 gBgTileBuffer[];

extern u8 *gMapAnimationPages;

#endif
