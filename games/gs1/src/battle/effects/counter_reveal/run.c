#include "types.h"
#include "scene.h"
#include "abi/battle/effects/counter_reveal/run.h"

/*
 * Battle-presentation sub-effect at 0x080cfef4, structurally related to the
 * "0x03001eec battle work" family already recovered in
 * games/gs1/src/battle/effects/member_orbit/run.c (owner 080ce85c) and
 * games/gs1/recon/en/main/080e7404.c: same heap_cache/work/canvas prologue,
 * the same raw-offset field-access spelling, and the same
 * BG2-affine + rectangle-blit (Battle_SetRange heap kinds 46/47) setup.
 *
 * Unlike the member-orbit effect this owner drives a single fixed party
 * member (its slot index lives at object+0x24, not an iterated array from
 * +0x24), runs a fixed 132-frame count rather than member_count*16+48, loads
 * a second graphics resource (Value_000000ac) straight into OBJ VRAM at
 * 0x02010000, and drives BG2PC (0x04000052) directly rather than the BG2
 * reference-point registers.  Frames 88-99 additionally reveal a run of
 * glyph-style rectangles (width/height pairs 57x98, 99x69, 128x91, 128x59,
 * 122x29, 76x25) read from two seven-byte-stride tables at 0x080ee10c and
 * 0x080ee11a, selected by the same object+4 "kind" field used for the
 * rectangle-routine pick -- most plausibly a spinning number/counter reveal
 * rather than the orbiting-member sprite loop of the sibling effect.
 *
 * `status` and `Battle_Run`/`Battle_Run2` follow the established
 * sibling reading: both addresses are `_call_via_rN` thunk slots
 * (games/gs1/asm/080072e4.s) -- r3 for Battle_Run, r4 for Battle_Run2
 * -- so each call is a genuine indirect call through a traced function
 * pointer, not a call to a real symbol at that address.
 */
typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_000000ab;
extern u8 Value_000000ac;
extern u8 *gBattleWork;
/* The two seven-byte-stride rectangle tables are plain, non-const arrays:
 * their element loads are ordered against the outgoing-argument stores at
 * each blit call site, which a const spelling would let float away. */
extern u8 gRom[];
extern u8 gRom2[];

void *Battle_Run3(s32 id);

void **Battle_Run4(s32 member_id);

void BattleFx_RunCounterReveal(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    void *sprite_vram;
    u32 status;
    void *rectangle[2];
    void *second_rectangle;
    void *member_object;
    s32 pos[6];
    s32 curve[2];
    s32 draw_enabled;
    s32 idx_a;
    s32 idx_b;
    s32 zero_val;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    zero_val = 0;
    (*(void **)((u8 *)(work) + (0x7828))) = object;
    Battle_Check(0);
    (*(s16 *)((u8 *)((void *)0x04000020) + (0))) = 0x100;
    (*(s16 *)((u8 *)((void *)0x04000020) + (0x32))) = 0x1010;
    palette = Battle_Run3((s32)&Value_000000ab);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    palette = (u8 *)palette + 128;
    status = Battle_Apply(palette, work);
    sprite_vram = (void *)0x02010000;
    palette = Battle_Run3((s32)&Value_000000ac);
    palette = (u8 *)palette + 128;
    status = Battle_Apply(palette, sprite_vram);
    status = Battle_SetRange(46, 7, 7, 3, 1);
    rectangle[0] = heap_cache[7];
    status = Battle_SetRange(47, 7, 7, 7, 1);
    second_rectangle = heap_cache[8];
    rectangle[1] = second_rectangle;
    Battle_Apply2(0x080DBB9D, 0x480);
    (*(s32 *)((u8 *)(work) + (0x7780))) = 1;
    (*(s32 *)((u8 *)(work) + (0x7784))) = zero_val;
    Battle_Apply2(0x080CD261, 0x480);
    draw_enabled = 1;
    if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
        curve[0] = -0x500000;
    } else {
        curve[0] = 0x700000;
    }
    curve[1] = -0x200000;
    for (frame = 0; frame != 132; frame++) {
        s32 spin;
        s32 screen_x;
        s32 screen_y;
        s32 amp;
        s32 row_base;
        s32 angle;
        s32 i;
        s32 *scanline;

        spin = frame << 9;
        screen_x = (curve[0] >> 16) + ((Battle_Check2(spin) << 4) >> 16) + 48;
        screen_y = (curve[1] >> 16) + ((Battle_Check3(spin) << 2) >> 16) + 16;
        if (frame == 88) {
            Battle_Do(134);
        }
        if (frame == 32) {
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
                curve[0] = -0x200000;
            } else {
                curve[0] = 0x480000;
            }
            curve[1] = 0x180000;
            draw_enabled = 0;
        }
        if (frame == 33) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = 0x1010;
            draw_enabled = 1;
        }
        if (frame == 64) {
            Battle_Apply3(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))),
                pos);
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
                curve[0] = (pos[0] - 128) << 16;
            } else {
                curve[0] = (pos[0] - 64) << 16;
            }
            curve[1] = 0;
            draw_enabled = 0;
        }
        if (frame == 65) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = 0x1010;
            draw_enabled = 1;
        }
        scanline = (s32 *)((u8 *)work + 0x6980);
        amp = 0;
        if (frame <= 31) {
            if (frame > 15) {
                amp = (frame * 2) - 32;
                (*(s16 *)((u8 *)((void *)0x04000052) + (0))) =
                    (s16)((31 - frame) | 0x1000);
            }
        } else if (frame <= 63) {
            if (frame > 47) {
                amp = (frame * 2) - 96;
                (*(s16 *)((u8 *)((void *)0x04000052) + (0))) =
                    (s16)((63 - frame) | 0x1000);
            }
        }
        if (amp < 0) {
            amp = 0;
        }
        /* The counter is live from here, so it shares no register with
         * screen_x, which dies in the row-base expression below. */
        i = 0;
        row_base = (6 - screen_x) << 8;
        angle = frame << 11;
        for (; i != 160; i++) {
            *scanline++ =
                row_base - ((Battle_Check2(angle) * amp) >> 10);
            angle += 0x800;
        }
        if (draw_enabled != 0) {
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 0) {
                idx_a = 0;
                idx_b = 0;
            } else {
                idx_a = 1;
                idx_b = i >> 31;
            }
            if (frame <= 87) {
                ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                    canvas, work, gRom[idx_a * 7],
                    gRom2[idx_b * 7] + screen_y, 57, 98);
            } else {
                if (frame <= 91) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, work, gRom[idx_a * 7],
                        gRom2[idx_b * 7] + screen_y, 57, 98);
                }
                ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                    canvas, (u8 *)work + 0x15D2, gRom[idx_a * 7 + 1],
                    gRom2[idx_b * 7 + 1] + screen_y, 99, 69);
                if ((u32)(frame - 88) <= 1U) {
                    status = ((WordCopyFn)0x03000168)(
                        canvas, (void *)0x4000, (void *)0x3F3F3F3F);
                }
                if ((u32)(frame - 90) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (u8 *)work + 0x3081,
                        gRom[idx_a * 7 + 2],
                        gRom2[idx_b * 7 + 2] + screen_y, 128, 91);
                }
                if ((u32)(frame - 92) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (void *)0x02010000,
                        gRom[idx_a * 7 + 3],
                        gRom2[idx_b * 7 + 3] + screen_y, 128, 91);
                }
                if ((u32)(frame - 94) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (void *)0x02012D80,
                        gRom[idx_a * 7 + 4],
                        gRom2[idx_b * 7 + 4] + screen_y, 128, 59);
                }
                if ((u32)(frame - 96) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (void *)0x02014B00,
                        gRom[idx_a * 7 + 5],
                        gRom2[idx_b * 7 + 5] + screen_y, 122, 29);
                }
                if ((u32)(frame - 98) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (void *)0x020158D2,
                        gRom[idx_a * 7 + 6],
                        gRom2[idx_b * 7 + 6] + screen_y, 76, 25);
                }
            }
        }
        if (frame == 88) {
            member_object = *Battle_Run4((*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))));
            (*(s32 *)((u8 *)(member_object) + (0x28))) = 0x10000;
            (*(s32 *)((u8 *)(member_object) + (0x34))) = 0x20000;
            (*(s32 *)((u8 *)(member_object) + (0x30))) = 0x20000;
            (*(s32 *)((u8 *)(member_object) + (0x48))) = 0;
            (*(s8 *)((u8 *)(member_object) + (0x5A))) = 0;
            (*(s8 *)((u8 *)(member_object) + (0x58))) = 0;
            Battle_SetMode(member_object,
                (*(s32 *)((u8 *)(member_object) + (8))) << 1, 0,
                (*(s32 *)((u8 *)(member_object) + (16))));
            Battle_SetRange2(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), -1,
                5, -1, 0);
        }
        if (frame == 120) {
            (*(s32 *)((u8 *)(*Battle_Run4((*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))))) + (0x48))) = 0xAB85;
        }
        (*(s32 *)((u8 *)(work) + (0x7824))) = 1;
        Battle_Do2(1);
    }
    Battle_Do3((void *)0x080CD261);
    Battle_Do3((void *)0x080DBB9D);
    Battle_Place(1, (*(u16 *)((u8 *)(gBattleWork) + (0x648))), 24);
    Battle_Do4(47);
    Battle_Do4(46);
    Battle_Check4();
}
