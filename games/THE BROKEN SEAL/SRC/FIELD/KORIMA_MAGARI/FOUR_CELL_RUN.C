#include "TYPES.H"
#include "MAP.H"

/* The map's cells, 128 to a row. */
extern struct MapCell gMapCellBuffer[];

/* Whether the four cells from (x, z) onward, rightward or in any other mode
   downward, are all open: none is unwalkable and none has an attribute
   whose collision entry is set. */
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct MapCell *cell = &gMapCellBuffer[x + (z << 7)];

        if (cell->collision_code == 0xff
            || *(u8 *)((cell->attribute_b << 2) + (s32)gMapCollision) != 0) {
            return -1;
        }
        if (mode == 0) {
            x++;
        } else {
            z++;
        }
    }
    return 0;
}
