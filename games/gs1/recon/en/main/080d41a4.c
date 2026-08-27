#include "types.h"

/*
 * Battle-presentation sub-effect at 0x03001eec "battle work" subsystem
 * (family of games/gs1/src/battle/effects/member_orbit/run.c, owner
 * 0x080ce85c, already adopted).  Field offsets and calling shapes follow
 * that owner's documented layout (work + 0x7780/0x7784/0x7824/0x7828, the
 * M2C_FIELD(expr,type_ptr,offset) macro, the DrawRectangleFn typedef, the
 * heap_cache cursor-advance-then-index-N idiom) and the r4-slot
 * `_call_via_rN` trampoline finding recorded in
 * games/gs1/recon/en/main/080dc1ec.json: a `bl` into
 * games/gs1/asm/080072e4.s's r4 veneer is an indirect call through a
 * DrawRectangleFn value carried in r4, not a call to a real function named
 * "Func_080072f4".
 *
 * Unlike the template, this owner keeps a third heap_cache slot
 * (heap_cache[2], "sprite_sheet") alongside work/canvas, runs an inner
 * per-"kind" loop (0..count-1, count from Data_080ee25e[state->0x18]) that
 * draws a growing/shrinking marker via Func_080072f0 (the r3-slot
 * trampoline, modeled per games/gs1/src/unidentified/main/battle/
 * battle_owner_12.c's established direct-call-with-trailing-target
 * convention) and the rectangle[0] blit routine, spawns particles into the
 * shared 1024-entry 0x02010000 pool on the middle frame of that window, and
 * updates every party member's position/rotation once per kind.  A second,
 * unconditional pass over the whole particle pool ages and redraws every
 * live particle every frame.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_00000060;
extern u8 Value_000000d1;
extern u8 Value_00000073;
extern u8 Data_080ee25e[4];
extern u8 Data_080ee250[14];
extern u16 Data_080ee244[6];
extern u16 Data_080ede48[];

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
void Func_080072f0(s32 a, s32 b, s32 c, s32 target);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080e0524(s32 id, void *work, s32 a, s32 b);
s32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080b5088(s32 member_id, s32 a);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080e3908(void *particle, s32 a, s32 b);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
s32 Func_080cdbc0(void);

void Func_080d41a4(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *sprite_sheet;
    void *rectangle[2];
    void **rectangle_slot;
    s32 count;
    s32 count8;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sprite_sheet = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);
    *(u16 *)0x04000052 = 0x1010;
    Func_080ed408(46, 7, 7, 3, 3);
    rectangle[0] = heap_cache[7];
    Func_080ed408(47, 7, 7, 7, 2);
    {
        void *second_rectangle;

        second_rectangle = heap_cache[8];
        rectangle_slot = rectangle;
        rectangle_slot[1] = second_rectangle;
    }
    Func_080e0524((s32)&Value_000000d1, work, 1, 1);
    Func_080e0524((s32)&Value_00000073, sprite_sheet, 0, 0);
    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18) != 2) {
        void *palette = Func_08002f40((s32)&Value_00000060);
        Func_080072f0(0x05000000, (s32)palette, 128, 0x03001388);
    }
    {
        s32 *particle_life;
        s32 i;

        particle_life = (s32 *)0x02010018;
        i = 0;
        do {
            i++;
            *particle_life = 0;
            particle_life = (s32 *)((u8 *)particle_life + 28);
        } while (i != 1024);
    }
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);
    count = Data_080ee25e[M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)];
    count8 = count * 8;
    for (frame = 0; frame != (count8 - count) + 48; frame++) {
        s32 j;

        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18) == 2
                && frame <= 63) {
            void *facing;
            s32 shift;

            facing = *(void **)0x03001E80;
            shift = 256;
            if (frame > 55) {
                shift = 704 - (frame * 8);
            }
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
                M2C_FIELD(facing, u16 *, 54) -= shift;
            } else {
                M2C_FIELD(facing, u16 *, 54) += shift;
            }
        }
        if (frame == 32) {
            Func_080b50e8(0x86);
        }
        for (j = 0; j != count; j++) {
            s32 base;

            base = j * 8;
            if (frame == base) {
                Func_080f9010(0x86);
                Func_080072f0((s32)canvas, 0x4000, 0x10101010, 0x03000168);
            }
            if (frame < base) {
                goto after_range;
            }
            if (frame >= base + 9) {
                goto after_range;
            }
            if (frame >= base + 1) {
                if (frame >= base + 2) {
                    goto range_high;
                }
                {
                    s32 kind4 = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 4);
                    u8 sprite = Data_080ee250[j + (kind4 * 7)];
                    ((DrawRectangleFn)rectangle[0])(
                        canvas, work, sprite - 24, 0, 48, 112);
                }
            }
            if (frame < base + 2) {
                goto after_range;
            }
        range_high:
            if (frame < base + 4) {
                s32 kind4 = M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 4);
                u8 sprite = Data_080ee250[j + (kind4 * 7)];
                ((DrawRectangleFn)rectangle[0])(
                    canvas, (u8 *)work + 0x1500, sprite - 24, 0, 48, 112);
            }
            if (frame == base + 2) {
                void *particle;
                s32 spawned;
                s32 scan;
                s32 kind;

                particle = (void *)0x02010000;
                spawned = 0;
                scan = 0;
                do {
                    if (M2C_FIELD(particle, s32 *, 0x18) == 0) {
                        s32 speed;
                        s32 speed32;
                        s32 angle;
                        s32 kind4;
                        u8 type;

                        speed = Func_08004458() & 0x3FF;
                        angle = (Func_08004458() & 0x7FFF) - 0x4000;
                        kind4 = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s32 *, 4);
                        type = Data_080ee250[j + (kind4 * 7)];
                        M2C_FIELD(particle, s32 *, 0) = type << 16;
                        M2C_FIELD(particle, s32 *, 4) = 0x680000;
                        speed32 = speed + 32;
                        M2C_FIELD(particle, s32 *, 0xC) =
                            (speed32 * Func_08002322(angle)) >> 7;
                        M2C_FIELD(particle, s32 *, 0x10) =
                            -((speed32 * Func_0800231c(angle)) * 2) >> 7;
                        M2C_FIELD(particle, s32 *, 0x18) =
                            (Func_08004458() & 7) + 32;
                        spawned++;
                        kind = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s32 *, 0x18);
                        if (spawned == Data_080ee244[kind * 2]) {
                            goto spawn_done;
                        }
                    }
                    scan++;
                    particle = (u8 *)particle + 28;
                } while (scan != 1024);
            spawn_done:
                kind = M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 0x18);
                M2C_FIELD(work, s32 *, 0x77A8) = Data_080ee244[kind * 2 + 1];
            }
        after_range:
            if (frame == base + 4) {
                s32 member_count;

                member_count = M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 0x14);
                if (member_count != 0) {
                    s32 k;
                    s32 id_offset;

                    id_offset = 36;
                    for (k = 0; k != member_count; k++) {
                        Func_080b5088(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, id_offset),
                            1);
                        Func_080d6888(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, id_offset),
                            7, 5, k, 8);
                        id_offset += 2;
                    }
                }
            }
        }
        {
            void *particle;
            s32 scan;

            particle = (void *)0x02010000;
            for (scan = 0; scan != 1024; scan++) {
                s32 life;

                life = M2C_FIELD(particle, s32 *, 0x18);
                if (life > 0) {
                    s32 altitude;

                    life -= 1;
                    M2C_FIELD(particle, s32 *, 0x18) = life;
                    Func_080e3908(particle, 60, 0x1000);
                    altitude = M2C_FIELD(particle, s32 *, 4);
                    if (altitude > 0x680000) {
                        M2C_FIELD(particle, s32 *, 0x10) =
                            -M2C_FIELD(particle, s32 *, 0x10) / 2;
                    } else {
                        s32 x_pos;
                        s32 x_screen;

                        x_pos = M2C_FIELD(particle, s32 *, 0);
                        x_screen = x_pos >> 16;
                        if (x_pos >= 0 && x_screen <= 119 && altitude >= 0) {
                            s32 half;
                            s32 full;

                            half = (M2C_FIELD(particle, s32 *, 0x18) / 8) + 1;
                            full = half * 2;
                            ((DrawRectangleFn)rectangle_slot[scan & 1])(
                                canvas,
                                (u8 *)sprite_sheet + Data_080ede48[half - 1],
                                x_screen - (half / 2),
                                (altitude >> 16) - half,
                                half, full);
                        }
                    }
                }
                particle = (u8 *)particle + 28;
            }
        }
        Func_080e155c(8, 16);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
