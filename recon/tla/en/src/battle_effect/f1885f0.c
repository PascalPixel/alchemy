#include "types.h"
#include "fixed_math.h"

// The two rings sit at y = -36 and y = -18.
#define FIRST_RING_Y -36
#define RING_SPACING 18

// Each side of the band shows an 8 x 16 column of the texture.
#define COLUMN_WIDTH 8
#define COLUMN_HEIGHT 16

struct MeshVertex {
  s8 x;
  s8 y;
  s8 z;
  u8 unk03;
};

struct MeshTriangle {
  u8 vertex[3];
  u8 unk03;
  u8 texCoord[3][2];
  u8 unk0a[2];
};

// Builds the mesh of an open cylinder, the band summon 20 shows: two rings of
// `segments` vertices of radius 96 around the y axis, and two textured
// triangles for each side of the band, joining the first ring (texture row 0)
// to the second (row 15). A triangle whose three vertices are 0 ends the list.
// Summon 20 builds it with 32 segments into the buffers at 0x02014400 and
// 0x02014800.
void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles,
                   s32 segments) {
  s32 ring;
  s32 i;

  for (ring = 0; ring != 2; ring++) {
    for (i = 0; i != segments; i++) {
      // The vertex: a point on the ring, 96 units out (3 * 0x10000 >> 11).
      vertices[ring * segments + i].x =
          (u32)(Trig_Sin(i * (0x10000 / segments)) * 3) >> 11;
      vertices[ring * segments + i].y = ring * RING_SPACING + FIRST_RING_Y;
      vertices[ring * segments + i].z =
          (u32)(Trig_Cos(i * (0x10000 / segments)) * 3) >> 11;

      // The side from this vertex to the next, as two triangles.
      if (ring < 1) {
        // A byte offset added to the base: as
        // &triangles[(ring * segments + i) * 2], 4 of 141 differ (base +
        // offset, where the native adds offset + base).
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((ring * segments + i) * 24 +
                                    (u32)triangles);
        s32 left = i * COLUMN_WIDTH;
        s32 right = left + COLUMN_WIDTH - 1;

        triangle[0].texCoord[0][0] = right;
        triangle[0].texCoord[0][1] = 0;
        triangle[0].texCoord[1][0] = right;
        triangle[0].texCoord[1][1] = COLUMN_HEIGHT - 1;
        triangle[0].texCoord[2][0] = left;
        triangle[0].texCoord[2][1] = COLUMN_HEIGHT - 1;
        {
          struct MeshTriangle *second = triangle + 1;

          second->texCoord[0][0] = left;
          second->texCoord[0][1] = 0;
          second->texCoord[1][0] = right;
          second->texCoord[1][1] = 0;
          second->texCoord[2][0] = left;
          second->texCoord[2][1] = COLUMN_HEIGHT - 1;

          // (next, next on ring 1, this on ring 1), then (this, next, this on
          // ring 1).
          triangle->vertex[0] = (i + 1) % segments + ring * segments;
          triangle->vertex[1] = (i + 1) % segments + segments + ring * segments;
          triangle->vertex[2] = i % segments + segments + ring * segments;
          second->vertex[0] = i % segments + ring * segments;
          second->vertex[1] = (i + 1) % segments + ring * segments;
          second->vertex[2] = i % segments + segments + ring * segments;
        }
      }
    }
  }

  // The end of the list.
  triangles[segments * 2].vertex[0] = 0;
  triangles[segments * 2].vertex[1] = 0;
  triangles[segments * 2].vertex[2] = 0;
}
