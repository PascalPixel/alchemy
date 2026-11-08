/// @file
/// The triangles of a grid mesh.

#include "types.h"

/// A mesh triangle: its three vertex numbers and their texture coordinates.
struct MeshTriangle {
  u8 vertex[3];
  u8 unk03;
  u8 texCoord[3][2];
  u8 unk0a[2];
};

/// Fills a grid of columns by rows vertices with two triangles a cell, each
/// cell cellWidth by cellHeight texels of the texture, and ends the list with
/// an all-zero triangle.
void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows,
                   s32 cellWidth, s32 cellHeight) {
  s32 row;
  s32 column;
  struct MeshTriangle *second;

  for (row = 0; row != rows; row++) {
    for (column = 0; column != columns; column++) {
      if (row < rows - 1) {
        // The address as an integer sum, which adds the offset to the base
        // as the native does.
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((row * columns + column) * 24 +
                                    (u32)triangles);
        s32 u0 = column * cellWidth;
        s32 u1 = u0 + cellWidth - 1;
        s32 v0 = row * cellHeight;
        s32 v1 = v0 + cellHeight - 1;

        triangle[0].texCoord[0][0] = u1;
        triangle[0].texCoord[0][1] = v0;
        triangle[0].texCoord[1][0] = u0;
        triangle[0].texCoord[1][1] = v1;
        triangle[0].texCoord[2][0] = u1;
        triangle[0].texCoord[2][1] = v1;
        second = triangle + 1;
        second->texCoord[0][0] = u0;
        second->texCoord[0][1] = v0;
        second->texCoord[1][0] = u0;
        second->texCoord[1][1] = v1;
        second->texCoord[2][0] = u1;
        second->texCoord[2][1] = v0;
        triangle->vertex[0] = (column + 1) % columns + row * columns;
        triangle->vertex[1] = column % columns + columns + row * columns;
        triangle->vertex[2] = (column + 1) % columns + columns + row * columns;
        second->vertex[0] = column % columns + row * columns;
        second->vertex[1] = column % columns + columns + row * columns;
        second->vertex[2] = (column + 1) % columns + row * columns;
      }
    }
  }
  triangles[(rows - 1) * columns * 2].vertex[0] = 0;
  triangles[(rows - 1) * columns * 2].vertex[1] = 0;
  triangles[(rows - 1) * columns * 2].vertex[2] = 0;
}
