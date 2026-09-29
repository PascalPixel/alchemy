/* 2026-09-29 alchemy permute: score 5112 to 3649 on the permuter's scorer
   (0 is exact); remaining 55 register-only, 4 stack-only, 20 operand, 23
   reordered, 7 inserted, 8 deleted. Kept rewrites: 14x swap commutative
   operands, 9x reorder independent statements, 7x add a same-width cast,
   5x drop a same-width cast, 4x reorder local declarations, 4x introduce a
   temporary, 4x remove a temporary, 4x split or join a compound
   assignment, 4x test truth or compare with zero, 3x change loop form, 1x
   pointer arithmetic or indexing. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
/* Draft, complete main:080e01e4 [080e01e4,080e0524), 832 bytes.
 * Baseline: 836/832 bytes, 390 differing halfwords, 235 aligned edits,
 * different topology, frame 40. Prior explicit variant-motion 6 retained.
 * H1: transfer the exact PUFF_ARC / MEMBER_ORBIT work and particle family,
 * with BATTLE_EFX returning rectangle callbacks and canonical void teardown.
 * Own-ROM fixes: signed size / 2 (old unsigned addition caused LSR);
 * immediate 512 loop bound (old Value_ symbol added a zero-count precheck);
 * actual typed work offsets instead of link-time symbolic field addresses.
 * Keep the const sprite-offset table: unlike 080e08c0, this reference hoists
 * it into r6 for the particle loop. No word-copy call exists in this owner.
 * Prediction: frame 40/callback slots intact, no loop precheck, signed ASR,
 * immediate 0x7080/0x320 offsets and void epilogue; compare whole extent.
 * This is one coherent corrected model, not a declaration/order sweep.
 * H1 result: 832/832 bytes, 233 differing halfwords, 171 aligned edits,
 * equal topology, frame 40, first 75 instructions exact. Literal pools,
 * signed size division, fixed-count loop and void return are now correct.
 * First divergence: initialization counter r5 / ring r8 vs ROM sl / r5;
 * later member fp and frame spill vs ROM member sl / frame fp. The ROM uses
 * sl for initialization, member traversal and final particle traversal.
 * H2 admission: reuse the existing traversal counter across these disjoint
 * phases, as in the exact family, rather than introducing saved identities.
 * Require recovered frame/counter roles, without extra calls or stack growth.
 * H2 result: 840/832 bytes, 343 differing halfwords, 176 aligned edits;
 * topology/frame 40 retained. The shared counter does recover sl throughout
 * all three traversals, but work moves r9 -> fp, frame remains spilled,
 * ring stays r8, and code grows eight bytes. Reject the whole-owner result.
 * H2 is preserved separately; canonical H1 restored with corrected semantics
 * (832/832 bytes, 233 differing halfwords, 171 aligned edits, frame 40).
 * Counter reuse alone is not a closing mechanism; no counter/order sweeps.
 * No credit until whole-owner match, ordinary-C lint, compare/coverage/verify.
 */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output);
void BattleFx_EndCanvasLayer(void);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 mask, u32 range);
void ObjectGroup_TickMemberTimers(void);

extern const u16 Data_080ede48[];
extern u8 Value_00000073;
extern u8 Value_00000090;
extern u8 Value_00000089;

#define WORK_EFX ((struct BattleEffectArgument *)work->effect)

/* Eight falling sprites emit thirty-two gravity particles on reaching the
 * lower edge. EffectStep.variant is the remaining particle lifetime here. */
void Func_080e01e4(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    void *sheet;
    void **heap_cache;
    DrawRectangle callbacks[2];
    void **cursor;
    DrawRectangle *draw;
    struct EffectStep *ring;
    struct EffectStep *particle;
    s32 member;
    s32 i;
    s32 frame;
    s32 member_offset;
    s32 angle;

    heap_cache = (void **)0x03001eec;
    cursor = heap_cache;
    work = *cursor++;
    canvas = cursor[0];
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000052 = 0x1010;
    draw = callbacks;
    BattleFx_FetchRectangleBlitters((s32)0, (u32 *)draw);
    Resource_LoadAndDecompress((s32)&Value_00000073, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000090, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000089, (u8 *)work + 0x320, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);
    i = 0;
    ring = (struct EffectStep *)(0x7080 + (u8 *)work);
    if (i != 32) {
        do {
            ring->x = (Random16() & 63) + 64;
            i++;
            ring->y = (Random16() & 63) - 80;
            ring += 1;
        } while (i != 32);
    }
    {
        s32 *life;
        i = 0;
        life = (s32 *)0x02010018;
        do {
            i++;
            *life = -1;
            life += sizeof(struct EffectStep) / sizeof(s32);
        } while (512 != i);
    }
    Audio_PlayCue(171);
    angle = 0x8000;
    frame = 0;
    while (1) {
        member = 0;
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(133);
        if (frame <= 95) {
            s32 sine;
            s32 x;
            s32 scale;
            s32 tmp;
            s32 tmp2;
            sine = Trig_Sin(angle);
            scale = 64 - frame * 2;
            tmp2 = sine * scale;
            x = 86 + (tmp2 >> 17);
            tmp = Trig_Cos(angle);
            callbacks[0](canvas, work, x, 28 + ((tmp * scale) >> 16), 20, 40);
        }
        member_offset = 0;
        ring = (struct EffectStep *)((u8 *)work + 0x7080);
        do {
            if (frame >= member * 4 + 8 && 95 >= ring->y) {
                callbacks[0](canvas, 0x320 + (u8 *)work, ring->x - 20, ring->y - 32, 40, 64);
                ring->x -= 6;
                ring->y += 12;
                if (ring->y > 95) {
                    s32 burst;
                    particle = (struct EffectStep *)(0x02010000 + member_offset);
                    burst = 0;
                    while (1 != 0) {
                        s32 direction;
                        s32 speed;
                        direction = Random16() & 0xffff;
                        speed = (Random16() & 0x1ff) + 256;
                        particle->x = ring->x << 16;
                        particle->y = ring->y << 16;
                        burst++;
                        particle->velocity_x = (Trig_Sin(direction) * speed) >> 7;
                        particle->velocity_y = (Trig_Cos(direction) * speed) >> 6;
                        particle->variant = (Random16() & 15) + 32;
                        particle++;
                        if (32 == burst)
                            break;
                    }
                    Audio_PlayCue(133);
                    *(s32 *)((u8 *)work + 0x77a8) = 4;
                    if (WORK_EFX->count) {
                        s32 actor;
                        actor = 0;
                        do {
                            ObjectGroup_UpdateMembers(WORK_EFX->actors[actor], 7, 5, actor, 6);
                            BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[actor], 6);
                            actor++;
                        } while (actor != WORK_EFX->count);
                    }
                }
            }
            ring++;
            member_offset += sizeof(struct EffectStep) * 32;
            member++;
        } while (member != 8);
        particle = (struct EffectStep *)0x02010000;
        for (i = 0; i != 512; ++i) {
            if (particle->variant != -1) {
                s32 size;
                size = particle->variant / 16 + 1;
                draw[1](canvas, (u8 *)sheet + Data_080ede48[size - 1], ((s16 *)&particle->x)[1] - size / 2, ((s16 *)&particle->y)[1] - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(particle, 62, 0x2000);
                particle->variant--;
            }
            ++particle;
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
        angle -= 0x800;
        if ((u32)frame == 96)
            break;
    }
    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
