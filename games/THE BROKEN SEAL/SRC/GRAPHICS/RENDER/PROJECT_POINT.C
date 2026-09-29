#include "IWRAM_CALL.H"

struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

extern struct Projection gProjection;

/* Transforms point through the IWRAM matrix routine and projects it to
   screen x, y and depth; returns the perspective scale, or 0 when the depth
   lies outside the near and far planes. */
s32 Render_ProjectPoint(s32 *point, s32 *screen)
{
    struct Projection *projection;
    s32 depth;
    s32 scale;
    s32 result;

    Iwram_TransformVector(point, screen);
    projection = &gProjection;
    depth = -screen[2];
    result = 0;
    if (depth >= projection->near && depth <= projection->far) {
        screen[2] = depth >> 16;
        if (projection->focal != 0) {
            u32 shifted = (u32)depth >> 11;

            scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
        } else {
            scale = 0x151eb;
        }
        screen[0] = projection->center_x + Iwram_MulQ16(screen[0], scale) / 0x10000;
        screen[1] = projection->center_y - Iwram_MulQ16(screen[1], scale) / 0x10000;
        result = scale;
    }
    return result;
}
