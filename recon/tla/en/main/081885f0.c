/* Near miss: listing 141 instructions, draft 141, differ 6 (three pairs; -mtune=arm7tdmi as an
 * analysis: also 6, the prologue pair fixed and the outer loop's y += 18 one slot early).
 * Registers and stores match. Left: the native copies segments into r4 after the loop's
 * stores of y and the ring base; it loads the spilled triangles pointer before the * 24 chain,
 * in the ring's triangle block and at the end marker. Measured: the angle in a local: differ
 * 8 (the native's calls take i * (0x10000 / segments) as written); a vertex pointer computed
 * before Trig_Sin: 62 (the native computes the address after the call returns); the 15 in a
 * variable: 42; the triangle as &triangles[index * 2]: 8 (base + mult, where the native adds
 * mult + base); both triangles indexed directly: 96; segments as u8, s8, u16, s16: 11, 102,
 * 135, 7; the index as i + ring * segments: 10. permute (6 min, then 18 more with seed 7): nothing
 * below 6. */
#include "types.h"
#include "fixed_math.h"

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

void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles, s32 segments)
{
    s32 ring;
    s32 i;

    for (ring = 0; ring != 2; ring++)
    {
        for (i = 0; i != segments; i++)
        {
            vertices[ring * segments + i].x = (u32)(Trig_Sin(i * (0x10000 / segments)) * 3) >> 11;
            vertices[ring * segments + i].y = ring * 18 - 36;
            vertices[ring * segments + i].z = (u32)(Trig_Cos(i * (0x10000 / segments)) * 3) >> 11;
            if (ring < 1)
            {
                struct MeshTriangle *triangle = (struct MeshTriangle *)((ring * segments + i) * 24 + (u32)triangles);
                s32 u0 = i * 8;
                s32 u1 = u0 + 7;

                triangle[0].texCoord[0][0] = u1;
                triangle[0].texCoord[0][1] = 0;
                triangle[0].texCoord[1][0] = u1;
                triangle[0].texCoord[1][1] = 15;
                triangle[0].texCoord[2][0] = u0;
                triangle[0].texCoord[2][1] = 15;
                {
                    struct MeshTriangle *second = triangle + 1;

                    second->texCoord[0][0] = u0;
                    second->texCoord[0][1] = 0;
                    second->texCoord[1][0] = u1;
                    second->texCoord[1][1] = 0;
                    second->texCoord[2][0] = u0;
                    second->texCoord[2][1] = 15;
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
    triangles[segments * 2].vertex[0] = 0;
    triangles[segments * 2].vertex[1] = 0;
    triangles[segments * 2].vertex[2] = 0;
}
