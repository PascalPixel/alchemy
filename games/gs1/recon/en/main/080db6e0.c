#include "types.h"

/*
 * Battle/overlay particle-field effect owner at 0x080db6e0.  The retained
 * assembly already names the global entry point RunParticleFieldEffect, so
 * that name is kept in games/gs1/source-paths.json, but the working symbol
 * here is Func_080db6e0 so the family-transplant/candidate-show tooling can
 * find it by address like every other in-progress owner.
 *
 * Structural template: games/gs1/src/battle/effects/member_orbit/run.c
 * (owner 080ce85c, already exact) establishes the 0x03001eec heap_cache /
 * work-pointer prologue, the M2C_FIELD(work, 0x7828) object-pointer field,
 * the Value_ pool-symbol convention for small link-time byte constants, and
 * the DrawRectangleFn / WordCopyFn trampoline call shapes.  This owner is a
 * different, size-mismatched sibling (1092 bytes vs the template's own
 * size): it drives no member-orbit sprites and no per-frame sine sweep at
 * all.  Instead it runs two independently-seeded particle arrays (32
 * entries at work+0x7080, 1024 entries at the fixed EWRAM buffer
 * 0x02010000, both stride 0x1C) through a per-frame while loop whose
 * iteration count, and whose two active-particle counts, come from a
 * 3-bytes-per-row lookup table at 0x080eeae2 indexed by a runtime "mode"
 * selector (mode 1 is forced by the palette variant; other variants read
 * the mode from the caller's object at offset 24).  games/gs1/recon/en/
 * main/080e7404.c is the fuller sibling draft in this same 0x03001eec
 * family: its 128- and 512- record particle initializers at 0x02010000 /
 * 0x02010e00 (masked-RNG field 0xC/0x10/0x14 triples) are the same
 * structural shape as this owner's two initializer loops, and its
 * Func_080e38b8 / Func_080e0524 / Data_03001e50 / Data_080ede48 usages are
 * reused here verbatim.  Following that draft's own established style,
 * every temporary below is declared flat at the top of the function
 * (never in a nested block) so the compiler's size-class frame allocator
 * lays the stack out the same way the reference does.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kind 46 only (its single rectangle-blit routine). */
extern void *Data_03001e50[];

/* Ten halfword cell offsets indexed by a clamped depth bucket 0..9; the
   same symbol games/gs1/recon/en/main/080e7404.c already declares and
   indexes the same way. */
extern u16 Data_080ede48[];

/* Per-mode 3-byte row: [far_active_count, near_active_count, total_frames],
   selected by the mode/variant value this owner computes into `mode`. */
extern u8 Data_080eeae2[];

/* Sprite-offset / size halfword pair tables, indexed together by the same
   Func_080022ec selection while a near-field particle is still growing. */
extern u16 Data_080eeaec[];
extern u16 Data_080eeafa[];

extern u8 Value_000000c0;
extern u8 Value_00000096;

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *work, s32 a, s32 b);
void *Func_08002f40(s32 id);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080e3944(void *source, void *screen);
void Func_080e38b8(void *record, s32 a, s32 b);
s32 Func_08004458(void);
s32 Func_080022ec(s32 a, s32 b);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 mode);
void Func_080030f8(s32 frames);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);

void Func_080db6e0(void *object, s32 variant)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    s32 mode;
    void *draw_rectangle;
    s32 frame;
    s32 base;
    s32 far_idx;
    s32 near_idx;
    s32 total_idx;
    s32 facing;
    s32 i;
    s16 *pal;
    s32 gray;
    void *entry;
    void *particle;
    s32 screen[3];
    s32 depth;
    s32 index;
    s32 size;
    s32 x;
    s32 age;
    s32 which;
    s32 sprite_off;
    s32 member_count;
    s32 member_id_offset;
    s16 member_id;
    u32 half;
    u8 *table;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);
    Func_080e0524((s32)&Value_000000c0, work, 1, 0);
    if (variant == 1) {
        i = 0;
        pal = (s16 *)0x05000000;
        do {
            gray = i / 2;
            *pal = (s16) ((gray << 10) | (gray << 5) | gray);
            i++;
            pal++;
        } while (i != 64);
        mode = 1;
    } else {
        palette = Func_08002f40((s32)&Value_00000096);
        status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
        mode = M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 24);
    }
    entry = (u8 *)work + 0x7080;
    i = 0;
    do {
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
            status = 200 << 14;
        } else {
            status = -(200 << 14);
        }
        M2C_FIELD(entry, s32 *, 0) = status;
        M2C_FIELD(entry, s32 *, 4) = 0;
        M2C_FIELD(entry, s32 *, 8) = 0;
        M2C_FIELD(entry, s32 *, 12) =
            (s32) (((Func_08004458() & 63) - 32) << 13);
        M2C_FIELD(entry, s32 *, 16) =
            (s32) (((Func_08004458() & 63) + 16) << 12);
        M2C_FIELD(entry, s32 *, 20) =
            (s32) (((Func_08004458() & 63) - 32) << 13);
        i++;
        M2C_FIELD(entry, s32 *, 24) = 0;
        entry = (u8 *)entry + 28;
    } while (i != 32);
    entry = (void *)0x02010000;
    i = 0;
    do {
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
            status = 200 << 14;
        } else {
            status = -(200 << 14);
        }
        M2C_FIELD(entry, s32 *, 0) = status;
        M2C_FIELD(entry, s32 *, 4) = 0;
        M2C_FIELD(entry, s32 *, 8) = 0;
        M2C_FIELD(entry, s32 *, 12) =
            (s32) (((Func_08004458() & 63) - 32) << 13);
        M2C_FIELD(entry, s32 *, 16) =
            (s32) (((Func_08004458() & 31) + 8) << 13);
        M2C_FIELD(entry, s32 *, 20) =
            (s32) (((Func_08004458() & 63) - 32) << 13);
        i++;
        M2C_FIELD(entry, s32 *, 24) = 0;
        entry = (u8 *)entry + 28;
    } while (i != 1024);
    status = Func_080ed408(46, 7, 7, 3, 2);
    draw_rectangle = Data_03001e50[46];
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    base = mode * 2;
    far_idx = base + mode;
    near_idx = far_idx + 1;
    total_idx = far_idx + 2;
    table = Data_080eeae2;
    frame = 0;
    while (frame != table[total_idx]) {
        facing = *(s32 *)0x03001E80;
        Func_080049ac();
        Func_080051d8(facing, facing + 12);
        if (frame == 2) {
            Func_080f9010(144);
        }
        if (frame == Data_080eeae2[base + mode + 2] - 48) {
            Func_080b50e8(133);
        }
        if (table[far_idx] != 0) {
            particle = (void *)0x02010000;
            i = 0;
            do {
                if (M2C_FIELD(particle, s32 *, 4) >= 0) {
                    Func_080e3944(particle, screen);
                    screen[0] = screen[0] >> 1;
                    screen[0] = screen[0]
                        + (M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                            s32 *, 4) << 5)
                        - 16;
                    if (screen[2] <= 159) {
                        screen[2] = 160;
                    }
                    if (screen[2] > 0x31F) {
                        screen[2] = 0x31F;
                    }
                    depth = screen[2] - 160;
                    if (depth < 0) {
                        depth += 63;
                    }
                    depth >>= 6;
                    index = 9 - depth;
                    size = index * 2;
                    ((DrawRectangleFn)draw_rectangle)(
                        canvas,
                        (u8 *)work + Data_080ede48[index - 1]
                            + ((i & 1) * 0x302) + 0x3200,
                        screen[0] - (index / 2), screen[1] - index,
                        index, size);
                    Func_080e38b8(particle, 64, 0xFFFFE000);
                }
                i++;
                particle = (u8 *)particle + 28;
            } while (i != table[far_idx]);
        }
        if (frame > 2 && table[near_idx] != 0) {
            particle = (u8 *)work + 0x7080;
            i = 0;
            while (i < frame) {
                if (M2C_FIELD(particle, s32 *, 4) >= 0) {
                    Func_080e3944(particle, screen);
                    x = screen[0] >> 1;
                    x = x
                        + (M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                            s32 *, 4) << 5)
                        - 16;
                    age = M2C_FIELD(particle, s32 *, 24);
                    if (age <= 20) {
                        which = Func_080022ec(age, 3);
                        sprite_off = Data_080eeaec[which];
                        size = Data_080eeafa[which];
                        half = (u32) size >> 1;
                        ((DrawRectangleFn)draw_rectangle)(canvas,
                            (u8 *)work + sprite_off,
                            x - (s32) half,
                            screen[1] - (s32) half,
                            size, size);
                    }
                    if (age <= 20) {
                        M2C_FIELD(particle, s32 *, 24) = age + 1;
                    }
                    Func_080e38b8(particle, 64, 0xFFFFE000);
                }
                i++;
                particle = (u8 *)particle + 28;
            }
        }
        if (variant == 0) {
            member_count =
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20);
            if (member_count != 0) {
                i = 0;
                member_id_offset = 36;
                do {
                    if (frame == i + 6) {
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        Func_080d6888(member_id, 7, 5, i, 10);
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        Func_080b5088(member_id, 2);
                    }
                    i++;
                    member_id_offset += 2;
                    member_count = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 20);
                } while (i != member_count);
            }
        } else {
            member_count =
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20);
            if (member_count != 0) {
                i = 0;
                member_id_offset = 36;
                do {
                    if (frame == i + 6) {
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        Func_080d6888(member_id, 7, 5, i, 10);
                    }
                    i++;
                    member_id_offset += 2;
                    member_count = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 20);
                } while (i != member_count);
            }
        }
        if (frame == 2) {
            M2C_FIELD(work, s32 *, 0x77A8) = 6;
        }
        Func_080e155c(16, 16);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
