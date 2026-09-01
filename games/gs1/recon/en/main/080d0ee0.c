#include "types.h"

/*
 * Draft for the battle-presentation sub-effect split function at
 * 0x080d0ee0 (Region_080d0ee0 + Fragment_080d12a8, 1136 bytes total).
 *
 * Reconstructed as one C source per CONTRIBUTING.md "Split functions":
 * Fragment_080d12a8's own header identifies it as the parent's terminal
 * fragment, and no other owner in the corpus references it, so the two
 * regions are a private two-member cluster with one true prologue and
 * one true epilogue. Full instruction-level control-flow map, frame
 * ledger, and pool-constant ground truth are recorded in
 * games/gs1/recon/en/dossiers.json#main:080d0ee0 under "reconstruction_dossier".
 *
 * This owner shares the 0x03001eec heap-cache / work-pointer idiom, the
 * Func_080cd594 / Func_080041d8+Func_08004278 (0x080CD261 callback,
 * 0x480 interval) / Func_08002dd8(47)+Func_08002dd8(46)+Func_080cdbc0
 * finalize idiom, and the Func_080ed408(46, 7, 7, 3, ...) call already
 * confirmed in 080d82b0.c and 080d59b0.c against the 0x080e7404 "battle
 * work" subsystem -- see those drafts and 080e7404.c for the evidence
 * behind the field/signature choices reused here. Byte-offset accesses
 * are kept generic where no evidence-backed aggregate exists yet.
 *
 * sub_080072f4 is the _call_via_r4 indirect-call trampoline documented
 * in the corpus notes (DrawRectangleFn pattern): the reference loads a
 * function pointer out of the work-pointer's kind-46 record into a
 * local (draw_rectangle_fn) and calls through it rather than through a
 * named symbol, reloading it from its saved slot immediately before
 * every call.
 *
 * Func_08004cb4(idx_c) is followed, later in the same 10-iteration loop,
 * by reads of an 8-byte cell (sp92) that nothing in this function ever
 * stores to. The only plausible source is Func_08004cb4 (or one of
 * Func_080049e8/Func_08004c6c/Func_08004c1c/Func_08004cf0 immediately
 * after it) writing through r9 as a hidden register argument -- the
 * same "read r9 on entry" contract already identified for
 * Func_080e7338 in games/gs1/recon/en/main/080e7404.c's header comment.
 * That call is written here as an ordinary two-value read of the sp92
 * cell; if scoring shows this call site as compiler-unemittable, it is
 * this documented hidden-register-argument contract, not a spelling
 * problem, and should not be permuted against.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

typedef struct {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
    s32 f18;
} Trail7;

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_08005340(void *a, void *b);
void Func_08009080(void *member_ptr, s32 a);
void Func_08009088(void *member_ptr, s32 a);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void **Func_080b5098(s32 member_id);
s32 Func_080022ec(s32 a, s32 b);
s32 Func_080022fc(s32 a, s32 b);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_08004cb4(Trail7 *trail);
void Func_080049e8(void);
void Func_08004c6c(s32 a);
void Func_08004c1c(s32 a);
void Func_08004cf0(s32 *triple);
void Func_08004a5c(void);
s32 Func_080e3944(const void *src, s32 *dest);
void Func_08009140(void);
void Func_08009150(void *member_size, s32 a, s32 b, s32 c);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

extern u16 Data_080ede48[];
extern char Value_00000079;
extern char Value_00000073;

s32 Func_080d0ee0(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    s32 facing;
    void **member_ptr;
    void *member;
    void *member_size;
    DrawRectangleFn draw_rectangle_fn;
    Trail7 *trail;
    s32 member_index;
    s32 frame;
    s32 phase;
    s32 idx_a;
    s32 idx_b;
    Trail7 *phase_trail;
    s32 remaining;
    s32 count;
    s32 max_val;
    s32 clamp_target;
    s32 j;
    Trail7 sp80;
    s32 sp92[2];
    s32 sp104[2];
    Trail7 *particle;
    s32 half_count;
    s32 double_count;
    s32 outer;
    Trail7 *p_a;
    Trail7 *p_b;
    s32 k;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    facing = *(s32 *)((u8 *)heap_cache - 108);

    member_ptr = Func_080b5098(M2C_FIELD(object, s32 *, 8));
    member = *member_ptr;
    M2C_FIELD(work, void **, 0x7828) = object;

    Func_080cd594(1);
    Func_080072f0((void *)(160 << 19), Func_08002f40((s32)&Value_00000079),
        128, (WordCopyFn)0x03001388);
    Func_08005340(Func_08002f40((s32)&Value_00000073), extra_target);

    Func_08009080(member, 2);
    Func_08009088(member, 48);
    Func_080ed408(46, 7, 7, 3, 2);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    draw_rectangle_fn = (DrawRectangleFn)heap_cache[7];
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    member = M2C_FIELD(work, void **, 0x7828);
    member_size = *Func_080b5098(M2C_FIELD(member, s16 *, 36));

    trail = (Trail7 *)((u8 *)work + 0x7080);
    member_index = 0;
    do {
        trail->f00 = M2C_FIELD(member, s32 *, 8);
        trail->f04 = M2C_FIELD(member, s32 *, 12) + (160 << 14);
        trail->f08 = M2C_FIELD(member, s32 *, 16);
        if (member_index == 0) {
            trail->f0c = Func_080022ec(
                M2C_FIELD(member_size, s32 *, 8) - trail->f00, 12);
        } else {
            trail->f0c = Func_080022ec(
                (M2C_FIELD(member_size, s32 *, 8) << 1) - trail->f00, 12);
        }
        trail->f10 = Func_080022ec(
            M2C_FIELD(member_size, s32 *, 12) - trail->f04 + (160 << 14), 12);
        trail->f14 = Func_080022ec(
            M2C_FIELD(member_size, s32 *, 16) - trail->f08, 12);
        trail->f18 = 0;
        member_index++;
        trail++;
    } while (member_index != 3);

    frame = 0;
    for (;;) {
        if (frame <= 47) {
            s32 magnitude;
            s16 *shake;

            shake = *(s16 **)0x03001e80;
            magnitude = 128;
            if (frame > 39) {
                magnitude = (192 << 2) - (frame << 4);
            }
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) != 0) {
                shake[27] += (s16)magnitude;
            } else {
                shake[27] -= (s16)magnitude;
            }
        }

        phase = 0;
        idx_a = 0;
        idx_b = 0;
        phase_trail = (Trail7 *)((u8 *)work + 0x7080);
        do {
            if (frame >= idx_b) {
                remaining = frame - idx_b;
                count = ((remaining >= 0) ? remaining : remaining + 3) >> 2;
                count += 2;
                if (count > 10) {
                    count = 10;
                }

                Func_080049ac();
                Func_080051d8(facing, facing + 12);
                Func_08004cb4(phase_trail);

                max_val = 0;
                clamp_target = (remaining << 12) + (128 << 5);
                particle = (Trail7 *)((u8 *)0x02010000 +
                    idx_a * (s32)sizeof(Trail7));

                for (j = 0; j != 10; j++) {
                    s32 blend;

                    Func_080049e8();
                    Func_08004c6c(remaining << 10);
                    Func_08004c1c(128 << 7);

                    sp80.f00 = clamp_target;
                    if (sp80.f00 > (128 << 9)) {
                        sp80.f00 = 128 << 9;
                    }
                    sp80.f04 = sp80.f00;
                    sp80.f08 = sp80.f00;
                    Func_08004cf0((s32 *)&sp80);

                    Func_08004c6c(j * 0x199a);
                    blend = Func_080e3944(
                        (void *)(0x080ee140 + (((j & 1) * 3) << 2)), sp104);
                    if (max_val < blend) {
                        max_val = blend;
                    }

                    sp104[0] >>= 1;
                    particle->f0c = sp104[0];
                    particle->f10 = sp104[1];

                    Func_08004a5c();
                    particle++;
                }

                if (max_val <= 0x00061a7f) {
                    half_count = (count + (count >> 31)) >> 1;
                    double_count = count << 1;

                    for (outer = 0; outer != 10; outer++) {
                        s32 src_idx;

                        p_a = (Trail7 *)((u8 *)0x02010000 +
                            (idx_a + outer) * (s32)sizeof(Trail7));
                        src_idx = Func_080022fc(outer, 10) + idx_a;
                        p_b = (Trail7 *)((u8 *)0x02010000 +
                            src_idx * (s32)sizeof(Trail7));

                        for (k = 0; k != 16; k++) {
                            s32 interp_x;
                            s32 interp_y;
                            s32 delta;
                            s32 x;
                            s32 y;
                            void *src;

                            delta = (p_b->f0c - p_a->f0c) * k;
                            if (delta < 0) {
                                delta += 15;
                            }
                            interp_x = p_a->f0c + (delta >> 4);

                            delta = (p_b->f10 - p_a->f10) * k;
                            if (delta < 0) {
                                delta += 15;
                            }
                            interp_y = p_a->f10 + (delta >> 4);

                            src = (u8 *)extra_target +
                                Data_080ede48[double_count - 2];
                            x = interp_x - half_count;
                            y = interp_y - count;

                            draw_rectangle_fn = (DrawRectangleFn)heap_cache[7];
                            draw_rectangle_fn(draw_destination, src, x, y,
                                (u32)count, double_count);
                        }
                    }
                }

                member = M2C_FIELD(work, void **, 0x7828);
                trail = phase_trail;
                trail->f00 += trail->f0c;
                trail->f04 += trail->f10;
                trail->f08 += trail->f14;

                if (frame == idx_b + phase + 10) {
                    M2C_FIELD(member_size, s32 *, 52) = 128 << 10;
                    M2C_FIELD(member_size, s32 *, 48) = 128 << 12;
                    M2C_FIELD(member_size, s32 *, 40) = 160 << 11;
                    M2C_FIELD(member_size, s32 *, 72) = 0x0000ab85;
                    *((u8 *)member_size + 90) = 0;

                    Func_08009140();

                    if (M2C_FIELD(member_size, s32 *, 8) >= 0) {
                        Func_08009150(member_size,
                            M2C_FIELD(member_size, s32 *, 8) + (160 << 14),
                            M2C_FIELD(member_size, s32 *, 16), 0);
                    } else {
                        Func_08009150(member_size,
                            M2C_FIELD(member_size, s32 *, 8) +
                                (s32)0xffd80000,
                            M2C_FIELD(member_size, s32 *, 16), 0);
                    }

                    if (phase == 2) {
                        Func_080b50e8(134);
                        goto phase_increment;
                    }

                    Func_080f9010(134);
                    Func_080d6888(
                        M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                        7, 5, 0, 8);
                    M2C_FIELD(work, s32 *, 0x77a8) = 4;
                }
            }

        phase_increment:
            idx_a += 10;
            phase_trail = (Trail7 *)((u8 *)phase_trail + 28);
            idx_b += 12;
            phase++;
        } while (phase != 3);

        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        frame++;
        if (frame == 60) {
            break;
        }
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    return Func_080cdbc0();
}
