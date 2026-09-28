#include "TYPES.H"
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
extern u16 Data_080edebe[];

extern u8 Value_00000073, Value_000000ce, Value_000000c4;
extern u8 Data_080eebd6[];      /* per-group [4] byte table: gate/count selectors */
extern u8 Data_080edeca[];      /* per-mask byte table (announce geometry A) */
extern u8 Data_080eded0[];      /* per-mask byte table (announce geometry B) */
extern u8 Data_080eebe2[];     /* random draw-mode flags */
extern u8 Data_080eebe6[];      /* effect-variant draw selector */
extern u16 Data_080ede48[];     /* per-step halfword table (shared w/ 080d82b0.c) */

/* The particle draw uses the kind-46 callback cached at sp+32. The fill
 * routine is the entry at 0x03000168 itself, not a pointer stored there.
 * Each kind-47 draw uses the freshly allocated callback. */

void Func_080ddde0(struct BattleEffectArgument *table_param)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *draw_destination;
    s32 member_offset;
    s32 member_field_offset;
    struct BattleEffectArgument *table;
    void *extra_target;
    DrawRectangleFn draw_cb_46;
    s32 pass;
    s32 member_index;
    s32 cnt;
    struct EffectPosition pos;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    work->effect = table_param;

    BattleFx_BeginCanvasLayer(1);
    (void) BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_cb_46 = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    Resource_LoadAndDecompress((s32)&Value_000000ce, work, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_000000c4, (u8 *)work + 0xc56, 1, 1);
    Resource_LoadAndDecompress((s32) &Value_00000073, extra_target, 0, 0);

    {
        s32 *slot;

        slot = &((struct EffectStep *)0x02010000)->variant;
        cnt = 0;
        do {
            cnt++;
            *slot = 0;
            slot = (s32 *)((u8 *)slot + sizeof(struct EffectStep));
        } while (cnt != 1024);
    }

    {
        s32 *slot;

        slot = &work->particles[0].variant;
        cnt = 0;
        do {
            cnt++;
            *slot = -1;
            slot = (s32 *)((u8 *)slot + sizeof(struct EffectStep));
        } while (cnt != 64);
    }

    work->transfer_mode = 2;
    work->transfer_value = 75;

    Scheduler_AddOrUpdateCallback(0x080cd261, 144 << 3);
    Audio_PlayCue(138);

    pass = 0;
    while (pass != work->effect->count * 8 + 40) {
        if (pass == 24)
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
        while (member_index != work->effect->count) {
            s32 base;
            s32 group;
            base = member_index * 8;
            table = work->effect;
            EffectPosition_ApplyAlternateStepAndYOffset(*(s16 *)((u8 *)table + member_field_offset), &pos);
            pos.x /= 2;
            if (pass == base + 1)
                work->shake_frames = 4;
            if (pass == base + 4) {
                table = work->effect;
                ObjectGroup_UpdateMembers(*(s16 *)((u8 *)table + member_field_offset), 7, 5, member_index, 6);
                table = work->effect;
                BattleMotion_ApplyVariantMotionFar(*(s16 *)((u8 *)table + member_field_offset), 6);
            }
            if (pass >= base && pass < base + 16) {
                s32 height;
                height = (pass - base) << 6;
                if (height > 104) height = 104;
                cnt = 0;
                table = work->effect;
                group = table->variant;
                while (cnt != Data_080eebd6[group * 4 + 3]) {
                    s32 cell;
                    cell = ((member_index + pass + cnt) / 2) & 3;
                    draw_cb_46(draw_destination, (u8 *)work + 0xc56 + cell * 2880,
                        pos.x - 12, 0, 24, height);
                    cnt++;
                    table = work->effect;
                    group = table->variant;
                }
                if (pass == base + 2) {
                    struct EffectStep *particle;
                    particle = (struct EffectStep *)(0x02010000 + member_offset);
                    cnt = 0;
                    table = work->effect;
                    group = table->variant;
                    while (cnt != Data_080eebd6[group * 4]) {
                        s32 magnitude, angle, value;
                        magnitude = Random16() & 0x1ff;
                        angle = Random16();
                        particle->x = pos.x << 16;
                        angle = (angle & 0x7fff) - 0x4000;
                        particle->y = 208 << 15;
                        value = Trig_Sin(angle);
                        magnitude += 64;
                        particle->velocity_x = (magnitude * value) >> 5;
                        value = Trig_Cos(angle);
                        particle->velocity_y = -(magnitude * value) >> 6;
                        particle->variant = (Random16() & 7) + 32;
                        cnt++;
                        particle++;
                        table = work->effect;
                        group = table->variant;
                    }
                }
            }
            if (pass >= base + 2 && pass < base + 24) {
                cnt = 0;
                table = work->effect;
                group = table->variant;
                while (cnt != Data_080eebd6[group * 4 + 1]) {
                    s32 mask, range, offset, x, y, random;
                    DrawRectangleFn draw47;
                    mask = cnt & 3;
                    random = Random16();
                    table = work->effect;
                    range = Data_080eebd6[table->variant * 4 + 2];
                    offset = Math_ModU(random, range);
                    y = pos.y - offset;
                    range -= offset;
                    y = y - Data_080eded0[mask] / 2 + 8;
                    random = Random16();
                    range++;
                    x = pos.x + Math_ModU(random, range);
                    x -= range / 2;
                    x -= Data_080edeca[mask] / 2;
                    random = Random16();
                    table = work->effect;
                    BattleEffect_LoadWork(47, 7, 7, 3 | Data_080eebe2[random & 3],
                        Data_080eebe6[table->variant]);
                    draw47 = *(DrawRectangleFn *)0x03001f0c;
                    draw47(draw_destination, (u8 *)work + Data_080edebe[mask],
                        x, y, Data_080edeca[mask], Data_080eded0[mask]);
                    Runtime_ReleaseHeapBlock(47);
                    cnt++;
                    table = work->effect;
                    group = table->variant;
                }
            }
            member_field_offset += 2;
            member_offset += 0xe00;
            member_index++;
        }
        {
            struct EffectStep *slot3;

            slot3 = (struct EffectStep *)0x02010000;
            for (cnt = 0; cnt != 1024; cnt++, slot3++) {
                s32 life = slot3->variant;

                if (life <= 0) continue;

                slot3->variant = life - 1;
                EffectStep_AdvanceWithGravity2D(slot3, 60, 128 << 5);

                if (slot3->y > (208 << 15)) {
                    s32 vel = slot3->velocity_y;
                    slot3->velocity_y = -vel / 2;
                    continue;
                }

                if ((u32) slot3->x <= 0x7effffu && slot3->y >= 0) {
                    s32 step = slot3->variant;

                    if (step < 0) step += 15;
                    step = (step >> 4) + 1;

                    {
                        const u8 *source = (const u8 *)extra_target
                            + Data_080ede48[step - 1];
                        s32 half = step / 2;
                        s32 x = (slot3->x >> 16) - half;
                        s32 y = (slot3->y >> 16) - step;

                        draw_cb_46(draw_destination, source, x, y,
                            step, step * 2);
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
        if (pass == table->count * 8 + 40) {
            break;
        }
    }

    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
