#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080dc6bc, transplant-assigned from the
 * member_orbit compiler family (template-main-080ce85c, score 8895/10000,
 * games/gs1/src/battle/effects/member_orbit/run.c).  The prologue (cursor-
 * advance work/draw_destination read, work + 0x7828 state publish,
 * Func_080cd594(0)) and epilogue (Func_08004278/Func_08002dd8 x2/
 * Func_080cdbc0) match that template's shape, but the body is a different
 * effect from the same 0x03001eec "battle work" subsystem documented at
 * games/gs1/recon/en/main/080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c,
 * 080e01e4.c and the already-adopted games/gs1/src/battle/effects/
 * puff_arc/run.c: it seeds a 64-entry, 28-byte particle table at
 * work + 0x7080 (the same record layout and work-relative base address as
 * puff_arc's Puff array) with a random polar offset per particle (masked
 * 16-bit angle, masked 8-bit magnitude; a modulo-6 "element" of 5 gets a
 * zeroed offset instead), then runs 96 frames.  Each frame walks the active
 * effect state's member list (games/gs1/asm/080dc6bc.s reads the same
 * count-at-+20 / id-array-at-+36 layout as puff_arc's Efx struct), replaying
 * the shared facing/portrait setup (Func_080049ac/Func_080051d8/
 * Func_08004cb4) for every member, and for a member whose 40-frame window
 * [member*8, member*8+40) is open, walks that member's six-particle group
 * (base index member*6 into the shared table) and draws each one through
 * one of two rectangle-callback function pointers cached from heap kinds 46
 * and 47 (Func_080ed408), selecting the callback and source-sprite-sheet
 * offset by whether it is the group's sixth (slot 5) particle or one of the
 * other five.
 *
 * `bl Func_080072f0`/`bl Func_080072f4` are the r3/r4 entries of the
 * fourteen-slot `_call_via_rN` trampoline bundle at games/gs1/asm/
 * 080072e4.s (`bx rN; mov r8,r8` at 0x080072e4+4*N), not real functions:
 * arm_function_arg (alchemy-gcc/agscc/gcc/config/arm/arm.c) never places an
 * argument in r4, and tracing r3/r4's values back confirms both are
 * function-pointer casts, exactly the already-adopted member_orbit/run.c's
 * own `((DrawRectangleFn)rectangle_slot[slot & 1])(...)` idiom.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

/* One 28-byte record; the array starts at work + 0x7080, the same base
 * address and record size as the already-adopted puff_arc/run.c Puff
 * array.  Field roles beyond the two written here are not evidenced. */
typedef struct Particle {
    s32 x;
    s32 y;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 tick;
} Particle;

extern u8 Value_0000009e;
extern u8 Value_0000006c;
extern u8 Value_000000bb;

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
u32 Func_08004458(void);
s32 Func_080022fc(s32 a, s32 b);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
void Func_08004cb4(void *record);
s32 Func_080022ec(s32 a, s32 b);
void Func_080e3944(void *source, void *screen);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080dc6bc(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *palette;
    s32 status;
    void *rectangle_a;
    void *rectangle_b;
    Particle *particle;
    s32 i;
    s32 record[3];
    s32 screen[3];
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    Func_080e0524((s32)&Value_0000009e, work, 1, 1);
    Func_080e0524((s32)&Value_0000006c, (u8 *)work + 0x1B00, 0, 0);
    palette = Func_08002f40((s32)&Value_000000bb);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);

    status = Func_080ed408(46, 7, 7, 3, 3);
    rectangle_a = heap_cache[7];
    status = Func_080ed408(47, 7, 7, 3, 2);
    rectangle_b = heap_cache[8];

    particle = (Particle *)((u8 *)work + 0x7080);
    i = 0;
    do {
        s32 angle;
        s32 magnitude;
        s32 element;

        angle = Func_08004458() & 0xFFFF;
        magnitude = Func_08004458() & 0xFF;
        particle->x = 0;
        particle->y = 0;
        particle->unk08 = 0;
        element = Func_080022fc(i, 6);
        if (element == 5) {
            particle->unk0C = 0;
            particle->unk10 = 0;
        } else {
            particle->unk0C = (Func_08002322(angle) * magnitude) >> 7;
            particle->unk10 = (Func_0800231c(angle) * magnitude) >> 9;
        }
        particle->unk14 = 0;
        particle->tick = 0;
        i++;
        particle++;
    } while (i != 64);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    frame = 0;
    do {
        s32 facing;

        facing = *(s32 *)0x03001E80;
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
            s32 member;
            s32 base_idx;

            member = 0;
            base_idx = 0;
            do {
                void *member_object;

                member_object = *Func_080b5098(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                        member * 2 + 36));
                Func_080049ac();
                Func_080051d8(facing, facing + 12);
                record[0] = M2C_FIELD(member_object, s32 *, 8);
                record[1] = 160 << 13;
                record[2] = M2C_FIELD(member_object, s32 *, 16);
                Func_08004cb4(record);

                if (frame >= member * 8) {
                    if (frame < member * 8 + 40) {
                        Particle *p;
                        s32 slot;

                        p = (Particle *)((u8 *)work + 0x7080
                            + ((base_idx << 3) - base_idx) * 4);
                        slot = 0;
                        do {
                            s32 clamped;

                            clamped = Func_080022ec(p->tick, 6);
                            if (clamped > 5) {
                                clamped = 5;
                            }
                            Func_080e3944(p, screen);
                            screen[0] >>= 1;
                            if (slot == 5) {
                                Func_080e38b8(p, 62, 0x800);
                                ((DrawRectangleFn)rectangle_a)(
                                    draw_destination,
                                    (u8 *)work
                                        + (((clamped * 9) << 7) + 0x1B00),
                                    screen[0] - 12, screen[1] - 36, 24, 48);
                            } else {
                                Func_080e38b8(p, 60, 0x200);
                                ((DrawRectangleFn)rectangle_b)(
                                    draw_destination,
                                    (u8 *)work + ((clamped * 9) << 7),
                                    screen[0] - 12, screen[1] - 36, 24, 48);
                            }
                            p->tick += 1;
                            slot++;
                            p++;
                        } while (slot != 6);
                    }
                }

                base_idx += 6;
                member++;
            } while (member
                != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 96);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
