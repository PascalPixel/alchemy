#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

/*
 * Draft for the battle-presentation sub-effect at 0x080d82b0.
 * Whole owner: [080d82b0, 080d85d0), 800 bytes including its pool.
 * The mode dispatcher calls this as a void effect (table entry 42,
 * effect 43); the reference allocates two separate three-word position
 * records at stack +32 and +44 in a 56-byte frame.  The projection
 * callee writes all three words, so the old two-word screen was invalid.
 * The palette transfer is an ordinary indirect call to the IWRAM word
 * copier at 03001388, with a byte count, not a four-argument veneer.
 *
 * 2026-09-26 bounded reconstruction, hypothesis 1: recover the two
 * position records, indirect copy and void effect/cleanup contracts,
 * and the resource-ID link constants.  Candidate 792/800 bytes,
 * 352 differing halfwords, 202 aligned instruction edits; frame 60/56.
 * Baseline was 784 bytes, 349 halfwords and 206 aligned edits.  The
 * correct records expose an extra hoisted effect-slot pointer spill:
 * reference work/destination/callback are +28/+24/+20, candidate
 * +32/+28/+20.  Separate clear, seed and draw counters also disagree
 * with the reference's shared r8 counter.  No new credited bytes.
 * Hypothesis 2: share the clear/seed/draw counter and express the seed
 * member backedge explicitly.  Candidate 820/800 bytes, 345 differing
 * halfwords, 208 aligned edits, equal topology.  The common r8 counter
 * now matches all three loops and the effect-slot spill is gone, but
 * hoisting 255 into r9 displaces the member counter onto the stack;
 * frame remains 60.  World/screen occupy fp/sl instead of r9/sl.
 * This is an independently preserved intermediate, not an adoption.
 * Hypothesis 3: explicit particle backedges plus shared work/argument
 * records.  Candidate 812/800 bytes, 346 differing halfwords, 207
 * aligned edits, equal topology; frame still 60/56.  The indexed actor
 * load and per-draw table reload are recovered.  The mask is now formed
 * inside the seed loop but CSE still carries it across calls in r8,
 * displacing the shared counter to sl and leaving member at stack +16.
 * Remaining structural work: eliminate that member spill without a
 * retained mask, recover the reference's incremented rectangle-size
 * index/table lookup, and place the tick clear after the fourth random
 * call.  Also retain the known multiply operand and copy-call schedule
 * differences.  Three hypotheses used; stop, no new credited bytes.
 *
 * Assigned from the orbiting_particles/run.c compiler-family cluster
 * (template-main-08099160); like its siblings 080d59b0/080dc1ec/080e01e4
 * this owner's real callee set and constants match the 0x03001eec "battle
 * work" subsystem already partly recovered at recon/tbs/en/main/
 * 080e7404.c and 080d59b0.c, not the assigned template.  See those files
 * for the evidence behind the field/signature choices below.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
s32 Battle_GetObjectTableValueFar(s32 member_id);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
void BattleFx_EndCanvasLayer(void);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 __modsi3(s32 a, s32 b);
void EffectStep_AdvanceWithGravity2D(void *particle, s32 count, s32 flags);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);

extern const u16 ParticleStreams_CellOffsets[];
extern const s32 Data_080ee9f8[];

void Unnamed_080d82b0(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *draw_destination;
    void *extra_target;
    DrawRectangleFn draw_rectangle_fn;
    s32 facing;
    struct BattleEffectArgument *target;
    s32 *pool_cursor;
    s32 pool_index;
    s32 outer;
    s32 member;
    s32 member_offset;
    s32 member_id_offset;
    s32 particle_base;
    s32 result0;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    facing = *(s32 *)((u8 *)heap_cache - 108);
    work->effect = object;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, extra_target, 0, 0);
    ((WordCopyFn)0x03001388)(
        (void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_rectangle_fn = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    pool_cursor = (s32 *)0x02010018;
    pool_index = 0;
    do {
        pool_index++;
        *pool_cursor = -1;
        pool_cursor += 7;
    } while (pool_index != 1024);

    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(facing, facing + 12);

    target = work->effect;
    member = 0;
    if (target->count != 0) {
        s32 sp44[3];
        struct EffectPosition screen;
        s32 *sp44_ptr;
        s32 *sp32_ptr;

        sp44_ptr = sp44;
        sp32_ptr = &screen.x;
        member_id_offset = 36;
        member_offset = 0;
seed_member:
        {
            void *member_ptr;
            s32 member_id;
            s32 *particle;

            target = work->effect;
            member_id = M2C_FIELD(target, s16 *, member_id_offset);
            member_ptr = *GetBattleObjectSlotFar(member_id);
            target = work->effect;
            member_id = M2C_FIELD(target, s16 *, member_id_offset);
            result0 = Battle_GetObjectTableValueFar(member_id);
            result0 = result0 / 2;
            sp44_ptr[0] = M2C_FIELD(member_ptr, s32 *, 8);
            sp44_ptr[1] = result0;
            sp44_ptr[2] = M2C_FIELD(member_ptr, s32 *, 16);
            EffectPosition_ApplyBaseAndYOffset(sp44_ptr, &screen);
            sp32_ptr[0] = sp32_ptr[0] >> 1;

            particle_base = member_offset;
            particle = (s32 *)((u8 *)0x02010000 + particle_base);
            pool_index = 0;
seed_particle:
            {
                u32 kind;
                u32 angle;
                s32 sin_val;
                s32 cos_val;

                kind = Random16() & 0xFF;
                angle = Random16() & 0xFFFF;
                sin_val = Trig_Sin((s32) angle);
                *(s32 *)particle =
                    (s32) (((s32) (kind * sin_val) >> 7)
                        + (sp32_ptr[0] << 16));
                cos_val = Trig_Cos((s32) angle);
                *(s32 *)((u8 *)particle + 4) =
                    (s32) (((s32) (kind * cos_val) >> 3)
                        + (sp32_ptr[1] << 16));
                *(s32 *)((u8 *)particle + 12) =
                    (s32) ((128 - (s32) (Random16() & 0xFF)) << 9);
                *(s32 *)((u8 *)particle + 24) = 0;
                *(s32 *)((u8 *)particle + 16) =
                    (s32) ((-(s32) (Random16() & 0xFF) - 128) << 10);
                particle = (s32 *)((u8 *)particle + 28);
                pool_index++;
            }
            if (pool_index != 128)
                goto seed_particle;

            member_id_offset += 2;
            member_offset += 0xE00;
            target = work->effect;
            member++;
        }
        if (member != target->count)
            goto seed_member;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    target = work->effect;
    if (target->count * 20 != -56) {
        outer = 0;
        do {
            s32 pass_count;

            if (outer == 32) {
                BattleEventRuntime_BeginPhaseFar(0);
            }
            target = work->effect;
            pass_count = target->count;
            member = 0;
            if (pass_count != 0) {
                s32 stagger;
                s32 offset;

                stagger = 0;
                offset = 0;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        Audio_PlayCue(143);
                        target = work->effect;
                        member_id = target->actors[member];
                        ObjectGroup_UpdateMembers(member_id, 7, -1, member, 20);
                    }
                    if (outer > stagger) {
                        s32 *particle;

                        particle = (s32 *)((u8 *)0x02010000 + offset);
                        pool_index = 0;
draw_particle:
                        {
                            if (*(s32 *)((u8 *)particle + 24) >= 0) {
                                s32 raw;
                                s32 idx;
                                s32 half;
                                s32 y;
                                s32 h;

                                raw = __modsi3(pool_index, 3);
                                idx = raw + 1;
                                half = idx / 2;
                                y = *(s16 *)((u8 *)particle + 2) - half;
                                h = *(s16 *)((u8 *)particle + 6) - idx;
                                draw_rectangle_fn(
                                    draw_destination,
                                    (u8 *) extra_target
                                        + ParticleStreams_CellOffsets[raw],
                                    y, h, idx, idx * 2);
                                EffectStep_AdvanceWithGravity2D(
                                    particle, 62,
                                    Data_080ee9f8[pool_index & 3]);
                                *(s32 *)((u8 *)particle + 24) += 1;
                                if (*(s32 *)((u8 *)particle + 16) > 0
                                    && *(s16 *)((u8 *)particle + 6) > 112) {
                                    *(s32 *)((u8 *)particle + 24) = -1;
                                }
                            }
                            particle = (s32 *)((u8 *)particle + 28);
                            pool_index++;
                        }
                        if (pool_index != 128)
                            goto draw_particle;
                    }
                    member++;
                    stagger += 20;
                    offset += 0xE00;
                    target = work->effect;
                } while (member != target->count);
            }

            ObjectGroup_TickMemberTimers();
            work->transfer_pending = 1;
            WaitFrames(1);

            target = work->effect;
            outer++;
        } while (outer != target->count * 20 + 56);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
