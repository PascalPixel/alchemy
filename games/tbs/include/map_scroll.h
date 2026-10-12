#ifndef ALCHEMY_MAP_SCROLL_H
#define ALCHEMY_MAP_SCROLL_H

#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "types.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "map.h"

struct MapLayerScroll
{
	s32				x, y;
	s32				offset_x, offset_y;
	s32				scale_x, scale_y;
	s32				speed_x, speed_y;
	s32				phase_x, phase_y;
	u16				mask_x, mask_y;
	struct MapCell *cells;
};

struct MapScrollWork
{
	s32					   *origin;
	s32						shake_x, shake_y, shake_decay;
	u8					   *script;
	u16						flags;
	u8						wide_tiles;
	u8						footprints;
	struct MapAnimation		anim[16];
	struct BlendScriptState blend;
	s32						view_x, view_y;
	s32						min_x, min_y, max_x, max_y;
	s8						animation_active;
	u8						unknown_0fd[3];
	u8						enabled[3];
	u8						unknown_103;
	struct MapLayerScroll	layers[3];
};

#include "perspect.h"

LAYOUT_SIZE_GUARD(MapLayerScroll_Size, struct MapLayerScroll, 0x30);
LAYOUT_OFFSET_GUARD(MapLayerScroll_Cells, struct MapLayerScroll, cells, 0x2c);
LAYOUT_SIZE_GUARD(MapScrollWork_Size, struct MapScrollWork, 0x194);
LAYOUT_OFFSET_GUARD(MapScrollWork_Animation, struct MapScrollWork, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapScrollWork_Blend, struct MapScrollWork, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapScrollWork_View, struct MapScrollWork, view_x, 0xe4);
LAYOUT_OFFSET_GUARD(MapScrollWork_Active, struct MapScrollWork, animation_active, 0xfc);
LAYOUT_OFFSET_GUARD(MapScrollWork_Layers, struct MapScrollWork, layers, 0x104);

struct BgScroll
{
	u16 x, y;
};

extern struct BgScroll gBgScroll[];

#endif
