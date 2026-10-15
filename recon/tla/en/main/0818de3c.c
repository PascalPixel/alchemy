/* Near miss: listing 142 instructions, draft 142, differ 20 (-mtune=arm7tdmi as an analysis: 18).
 * Registers and stores all match; what is left is sched2 order: the inner loop's preheader loads
 * row * columns (sp+8) before row * cellHeight (sp+12), where the native loads cellHeight's first,
 * and the triangle address waits for its pool of the spilled triangles pointer (the native loads
 * it before the * 24 chain), and two ldrb of the spilled v0 sit one slot apart. Measured: the
 * triangle as &triangles[index * 2], triangles + index * 2, a quad struct, or index * 2 +
 * triangles: differ 22 (base + mult; the native adds mult + base, which only the integer form
 * gives); the second triangle as triangle + 1 declared with the first: 146 (it must come after
 * the first triangle's stores); v0 declared before the triangle: 30; v0 and v1 as running
 * variables of the outer loop: 96; the texture coordinates written inline: 20. */
#include "types.h"

struct MeshTriangle {
    u8 vertex[3];
    u8 unk03;
    u8 texCoord[3][2];
    u8 unk0a[2];
};

void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight)
{
    s32 row;
    s32 column;

    for (row = 0; row != rows; row++)
    {
        for (column = 0; column != columns; column++)
        {
            if (row < rows - 1)
            {
                struct MeshTriangle *triangle = (struct MeshTriangle *)((row * columns + column) * 24 + (u32)triangles);
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
                {
                    struct MeshTriangle *second = triangle + 1;

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
    }
    triangles[(rows - 1) * columns * 2].vertex[0] = 0;
    triangles[(rows - 1) * columns * 2].vertex[1] = 0;
    triangles[(rows - 1) * columns * 2].vertex[2] = 0;
}
