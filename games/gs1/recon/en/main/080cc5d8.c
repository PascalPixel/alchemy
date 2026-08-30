#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080cc5d8.  Family-matched to
 * games/gs1/src/battle/effects/member_orbit/run.c (owner 080ce85c) at
 * structural score 8031/10000, but this owner allocates its own kind-39
 * (work), kind-40 (canvas) and kind-41 (trail_source) heap blocks up front
 * via Func_080048b0 instead of reading pre-existing ones out of the shared
 * heap-allocation cache, and frees them again (in LIFO order) at the end --
 * see games/gs1/src/battle/effects/objects/start_effect_22.c for the
 * Func_080048b0(asset_id, size) signature.
 *
 * Data_03001e50[kind] is that same heap-allocation cache (see the comment
 * in games/gs1/recon/en/main/080e7404.c and games/gs1/src/battle/effects/
 * puff_arc/run.c); this owner reads it directly at kinds 46 and 47 rather
 * than through a locally-renamed "heap_cache" pointer, since it never reads
 * kinds 39/40/41 back out of it (it made those blocks itself).
 *
 * Field offsets 0x7780/0x7784/0x7824/0x7828 and the Func_080cd594/
 * Func_080e0524/Func_080041d8/Func_08004278/Func_080ed408/Func_08002dd8/
 * Func_080cdbc0/Func_080d6888 calling shapes follow the 0x03001eec "battle
 * work" subsystem already recovered in
 * games/gs1/src/battle/effects/member_orbit/run.c and
 * games/gs1/recon/en/main/080d59b0.c.  The fixed 0x02010000 "star" array and
 * its 28-byte, {f0,f4,f8,f24}-field record shape is the same one used by
 * games/gs1/recon/en/main/080d59b0.c; this owner also keeps a second,
 * independent copy of that record shape inside its own work block at
 * +0x7080 (64 entries) for a per-frame sparkle trail around a single
 * projected point, which is why the fixed 0x02010000 array here is only
 * ever written, never read back inside this function.
 *
 * Value_00000045/00000046/00000047/00000048/00000057/00000076 are the
 * established spelling for small absolute link-time constants (see
 * games/gs1/src/battle/effects/member_orbit/run.c's Value_000000af comment):
 * every one of these is loaded from a literal pool rather than an
 * immediate, which an ordinary integer literal cannot produce.
 *
 * Data_080ee058/080ee05c/080ee060 and Data_080ede5c are pre-existing ROM
 * tables already catalogued in games/gs1/assets/code/sentou_kouka_hyou_a.json
 * (hyou_a_030/031/032 and hyou_a_001 respectively); Data_080ede48 in
 * games/gs1/recon/en/main/080dc1ec.c documents the extern-array convention
 * used for that same asset.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern void *Data_03001e50[];
extern const u8 Data_080ee058[4];
extern const u8 Data_080ee05c[4];
extern const u8 Data_080ee060[4];
extern const u16 Data_080ede5c[];
extern u8 Value_00000045;
extern u8 Value_00000046;
extern u8 Value_00000047;
extern u8 Value_00000048;
extern u8 Value_00000057;
extern u8 Value_00000076;

void *Func_080048b0(s32 kind, s32 size);
void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
u32 Func_08004458(void);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 id);
void Func_080e396c(s32 source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
void Func_08002dd8(s32 id);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
s32 Func_080cdbc0(void);

void Func_080cc5d8(void *object)
{
    void *work;
    void *canvas;
    DrawRectangleFn rectangle[2];
    void *trail_source;
    void *palette;
    s32 status;
    s32 palette_id;
    u8 *star;
    s32 i;
    s32 frame;
    s32 screen[3];
    WordCopyFn copy;
    s32 callback_interval;

    work = Func_080048b0(39, 0x782c);
    canvas = Func_080048b0(40, 0x4000);
    trail_source = Func_080048b0(41, 0x60e);
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);

    M2C_FIELD(work, s32 *, 0x77b4) = 24;
    M2C_FIELD(work, s32 *, 0x77b8) = 0;
    M2C_FIELD((void *)0x04000052, u16 *, 0) = 0x100c;
    M2C_FIELD((void *)0x04000020, u16 *, 0) = 0x100;

    Func_080e0524((s32)&Value_00000045, work, 1, 0);
    Func_080e0524((s32)&Value_00000076, trail_source, 0, 0);

    switch (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0)) {
    case 0:
        palette_id = (s32)&Value_00000048;
        break;
    case 1:
        palette_id = (s32)&Value_00000057;
        break;
    case 2:
        palette_id = (s32)&Value_00000047;
        break;
    default:
        palette_id = (s32)&Value_00000046;
        break;
    }
    palette = Func_08002f40(palette_id);
    copy = (WordCopyFn)0x03001388;
    status = copy((void *)0x05000000, palette, 128);

    star = (u8 *)0x02010000;
    for (i = 0; i != 128; i++) {
        M2C_FIELD(star, s32 *, 4) = 0x800000;
        M2C_FIELD(star, s32 *, 0) = (s32)(Func_08004458() & 0xFFFF);
        M2C_FIELD(star, s32 *, 8) = (s32)(Func_08004458() & 0x1FF) + 1024;
        M2C_FIELD(star, s32 *, 24) = -i;
        star += 28;
    }

    for (i = 0; i != 64; i++) {
        u8 *slot = (u8 *)work + 0x7080 + i * 28;
        M2C_FIELD(slot, s32 *, 0) = (s32)(Func_08004458() & 0xFFFF);
        M2C_FIELD(slot, s32 *, 4) = (s32)(Func_08004458() & 31) + 16;
        M2C_FIELD(slot, s32 *, 24) = (i & 15) + 16;
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    callback_interval = 0x480;
    Func_080041d8((void *)0x080CD261, callback_interval);

    status = Func_080ed408(46, 7, 7, 7, 3);
    rectangle[0] = (DrawRectangleFn)Data_03001e50[46];
    Func_080f9010(140);

    for (frame = 0; frame != 56; frame++) {
        Func_080e396c(M2C_FIELD(object, s32 *, 8), screen);
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (64 - screen[0]) << 8;
        if (frame > 49) {
            M2C_FIELD((void *)0x04000028, u16 *, 42) =
                (112 - frame * 2) | 0x1000;
        }

        if (frame == 26) {
            Func_080f9010(212);
            Func_080d6888(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, -1, 0, 20);
        }

        if ((u32)(frame - 28) <= 20) {
            s32 sprite_frame = Func_080022ec(frame - 28, 3);

            rectangle[0](
                canvas, (u8 *)work + 0x1400 + sprite_frame * 0x900,
                40, screen[1] - 24, 48, 48);
        }

        if ((u32)frame <= 14) {
            s32 offset = (Func_080022fc(Func_080022ec(frame, 3), 5)) << 10;

            for (i = 0; i != 4; i++) {
                s32 x;
                s32 y;

                status = Func_080ed408(47, 7, 7, Data_080ee060[i] | 3, 2);
                x = (s8)Data_080ee058[i] + 32;
                y = (screen[1] + (s8)Data_080ee05c[i]) - 32;
                rectangle[1] = (DrawRectangleFn)Data_03001e50[47];
                rectangle[1](canvas, (u8 *)work + offset, x, y, 32, 32);
                Func_08002dd8(47);
            }
        }

        if (frame >= 0) {
            u8 *slot = (u8 *)work + 0x7080;

            for (i = 0; i != 64; i++) {
                if (M2C_FIELD(slot, s32 *, 24) >= 0
                        && M2C_FIELD(slot, s32 *, 4) > 0) {
                    s32 radius_x;
                    s32 radius_y;
                    s32 half;
                    s32 full;

                    half = (M2C_FIELD(slot, s32 *, 24) >> 3) + 1;
                    radius_x = ((M2C_FIELD(slot, s32 *, 4)
                        * Func_08002322(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + 64;
                    radius_y = ((M2C_FIELD(slot, s32 *, 4)
                        * Func_0800231c(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + screen[1];
                    if (half <= 0) {
                        half = 1;
                    }
                    full = half << 1;
                    rectangle[0](
                        canvas, trail_source + Data_080ede5c[half - 1],
                        radius_x - half, radius_y - half, full, full);
                    M2C_FIELD(slot, s32 *, 4) -= 2;
                    M2C_FIELD(slot, s32 *, 24) -= 1;
                }
                slot += 28;
            }
        }

        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(46);
    Func_08004278((void *)0x080CD261);
    Func_080cdbc0();
    Func_08002dd8(41);
    Func_08002dd8(40);
    Func_08002dd8(39);
}
