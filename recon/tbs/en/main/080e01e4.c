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
    void **cursor;
    DrawRectangle callbacks[2];
    DrawRectangle *draw;
    struct EffectStep *ring;
    struct EffectStep *particle;
    s32 i;
    s32 member;
    s32 frame;
    s32 angle;
    s32 member_offset;

    heap_cache = (void **)0x03001eec;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000052 = 0x1010;
    draw = callbacks;
    BattleFx_FetchRectangleBlitters(0, (u32 *)draw);
    Resource_LoadAndDecompress((s32)&Value_00000073, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000090, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000089, (u8 *)work + 0x320, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);

    ring = (struct EffectStep *)((u8 *)work + 0x7080);
    for (i = 0; i != 32; i++) {
        ring->x = (Random16() & 63) + 64;
        ring->y = (Random16() & 63) - 80;
        ring++;
    }
    {
        s32 *life;

        life = (s32 *)0x02010018;
        i = 0;
        do {
            i++;
            *life = -1;
            life += sizeof(struct EffectStep) / sizeof(s32);
        } while (i != 512);
    }

    Audio_PlayCue(171);
    angle = 0x8000;
    frame = 0;
    do {
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(133);

        if (frame <= 95) {
            s32 sine;
            s32 cosine;
            s32 scale;
            s32 x;
            s32 y;

            sine = Trig_Sin(angle);
            scale = 64 - frame * 2;
            x = ((scale * sine) >> 17) + 86;
            cosine = Trig_Cos(angle);
            y = ((scale * cosine) >> 16) + 28;
            callbacks[0](canvas, work, x, y, 20, 40);
        }

        ring = (struct EffectStep *)((u8 *)work + 0x7080);
        member_offset = 0;
        member = 0;
        do {
            if (frame >= member * 4 + 8 && ring->y <= 95) {
                callbacks[0](canvas, (u8 *)work + 0x320,
                    ring->x - 20, ring->y - 32, 40, 64);
                ring->x -= 6;
                ring->y += 12;
                if (ring->y > 95) {
                    s32 burst;

                    particle = (struct EffectStep *)(0x02010000 + member_offset);
                    burst = 0;
                    do {
                        s32 direction;
                        s32 speed;

                        direction = Random16() & 0xffff;
                        speed = (Random16() & 0x1ff) + 256;
                        particle->x = ring->x << 16;
                        particle->y = ring->y << 16;
                        particle->velocity_x = (speed * Trig_Sin(direction)) >> 7;
                        particle->velocity_y = (speed * Trig_Cos(direction)) >> 6;
                        particle->variant = (Random16() & 15) + 32;
                        particle++;
                        burst++;
                    } while (burst != 32);
                    Audio_PlayCue(133);
                    *(s32 *)((u8 *)work + 0x77a8) = 4;
                    if (WORK_EFX->count != 0) {
                        s32 actor;

                        actor = 0;
                        do {
                            ObjectGroup_UpdateMembers(WORK_EFX->actors[actor],
                                7, 5, actor, 6);
                            BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[actor], 6);
                            actor++;
                        } while (actor != WORK_EFX->count);
                    }
                }
            }
            member++;
            member_offset += 32 * sizeof(struct EffectStep);
            ring++;
        } while (member != 8);

        particle = (struct EffectStep *)0x02010000;
        for (i = 0; i != 512; i++) {
            if (particle->variant != -1) {
                s32 size;

                size = particle->variant / 16 + 1;
                draw[1](canvas, (u8 *)sheet + Data_080ede48[size - 1],
                    ((s16 *)&particle->x)[1] - size / 2,
                    ((s16 *)&particle->y)[1] - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(particle, 62, 0x2000);
                particle->variant--;
            }
            particle++;
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
        angle -= 0x800;
    } while (frame != 96);

    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
