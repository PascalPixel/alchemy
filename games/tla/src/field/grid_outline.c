/* @file */
/* The grid outliner, compiled as ARM code and copied to RAM to run, entered */
/* in Thumb through fdGridDrawOutlineCode (grid_outline_entry.s). */

#include "types.h"

/* A drawn pixel inside the shape, its four neighbours drawn too. */
#define GRID_OUTLINE_INSIDE 254
/* A blank pixel the first pass found inside the shape. */
#define GRID_OUTLINE_INSIDE_BLANK 255

/* Redraws a width by height picture of 8-bit pixels as an outline: each drawn
 */
/* pixel inside the shape (its four neighbours drawn too) takes colour fill, */
/* and every other drawn pixel colour edge. */
void fdGridDrawOutlineArm(u32 width, u32 height, u8 *grid, u32 edge, u32 fill) {
	u32 size = width * height;
	u8 *next = grid + width + 1;
	s32 above = -width - 1;
	s32 below = width - 1;
	u8 *cell;
	u8 *end;
	u32 x;
	u32 y;
	u32 value;

	for(y = 1; y < height - 1; y++, next += 2) {
		for(x = 1; x < width - 1; x++) {
			value = *++next;
			if(value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[-2];
			if(value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[above];
			if(value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			value = next[below];
			if(value == 0 || value == GRID_OUTLINE_INSIDE_BLANK)
				continue;
			if(next[-1] != 0)
				next[-1] = GRID_OUTLINE_INSIDE;
			else
				next[-1] = GRID_OUTLINE_INSIDE_BLANK;
		}
	}
	cell = grid;
	end = grid + size;
	while(cell != end) {
		value = *cell++;
		if(value == 0)
			continue;
		if(value >= GRID_OUTLINE_INSIDE)
			cell[-1] = fill;
		else
			cell[-1] = edge;
	}
}
