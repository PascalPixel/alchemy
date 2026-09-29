/* 2026-09-29: eight minutes of permutation found 1656 from 1936; the
 * natural reorders kept here (locals, cursor steps, the puff base as offset
 * plus work) score 1856. The rest of the 1656 candidate repeats the
 * particle-cursor assignment before i = 0 and spells the size-table read as
 * pointer arithmetic, which is not kept. Resource numbers 0x73, 0x8e, 0xb4
 * and 0xb7 are still Value_ symbols, and 0x02010000 is still a literal
 * address. */
/* Draft, complete main:080e08c0 [080e08c0,080e0c84), 964 bytes.
 * Baseline: 968 bytes, 307 differing halfwords, 147 aligned edits, frame 32.
 * H1: recover the puff/particle module using exact PUFF_ARC's work/argument
 * layout, EFFECT_STEP's 28-byte physics record, BATTLE_EFX's returning
 * rectangle pair and MEMBER_ORBIT's returning word-copy interface.
 * Own-ROM fixes: random angles use 0xffff (not the particle-buffer address);
 * frames 20..23 use blitter zero; the owner and canvas teardown return void.
 * The sprite-offset table is a mutable extern like the exact puff tables,
 * not a const-qualified global hoisted across unknown calls.
 * Prediction: preserve frame 32/callback slots, restore angle pool and
 * callback selection, remove trampoline pseudo-callee and cached table base.
 * Compare full owner, ordinary-C lint, compare/coverage/verify before credit.
 * H1 result: 964/964 bytes, 143 differing halfwords, 99 aligned edits,
 * equal topology and frame 32; first 91 instructions exact. Correct mask,
 * returning palette-copy setup, frame-20 blitter and void epilogue verified.
 * The sprite-offset base now reloads inside the draw loop as in ROM.
 * Residual: seed mask fp/r9 swap; shared particle pointer uses r5 in both
 * phases while ROM uses seed r5 / draw r6; actor precheck caches effect-cell
 * address early; final two-byte alignment pad is absent from the reference.
 * H2 admission: transfer the proven cc960 phase-local cursor model, splitting
 * seed and draw particle pointers without changing statements or declarations
 * within either phase. Require recovered draw r6 without losing frame/calls.
 * H2 result: byte-identical to H1 (964/964, 143 halfwords, 99 edits).
 * Splitting the seed pointer does not change allocator ancestry here;
 * reject it as a closing mechanism. Preserve the attempt before restoring
 * the simpler canonical H1 pointer model. Do not sweep local declarations.
 * H3: reference particle size is saved r5, but H1/H2 size is caller-saved
 * r0 and the draw cursor is r5. Test the actual call boundary: select the
 * cell before Trig_Sin, keeping its width live across the projection call.
 * Admission requires width r5 / cursor r6 with all calls and frame intact.
 * This is the final structural hypothesis, not a setup-order sweep.
 * H3 result: 984/964 bytes, 433 differing halfwords, 217 aligned edits.
 * Width r5 / draw cursor r6 is recovered, including the draw stack stores,
 * but member moves r7 -> r8 throughout the owner, rotating work/frame/puff
 * roles and adding 20 bytes. Reject globally; the result shows width is not
 * independently live across Trig_Sin in the reference source model.
 * Canonical H1 restored: 964/964, 143 halfwords, 99 edits; all witnesses
 * remain committed separately. Retain the semantic/interface corrections.
 * Three hypotheses complete. No credit and no further cursor/order sweeps.
 * H4: EffectStep-typed motes (one array, x/y as high halves, z as a phase)
 * and a returning-void word copier: 968/964 bytes, 135 aligned edits, worse
 * than H1; the motes also need a linker-placed name for their EWRAM buffer
 * and an Iwram_CopyWords entry. H1 stays canonical.
 */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

/* Same nine 28-byte puff records as PUFF_ARC.C, at work + 0x7080. */
struct Puff {
    s32 x;
    s32 y;
    s32 unused_08[4];
    s32 tick;
};

typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output);
void BattleFx_EndCanvasLayer(void);
void *Resource_GetTableEntry(s32 id);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 mask, u32 range);
void ObjectGroup_TickMemberTimers(void);

extern u16 Data_080ede48[];
extern u8 Data_080ede9f[];
extern u8 Data_080edea5[];
extern u8 Data_080edeab[];
extern u16 Data_080edeb2[];
extern u8 Value_00000073;
extern u8 Value_0000008e;
extern u8 Value_000000b7;
extern u8 Value_000000b4;

#define WORK_EFX ((struct BattleEffectArgument *)work->effect)

/* A rising curtain opens into nine staggered puffs, each releasing sixteen
 * particles. Particle z is an angle here; the 2-D physics step leaves it
 * untouched while advancing x/y and applying gravity. */
void Func_080e08c0(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *sheet;
    DrawRectangle callbacks[2];
    DrawRectangle *draw;
    struct Puff *puff;
    struct EffectStep *particle;
    s32 burst_offset;
    s32 member;
    s32 frame;
    s32 i;
    s32 curtain_y;

    heap_cache = (void **)0x03001eec;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    draw = callbacks;
    BattleFx_FetchRectangleBlitters(0, (u32 *)draw);

    Resource_LoadAndDecompress((s32)&Value_00000073, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_0000008e, work, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_000000b7, (u8 *)work + 0x320, 1, 1);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);

    burst_offset = 0;
    member = 0;
    puff = (struct Puff *)((u8 *)work + 0x7080);
    do {
        s32 angle = member << 11;

        puff->x = (Trig_Sin(angle) * 24) >> 16;
        puff->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
        if (member & 1)
            puff->x = 32 - puff->x;
        else
            puff->x += 32;
        puff->tick = -(member * 2);

        i = 0;
        particle = (struct EffectStep *)((u8 *)0x02010000 + burst_offset);
        {
            s32 mask = 0xffff;

            do {
                particle->x = (((Random16() & 15) + puff->x) - 8) << 16;
                particle->y = ((Random16() & 7) + 96) << 16;
                particle->velocity_x = ((Random16() & 127) - 64) << 11;
                particle->velocity_y = ((Random16() & 127) - 64) << 10;
                particle->z = Random16() & mask;
                particle->velocity_z = Random16() & mask;
                i++;
                particle++;
            } while (i != 16);
        }
        ++puff;
        member++;
        burst_offset += 16 * sizeof(struct EffectStep);
    } while (member != 9);

    Audio_PlayCue(0x88);
    frame = 0;
    curtain_y = -172;
    do {
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(0x85);
        if (frame <= 23) {
            callbacks[0](canvas, (u8 *)work + 0x320 + (frame / 4) * 0x640,
                40, 20, 40, 40);
        }
        if (frame == 20) {
            ((WordCopyFn)0x03001388)((void *)0x05000000,
                Resource_GetTableEntry((s32)&Value_0000008e), 128);
        }
        if ((u32)(frame - 20) <= 11) {
            if (frame > 23)
                draw[1](canvas, work, 146 - frame * 4, curtain_y, 20, 40);
            else
                callbacks[0](canvas, work, 50, 20, 20, 40);
        }
        if (frame == 32) {
            Audio_PlayCue(0x91);
            *(s32 *)((u8 *)work + 0x77a8) = 8;
            Resource_LoadAndDecompress((s32)&Value_000000b4, work, 1, 1);
        }
        if (frame > 31) {
            member = 0;
            puff = (struct Puff *)(0x7080 + (u8 *)work);
            do {
                if (puff->tick >= 0 && puff->tick <= 47) {
                    s32 cell = puff->tick / 8;
                    u32 width;

                    callbacks[0](canvas, (u8 *)work + Data_080edeb2[cell],
                        puff->x - ((width = Data_080ede9f[cell]) >> 1),
                        puff->y + Data_080edeab[cell],
                        width, Data_080edea5[cell]);
                }
                member++;
                puff->tick++;
                puff++;
            } while (member != 9);
        }

        particle = (struct EffectStep *)0x02010000;
        member = 0;
        do {
            if (frame >= (member / 16) * 2 + 40) {
                s32 size;
                s32 x;
                s32 angle;
                x = ((s16 *)&particle->x)[1]
                    + ((Trig_Sin(particle->z) * 4) >> 16);
                size = (member & 1) + 3;
                draw[1](canvas, (u8 *)sheet + Data_080ede48[size - 1],
                    x - ((u32)size >> 1),
                    ((s16 *)&particle->y)[1] - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(particle, 64, -0x2000);
                angle = particle->z;
                particle->z = angle + 0x800;
                if (particle->z > 0xffff)
                    particle->z = angle + 0xffff0801;
            }
            member++;
            particle++;
        } while (member != 144);

        if (frame == 38) {
            member = 0;
            if (WORK_EFX->count != 0) {
                do {
                    ObjectGroup_UpdateMembers(WORK_EFX->actors[member],
                        7, 5, member, 16);
                    BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[member], 6);
                    member++;
                } while (member != WORK_EFX->count);
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        curtain_y += 8;
        frame++;
    } while (frame != 112);
    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
