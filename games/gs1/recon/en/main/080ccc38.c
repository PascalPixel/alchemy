#include "types.h"
#include "b5_context.h"
#include "effect_step.h"

/*
 * Battle-presentation sub-effect at 0x080ccc38.
 *
 * Structurally related to games/gs1/src/battle/effects/puff_arc/run.c
 * (main:080d9fc8) and games/gs1/src/battle/effects/member_orbit/run.c
 * (main:080ce85c): the same kind-39 "battle work" heap cache read directly
 * from 0x03001eec, the same caller-state pointer republished at
 * work + 0x7828, the same 0x04000020 BG2PA identity write, the same
 * kind-46 rectangle blitter fetched through Func_080ed408 and called
 * through the r4 trampoline slot (Func_080072f4 = 0x080072e4 + 0x10).
 * Despite the identical 644-byte length, the puff_arc template's body is
 * NOT a match here: this owner loads two resources (kind ids forced
 * through the pool via the Value_ idiom) into two different destinations,
 * conditionally streams a palette through the fixed word-copy routine at
 * 0x03001388 only when mode == 0, drives a frame loop bounded by 74 or 48
 * (not puff_arc's fixed 80), and its per-frame draw step indexes five
 * small tables at 0x080ee064-0x080ee088 by a signed cell = frame / 4
 * (valid range 0-5) rather than puff_arc's per-puff tick field.
 */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(void *, const void *, s32, s32, s32, s32);
typedef s32 (*WordCopyFn)(void *, const void *, s32);

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov; see puff_arc/run.c for the established
   convention this project uses for such call sites. */
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_000000a0;

void Func_080cd594(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
void Func_080041d8(s32, s32);
void Func_080e396c(s32, struct EffectPosition *);
void Func_080b50e8(s32);
void Func_080b5088(s32, s32);
void Func_080f9010(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e155c(s32, u32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(s32);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);

/* Five per-cell tables, six entries each (cell 0-5): source byte offset,
   an X value indexed by a second field-derived row, a width, a signed Y
   bias, and a height.  Addresses are read directly off the retained pool;
   no independent evidence of their exact original grouping exists yet. */
extern u16 Data_080ee070[];
extern u8 Data_080ee07c[];
extern u8 Data_080ee064[];
extern s8 Data_080ee088[];
extern u8 Data_080ee06a[];

void Func_080ccc38(void *param0, s32 mode)
{
    u32 *cache;
    u32 *entry;
    u8 *work;
    void *dst;
    s32 tag;
    s32 status;
    void *palette;
    struct B5Context *ctx;
    void *object;
    struct EffectPosition position;
    s32 frame_limit;
    s32 count;
    const u16 *tableA;

    cache = (u32 *)0x03001eec;
    entry = cache;
    work = (u8 *)*entry++;
    dst = (void *)*entry;
    M2C_FIELD(work, void **, 0x7828) = param0;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;

    tag = M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4);
    if (tag == 1) {
        Func_080ed408(46, 7, 7, 3, tag);
    } else {
        tag = 1;
        Func_080ed408(46, 7, 7, 7, tag);
    }
    Func_080e0524((s32)&Value_00000071, work, 1, 1);
    Func_080e0524((s32)&Value_00000072, (void *)0x02010000, 1, 0);

    if (mode == 0) {
        palette = Func_08002f40((s32)&Value_000000a0);
        status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8(0x080cd261, 0x480);

    ctx = Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));
    object = ctx->object;
    Func_080e396c(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), &position);
    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) != 0) {
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (112 - position.x) << 8;
    } else {
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (16 - position.x) << 8;
    }

    frame_limit = 74;
    if (mode != 1) {
        frame_limit = 48;
    }

    tableA = Data_080ee070;
    for (count = 0; count != frame_limit; count++) {
        s32 cell = count / 4;

        if (cell <= 5) {
            s32 side = M2C_FIELD(
                M2C_FIELD(work, void **, 0x7828), s32 *, 4);

            if (cell > 3) {
                ((DrawRectangleFn)cache[46 - 39])(dst,
                    (void *)(0x02010000 + tableA[cell]),
                    Data_080ee07c[cell + side * 6],
                    Data_080ee088[cell] + 32,
                    Data_080ee064[cell], Data_080ee06a[cell]);
            } else {
                ((DrawRectangleFn)cache[46 - 39])(dst, work + tableA[cell],
                    Data_080ee07c[cell + side * 6],
                    Data_080ee088[cell] + 32,
                    Data_080ee064[cell], Data_080ee06a[cell]);
            }
        }

        if (count == 8) {
            if (mode == 0) {
                Func_080b50e8(133);
                Func_080b5088(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    1);
            } else {
                Func_080f9010(134);
                Func_080d6888(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    7, 5, 0, 4);
            }
            M2C_FIELD(work, s32 *, 0x77a8) = 8;
        }

        if (mode == 1) {
            if (count == 13) {
                M2C_FIELD(object, s32 *, 40) = 0xC0000;
                M2C_FIELD(object, s32 *, 72) = 0x7851;
                M2C_FIELD(object, s32 *, 68) = 0x4000;
            }
            if (count == 65) {
                M2C_FIELD(work, s32 *, 0x77a8) = 4;
                Func_080b50e8(134);
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278(0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
