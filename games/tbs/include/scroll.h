#ifndef ALCHEMY_SCROLL_H
#define ALCHEMY_SCROLL_H

#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
#include "map_scroll.h"
#include "layout_guard.h"

struct DisplayScrollWork
{
	struct BgScroll rows[2][160][3];
	u8				page;
	u8				mode;
	u16				frame;
	u8				unknown_f04[4];
	s32				freq_x;
	s32				freq_y;
	s32				step_x;
	s32				step_y;
	s32				amp_x;
	s32				amp_y;
};

LAYOUT_SIZE_GUARD(DisplayScrollWork_Size, struct DisplayScrollWork, 0xf20);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Page, struct DisplayScrollWork, page, 0xf00);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Frequency, struct DisplayScrollWork, freq_x, 0xf08);

extern struct DisplayScrollWork *gHBlankScrollWork;
extern const s16				 DisplayScroll_WaveSine[];

#endif
