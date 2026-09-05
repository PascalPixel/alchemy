#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080ceb54.
 *
 * Assigned from the member_orbit/run.c compiler-family cluster
 * (main:080ce85c, score 8078); this owner's real callee set and constants
 * match the 0x03001eec "battle work" subsystem already partly recovered at
 * games/gs1/recon/en/main/080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and
 * 080e01e4.c -- not the assigned template, which is a distinct sub-effect
 * (orbiting rectangles vs this owner's per-member particle burst with a
 * homing convergence step).
 *
 * Unlike its siblings this owner takes a second parameter (called `variant`
 * below) that selects among three resource ids (Value_00000069's copy target
 * uses a fixed id, but the big VRAM blit source is chosen 0xBB/0x8D/0x91 by
 * variant) and indexes a two-byte-per-variant table at 0x080ee090
 * (Data_080ee090) for the outer frame-loop length and the per-frame burst
 * count. Per Func_080cef64's already-recovered body
 * (games/gs1/src/battle/effects/fetch_rectangle_blitters.c) the function
 * unconditionally copies the kind-46/47 allocator-cache slot pair; this
 * owner never calls Func_080ed408 itself, so it is reading back whatever
 * kind-46/47 slots a prior effect already populated.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_080cef64(s32 flag, DrawRectangleFn *out_pair);
void **Func_080b5098(s32 member_id);
s32 Func_080b5070(s32 member_id);
u32 Func_08004458(void);
void Func_080041d8(void *callback, s32 interval);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
s32 Func_08002322(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e3944(void *src, void *dest);
s32 Func_080022fc(s32 a, s32 b);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

extern u8 Value_00000069;
extern u8 Value_000000bb;
extern u8 Value_0000008d;
extern u8 Value_00000091;
extern const u8 Data_080ee090[];

s32 Func_080ceb54(void *object, s32 variant)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    s32 facing;
    s32 palette_id;
    DrawRectangleFn callback_pair[2];
    s32 *pool_cursor;
    s32 pool_index;
    s32 loop_start;
    s32 outer;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);

    Func_08005340(Func_08002f40((s32)&Value_00000069), work);

    if (variant == 0) {
        palette_id = (s32)&Value_000000bb;
    } else if (variant == 1) {
        palette_id = (s32)&Value_0000008d;
    } else {
        palette_id = (s32)&Value_00000091;
    }
    Func_080072f0(
        (void *)(160 << 19), Func_08002f40(palette_id), 128,
        (WordCopyFn)0x03001388);

    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4),
        callback_pair);

    pool_cursor = (s32 *)0x02010018;
    pool_index = 0;
    do {
        pool_index++;
        *pool_cursor = -1;
        pool_cursor += 7;
    } while (pool_index != 1024);

    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
        s32 spawn_index;
        s32 member_offset;

        spawn_index = 0;
        member_offset = 0;
        do {
            void *state;
            void *member_ptr;
            u32 handle;
            s32 *particle;
            s32 i;

            state = M2C_FIELD(work, void **, 0x7828);
            member_ptr = *Func_080b5098(M2C_FIELD(state, s32 *, 8));
            state = M2C_FIELD(work, void **, 0x7828);
            handle = Func_080b5070(M2C_FIELD(state, s32 *, 8));

            particle = (s32 *)((u8 *)0x02010000 + member_offset);
            for (i = 0; i != 128; i++) {
                M2C_FIELD(particle, s32 *, 0) = M2C_FIELD(member_ptr, s32 *, 8);
                M2C_FIELD(particle, u32 *, 4) = handle;
                M2C_FIELD(particle, s32 *, 8) = M2C_FIELD(member_ptr, s32 *, 16);
                M2C_FIELD(particle, s32 *, 12) =
                    (s32) ((Func_08004458() & 0xFF) - 128) << 10;
                M2C_FIELD(particle, s32 *, 16) =
                    (s32) ((Func_08004458() & 0xFF) - 128) << 10;
                M2C_FIELD(particle, s32 *, 20) =
                    (s32) ((Func_08004458() & 0xFF) - 128) << 10;
                M2C_FIELD(particle, s32 *, 24) = 0;
                particle = (s32 *)((u8 *)particle + 28);
            }

            spawn_index++;
            member_offset += 0xE00;
        } while (spawn_index
            != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
    }

    Func_080041d8((void *)0x080DBB9D, 0x480);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080f9010(146);

    loop_start = variant * 2;
    if (Data_080ee090[loop_start + 1]
            + M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) * 20
            != 0) {
        s32 facing_b;

        facing_b = facing + 12;
        outer = 0;
        do {
            s32 *scanline;
            s32 angle;
            s32 i;

            if (outer == 80) {
                if (variant == 0) {
                    Func_080b50e8(134);
                } else {
                    Func_080b50e8(133);
                }
            }
            Func_080049ac();
            Func_080051d8(facing, facing_b);

            scanline = (s32 *)((u8 *)work + 0x6980);
            angle = outer << 10;
            for (i = 0; i != 160; i++) {
                *scanline++ = (0x100000 - (Func_08002322(angle) << 4)) >> 10;
                angle += 1024;
            }

            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                s32 member;
                s32 member_id_offset;
                s32 burst_stagger;
                s32 member_offset;
                s32 fp;

                member = 0;
                member_id_offset = 36;
                burst_stagger = 0;
                member_offset = 0;
                fp = 0;
                do {
                    void *state;
                    s32 member_id;
                    void *member_ptr;
                    s32 growth;

                    state = M2C_FIELD(work, void **, 0x7828);
                    member_id = M2C_FIELD(state, s16 *, member_id_offset);
                    member_ptr = *Func_080b5098(member_id);
                    state = M2C_FIELD(work, void **, 0x7828);
                    member_id = M2C_FIELD(state, s16 *, member_id_offset);
                    growth = Func_080b5070(member_id) / 2;

                    if (outer == fp + 71) {
                        if (variant == 0) {
                            Func_080f9010(134);
                        } else {
                            Func_080f9010(133);
                        }
                    }
                    if (outer == fp + 70) {
                        state = M2C_FIELD(work, void **, 0x7828);
                        member_id = M2C_FIELD(state, s16 *, member_id_offset);
                        Func_080d6888(member_id, 7, 5, member, 26);
                    }

                    if (outer > fp) {
                        s32 burst_count;

                        burst_count = Data_080ee090[loop_start];
                        if (burst_count != 0) {
                            s32 k;
                            s32 base_stagger;
                            s32 base_fp;
                            s32 *particle;

                            base_stagger = burst_stagger;
                            base_fp = fp;
                            particle = (s32 *)((u8 *)0x02010000 + member_offset);
                            for (k = 0; k != burst_count; k++) {
                                if (outer > (base_stagger * 2 + k) * 2
                                        && M2C_FIELD(particle, s32 *, 24) >= 0) {
                                    s32 screen[2];
                                    s32 x;

                                    Func_080e3944(particle, screen);
                                    screen[0] = screen[0] >> 1;
                                    x = screen[0] - 10;
                                    ((DrawRectangleFn)callback_pair[0])(
                                        draw_destination,
                                        (u8 *)work
                                            + (Func_080022fc(k, 3) * 5 << 7),
                                        x, screen[1] - 16, 20, 32);
                                    Func_080e38b8(particle, 62, 0);
                                    if (outer > base_fp + k + 30) {
                                        s32 dx;
                                        s32 dy;
                                        s32 dz;

                                        dx = (M2C_FIELD(member_ptr, s32 *, 8)
                                            - M2C_FIELD(particle, s32 *, 0)) >> 9;
                                        M2C_FIELD(particle, s32 *, 12) =
                                            M2C_FIELD(particle, s32 *, 12) + dx;
                                        dy = ((M2C_FIELD(member_ptr, s32 *, 12)
                                            + growth)
                                            - M2C_FIELD(particle, s32 *, 4)) >> 9;
                                        M2C_FIELD(particle, s32 *, 16) =
                                            M2C_FIELD(particle, s32 *, 16) + dy;
                                        dz = (M2C_FIELD(member_ptr, s32 *, 16)
                                            - M2C_FIELD(particle, s32 *, 8)) >> 9;
                                        M2C_FIELD(particle, s32 *, 20) =
                                            M2C_FIELD(particle, s32 *, 20) + dz;
                                        if ((u32)(dx + 0xFFF) <= 0x1FFE
                                                && (u32)(dz + 0xFFF) <= 0x1FFE) {
                                            M2C_FIELD(particle, s32 *, 24) = -1;
                                        }
                                    }
                                }
                                particle = (s32 *)((u8 *)particle + 28);
                            }
                        }
                    }

                    member_id_offset += 2;
                    member_offset += 0xE00;
                    member++;
                    burst_stagger += 5;
                    fp += 20;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }

            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            outer++;
        } while (outer != Data_080ee090[loop_start + 1]
            + M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) * 20);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((void *)0x080DBB9D);
    return Func_080cdbc0();
}
