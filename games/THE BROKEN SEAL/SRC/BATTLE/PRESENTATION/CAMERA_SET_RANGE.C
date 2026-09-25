#include "TYPES.H"
#include "IWRAM_CALL.H"

/* battle/presentation/camera_set_range.c */
typedef s32 (*ArmRatio)(s32 numerator, s32 denominator);

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

struct BattleCamera {
    u8 padding00[0x36];
    s16 pitch;
};

extern u16 gBgScroll[];
extern u8 Data_03001f00[];

s32 GameFlag_TestFar(s32 flag);

static __inline__ s32 Iwram_DivQ16(s32 numerator, s32 denominator)
{
    return ((ArmRatio)0x0300013c)(numerator, denominator);
}

/* Zoom the battle floor about (cx, cy) by scale (16.16): set the BG2 affine
 * matrix and origin, then rebuild the back page of scanline BG2CNT values,
 * the floor's lines between the horizon and the zoomed floor edge wrapping
 * once the view is zoomed in. */
void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
    /* FAKEMATCH: the work and camera pointers are addressed from the view
     * pointer's symbol, as the reference derives them from its pool entry. */
    struct BattleView *view = *(struct BattleView **)Data_03001f00;
    struct AffineHdma *work = *(struct AffineHdma **)(Data_03001f00 - 136);
    struct BattleCamera *camera = *(struct BattleCamera **)(Data_03001f00 - 128);
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
        horizon = 0x6800 + -camera->pitch * 3;
    }
    if (work == NULL)
        return;
    if ((view->mode == 1 || view->mode2 == 1) && view->busy == 0)
        gBgScroll[2] = horizon >> 8;
    if (view->mode != 2)
        return;
    line = work->lines[work->page ^ 1];
    ratio = Iwram_DivQ16(scale, 0x10000);
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
    count = (Iwram_DivQ16((s16)step, 0x4000 - y) >> 16) + 1;
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
