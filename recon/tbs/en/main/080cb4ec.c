/* Draft, not exact (2026-09-26): baseline 792 of 780 bytes, 153 aligned edits.
   Complete owner and frame (36 bytes); remaining branch stores hoist 3 and 2
   into saved registers instead of the reference's shared immediate/store tail;
   rectangle-slot spill formation, particle draw registers and literal offsets differ.
   Bounded trials: union word/halfword position views emitted the same 792-byte,
   153-edit result: GCC already selects ldrsh at particle +2/+6 from the shifts.
   A pooled Value_00000078 call operand alone gave 796 bytes / 155 edits, placing
   its word in the first rather than the second pool. A scalar drift bucket plus
   that symbol gave 772 / 177: constants stayed inside branches, but CSE removed
   the reference store/reload at +12 and merged the branch tails. Neither spelling
   is retained. Need a new alias/control-flow fact, not high-half respelling. */
#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"

/*
 * Battle-presentation sub-effect at 0x080cb4ec, part of the same
 * 0x03001eec "battle work" family as games/THE BROKEN SEAL/src/battle/effects/puff_arc
 * (0x080d9fc8, the closest structural template, score 8362/10000) and
 * games/THE BROKEN SEAL/src/battle/effects/member_orbit (0x080ce85c). This owner is
 * 780 bytes against the template's 644: it shares the template's overall
 * shape (WORK_EFX republish, BattleFx_BeginCanvasLayer, two BattleEffect_LoadWork heap-kind
 * loads, Resource_LoadAndDecompress, a fixed-length outer frame loop, the
 * Scheduler_AddOrUpdateCallback/RemoveCallback bracket at 0x080CD261, and
 * the Runtime_ReleaseHeapBlock(47)/Runtime_ReleaseHeapBlock(46) unload order also seen in
 * member_orbit) but replaces the template's 9-puff sine/cosine arc with a
 * 64-particle randomized field seeded by Random16()/UnsignedModulo, and
 * replaces the template's single draw callback with member_orbit's
 * two-callback (heap kinds 46 and 47) selection idiom, chosen here per
 * particle by the sign of its drift velocity rather than by frame parity.
 *
 * Each frame redraws 16 of the 64 particles (the ones seeded at array
 * indices 0..15, walked back to front) through a 4-entry size/offset table
 * keyed by the particle's |drift bucket| (0..3), and after each particle's
 * staggered opening window elapses further, draws it with height reduced by
 * 4 rather than advancing its position -- a fade/settle tail rather than the
 * template's continued motion.
 */


void BattleFx_BeginCanvasLayer(s32 mode);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 interval);
void Scheduler_RemoveCallback(s32 callback);
void Audio_PlayCue(s32 cue);
void EffectPosition_ApplyStepAndYOffset(
    s32 actor, struct EffectPosition *position);
u32 Random16(void);
s32 Math_ModU(u32 value, s32 modulus);
void ObjectGroup_UpdateMembers(s32 a, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, u32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

/* Size/offset table for the four |drift bucket| classes (0..3): source data
   offset within the work block, width, and height. This owner's own table,
   distinct from puff_arc's PuffArc_CellWidths/PuffArc_CellHeights/PuffArc_CellSourceOffsets. */
extern const u16 Data_080edf88[4];
extern const u8 Data_080edf7f[4];
extern const u8 Data_080edf83[4];

/* Same effect-state layout established by puff_arc/run.c, republished at
   work + 0x7828. */
#define WORK_EFX (*(struct BattleEffectArgument **)(work + 0x7828))

/* One 28-byte particle record, matching the template's Puff stride. Only
   offsets 0, 4, 0xC and 0x10 are ever touched by this owner; offset 8 and
   the tail bytes are unused padding. pos_x/pos_y are 16.16 fixed point --
   only their integer half is ever read back. vel_x doubles as the per-
   particle |drift bucket| << 17 at init and as the signed per-frame "age"
   test afterward. */
typedef struct Particle {
    s32 pos_x;
    s32 pos_y;
    s32 pad08;
    s32 vel_x;
    s32 vel_y;
    u8 pad14[8];
} Particle;

void Func_080cb4ec(struct BattleEffectArgument *efx)
{
    void **heap_cache;
    void **cursor;
    u8 *work;
    void *canvas;
    struct EffectPosition pos;
    void *rectangle[2];
    void **rectangle_slot;
    s32 frame;
    s32 i;
    Particle *p;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    WORK_EFX = efx;
    BattleFx_BeginCanvasLayer(1);
    *(s16 *)0x04000020 = 0x0100;
    *(s16 *)0x04000052 = 0x1000;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    rectangle[0] = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    rectangle[1] = heap_cache[8];
    rectangle_slot = rectangle;
    Resource_LoadAndDecompress((void *)0x78, work, 1, 1);
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = 0;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[0], &pos);
    *(s32 *)0x04000028 = (0x40 - pos.x) << 8;

    {
        i = 0;
        p = (Particle *)(work + 0x7080);
        do {
            s32 v;

            v = Math_ModU(Random16(), 0x60) + 16;
            p->pos_x = v;
            p->pos_y = (24 - (i / 4)) << 16;
            if (v <= 0x2B) {
                p->vel_x = 3;
            } else if (v <= 0x33) {
                p->vel_x = 2;
            } else if (v <= 0x3B) {
                p->vel_x = 1;
            } else if (v <= 0x43) {
                p->vel_x = 0;
            } else {
                if (v <= 0x4B) {
                    p->vel_x = 1;
                } else if (v <= 0x53) {
                    p->vel_x = 2;
                } else {
                    p->vel_x = 3;
                }
                p->vel_x = -p->vel_x;
            }
            p->vel_x = p->vel_x << 17;
            p->vel_y = 0x80000;
            i += 1;
            p->pos_x = p->pos_x << 16;
            p += 1;
        } while (i != 64);
    }

    Audio_PlayCue(0xD4);
    frame = 0;
    do {
        if (frame <= 0x10) {
            *(s16 *)0x04000052 = frame | 0x1000;
            if (frame == 0x10) {
                *(s16 *)0x04000050 = 0;
            }
        }
        if (frame > 0x67) {
            *(s16 *)0x04000052 = (0x78 - frame) | 0x1000;
            if (frame == 0x68) {
                *(s16 *)0x04000050 = 0x3F44;
            }
        }

        i = 15;
        p = (Particle *)(work + 0x7224);
        do {
            s32 tick;
            s32 abs_tick;
            s32 cell;
            s32 threshold;
            s32 width;
            s32 height;
            s32 x;
            s32 y;
            void *src;

            tick = p->vel_x;
            abs_tick = (tick < 0) ? -tick : tick;
            cell = abs_tick >> 17;
            threshold = i * 4;
            if (frame < threshold + 25) {
                src = work + Data_080edf88[cell];
                width = Data_080edf7f[cell];
                x = (p->pos_x >> 16) - (width / 2);
                height = Data_080edf83[cell];
                y = (p->pos_y >> 16) - (height / 2);
                ((DrawRectangleFn)rectangle_slot[(u32)tick >> 31])(
                    canvas, src, x, y, width, height);
                if (frame >= threshold + 16) {
                    p->pos_x += p->vel_x;
                    p->pos_y += p->vel_y;
                }
            } else {
                src = work + Data_080edf88[cell];
                width = Data_080edf7f[cell];
                x = (p->pos_x >> 16) - (width / 2);
                height = Data_080edf83[cell];
                y = (p->pos_y >> 16) - (height / 2);
                height -= 4;
                ((DrawRectangleFn)rectangle_slot[(u32)tick >> 31])(
                    canvas, src, x, y, width, height);
            }
            i -= 1;
            p -= 1;
        } while (i != -1);

        if (((u32)(frame - 0x17) <= 0x40) && !(3 & frame)) {
            ObjectGroup_UpdateMembers(WORK_EFX->actors[0], 7, 5, 0, 2);
            *(s32 *)(work + 0x77A8) = 1;
            if (!(7 & frame)) {
                Audio_PlayCue(0x85);
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        *(s32 *)(work + 0x7824) = 1;
        WaitFrames(1);
        frame += 1;
    } while (frame != 0x78);

    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
