#include "types.h"
#include "global_cells.h"
#include "map.h"

extern u8 gMapCellBuffer[];

s32 StagedActor_FillGridAttributeRectangle(
	u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
	struct MapState *map = gMapWork[0];
	struct MapCell	*cells;
	u32				 row;
	u32				 column;

	if (map != 0)
	{
		if (layer <= 2)
		{
			cells = map->layers[layer].cells;
		}
		else
		{
			cells = (struct MapCell *)gMapCellBuffer;
		}
		cells += x + (z << 7);
		for (row = 0; row < height; row++)
		{
			struct MapCell *cell = cells + (row << 7);

			for (column = 0; column < width; column++)
			{
				cell->collision_code = value;
				cell++;
			}
		}
	}
	return 0;
}
