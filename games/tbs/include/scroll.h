#ifndef ALCHEMY_SCROLL_H
#define ALCHEMY_SCROLL_H

#include "map_scroll.h"
#include "layout_guard.h"

struct DisplayScrollWork
{
	struct BgScroll rows[2][160][3];
	u8				page;
	u8				mode;
	u16				frame;
	u8				unkf04[4];
	s32				freqX;
	s32				freqY;
	s32				stepX;
	s32				stepY;
	s32				ampX;
	s32				ampY;
};

LAYOUT_SIZE_GUARD(DisplayScrollWork_Size, struct DisplayScrollWork, 0xf20);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Page, struct DisplayScrollWork, page, 0xf00);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Frequency, struct DisplayScrollWork, freqX, 0xf08);

extern struct DisplayScrollWork *gHBlankScrollWork;
extern const s16				 gDisplayScroll_WaveSine[];

#endif
