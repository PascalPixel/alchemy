/*
 * Draft: Render_ProjectPoint, ☀️'s projection through a stack copy of the
 * transformed point, which an out-of-range depth returns unprojected. 8 rows
 * differ: ⚓️ passes sp straight to the transform and takes sp into r6
 * again after the call; this C keeps one r6 = sp for both. A plain array,
 * and 150 devsearch steps, did not change it. It follows
 * Camera_StoreSceneParameters (whose u32 array becomes this struct
 * Projection) into GRAPHICS/CAMERA_STORE_SCENE_PARAMETERS.C.
 * Links as recon/tla/raw/08015778.s.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"

/* The projection Render_ProjectPoint applies after the scene transform. */
struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

/* The three components of a transformed point. */
struct ProjectedPoint {
    s32 x;
    s32 y;
    s32 z;
};

extern struct Projection gCameraSceneParameters;
/* Transforms point through the IWRAM matrix routine and projects it to
   screen x, y and depth; returns the perspective scale. A point whose depth
   lies outside the near and far planes comes back as transformed, and the
   result is 0. */
s32 Render_ProjectPoint(s32 *point, struct ProjectedPoint *screen)
{
    struct ProjectedPoint view;
    struct Projection *projection;
    s32 depth;
    s32 scale;
    s32 x;

    Iwram_TransformVector(point, &view.x);
    projection = &gCameraSceneParameters;
    depth = -view.z;
    if (depth >= projection->near && depth <= projection->far) {
        screen->z = depth >> 16;
        if (projection->focal != 0) {
            u32 shifted = (u32)depth >> 11;

            scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
        } else {
            scale = 0x151eb;
        }
        x = projection->center_x + Iwram_MulQ16(view.x, scale) / 0x10000;
        screen->y = projection->center_y - Iwram_MulQ16(view.y, scale) / 0x10000;
        screen->x = x;
        return scale;
    }
    *screen = view;
    return 0;
}
