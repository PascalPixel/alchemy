#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080e698c.
 *
 * Assigned family template: games/gs1/src/battle/effects/member_orbit/run.c
 * (main:080ce85c, template-main-080ce85c, family score 8067/10000).  Like
 * the measured siblings 080d59b0/080d82b0/080dc1ec/080e01e4/080e7404 in
 * this same 0x03001eec "battle work" subsystem, this owner's callee set,
 * work+0x7780/0x7784/0x7824/0x7828 field layout, and the M2C_FIELD /
 * DrawRectangleFn conventions those files established carry over directly.
 * The body itself is a materially different sub-effect from the template,
 * though: this owner runs a fixed 70-frame loop (not member_count*16+48),
 * never reads a member count at all, and spends its prologue computing a
 * distance/velocity setup between two specific tracked members (the ids at
 * target+8 and (s16)target+36) rather than looping over a member list.
 *
 * Func_080072ec is the r2-slot `_call_via_rN` veneer at
 * games/gs1/asm/080072e4.s (0x080072e4 + 4*2).  The retained assembly loads
 * 0x030001D8 -- the same relocated IWRAM square-root routine documented in
 * games/gs1/src/math/fixed_sqrt.c and games/gs1/src/unidentified/main/
 * battle/battle_owner_52.c -- into r2 immediately before the call, so it is
 * modeled the same way those files model their own veneer slot: a direct
 * call to the veneer's own symbol with the real jump target passed as a
 * trailing argument.  The middle argument is never assigned between the
 * preceding Func_080022ec call and this call in the retained assembly (no
 * instruction touches r1 in between), so it is passed uninitialized here,
 * matching FixedSqrt's own "unused1"/"unused2" idiom for the identical
 * situation.
 *
 * Func_080e3980 is EffectPosition_ApplyAlternateStepAndYOffset
 * (games/gs1/include/types.h), sibling of Func_080e3944
 * (EffectPosition_ApplyBaseAndYOffset, used by the template).  Its first
 * argument here is read from target+8 as a plain s32 -- the same field
 * passed directly to Func_080b5098 earlier in this owner -- so it is
 * modeled as taking a member id rather than a position-record pointer.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080cef64(s32 flag, DrawRectangleFn *out_pair);
void **Func_080b5098(s32 member_id);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080072ec(s32 a, s32 b, s32 target);
void Func_08009140(void *object);
void Func_08009150(void *object, s32 x, s32 y, s32 z);
void Func_08009080(void *object, s32 mode);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080e3980(s32 member_id, void *screen);
void Func_080b50e8(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 mode);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080e3908(void *particle, s32 count, s32 flags);
s32 Func_080022fc(s32 a, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

extern u8 Value_00000073;
extern u8 Value_00000061;
extern u8 Value_0000006d;
extern const u16 Data_080eee02[];
extern const s8 Data_080eee10[];
extern const s8 Data_080eee17[];
extern const u8 Data_080eedf4[];
extern const u8 Data_080eedfb[];

s32 Func_080e698c(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void *member_a;
    void *member_b;
    DrawRectangleFn draw_pair[2];
    s32 dx;
    s32 dz;
    s32 dx_scaled;
    s32 dz_scaled;
    s32 speed;
    s32 leftover;
    s32 new_x;
    s32 new_z;
    s32 frame;
    s32 screen[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    Func_080e0524((s32)&Value_00000073, extra_target, 0, 0);
    Func_080e0524((s32)&Value_00000061, work, 1, 1);
    Func_080e0524((s32)&Value_0000006d, (u8 *)work + 0x3E80, 1, 0);
    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4), draw_pair);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);

    {
        s32 *reset_cursor;
        s32 i;

        reset_cursor = (s32 *)0x02010018;
        i = 0;
        do {
            i++;
            *reset_cursor = 0;
            reset_cursor += 7;
        } while (i != 1024);
    }

    member_a = *Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    member_b = *Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));
    new_x = M2C_FIELD(member_a, s32 *, 8);
    dx = M2C_FIELD(member_b, s32 *, 8) - new_x;
    dx_scaled = Func_080022ec(dx * 80, 100);
    dz = M2C_FIELD(member_b, s32 *, 16) - M2C_FIELD(member_a, s32 *, 16);
    dz_scaled = Func_080022ec(dz * 80, 100);
    new_x = new_x + dx_scaled;
    new_z = M2C_FIELD(member_a, s32 *, 16) + dz_scaled;
    speed = Func_080022ec(
        Func_080072ec(
            ((dx_scaled >> 8) * (dx_scaled >> 8))
                + ((dz_scaled >> 8) * (dz_scaled >> 8)),
            leftover, 0x030001D8)
            << 8,
        20);
    M2C_FIELD(member_a, s32 *, 0x34) = speed;
    M2C_FIELD(member_a, s32 *, 0x30) = speed;
    M2C_FIELD(member_a, s8 *, 0x58) = 1;
    M2C_FIELD(member_a, s32 *, 0x28) = 0x70000;
    M2C_FIELD(member_a, s32 *, 0x48) = 0xDEB8;
    M2C_FIELD(member_a, s32 *, 0x44) = 0;
    M2C_FIELD(member_a, s8 *, 0x5A) = 1;
    Func_08009140(member_a);
    Func_08009150(member_a, new_x, 0, new_z);
    Func_08009080(member_a, 2);

    for (frame = 0; frame != 70; frame++) {
        Func_080e3980(
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8), screen);
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (0x50 - screen[0]) << 8;

        if ((u32)(frame - 8) <= 15) {
            s32 index;

            index = (frame - 8) / 2;
            if (index > 6) {
                index = 6;
            }
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
                draw_pair[0](
                    draw_destination,
                    (u8 *)work + Data_080eee02[index],
                    Data_080eee10[index] + 30,
                    (Data_080eee17[index] + screen[1]) - 60,
                    Data_080eedf4[index], Data_080eedfb[index]);
            } else {
                s32 width;

                width = Data_080eedf4[index];
                draw_pair[0](
                    draw_destination,
                    (u8 *)work + Data_080eee02[index],
                    (-Data_080eee10[index] - width) + 108,
                    (Data_080eee17[index] + screen[1]) - 60,
                    width, Data_080eedfb[index]);
            }
        }

        if (frame == 18) {
            u8 *particle;
            s32 i;

            Func_080b50e8(134);
            Func_080d6888(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, 5, 0, 8);
            Func_080b5088(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), 6);
            M2C_FIELD(work, s32 *, 0x77A8) = 4;

            particle = (u8 *)0x02010000;
            for (i = 0; i != 16; i++) {
                s32 magnitude;
                u32 angle;

                magnitude = (Func_08004458() & 63) + 256;
                angle = Func_08004458() & 0xFFFF;
                M2C_FIELD(particle, s32 *, 0) = 0x400000;
                M2C_FIELD(particle, s32 *, 4) = 0x500000;
                M2C_FIELD(particle, s32 *, 12) =
                    (magnitude * Func_08002322((s32)angle)) >> 7;
                M2C_FIELD(particle, s32 *, 16) =
                    -(magnitude * Func_0800231c((s32)angle)) >> 6;
                M2C_FIELD(particle, s32 *, 24) = (Func_08004458() & 15) + 16;
                particle += 28;
            }
        }

        {
            u8 *particle;
            s32 i;

            particle = (u8 *)0x02010000;
            for (i = 0; i != 128; i++) {
                s32 lifetime;

                lifetime = M2C_FIELD(particle, s32 *, 24);
                if (lifetime > 0) {
                    M2C_FIELD(particle, s32 *, 24) = lifetime - 1;
                    Func_080e3908(particle, 60, 0);
                    if (M2C_FIELD(particle, s32 *, 4) > 0x680000) {
                        M2C_FIELD(particle, s32 *, 16) =
                            -M2C_FIELD(particle, s32 *, 16) / 2;
                    } else {
                        s32 x;
                        s32 y;

                        x = M2C_FIELD(particle, s32 *, 0);
                        y = M2C_FIELD(particle, s32 *, 4);
                        if ((u32)x <= 0x7EFFFFU && y >= 0) {
                            s32 rx;
                            s32 ry;
                            s32 cell;

                            rx = x >> 16;
                            ry = y >> 16;
                            cell = frame + i;
                            if (cell < 0) {
                                cell += 3;
                            }
                            cell = Func_080022fc(cell >> 2, 6);
                            draw_pair[0](
                                draw_destination,
                                (u8 *)work + 0x3E80 + (cell << 8),
                                rx - 8, ry - 8, 16, 16);
                        }
                    }
                }
                particle += 28;
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    return Func_080cdbc0();
}
