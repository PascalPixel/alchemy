/* NONMATCHING: 636 of 636 bytes, 102 differing halfwords (2026-09-24). Readable
 * unit source (evconv Engine_* veneers). Remaining: the dust-drift temporaries
 * land in r2 instead of r3, the -0xcccc bias is added into dx in place in the
 * reference, and the params pointer and inner counter spill to swapped slots
 * (sp+16 and sp+20). */
#include "TYPES.H"

void Engine_AudioPlayCue(s32 cue);
void Engine_MapCopyCellsTo(s32 sx, s32 sy, s32 dx, s32 dy, s32 w, s32 h);
u32 Engine_RandomNext(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_EventWait(s32 frames);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[24];
};

/* A random drift of about -0.8 to +0.8 in steps of 0.2. */
#define STEP() ((s32)((Engine_RandomNext() << 3) >> 16) * 0x3333)

/* Slide one of three stone doors two cells open, with dust along its edge. */
void MakyuriHeya_Func02001db4(s32 side)
{
    struct EffectParams params;
    struct EffectParams *p;
    u32 i;
    u32 j;
    s32 down;
    s32 up;
    s32 dx;
    u32 r;

    Engine_AudioPlayCue(211);
    if (side == 0) {
        Engine_MapCopyCellsTo(111, 57, 113, 42, 1, 1);
        Engine_MapCopyCellsTo(111, 59, 113, 43, 1, 1);
    } else if (side == 1) {
        Engine_MapCopyCellsTo(113, 58, 112, 46, side, side);
        Engine_MapCopyCellsTo(115, 58, 113, 46, side, side);
    } else {
        Engine_MapCopyCellsTo(115, 57, 116, 44, 1, 1);
        Engine_MapCopyCellsTo(113, 57, 115, 44, 1, 1);
    }
    p = &params;
    p->kind = 7;
    p->spread = 0x8000;
    p->rise = 0x8000;
    for (i = 0; i <= 1; i++) {
        j = 0;
        down = 0x32c0000 - (i << 20);
        up = (i << 20) + 0x2c00000;
        for (; j <= 7; j++) {
            if (j & 1) {
                if (side == 0) {
                    r = (Engine_RandomNext() << 3) >> 16;
                    dx = r << 1;
                    dx += r;
                    dx += dx << 4;
                    dx += dx << 8;
                    Effect_Spawn(0x3180000, 0, up, dx + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                } else if (side == 1) {
                    r = (Engine_RandomNext() << 3) >> 16;
                    dx = r << 1;
                    dx += r;
                    dx += dx << 4;
                    dx += dx << 8;
                    Effect_Spawn(up + 0x600000, 0, 0x2ea0000, dx + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                } else {
                    r = (Engine_RandomNext() << 3) >> 16;
                    dx = r << 1;
                    dx += r;
                    dx += dx << 4;
                    dx += dx << 8;
                    Effect_Spawn(down, 0, 0x2ca0000, dx + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                }
                Engine_EventWait(1);
            }
            down += -0x10000;
            up += 0x10000;
        }
        if (side == 0) {
            Engine_MapCopyCellsTo(111, 58, 113, i + 43, 1, 1);
            Engine_MapCopyCellsTo(111, 59, 113, i + 44, 1, 1);
        } else if (side == 1) {
            Engine_MapCopyCellsTo(114, 58, i + 113, 46, side, side);
            Engine_MapCopyCellsTo(115, 58, i + 114, 46, side, side);
        } else {
            Engine_MapCopyCellsTo(114, 57, 115 - i, 44, 1, 1);
            Engine_MapCopyCellsTo(113, 57, 114 - i, 44, 1, 1);
        }
    }
}
