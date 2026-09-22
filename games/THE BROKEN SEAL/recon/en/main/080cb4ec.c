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
 * shape (WORK_EFX republish, Func_080cd594, two BattleEffect_LoadWork heap-kind
 * loads, Resource_LoadAndDecompress, a fixed-length outer frame loop, the
 * Scheduler_AddOrUpdateCallback/RemoveCallback bracket at 0x080CD261, and
 * the Func_08002dd8(47)/Func_08002dd8(46) unload order also seen in
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


void Func_080cd594(s32 mode);
s32 Func_080041d8(s32 callback, s32 interval);
void Func_08004278(s32 callback);
void Func_080f9010(s32 cue);
void EffectPosition_ApplyStepAndYOffset(
    s32 actor, struct EffectPosition *position);
u32 Func_08004458(void);
s32 Func_08002304(u32 value, s32 modulus);
void Func_080d6888(s32 a, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, u32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

/* Size/offset table for the four |drift bucket| classes (0..3): source data
   offset within the work block, width, and height. This owner's own table,
   distinct from puff_arc's Data_080ede9f/Data_080edea5/Data_080edeb2. */
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
    Func_080cd594(1);
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
    Func_080041d8(0x080CD261, 0x480);
    EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[0], &pos);
    *(s32 *)0x04000028 = (0x40 - pos.x) << 8;

    {
        i = 0;
        p = (Particle *)(work + 0x7080);
        do {
            s32 v;

            v = Func_08002304(Func_08004458(), 0x60) + 16;
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

    Func_080f9010(0xD4);
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
            Func_080d6888(WORK_EFX->actors[0], 7, 5, 0, 2);
            *(s32 *)(work + 0x77A8) = 1;
            if (!(7 & frame)) {
                Func_080f9010(0x85);
            }
        }
        Camera_ApplyShake(8, 8);
        Func_080cd52c();
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    } while (frame != 0x78);

    Func_08004278(0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
