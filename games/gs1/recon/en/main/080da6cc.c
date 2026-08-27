#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080da6cc.  The family matcher's
 * closest structural template is games/gs1/src/battle/effects/member_orbit/
 * run.c (main:080ce85c, already exact), but this owner's size (1192 bytes
 * vs. the template's 724) shows it is a genuinely different sub-effect: a
 * per-member burst of falling "star" particles rather than an orbiting
 * ring.  Field offsets, the M2C_FIELD macro, the DrawRectangleFn calling
 * convention (an indirect call through the r4 slot of the _call_via_rN
 * trampoline at games/gs1/asm/080072e4.s -- see games/gs1/recon/en/main/
 * 080dc1ec.json's score.note for the full derivation), and most callee
 * signatures come from that template and from games/gs1/src/battle/
 * effects/puff_arc/run.c (also exact) and the measured-draft siblings
 * 080e01e4.c / 080d82b0.c in this same 0x03001eec "battle work" family.
 *
 * Sixty-four Star records (at 0x02010000, 28 bytes each) are seeded once
 * from a single member's screen position with random velocities.  Each
 * animation frame, a per-variant subset of them (the raw count at
 * Data_080eea41[variant]) is checked: once `idx / 2` frames have passed a
 * still-unreleased (state == -1) star draws a shrinking trail sprite and
 * drifts by its own velocity (kind 47 while still within its scheduled
 * window, kind 46 once 48 frames overdue); once 48 frames overdue it is
 * also re-homed every frame toward its member's live screen position
 * (damped for the first 37 of those frames), and once it crosses above the
 * top of the screen (y < 0) it "lands": state flips to 0 and a second,
 * member-independent scan over all 64 stars draws a fixed twelve-frame
 * "landed" glint sequence (kind 47, keyed by state / 2) through four small
 * per-frame tables.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

typedef struct Star {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 state;
} Star;

extern u8 Data_03001e50[];
extern const u16 Data_080ede48[];
extern const u8 Data_080eea41[];
extern const u8 Data_080eea44[];
extern const u8 Data_080eea4a[];
extern const u8 Data_080eea50[];
extern const u16 Data_080eea56[];
extern u8 Value_000000b4;
extern u8 Value_00000073;

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *target, s32 a, s32 b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void **Func_080b5098(s32 member_id);
s32 Func_080b5070(s32 member_id);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080b50e8(s32 id);
void Func_080b5028(s32 a, s32 b, s32 c, s32 d);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080e3944(void *source, void *screen);
s32 Func_080022fc(s32 a, s32 b);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080da6cc(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    s32 facing;
    s32 facing2;
    s32 status;
    void *rectangle[2];
    void **rectangle_slot;
    void *second_rectangle;
    void *member_obj;
    s32 y0;
    Star *star;
    s32 n;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;
    if (M2C_FIELD(object, s32 *, 4) == 1) {
        Func_080cd594(1);
    } else {
        Func_080cd594(0);
    }
    Func_080e0524((s32) &Value_000000b4, work, 1, 1);
    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);
    status = Func_080ed408(46, 7, 7, 3, 3);
    rectangle[0] = *(void **) (Data_03001e50 + 46 * 4);
    status = Func_080ed408(47, 7, 7, 3, 2);
    second_rectangle = *(void **) (Data_03001e50 + 47 * 4);
    rectangle_slot = rectangle;
    rectangle_slot[1] = second_rectangle;
    *(s16 *) 0x04000052 = 0x1010;

    member_obj = *Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    y0 = M2C_FIELD(member_obj, s32 *, 12)
        + Func_080b5070(M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));

    star = (Star *) 0x02010000;
    n = 0;
    do {
        u32 rand1;
        s32 mag;

        rand1 = Func_08004458();
        mag = (Func_08004458() & 127) + 127;
        star->vx = (mag * Func_08002322((s32) rand1)) >> 6;
        star->vy = (((Func_08004458() & 127) - 16) << 16) >> 6;
        star->vz = (mag * Func_0800231c((s32) rand1)) >> 6;
        star->x = M2C_FIELD(member_obj, s32 *, 8);
        star->y = y0;
        star->z = M2C_FIELD(member_obj, s32 *, 16);
        n++;
        star->state = -1;
        star++;
    } while (n != 64);

    M2C_FIELD(work, s32 *, 0x77AC) = 0;
    M2C_FIELD(work, s32 *, 0x77B0) = 0;
    Func_080041d8((void *) 0x080D6505, 0x480);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
    Func_080041d8((void *) 0x080CD261, 0x480);

    frame = 0;
    if ((Data_080eea41[
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)] >> 1)
            != -132) {
        facing2 = facing + 12;
        do {
            s32 variant;

            M2C_FIELD(work, s32 *, 0x77AC) =
                ((u32) (frame - 17) <= 62) ? 256 : 0;

            variant = M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18);
            if (frame == (Data_080eea41[variant] >> 1) + 108) {
                Func_080b50e8(133);
            }

            Func_080b5028(0, 0, 0, 100);
            Func_080049ac();
            Func_080051d8(facing, facing2);

            if (Data_080eea41[M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)] != 0) {
                s32 idx;
                Star *cur;

                idx = 0;
                cur = (Star *) 0x02010000;
                do {
                    s32 half_idx;
                    s32 screen[3];

                    half_idx = idx / 2;

                    if (frame > half_idx) {
                        if (cur->state == -1) {
                            s32 dist;
                            s32 size;
                            s32 size2;
                            s32 slot;

                            Func_080e3944(cur, screen);
                            dist = screen[2];
                            screen[0] = screen[0] >> 1;
                            if (dist <= 159) {
                                screen[2] = 160;
                            }
                            if (dist > 799) {
                                screen[2] = 799;
                                dist = 799;
                            }
                            size = 10 - ((dist - 160) / 64);
                            slot = (frame < half_idx + 48) ? 4 : 0;
                            size2 = size * 2;
                            ((DrawRectangleFn) *(void **)
                                ((u8 *) rectangle_slot + slot))(
                                draw_destination,
                                (u8 *) extra_target
                                    + Data_080ede48[size - 1],
                                screen[0] - (size / 2),
                                screen[1] - size,
                                size, size2);
                            cur->x += cur->vx;
                            cur->y += cur->vy;
                            cur->z += cur->vz;
                        }
                    }

                    if (frame > half_idx + 48) {
                        if (cur->state == -1) {
                            s32 member_index;
                            s32 member_id;
                            void *member_obj2;
                            s32 vx;
                            s32 vy;
                            s32 vz;

                            member_index = Func_080022fc(idx,
                                M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                    s32 *, 0x14));
                            member_id = M2C_FIELD(
                                M2C_FIELD(work, void **, 0x7828), s16 *,
                                36 + member_index * 2);
                            member_obj2 = *Func_080b5098(member_id);

                            vx = cur->vx
                                + ((M2C_FIELD(member_obj2, s32 *, 8) - cur->x)
                                    >> 9);
                            cur->vx = vx;
                            vy = cur->vy
                                + ((M2C_FIELD(member_obj2, s32 *, 12) - cur->y)
                                    >> 9);
                            cur->vy = vy;
                            vz = cur->vz
                                + ((M2C_FIELD(member_obj2, s32 *, 16) - cur->z)
                                    >> 9);
                            cur->vz = vz;

                            if (frame < half_idx + 85) {
                                cur->vx = (vx * 60) / 64;
                                cur->vy = (vy * 60) / 64;
                                cur->vz = (vz * 60) / 64;
                            }

                            if (cur->y < 0) {
                                s32 landed_index;
                                s32 landed_id;

                                cur->state = 0;
                                cur->x = screen[0];
                                cur->y = screen[1];
                                Func_080f9010(136);
                                landed_index = Func_080022fc(idx,
                                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                        s32 *, 0x14));
                                landed_id = M2C_FIELD(
                                    M2C_FIELD(work, void **, 0x7828), s16 *,
                                    36 + landed_index * 2);
                                Func_080d6888(landed_id, 10, 5, landed_index, 4);
                                M2C_FIELD(work, s32 *, 0x77A8) = 2;
                            }
                        }
                    }

                    idx++;
                    cur++;
                } while (idx != Data_080eea41[M2C_FIELD(
                    M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)]);
            }

            {
                s32 m;
                Star *cur2;

                m = 0;
                if (Data_080eea41[M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)]
                        != 0) {
                    cur2 = (Star *) 0x02010000;
                    do {
                        if ((u32) cur2->state <= 11) {
                            s32 sidx;
                            void *src;
                            u32 w;
                            s32 x;
                            s32 y;

                            sidx = cur2->state / 2;
                            src = (u8 *) work + Data_080eea56[sidx];
                            w = Data_080eea44[sidx];
                            x = cur2->x - (w >> 1);
                            y = (cur2->y + Data_080eea50[sidx]) - 56;
                            ((DrawRectangleFn) rectangle_slot[1])(
                                draw_destination, src,
                                x, y, w, Data_080eea4a[sidx]);
                            cur2->state += 1;
                        }
                        m++;
                        cur2++;
                    } while (m != Data_080eea41[M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)]);
                }
            }

            if (M2C_FIELD(work, s32 *, 0x77B0) == 0) {
                M2C_FIELD(work, s32 *, 0x77B0) = 1;
            }
            Func_080e155c(8, 8);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            frame++;
        } while (frame != (
            (Data_080eea41[
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)] >> 1)
            + 132));
    }

    Func_08004278((void *) 0x080CD261);
    Func_08004278((void *) 0x080D6505);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
