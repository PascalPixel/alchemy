#ifndef ALCHEMY_DJINN_PREVIEW_H
#define ALCHEMY_DJINN_PREVIEW_H

#include "output.h"
#include "resource.h"

struct PreviewSprite
{
	u32 unknown;
	union
	{
		u32 word;
		struct
		{
			unsigned y : 8;
			unsigned flags : 8;
			unsigned x : 9;
			unsigned high : 7;
		} bits;
	} attributes;
	union RenderTableValue tile;
};

extern u8 gDjinnMenu_ArrowUpTiles[];
extern u8 gDjinnMenu_ArrowDownTiles[];

#endif
