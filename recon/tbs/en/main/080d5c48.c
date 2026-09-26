/* Draft, not exact (2026-09-26): 524 of 524 bytes, 16 differing halfwords,
   13 aligned edits. Complete owner, both literal pools included.
   The paired callback slot restores the 36-byte frame. Distinct guarded
   phase and record cursors recover the full drawing loop and its registers.
   Baseline: 524/127/88; callback pair: 520/140/99; guarded cursors: 524/19/16;
   explicit phase back edge and callback-load order: 524/16/13 (retained).
   Remaining: the phase loop rematerializes 63 instead of retaining r6;
   callback-load/state-clear scheduling differs at +0x7c and +0x88.
   Three structural hypotheses stopped; no new byte credit. */
#include "TYPES.H"
extern u8 Value_00000100;
extern u8 Value_00007000;
extern u8 Value_00007780;
extern u8 Value_00007784;
extern u8 Value_00000480;
extern u8 Value_00007080;
extern u8 Value_000006c0;
extern u8 Value_00007824;
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"

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
 * drawn within that same 72-frame window. Both range guards precede the
 * first draw and skip the second draw as well. Total animation length is
 * (WORK_EFX->cnt + 1) * 32 frames, not the template's fixed 80.
 *
 * The draw calls go through the r7 slot of the recon/tbs/raw/080072e4.s
 * call-via-rN trampoline bundle (bl 0x08007300, matching the template's own
 * "r4 bx bank" DrawRectangle idiom one register over), so they are modeled
 * as an indirect call through a traced function-pointer local, never as a
 * direct call to a fictional Func_08007300.
 */


/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_0000007a;

void Func_080cd594(s32);
s32 Func_080041d8(s32, s32);
u32 Func_08004458(void);
void Func_080f9010(s32);
void Func_080b50e8(s32);
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
#define WORK_EFX (*(struct BattleEffectArgument **)(work + 0x7828))

void Func_080d5c48(struct BattleEffectArgument *efx)
{
    u32 *entry;
    u8 *work;
    void *dst;
    /* FAKEMATCH: retain the paired blitter slot used by this effect family. */
    DrawRectangle draw[2];
    s32 *phase;
    Puff *cur;
    s32 i;
    s32 frame;
    s32 base;
    s32 cel;
    s32 old_y;
    /* Three-word coordinate record, as in the exact burst-shower sibling. */
    s32 pos[3];

    entry = (u32 *)(Data_03001e50 + 40 * 4);
    dst = (void *)*entry;
    work = (u8 *)*(entry - 1);
    WORK_EFX = efx;
    Func_080cd594(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&Value_0000007a, work, 1, 1);
    if (WORK_EFX->side == 1) {
        *(s32 *)0x04000028 = -0x7000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    *(s32 *)(work + 0x7780) = 1;
    /* FAKEMATCH: preserve the observed callback load before the state clear. */
    draw[0] = (DrawRectangle)entry[6];
    *(s32 *)(work + 0x7784) = 0;
    Func_080041d8(0x080CD261, 0x480);
    i = 0;
    if (WORK_EFX->count != 0) {
        phase = (s32 *)(work + 0x7098);
next_phase:
        *phase = Func_08004458() & 63;
        i++;
        phase += 7;
        if (i != WORK_EFX->count)
            goto next_phase;
    }
    frame = 0;
    while (frame != (WORK_EFX->count << 5) + 32) {
        if (frame == 32) {
            Func_080b50e8(0);
        }
        i = 0;
        if (WORK_EFX->count != 0) {
        cur = (Puff *)(work + 0x7080);
        do {
            base = i << 4;
            if (frame == base) {
                Func_080f9010(143);
            }
            if (frame < base) {
                goto next_actor;
            }
            if (frame >= base + 72) {
                goto next_actor;
            }
            Func_080e396c(WORK_EFX->actors[i], pos);
            if (WORK_EFX->side == 1) {
                pos[0] -= 112;
            }
            old_y = pos[1];
            pos[1] = old_y - 16;
            draw[0](dst, work + 1728, pos[0] - 8, old_y - 20, 16, 20);
            if (frame < base) {
                goto next_actor;
            }
            cel = Func_080022fc(
                Func_080022ec((frame - base) + cur->tick, 6), 9);
            draw[0](dst, work + cel * 192, pos[0] - 8, pos[1] - 16, 16, 12);
next_actor:
            cur++;
            i++;
        } while (i != WORK_EFX->count);
        }
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
