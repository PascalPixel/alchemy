/* Draft (tla-en): listing 72 instructions, draft 72, common 65, differ 14.
 * The loop body's stores match in the listing's order. Left: loop.c's
 * strength-reduced counters come out in another order. The native sets
 * the 3 + 2i counter (r8) before it hoists the constant 1 into r10, and
 * steps it first (r8, r6, r0, r2, r1, then the pointer); the draft steps
 * r6, r0, r8, the pointer, r2, r1. Tried: the bottom row or the top value
 * as a local, a local for 3 + 2i, 1 + i * 2 forms, a vertex counter, a
 * walking pointer (differ 14 to 91); five minutes of alchemy permute found
 * nothing better.
 */
#include "types.h"

struct StripTriangle {
    u8 vertex[3];
    u8 unknown_03;
    u8 uv[3][2];
    u8 unknown_0a[2];
};

struct StripQuad {
    struct StripTriangle upper;
    struct StripTriangle lower;
};

void Func_08178680(struct StripQuad *quad, s32 width, s32 height, s32 count)
{
    s32 i;

    for (i = 0; i != count; i++) {
        s32 left = i * width;
        s32 right = i * width + width - 1;
        s32 bottom = height - 1;

        quad[i].upper.uv[0][1] = 1;
        quad[i].upper.uv[2][1] = 1;
        quad[i].lower.uv[2][1] = 1;
        quad[i].upper.vertex[0] = i * 2;
        quad[i].lower.vertex[1] = i * 2 + 3;
        quad[i].upper.uv[0][0] = left;
        quad[i].upper.uv[1][0] = left;
        quad[i].upper.uv[2][0] = right;
        quad[i].lower.uv[0][0] = left;
        quad[i].lower.uv[1][0] = right;
        quad[i].lower.uv[2][0] = right;
        quad[i].upper.vertex[1] = i * 2 + 1;
        quad[i].upper.vertex[2] = i * 2 + 2;
        quad[i].lower.vertex[0] = i * 2 + 1;
        quad[i].lower.vertex[2] = i * 2 + 2;
        quad[i].upper.uv[1][1] = bottom;
        quad[i].lower.uv[0][1] = bottom;
        quad[i].lower.uv[1][1] = bottom;
    }
    quad[count].upper.vertex[0] = 0;
    quad[count].upper.vertex[1] = 0;
    quad[count].upper.vertex[2] = 0;
}
