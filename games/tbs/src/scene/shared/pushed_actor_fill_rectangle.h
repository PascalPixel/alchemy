/// @file
/// The pushed-actor scenes (staged_actor.h) and Venus Lighthouse's unused copy:
/// a rectangle of cells given one cell behavior.

/// Sets the cell behavior of a width by height rectangle of cells, from
/// cell (x, z), to value, on map grid layer 0 to 2, or (3 and up) in
/// gMapGridCellBuffer.
static s32 PushedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z,
                                                  u32 width, u32 height,
                                                  s32 value) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct MapGridCell *cells;
  u32 row;
  u32 column;

  if (grid != NULL) {
    if (layer <= 2) {
      cells = grid->layers[layer].cells;
    } else {
      cells = (struct MapGridCell *)gMapGridCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapGridCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->behavior = value;
        cell++;
      }
    }
  }
  return 0;
}
