#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_PRESENTATION.H"
#include "SCENE.H"

/* BG2 affine parameters as the HDMA work block keeps them. */
struct Affine {
    u16 pa;
    u16 pb;
    u16 pc;
    u16 pd;
    s32 x;
    s32 y;
};

/* Double-buffered per-scanline BG2CNT values; page selects the one shown. */
struct AffineHdma {
    u32 page;
    u8 padding04[12];
    struct Affine affine;
    u16 lines[2][160];
};

struct BattleView {
    u8 padding00[8];
    s32 mode;
    s32 mode2;
    s32 busy;
};

extern u16 gBgScroll[];
extern u8 gTransitionWork[];
s32 GameFlag_TestFar(s32 flag);

/* Inline, so the arguments are computed before the routine's address. */
static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    return Iwram_RatioMulQ14(divisor, value);
}

extern u8 gCameraWork[];
extern u8 gProjection[];
void Camera_StoreSceneParameters(s32, u32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Graphics_PrepareTransferInIwramWork();
s32 Render_ProjectPoint();

/* Zoom the battle floor about (cx, cy) by scale (16.16): set the BG2 affine
 * matrix and origin, then rebuild the back page of scanline BG2CNT values,
 * the floor's lines between the horizon and the zoomed floor edge wrapping
 * once the view is zoomed in. */
void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
    /* FAKEMATCH: the work and camera pointers are addressed from the view
     * pointer's symbol, as the reference derives them from its pool entry. */
    struct BattleView *view = *(struct BattleView **)gTransitionWork;
    struct AffineHdma *work = *(struct AffineHdma **)(gTransitionWork - 136);
    struct BattleCamera *camera = *(struct BattleCamera **)(gTransitionWork - 128);
    u16 wrap;
    s32 horizon;
    u16 *line;
    struct Affine *affine;
    s32 ratio;
    s32 step;
    s32 x;
    s32 y;
    u32 i;
    u32 count;

    horizon = 0x800;
    wrap = 0;
    if (scale >= 0x10000) {
        wrap = 0x2000;
        horizon = 0x6800 + -(s16)camera->yaw * 3;
    }
    if (work == NULL)
        return;
    if ((view->mode == 1 || view->mode2 == 1) && view->busy == 0)
        gBgScroll[2] = horizon >> 8;
    if (view->mode != 2)
        return;
    line = work->lines[work->page ^ 1];
    ratio = DivQ16(scale, 0x10000);
    step = ratio >> 8;
    affine = &work->affine;
    affine->pa = step;
    affine->pb = 0;
    affine->pc = 0;
    affine->pd = step;
    x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
    y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
    affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
    y = ((y + 0x7fff) >> 8) + oy - 0x1000;
    affine->y = y;
    count = (DivQ16((s16)step, 0x4000 - y) >> 16) + 1;
    i = 0;
    if (!GameFlag_TestFar(0x16b)) {
        for (; i < 16; i++)
            *line++ = 0x3f8e;
    }
    if (count > 136)
        count = 136;
    for (; i < count; i++)
        *line++ = (u16)wrap | 0x478a;
    for (; i < 136; i++)
        *line++ = (u16)wrap | 0x478e;
    for (; i < 160; i++)
        *line++ = 0x3f8e;
    work->page ^= 1;
}

/* returns a value its callers here ignore */

/* FAKEMATCH: mode intentionally remains uninitialized to preserve the match. */
void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
    s32 span = 0x01fe0000;
    s32 mode;
    struct BattleCamera *scene = *(struct BattleCamera **)gCameraWork;
    s32 *pos = scene->pos;
    s32 *hud = (s32 *)gProjection;
    s32 scale = (mode << 16) / 100;
    s32 render_bounds[3];
    s32 measured_bounds[3];
    s32 source_bounds[3];
    s32 (*blend)(s32, s32);
    s32 alpha;
    s32 half;
    s32 width;
    u32 result;

    pos[0] = x;
    pos[1] = depth;
    pos[2] = y;
    source_bounds[0] = 0;
    alpha = 0xc000;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    blend = Iwram_RatioMulQ14;
    result = blend(span, alpha);
    Camera_StoreSceneParameters(span, result, span * 2);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)scene->yaw);
    SceneTransform_ApplyPitch((s16)scene->pitch);
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = scene->distance;
    Iwram_TransformVector(render_bounds, (s32 *)scene);
    hud[3] = 120;
    hud[4] = 120;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(scene, pos);
    Render_ProjectPoint(source_bounds, measured_bounds);

    BattleCamera_SetRange(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    half = scale * 0xff;
    width = half * 2;
    result = blend(width, alpha);
    Camera_StoreSceneParameters(width, result, half * 4);
}
