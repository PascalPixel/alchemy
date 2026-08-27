#include "types.h"
#include "effect_step.h"

/*
 * Effect sequence at 0x080d5c48. Family-matched to puff_arc/run.c (0x080d9fc8,
 * score 7658/10000) but 524 bytes vs the template's 644: this owner skips the
 * template's fixed nine-puff sine/cosine arc placement entirely (no calls to
 * the trig helpers, no x/y seeding loop) and instead only randomizes each of
 * WORK_EFX->cnt puffs' tick field. It also sets BG2PA/BLDCNT (affine scale,
 * blend off) instead of the template's alpha-blend registers, and shifts
 * BG2X by the same -112 used later to offset sprite x when side == 1 (the
 * same motif as member_orbit/run.c's y_offset / BG2X pair at 0x04000020 and
 * 0x04000028). The per-actor draw is an icon popup: a fixed 16x20 sprite for
 * 72 frames starting at actor i's frame offset (i*16), plus a 16x12 animated
 * marker (9 cels of 192 bytes, indexed through Func_080022ec/Func_080022fc)
 * that keeps drawing at the last computed position for every frame at or
 * after that offset, with no upper bound. Total animation length is
 * (WORK_EFX->cnt + 1) * 32 frames, not the template's fixed 80.
 *
 * The draw calls go through the r7 slot of the games/gs1/asm/080072e4.s
 * call-via-rN trampoline bundle (bl 0x08007300, matching the template's own
 * "r4 bx bank" DrawRectangle idiom one register over), so they are modeled
 * as an indirect call through a traced function-pointer local, never as a
 * direct call to a fictional Func_08007300.
 */

typedef void (*DrawRectangle)(
    void *, const void *, s32, s32, s32, s32);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_0000007a;

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080041d8(s32, s32);
u32 Func_08004458(void);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080e396c(s32, struct EffectPosition *);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_08004278(s32);
s32 Func_080cdbc0(void);

/* One 28-byte record; the array starts at work + 0x7080. Shared shape with
   puff_arc/run.c's Puff, though this owner only ever touches ->tick. */
typedef struct Puff {
    s32 x;
    s32 y;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 tick;
} Puff;

/* The caller's effect state, republished at work + 0x7828. */
typedef struct Efx {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk0C;
    s32 unk10;
    s32 cnt;
    s32 layers;
    s32 unk1C;
    s32 unk20;
    s16 actors[8];
} Efx;

#define WORK_EFX (*(Efx **)(work + 0x7828))

void Func_080d5c48(Efx *efx)
{
    u32 *entry;
    u8 *work;
    void *dst;
    DrawRectangle draw;
    Puff *puff;
    Puff *cur;
    s32 i;
    s32 frame;
    s32 base;
    s32 cel;
    s32 old_y;
    struct EffectPosition pos;

    entry = (u32 *)(Data_03001e50 + 40 * 4);
    dst = (void *)*entry;
    work = (u8 *)*(entry - 1);
    WORK_EFX = efx;
    Func_080cd594(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Func_080e0524((s32)&Value_0000007a, work, 1, 1);
    if (WORK_EFX->side == 1) {
        *(s32 *)0x04000028 = -0x7000;
    }
    Func_080ed408(46, 7, 7, 3, 1);
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = 0;
    draw = (DrawRectangle)entry[6];
    Func_080041d8(0x080CD261, 0x480);
    puff = (Puff *)(work + 0x7080);
    for (i = 0; i != WORK_EFX->cnt; i++) {
        puff->tick = Func_08004458() & 63;
        puff++;
    }
    frame = 0;
    while (frame != (WORK_EFX->cnt << 5) + 32) {
        if (frame == 32) {
            Func_080b50e8(0);
        }
        cur = (Puff *)(work + 0x7080);
        for (i = 0; i != WORK_EFX->cnt; i++) {
            base = i << 4;
            if (frame == base) {
                Func_080f9010(143);
            }
            if (frame < base) {
                cur++;
                continue;
            }
            if (frame >= base + 72) {
                cur++;
                continue;
            }
            Func_080e396c(WORK_EFX->actors[i], &pos);
            if (WORK_EFX->side == 1) {
                pos.x -= 112;
            }
            old_y = pos.y;
            pos.y = old_y - 16;
            draw(dst, work + 1728, pos.x - 8, old_y - 20, 16, 20);
            if (frame < base) {
                cur++;
                continue;
            }
            cel = Func_080022fc(
                Func_080022ec((frame - base) + cur->tick, 6), 9);
            draw(dst, work + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
            cur++;
        }
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
