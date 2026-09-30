/* 2026-09-29 alchemy permute: score 6006 to 4582 on the permuter's scorer
   (0 is exact); remaining 135 register-only, 32 stack-only, 28 operand, 27
   reordered, 8 inserted, 8 deleted. Kept rewrites: 21x reorder independent
   statements, 15x swap commutative operands, 14x change loop form, 13x
   introduce a temporary, 12x test truth or compare with zero, 11x reorder
   local declarations, 10x move an assignment into or out of a condition,
   8x pointer arithmetic or indexing, 7x remove a temporary, 6x add a
   same-width cast, 4x toggle register, 3x drop a same-width cast, 2x split
   or join a compound assignment. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"

/* NONMATCHING: complete 1304-byte staggered burst effect, including both
 * literal pools. Score with --size 1304: the discovery head is only 684
 * bytes. Candidate 1308 bytes; 544 differing halfwords / 353 aligned edits.
 * Reuse the effect structs and correct the missing transfer_mode = 2.
 * Independent counters gave 1296 bytes / 366 edits; a shared counter and
 * pointers to the life fields recover the reference's initialization shape.
 * The 60-byte frame still differs from the reference's 64-byte frame, and
 * counter allocation and randomized burst spills remain unresolved. */

typedef void (*FillWordsFn)(void *dest, s32 size, u32 fill);

void BattleFx_BeginCanvasLayer(s32 mode);
void Runtime_ReleaseHeapBlock(s32 id);
void BattleFx_EndCanvasLayer(void);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 b);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *pos);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
u32 Math_ModU(u32 value, u32 bound);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Camera_ApplyShake(s32 a, s32 b);
extern u16 BattleFx_GlintCellOffsets[];

extern u8 Data_080eebd6[];      /* per-group [4] byte table: gate/count selectors */
extern u8 BattleFx_GlintCellWidths[];      /* per-mask byte table (announce geometry A) */
extern u8 BattleFx_GlintCellHeights[];      /* per-mask byte table (announce geometry B) */
extern u8 Data_080eebe2[];     /* random draw-mode flags */
extern u8 Data_080eebe6[];      /* effect-variant draw selector */
extern u16 ParticleStreams_CellOffsets[];     /* per-step halfword table (shared w/ 080d82b0.c) */

/* The particle draw uses the kind-46 callback cached at sp+32. The fill
 * routine is the entry at 0x03000168 itself, not a pointer stored there.
 * Each kind-47 draw uses the freshly allocated callback. */

void Region_080ddde0(struct BattleEffectArgument *table_param)
{
    void *draw_destination;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    s32 member_offset;
    s32 member_field_offset;
    struct BattleEffectArgument *table;
    DrawRectangleFn draw_cb_46;
    void *extra_target;
    s32 pass;
    s32 cnt;
    struct EffectPosition pos;
    register s32 member_index;
    u8 *tmp3;
    void *tmp8;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    tmp8 = *cursor++;
    work = tmp8;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    work->effect = table_param;
    BattleFx_BeginCanvasLayer(1);
    (void)BattleEffect_LoadWork(46, 7, 7, 3, 2);
    tmp3 = (u8 *)heap_cache;
    draw_cb_46 = *(DrawRectangleFn *)(tmp3 + 28);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, (u8 *)work + 0xc56, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, extra_target, 0, 0);
    {
        s32 *slot;
        cnt = 0;
        slot = &((struct EffectStep *)0x02010000)->variant;
        while (1) {
            *slot = 0;
            cnt++;
            slot = (s32 *)((u8 *)slot + sizeof(struct EffectStep));
            if (1024 == cnt)
                break;
        }
    }
    {
        s32 *slot;
        slot = &work->particles[0].variant;
        cnt = 0;
        do {
            *slot = -1;
            cnt++;
            slot = (s32 *)((u8 *)slot + sizeof(struct EffectStep));
        } while (64 != cnt);
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(0x080cd261, 144 << 3);
    Audio_PlayCue(138);
    pass = 0;
    if (pass != work->effect->count * 8 + 40) {
        do {
            if (24 == pass)
                BattleEventRuntime_BeginPhaseFar(133);
            member_index = 0;
            while (member_index != work->effect->count) {
                if (pass == member_index * 8)
                    ((FillWordsFn)0x03000168)(draw_destination, 0x4000, 0x10101010);
                member_index++;
            }
            member_index = 0;
            member_field_offset = 36;
            member_offset = 0;
            for (; member_index != work->effect->count; member_index++) {
                s32 group;
                s32 base;
                struct BattleEffectArgument *tmp6;
                tmp6 = work->effect;
                table = tmp6;
                base = member_index * 8;
                EffectPosition_ApplyAlternateStepAndYOffset(*(s16 *)((u8 *)table + member_field_offset), &pos);
                pos.x = pos.x / 2;
                if (pass == base + 1)
                    work->shake_frames = 4;
                if (pass == base + 4) {
                    table = work->effect;
                    ObjectGroup_UpdateMembers(*(s16 *)(member_field_offset + (u8 *)table), 7, 5, member_index, 6);
                    table = work->effect;
                    BattleMotion_ApplyVariantMotionFar(*(s16 *)((u8 *)table + member_field_offset), 6);
                }
                if ((u32)(pass >= base) && pass < base + 16) {
                    s32 height;
                    if (104 < (height = (pass - base) << 6))
                        height = 104;
                    table = work->effect;
                    cnt = 0;
                    for (group = table->variant; cnt != Data_080eebd6[group * 4 + 3]; cnt++) {
                        s32 tmp9;
                        tmp9 = (member_index + pass + cnt) / 2;
                        draw_cb_46(draw_destination, (u8 *)work + 0xc56 + (tmp9 & 3) * 2880, pos.x - 12, 0, 24, height);
                        table = work->effect;
                        group = table->variant;
                    }
                    if (pass == base + 2) {
                        struct EffectStep *particle;
                        u8 *tmp4;
                        table = work->effect;
                        particle = (struct EffectStep *)(0x02010000 + member_offset);
                        group = table->variant;
                        cnt = 0;
                        tmp4 = Data_080eebd6;
                        if (cnt != tmp4[group * 4]) {
                            do {
                                s32 tmp2;
                                s32 angle, magnitude, value;
                                magnitude = Random16() & 0x1ff;
                                angle = Random16();
                                particle->x = pos.x << 16;
                                tmp2 = (angle & 0x7fff) - 0x4000;
                                angle = tmp2;
                                particle->y = 208 << 15;
                                value = Trig_Sin(angle);
                                magnitude += 64;
                                particle->velocity_x = (value * magnitude) >> 5;
                                value = Trig_Cos(angle);
                                particle[0].velocity_y = -(value * magnitude) >> 6;
                                particle->variant = (7 & Random16()) + 32;
                                table = work->effect;
                                particle++;
                                cnt++;
                                group = table->variant;
                            } while (cnt != Data_080eebd6[group * 4]);
                        }
                    }
                }
                if (pass >= base + 2 && pass < base + 24) {
                    table = work->effect;
                    cnt = 0;
                    if (cnt != Data_080eebd6[(group = table->variant) * 4 + 1]) {
                        do {
                            register s32 mask, range, offset, x, y, random;
                            DrawRectangleFn draw47;
                            s32 tmp;
                            s32 tmp5;
                            u8 tmp7;
                            mask = 3 & cnt;
                            random = (s32)Random16();
                            table = work->effect;
                            range = Data_080eebd6[table->variant * 4 + 2];
                            offset = Math_ModU(random, range);
                            y = pos.y - offset;
                            range -= offset;
                            y = y - BattleFx_GlintCellHeights[mask] / 2 + 8;
                            cnt++;
                            random = Random16();
                            range++;
                            x = pos.x + Math_ModU(random, range);
                            x -= range / 2;
                            tmp7 = BattleFx_GlintCellWidths[mask];
                            x = x - tmp7 / 2;
                            random = Random16();
                            table = work->effect;
                            tmp5 = 3 & random;
                            BattleEffect_LoadWork(47, 7, 7, 3 | Data_080eebe2[tmp5], *(Data_080eebe6 + table->variant));
                            draw47 = *(DrawRectangleFn *)0x03001f0c;
                            draw47(draw_destination, (u8 *)work + BattleFx_GlintCellOffsets[mask], x, y, *(mask + BattleFx_GlintCellWidths), BattleFx_GlintCellHeights[mask]);
                            Runtime_ReleaseHeapBlock(47);
                            table = work->effect;
                            tmp = table->variant;
                            group = tmp;
                        } while (cnt != Data_080eebd6[group * 4 + 1]);
                    }
                }
                member_field_offset += 2;
                member_offset += 0xe00;
            }
            {
                struct EffectStep *slot3;
                slot3 = (struct EffectStep *)0x02010000;
                for (cnt = 0; 1024 != cnt; cnt++, slot3++) {
                    s32 life = slot3->variant;
                    if (life <= 0)
                        continue;
                    slot3[0].variant = life - 1;
                    EffectStep_AdvanceWithGravity2D(slot3, 60, 128 << 5);
                    if (slot3->y > (208 << 15)) {
                        slot3->velocity_y = -slot3->velocity_y / 2;
                        continue;
                    }
                    if ((u32)slot3->x <= 0x7effffu && 0 <= slot3->y) {
                        s32 step = slot3->variant;
                        if (step < 0)
                            step += 15;
                        step = 1 + (step >> 4);
                        {
                            const u8 *source = (const u8 *)extra_target + ParticleStreams_CellOffsets[step - 1];
                            s32 x = (slot3->x >> 16) - step / 2;
                            s32 y = (slot3->y >> 16) - step;
                            draw_cb_46(draw_destination, source, x, y, step, step * 2);
                        }
                    }
                }
            }
            Camera_ApplyShake(2, 8);
            ObjectGroup_TickMemberTimers();
            work->transfer_pending = 1;
            WaitFrames(1);
            pass++;
            table = work->effect;
            if (pass == table->count * (s32)8 + 40) {
                break;
            }
        } while (pass != 8 * work->effect->count + 40);
    }
    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
