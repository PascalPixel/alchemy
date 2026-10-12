#ifndef ALCHEMY_MAPFRAME_H
#define ALCHEMY_MAPFRAME_H

#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "types.h"
#include "layout_guard.h"

struct MapScanline
{
	s32 scale, depth;
	s32 offset_x, offset_y;
	s16 pa, pc;
};

struct MapAffineLine
{
	s16 pa, pb, pc, pd;
	s32 x, y;
};

struct MapAffinePair
{
	struct MapAffineLine bg2, bg3;
};

struct MapFrameWork
{
	struct MapScanline	 scanlines[160];
	struct MapAffinePair pages[2][160];
	u8					 unknown_3480[4];
};

LAYOUT_SIZE_GUARD(MapScanline_Size, struct MapScanline, 20);
LAYOUT_OFFSET_GUARD(MapScanline_Coefficients, struct MapScanline, pa, 16);
LAYOUT_SIZE_GUARD(MapAffineLine_Size, struct MapAffineLine, 16);
LAYOUT_SIZE_GUARD(MapAffinePair_Size, struct MapAffinePair, 32);
LAYOUT_OFFSET_GUARD(MapFrameWork_Pages, struct MapFrameWork, pages, 0xc80);
LAYOUT_OFFSET_GUARD(MapFrameWork_UnknownTail, struct MapFrameWork, unknown_3480, 0x3480);
LAYOUT_SIZE_GUARD(MapFrameWork_Size, struct MapFrameWork, 0x3484);

#endif
