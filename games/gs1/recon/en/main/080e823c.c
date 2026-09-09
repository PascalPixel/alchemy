#include "types.h"

/*
 * Battle-presentation scene driver for the 0x03001EEC "battle work" subsystem,
 * same family as the already drafted owner 0x080d41a4 (games/gs1/recon/en/main/
 * 080d41a4.c) and the adopted 0x080ce85c template: identical heap_cache layout
 * (work / canvas / sprite_sheet at 0x03001EEC..0x03001EF4, blit routines at
 * heap_cache[7] and heap_cache[8] via absolute_03001e50 + 0xb8/0xbc), identical
 * work-block offsets (0x7780/0x7784/0x77A8/0x7824/0x7828) and the same shared
 * 1024-entry 28-byte particle pool at 0x02010000.
 *
 * Shape of the scene, read from the owner's own reference:
 *   - Create six drawable objects into work + 0x77FC (handles 9..14 of the
 *     fifteen-handle table that starts at work + 0x77D8) and force their
 *     blend field to 4.
 *   - Reserve two graphics jobs (46 and 47), keeping their blit routines in
 *     rectangle[0] / rectangle[1], then load two compressed images.
 *   - Seed a six-entry falling-object array at work + 0x7080, a 58-entry
 *     sprite array at work + 0x7128 and the whole 0x02010000 pool.
 *   - Run at most 320 frames, aborting early when 0x03001B04 & 3 is set.
 *     Each frame draws seven background objects from a byte-offset table,
 *     sweeps two of the objects along a circle for the first 91 frames,
 *     nudges three four-particle bursts at frames 94/134/174, advances and
 *     redraws the falling objects, ages the first 56 of the 0x7128 sprites,
 *     and on frame 260 re-poses every party member and refills the first 512
 *     of the pool's 1024 entries.  Only those first 512 are drawn each frame.
 *   - Tear down: release the fifteen handles, drop the scheduler callback and
 *     both graphics jobs.
 *
 * `Func_080072f4` is not a real symbol: it is the r4 entry of the
 * `_call_via_rN` trampoline block at games/gs1/asm/080072e4.s, so a `bl` there
 * is an indirect call through the DrawRectangleFn value carried in r4. That
 * finding is recorded in games/gs1/recon/en/dossiers.json#main:080dc1ec and is
 * modelled here the way the 0x080d41a4 draft models it.
 *
 * Uncertain, left neutral: the roles of Func_080cd594/Func_080c9048/
 * Func_080cd104/Func_080dbb24 and of the particle words at +0x08 and +0x14;
 * the small link-time constants that the reference materialises from its
 * literal pool rather than as immediates (0, 0x3C, 0x73, 0xC0) are spelled as
 * `Value_<hex>` externs, which is the only ordinary-C reading that reproduces a
 * pool load for a value that would otherwise fit in `movs`.
 *
 * This is a draft, not a match.  Every reference branch, loop, call, store and
 * constant is represented, but the reference spills `work` to its frame and
 * reloads it at roughly twenty sites while this source lets the compiler keep
 * it in a callee-saved register; that one allocation choice renames registers
 * through the whole body and shifts every frame offset.  The axis was searched
 * twice without a new structural fact and is reported rather than reopened.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

/* 28-byte record shared by work + 0x7080, work + 0x7128 and the 0x02010000
   pool.  Only the six words the owner touches are named. */
typedef struct {
    s32 x;
    s32 y;
    s32 unk_08;
    s32 vx;
    s32 vy;
    s32 unk_14;
    s32 timer;
} SceneParticle;

/* The four fixed-point coordinates the frame loop carries.  The reference
   keeps all four in memory and never propagates their constant initialisers,
   which is what an ordinary aggregate local does under this compiler. */
typedef struct {
    s32 x;
    s32 y;
    s32 bg_x;
    s32 bg_y;
} SceneOrigin;

extern u8 Value_00000000;
extern u8 Value_0000003c;
extern u8 Value_00000073;
extern u8 Value_000000c0;

extern s32 Data_080edac8[2];
extern u16 Data_080ede48[];
extern u8 Data_080eeed8[7];
extern u8 Data_080eeee1[7];
extern u16 Data_080eeeea[];
extern u16 Data_080eeef8[];

void Func_080cd594(s32 mode);
void Func_080c9048(void);
void Func_080041d8(void *callback, s32 interval);
void Func_080cd104(s32 a, s32 b);
void Func_080d6750(void *object);
void Func_080dbb24(s32 a, s32 b, s32 c);
void *Func_08009030(s32 id);
s32 Func_080022fc(s32 a, s32 b);
void Func_08009020(void *object, s32 value);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080030f8(s32 frames);
void Func_080b5040(s32 a, s32 b, s32 c);
void Func_080e0524(s32 id, void *dest, s32 a, s32 b);
s32 Func_08004458(void);
void Func_080f9010(s32 id);
void Func_08009008(s32 handle, const s32 *pos, const s32 *clip, s32 mode);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022ec(s32 a, s32 b);
void Func_080e3908(SceneParticle *particle, s32 a, s32 b);
void Func_080b5088(s32 member, s32 a);
void Func_080d6888(s32 member, s32 a, s32 b, s32 c, s32 d);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080b50e8(s32 id);
void Func_080d67dc(void);
void Func_08009038(s32 handle);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
void Func_080cdbc0(void);

void Func_080e823c(void *object)
{
    void **cursor;
    void *work;
    void *canvas;
    void *sprite_sheet;
    void *rectangle[2];
    void **rectangle_slot;
    void *drawable;
    s32 pos[4];
    s32 clip[2];
    s32 *pos_ptr;
    s32 *clip_ptr;
    u16 *io;
    u8 *gfx;
    u8 *group;
    u8 *spawn_base;
    SceneParticle *entry;
    SceneParticle *spark;
    s32 *handle;
    s32 handle_off;
    s32 frame;
    SceneOrigin org;
    s32 base;
    s32 angle;
    s32 speed;
    s32 life;
    s32 half;
    s32 full;
    s32 idx;
    s32 vy;
    s32 count;
    s32 off;
    u16 size;
    u16 offset;
    s32 i;
    s32 n;

    cursor = (void **)0x03001EF0;
    canvas = cursor[0];
    work = cursor[-1];
    sprite_sheet = cursor[1];
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    Func_080c9048();
    *(u16 *)0x05000000 = (u16)(s32)&Value_00000000;
    *(u16 *)0x05000002 = (u16)(s32)&Value_00000000;
    M2C_FIELD(work, s32 *, 0x7780) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080cd104(1, 0);
    Func_080d6750(M2C_FIELD(work, void **, 0x7828));
    Func_080dbb24(9, 0x17B, 2);

    /* Six drawable objects into handles 9..14 of the work table. */
    handle_off = 0x77FC;
    for (i = 0; i != 6; i++) {
        drawable = Func_08009030(390);
        M2C_FIELD(work, void **, handle_off) = drawable;
        if (drawable != NULL) {
            M2C_FIELD(drawable, s8 *, 0x26) = 0;
            Func_08009020(drawable, Func_080022fc(i, 3));
            drawable = M2C_FIELD(work, void **, handle_off);
            M2C_FIELD(drawable, u8 *, 9) =
                (u8)((M2C_FIELD(drawable, u8 *, 9) & ~0xC) | 4);
        }
        handle_off += 4;
    }

    gfx = (u8 *)0x03001E50;
    Func_080ed408(46, 7, 7, 3, 2);
    rectangle[0] = *(void **)(gfx + 184);
    Func_080ed408(47, 7, 7, 3, 3);
    drawable = *(void **)(gfx + 188);
    rectangle_slot = rectangle;
    rectangle_slot[1] = drawable;

    io = (u16 *)0x04000048;
    *io = 0x2737;
    io -= 4;
    *io = 0xF0;
    io += 3;
    *io = 0x1088;
    Func_080030f8(1);
    Func_080b5040(1, (s32)&Value_0000003c, 0);
    Func_080cd104(1, 1);
    Func_080e0524((s32)&Value_00000073, sprite_sheet, 0, 0);
    Func_080e0524((s32)&Value_000000c0, work, 1, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3F44;
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;

    org.bg_x = 0xBC0000;
    org.bg_y = 0x5C0000;
    org.y = 0x5C0000;
    org.x = 0xA00000;

    /* Six falling objects, stacked one screen apart above the view. */
    entry = (SceneParticle *)((u8 *)work + 0x7080);
    n = 0;
    for (i = 0; i != 6; i++) {
        entry->x = (Func_08004458() & 0x7F) << 16;
        entry->y = n;
        entry->vx = 0;
        entry->vy = 0;
        entry->timer = 0;
        n += (s32)0xFFF00000;
        entry++;
    }

    entry = (SceneParticle *)((u8 *)work + 0x7128);
    for (i = 0; i != 58; i++) {
        entry->timer = 24;
        entry++;
    }

    entry = (SceneParticle *)0x02010000;
    for (i = 0; i != 1024; i++) {
        entry->timer = -1;
        entry++;
    }

    M2C_FIELD(work, s32 *, 0x77B4) = 24;
    M2C_FIELD(work, s32 *, 0x77B8) = 0;

    frame = 0;
    if ((*(s32 *)0x03001B04 & 3) == 0) {
        pos_ptr = pos;
        clip_ptr = clip;
        do {
            if (frame == 94) {
                Func_080f9010(156);
            }
            if (frame == 136) {
                Func_080f9010(156);
            }
            if (frame == 178) {
                Func_080f9010(156);
            }
            if (frame == 260) {
                Func_080f9010(145);
            }
            clip[0] = Data_080edac8[0];
            clip[1] = Data_080edac8[1];
            if ((u32)(frame - 96) <= 155U) {
                M2C_FIELD(work, s32 *, 0x77A8) = 1;
            } else if ((u32)(frame - 260) <= 3U) {
                M2C_FIELD(work, s32 *, 0x77A8) = 1;
            }

            /* Seven fixed background objects from the byte-offset tables. */
            pos[3] = 0;
            pos[1] = 0;
            handle = (s32 *)((u8 *)work + 0x77D8);
            for (i = 0; i != 7; i++) {
                pos_ptr[0] =
                    (Data_080eeed8[i] << 16) + org.bg_x + (s32)0xFFE00000;
                pos_ptr[2] =
                    (Data_080eeee1[i] << 16) + org.bg_y + (s32)0xFFE00000;
                Func_08009008(*handle++, pos_ptr, clip_ptr, 0);
            }

            /* Sweep the two lead objects around a circle. */
            if (frame <= 90) {
                angle = frame << 9;
                org.x = (Func_08002322(angle) << 4) + 0x9C0000;
                org.y = (Func_0800231c(angle) << 4) + 0x5C0000;
            }

            if (frame <= 196) {
                group = (u8 *)work;
                base = 91;
                for (i = 0; i != 3; i++) {
                    if (frame >= base && frame < base + 4) {
                        org.y += 0x80000;
                    }
                    if (frame == base + 3) {
                        spark = (SceneParticle *)(group + 0x7128);
                        for (n = 0; n != 4; n++) {
                            spark->x = 0x400000;
                            spark->y = 0x600000;
                            spark->vx =
                                ((Func_08004458() & 0xFF) - 127) << 10;
                            spark->vy =
                                ((Func_08004458() & 0xFF) - 127) << 10;
                            spark->timer = Func_08004458() & 15;
                            spark++;
                        }
                    }
                    if (frame >= base + 20 && frame < base + 36) {
                        org.y += (s32)0xFFFE0000;
                    }
                    base += 40;
                    group += 0xE0;
                }
            }

            if ((u32)(frame - 244) <= 7U) {
                org.x += (s32)0xFFFF0000;
            }
            if ((u32)(frame - 252) <= 23U) {
                org.x -= (frame - 250) << 16;
            }

            if (frame <= 0x103) {
                pos[1] = (s32)0xFF000000;
                pos[2] = org.y + (s32)0xFF000000;
                pos[0] = org.x;
                Func_08009008(
                    M2C_FIELD(work, s32 *, 0x77F4), pos, clip_ptr, 0);
                pos[0] = org.x + 0x200000;
                Func_08009008(
                    M2C_FIELD(work, s32 *, 0x77F8), pos, clip_ptr, 0);
            }

            /* Advance and redraw the six falling objects. */
            pos_ptr[1] = 0;
            entry = (SceneParticle *)((u8 *)work + 0x7080);
            spawn_base = (u8 *)work;
            for (i = 0; i != 6; i++) {
                if (entry->timer != 2) {
                    pos_ptr[0] = entry->x;
                    pos_ptr[2] = entry->y;
                    Func_08009008(
                        *(s32 *)((u8 *)work + (i * 4) + 0x77FC),
                        pos_ptr, clip_ptr, 0);
                    entry->x += entry->vx;
                    vy = entry->vy;
                    entry->y += vy;
                    if (frame > 96) {
                        entry->vy = vy + 0x4000;
                    }
                    if (entry->y > 0x780000) {
                        entry->timer += 1;
                        if (entry->timer == 1) {
                            /* First landing: bounce and throw two sparks. */
                            entry->vy = -entry->vy / 2;
                            spark = (SceneParticle *)(spawn_base + 0x73C8);
                            for (n = 0; n != 2; n++) {
                                spark->x = entry->x / 2;
                                spark->y = entry->y + (s32)0xFFE00000;
                                spark->vx =
                                    ((Func_08004458() & 0xFF) - 127) << 10;
                                spark->vy =
                                    ((Func_08004458() & 0xFF) - 127) << 10;
                                spark->timer = Func_08004458() & 15;
                                spark++;
                            }
                        } else if (frame <= 199) {
                            entry->y = 0;
                            entry->vy = 0;
                            entry->timer = 0;
                        }
                    }
                }
                entry++;
                spawn_base += 0x38;
            }

            /* Age and redraw the 58-entry sprite array. */
            entry = (SceneParticle *)((u8 *)work + 0x7128);
            for (i = 0; i != 56; i++) {
                life = entry->timer;
                if (life >= 0) {
                    if ((u32)life <= 23U) {
                        idx = Func_080022ec(life, 6) + 3;
                        offset = Data_080eeeea[idx];
                        size = Data_080eeef8[idx];
                        ((DrawRectangleFn)rectangle[0])(
                            canvas,
                            (u8 *)work + offset,
                            M2C_FIELD(entry, s16 *, 2) - (size >> 1),
                            M2C_FIELD(entry, s16 *, 6) - (size >> 1),
                            size, size);
                    }
                    Func_080e3908(entry, 60, (s32)0xFFFFC000);
                    entry->timer += 1;
                }
                entry++;
            }

            if (frame == 260) {
                n = 0;
                count = M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 0x14);
                if (count != 0) {
                    off = 36;
                    do {
                        Func_080b5088(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, off),
                            4);
                        Func_080d6888(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, off),
                            7, -1, n, 8);
                        n++;
                        off += 2;
                    } while (n != M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x14));
                }
                M2C_FIELD(work, s32 *, 0x77A8) = 8;
                if (frame == 260) {
                    entry = (SceneParticle *)0x02010000;
                    for (n = 0; n != 512; n++) {
                        speed = Func_08004458() & 0x3FF;
                        angle = Func_08004458() & 0xFFFF;
                        entry->x = 0x200000;
                        entry->y = 0x5C0000;
                        speed += 32;
                        entry->vx = (speed * Func_08002322(angle)) >> 7;
                        entry->vy =
                            -((speed * Func_0800231c(angle)) * 2) >> 7;
                        entry->timer = (Func_08004458() & 15) + 32;
                        entry++;
                    }
                }
            }

            /* Age and redraw the shared particle pool. */
            entry = (SceneParticle *)0x02010000;
            for (i = 0; i != 512; i++) {
                life = entry->timer;
                if (life >= 0) {
                    half = (life >> 3) + 1;
                    full = half * 2;
                    ((DrawRectangleFn)rectangle_slot[i & 1])(
                        canvas,
                        (u8 *)sprite_sheet + Data_080ede48[half - 1],
                        M2C_FIELD(entry, s16 *, 2) - (half / 2),
                        M2C_FIELD(entry, s16 *, 6) - half,
                        half, full);
                    Func_080e3908(entry, 62, 0x1000);
                    entry->timer -= 1;
                }
                entry++;
            }

            Func_080e155c(8, 8);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != 320 && (*(s32 *)0x03001B04 & 3) == 0);
    }

    Func_080b50e8(0x86);
    Func_080d67dc();
    handle = (s32 *)((u8 *)work + 0x77D8);
    for (i = 0; i != 15; i++) {
        Func_08009038(*handle++);
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
