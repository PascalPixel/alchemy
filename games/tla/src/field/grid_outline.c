// The grid outliner, compiled as ARM code and copied to RAM to run, entered in Thumb through
// fdGridDrawOutlineCode (grid_outline_entry.s).

#include "types.h"

// A drawn pixel inside the shape, its four neighbours drawn too.
#define GRID_OUTLINE_INSIDE       254
// A blank pixel the first pass found inside the shape.
#define GRID_OUTLINE_INSIDE_BLANK 255

// Redraws a width by height picture of 8-bit pixels as an outline: each drawn pixel inside the
// shape (its four neighbours drawn too) takes colour fill, and every other drawn pixel colour edge.
void fdGridDrawOutlineArm(Uint32 width, Uint32 height, Uint8 *grid, Uint32 edge, Uint32 fill)
{
	Uint32 size  = width * height;
	Uint8 *next  = grid + width + 1;
	Sint32 above = -width - 1;
	Sint32 below = width - 1;
	Uint8 *metatile;
	Uint8 *end;
	Uint32 x;
	Uint32 y;
	Uint32 value;

	for (y = 1; y < height - 1; y++, next += 2)
	{
		for (x = 1; x < width - 1; x++)
		{
			value = *++next;
			if (value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[-2];
			if (value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[above];
			if (value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[below];
			if (value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			if (next[-1] != 0)
				next[-1] = GRID_OUTLINE_INSIDE;
			else
				next[-1] = GRID_OUTLINE_INSIDE_BLANK;
		}
	}
	metatile = grid;
	end      = grid + size;
	while (metatile != end)
	{
		value = *metatile++;
		if (value == 0)
			continue;
		if (value >= GRID_OUTLINE_INSIDE)
			metatile[-1] = fill;
		else
			metatile[-1] = edge;
	}
}
