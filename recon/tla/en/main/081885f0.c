// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 2 of
// 141: only the size's copy to r4 in the prologue, which the native makes
// after the three stack stores. A greedy device search found nothing lower.
// Near miss: listing 141 instructions, draft 141, differ 4 (was 6: three pairs;
// -mtune=arm7tdmi as an analysis: also 6, the prologue pair fixed and the
// outer loop's y += 18 one slot early). No ☀️ twin: The Broken Seal has no
// mesh builder of this kind (closest call order PsynergyMenu_BuildPageResult,
// 0.600, a false twin).
// Registers and stores match. Left: the native copies segments into r4 after
// the loop's stores of y and the ring base; it loads the spilled triangles
// pointer before the * 24 chain, in the ring's triangle block and at the end
// marker. Measured: the angle in a local: differ 8 (the native's calls take
// i * (0x10000 / segments) as written); a vertex pointer computed before
// Trig_Sin: 62 (the native computes the address after the call returns); the
// 15 in a variable: 42; the triangle as &triangles[index * 2]: 8 (base + mult,
// where the native adds mult + base); both triangles indexed directly: 96;
// segments as u8, s8, u16, s16: 11, 102, 135, 7; the index as
// i + ring * segments: 10. permute (6 min, then 18 more with seed 7): nothing
// below 6.
// 2026-10-15 (fx-n3): rewritten for reading (names, comments, clang-format);
// still 6. A running ring base and y stepped by the outer loop (the native
// keeps both in stack slots): 104.
// 2026-10-18: alchemy-devtools permute (6 min, 2 jobs, seed 31): no candidate below
// the draft.
// 2026-10-19 (run 33): the end marker's pinned pointer (tagged below): 6 to
// 4. The same device in the ring's triangle block (the pointer pinned to r3,
// the asm holding r5) orders that pair too, but without the triangles reload
// the reload registers rotate differently after it (mov r3, sl for i, and
// later r3 for r1): differ 8; with i pinned to r1 as well: 76. A volatile
// asm reading triangles adds a reference and triangles takes r9: 40 rows.
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
  {
    /* FAKEMATCH(pin): the triangles pointer pinned to r1 behind an empty asm
     * that holds r3, so its reload comes before the * 24 chain; as
     * triangles[segments * 2], 2 of 141 instructions differ in tla-en. */
    register u32 base __asm__("r1") = (u32)triangles;
    struct MeshTriangle *end;

    __asm__("" : "+l"(base) : : "r3");
    end = (struct MeshTriangle *)(segments * 24 + base);
    end->vertex[0] = 0;
    end->vertex[1] = 0;
    end->vertex[2] = 0;
  }
}
