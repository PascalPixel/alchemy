#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080d9ae8, named
 * RunPaletteRampEffect (games/gs1/source-paths.json) and called with
 * (effect, mode) from the four battle/effects/palette_ramp/mode_N.c
 * wrappers already adopted in this repository (mode in 0..3).
 *
 * Real callee set and constants match the 0x03001eec "battle work"
 * subsystem already partly recovered in games/gs1/recon/en/main/080e7404.c,
 * games/gs1/recon/en/main/080d82b0.c and games/gs1/recon/en/main/080dc1ec.c,
 * and the already-adopted games/gs1/src/battle/effects/member_orbit/run.c.
 * Field offsets (work+0x7780/0x7784/0x7824/0x7828, member+8/+16,
 * member-id halfword at effect+36+2*member) and the M2C_FIELD/WordCopyFn/
 * DrawRectangleFn conventions come from that evidence.
 *
 * Func_080cef64(flag, out_pair) is already adopted as
 * games/gs1/src/unidentified/main/runtime/runtime_owner_21.c: it
 * unconditionally writes two draw-callback words (the kind-46 and kind-47
 * allocator-cache slots) into out_pair.  The two `bl Func_080072f4` sites
 * below are `_call_via_r4` (games/gs1/asm/080072e4.s, N=4 slot) -- indirect
 * calls through a DrawRectangleFn already loaded into r4 from the saved
 * out_pair pointer, not calls to a real function of that name; modeled the
 * same way games/gs1/recon/en/main/080dc1ec.c and 080e01e4.c already do.
 *
 * Reconstruction notes from the reference bytes:
 *  - The epilogue pops the return address into r0 ("pop {r0}; bx r0"), so the
 *    function returns void and the trailing Func_080cdbc0() call is a
 *    statement, not a "return".
 *  - The whole per-member body (the two Func_080049ac/Func_080051d8 passes,
 *    the local_frame == 24 sound cue and the particle burst loop) sits inside
 *    "if (local_frame > 0)": the reference branches straight to the member
 *    increment block when local_frame <= 0.
 *  - Func_08004cb4(rec) runs after the second Func_080049ac/Func_080051d8
 *    pair, not immediately after the rec[] stores.
 *  - particle[2] uses "frame % 4", not "(frame / 4) % 4".
 *  - The burst clamp is spelled "if (local_frame > 28) { ... } else burst = 0;"
 *    so the positive branch reuses the spilled "threshold" directly, matching
 *    the reference's "adds r3, r2, #0 / cmp r2, #0".
 *  - The scratch counters are shared the way the reference allocates them:
 *    one counter (outer) drives the ramp and the particle-burst loops, "frame"
 *    drives the particle-seed loop and the outer frame loop, and "member"
 *    drives the ramp inner loop and the member loop.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))
#define RunPaletteRampEffect Func_080d9ae8

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Value_0000009d;
extern u8 Value_000000b7;
extern u8 Value_000000bb;

extern const u16 Data_080eea08[];
extern const u8 Data_080eea20[];
extern const u8 Data_080eea2c[];

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void Func_080041d8(s32 callback, s32 interval);
void Func_08004278(s32 callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
void Func_080e3944(const void *src, void *dest);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_08004cb4(void *record);
void Func_080cef64(s32 flag, DrawRectangleFn *out_pair);
u32 Func_08004458(void);
s32 Func_080022fc(s32 a, s32 b);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

s32 RunPaletteRampEffect(s32 effect, s32 mode)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    DrawRectangleFn callback_pair[2];
    s32 palette_id;
    void *palette;
    s32 outer;
    s32 particle_ceiling;
    u8 *ramp_src;
    u8 *ramp_dst;
    s32 frame;
    s32 member;
    s32 y_offset;
    s32 zero_vec[3];
    s32 screen[3];
    s32 rec[3];
    s32 local_frame;
    s32 threshold;
    s32 particle_base;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = (void *)effect;

    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;

    if (mode == 0) {
        Func_080e0524((s32) &Value_0000009c, work, 1, 1);
    } else {
        Func_080e0524((s32) &Value_0000009b, work, 1, 1);
    }

    if (mode == 0) {
        palette_id = (s32) &Value_000000bb;
    } else if (mode == 1) {
        palette_id = (s32) &Value_000000b7;
    } else {
        palette_id = (s32) &Value_000000bb;
    }
    palette = Func_08002f40(palette_id);
    ((WordCopyFn) 0x03001388)((void *) 0x05000000, palette, 128);

    Func_080e0524((s32) &Value_0000009d, (u8 *) work + 0x2580, 0, 0);

    {
        s32 dst_offset;

        particle_ceiling = 57;
        dst_offset = 696;
        for (outer = 1; outer != 8; outer++) {

            ramp_src = (u8 *) work + 0x2580;
            ramp_dst = (u8 *) work + 0x2580 + dst_offset;
            member = 0;
            do {
                s32 v;

                v = *ramp_src;
                ramp_src++;
                if (v > particle_ceiling) {
                    v = particle_ceiling;
                }
                if (v < 0) {
                    v = 0;
                }
                *ramp_dst = (u8) v;
                ramp_dst++;
                member++;
            } while (member != 696);

            dst_offset += 696;
            particle_ceiling -= 7;
        }
    }

    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
        M2C_FIELD((void *) 0x04000028, s32 *, 0) = 0xFFFF9000;
        y_offset = -112;
    } else {
        M2C_FIELD((void *) 0x04000028, s32 *, 0) = 0;
        y_offset = 0;
    }

    {
        s32 *particle;
        s32 radius;

        particle = (s32 *) 0x02010000;
        radius = 192;
        for (frame = 0; frame != 512; frame++) {
            s32 angle;
            s32 bucket;
            s32 quarter;

            angle = (s32) (Func_08004458() & 0xFFFF);
            particle[0] = 0;
            if (mode == 0) {
                bucket = ((frame & 31) / 4) * 3;
                particle[1] = (bucket << 17) - 0xA0000;
                quarter = frame % 4;
                particle[2] = (quarter << 17) - 0x20000;
            } else {
                bucket = ((frame & 31) / 4) * 3;
                particle[1] = (bucket << 17) - 0xA0000;
                quarter = frame % 4;
                particle[2] = (quarter << 19) - 0x100000;
            }
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
                particle[3] = 128 << 10;
            } else {
                particle[3] = -0x20000;
            }
            particle[4] = ((Func_0800231c(angle) * radius) >> 6) + 0x10000;
            particle[5] = (Func_08002322(angle) * radius) >> 6;
            particle[6] = (s32) (Func_08004458() & 0xFF);
            particle += 7;
        }
    }

    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4),
        callback_pair);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8(0x080CD261, 0x480);

    frame = 0;
    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) * 4 != -64) {
        do {
            s32 facing;
            s32 facing_end;
            s32 x;
            s32 y2;

            facing = *(s32 *) 0x03001E80;

            if (frame == 72) {
                Func_080b50e8(0);
            }

            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                local_frame = frame;
                threshold = frame - 24;
                facing_end = facing + 12;
                particle_base = 0;
                do {
                    void *member_object;
                    s32 member_id;

                    member_id = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s16 *,
                        36 + member * 2);
                    member_object = *Func_080b5098(member_id);

                    if (local_frame > 0) {
                        Func_080049ac();
                        Func_080051d8(facing, facing_end);

                        rec[0] = M2C_FIELD(member_object, s32 *, 8);
                        rec[1] = 160 << 13;
                        rec[2] = M2C_FIELD(member_object, s32 *, 16);

                        Func_080049ac();
                        Func_080051d8(facing, facing_end);
                        Func_08004cb4(rec);

                        zero_vec[0] = 0;
                        zero_vec[1] = 0;
                        zero_vec[2] = 0;
                        Func_080e3944(zero_vec, screen);

                        x = screen[0] + y_offset;
                        y2 = screen[1];

                        if (mode == 0) {
                            if (local_frame <= 26) {
                                s32 v;

                                v = Func_080022fc(local_frame / 4, 7);
                                ((DrawRectangleFn) callback_pair[0])(
                                    draw_destination,
                                    (u8 *) work + ((v * 15) << 6),
                                    x - 12, y2 - 20, 24, 40);
                            }
                        } else {
                            if (local_frame <= 23) {
                                s32 v;

                                v = Func_080022fc(local_frame / 4, 6);
                                ((DrawRectangleFn) callback_pair[1])(
                                    draw_destination,
                                    (u8 *) work + ((v * 25) << 6),
                                    x - 20, y2 - 20, 40, 40);
                            }
                        }

                    if (local_frame == 24) {
                        Func_080f9010(143);
                    }

                    if ((u32) threshold <= 36) {
                        s32 burst;
                        s32 ramp_offset;
                        s32 *particle;

                        if (local_frame > 28) {
                            burst = threshold / 4;
                            if (burst > 7) {
                                burst = 7;
                            }
                        } else {
                            burst = 0;
                        }
                        ramp_offset = burst * 696;

                        particle = (s32 *) ((u8 *) 0x02010000
                            + particle_base);
                        for (outer = 0; outer != 24; outer++) {
                            s32 sel;
                            s32 idx;

                            sel = (outer % 4) * 3;
                            idx = Func_080022fc(
                                (particle[6] + local_frame) / 8, 3);
                            Func_080e3944(particle, screen);
                            x = screen[0] + y_offset;
                            y2 = screen[1];

                            idx = sel + idx;
                            ((DrawRectangleFn) callback_pair[0])(
                                draw_destination,
                                (u8 *) work
                                    + (ramp_offset + Data_080eea08[idx])
                                    + 0x2580,
                                x, y2,
                                Data_080eea20[idx], Data_080eea2c[idx]);

                            Func_080e38b8(particle, 60, 0);
                            particle += 7;
                        }
                    }
                    }

                    member++;
                    particle_base += 896;
                    threshold -= 4;
                    local_frame -= 4;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        20));
            }

            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            frame++;
        } while (frame
            != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) * 4
                + 64);
    }

    Func_08004278(0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
