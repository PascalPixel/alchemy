#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "FIXED_MATH.H"

/* Three projected trail points form a closed triangle. Each edge receives
 * 24 interpolated particles, followed by a split 24x48 image. The optional
 * mode-1 image uses its own callback pair before the per-frame allocation.
 * Draft, not-yet-C: complete 1240-byte extent, pools included, is nonexact.
 * 2026-09-26 baseline: 1218 bytes, 128-byte frame versus reference 132,
 * equal topology, 386 aligned halfword edits (566 offset differences).
 * Shared EffectPosition stack/point records: 1212 bytes, still frame 128,
 * 402 aligned edits (560 offset differences). This type change did not
 * admit the required frame or stop induction-variable address replacement.
 * Indexed TriangleWork records and pre-division coordinate snapshots recover
 * frame 132: 1232 bytes, equal topology, 353 aligned edits (562 offset
 * differences). Coordinate capture now agrees with the reference; grouped
 * stack records still keep a common base instead of the screen pointer.
 * Distinct typed stack records retain frame 132 and offsets 84/96/108/120:
 * 1236 bytes, equal topology, 319 aligned edits (543 offset differences).
 * Screen is still spilled at +20 instead of retained in r9; angle takes r9.
 * The compiler still replaces the point-base index with address induction
 * and retains a different spill layout. This is a broad residual, not just
 * register choice. Stop after three structural hypotheses; no adoption,
 * declaration/operand permutation sweep, compiler change or byte credit.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 20,552
 * candidates; the best scored 6049 against 8034 (85 register-only, 29
 * stack-only, 30 operand, 36 reordered, 13 inserted, 14 deleted) after 52
 * rewrites (reorder independent statements, swap commutative operands,
 * reorder local declarations, test truth or compare with zero), none of
 * them kept. Statement moves and operand swaps across the function; its
 * resource numbers are pooled link-time Value_ symbols (0x79 and 0x8f are
 * not even in CONSTANTS.LD, and as plain numbers they would become movs),
 * which block adoption in any case.
 */

typedef void (*WordCopy)(void *, const void *, s32);
typedef struct Effect {
    s32 kind, side, actor, unknown0c, unknown10, count, layers, mode, unknown20;
    s16 actors[8];
} Effect;
typedef struct TrailPoint {
    s32 unused[3];
    struct EffectPosition position;
    s32 unused18;
} TrailPoint;
struct TriangleWork {
    u8 unknown_0000[0x7080];
    TrailPoint points[32];
    u8 unknown_7400[0x380];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x9c];
    s32 transfer_pending;
    Effect *effect;
};
extern u8 gWorkSlot[];
extern u16 BattleFx6_FlareCells[];
extern s32 Data_080ee128[];
void BattleFx_BeginCanvasLayer(s32);
void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);
u8 *Resource_GetTableEntry(s32);
void Resource_DecodeType01(const void *, void *);
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void Audio_PlayCue(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void Graphics_UpdatePhasePalette(s32, s32, s32, s32);
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
void BattleFx_FetchRectangleBlitters(s32, DrawRectangle *);
void Runtime_ReleaseHeapBlock(s32);
void **GetBattleObjectSlotFar(s32);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(const void *, const void *);
void SceneTransform_ApplyScale(const s32 *);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyYaw(s32);
void WaitFrames(s32);
void Scheduler_RemoveCallback(s32);
s32 BattleFx_EndCanvasLayer(void);
#define WORK_EFFECT (work->effect)

void Func_080d05fc(Effect *effect)
{
    u32 *cache, *entry;
    struct TriangleWork *work;
    u8 *sheet, *resource, *camera;
    void *dst;
    DrawRectangle draw[2];
    s32 origin_x, origin_y, shift;
    struct EffectPosition anchor;
    struct EffectPosition vector, screen, scale;
    s32 frame, member, tick, point_base, scale_phase;
    s32 x, y, radius, width, angle, scale_value, rotation;
    s32 point, sample;
    TrailPoint *trail, *from, *to;
    s32 *actor;

    cache = (u32 *)(gWorkSlot + 39 * 4);
    entry = cache;
    work = (struct TriangleWork *)*entry++;
    dst = (void *)*entry;
    camera = (u8 *)cache[-27];
    sheet = (u8 *)cache[2];
    WORK_EFFECT = effect;
    BattleFx_BeginCanvasLayer(1);
    if (WORK_EFFECT->mode == 1)
        BattleFx_PrepareCanvasEffect(effect, 3, WORK_EFFECT->side, 0, &origin_x, &origin_y);
    *(s16 *)0x04000020 = 0x100;
    resource = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    ((WordCopy)0x03001388)((void *)0x05000000, resource, 128);
    Resource_DecodeType01(resource + 128, work);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), (u8 *)work + 0x1000);
    resource = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    Resource_DecodeType01(resource + 128, (u8 *)work + 0x2000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);
    EffectPosition_ApplyStepAndYOffset(
        WORK_EFFECT->actors[0], &anchor);
    shift = 64 - anchor.x;
    *(s32 *)0x04000028 = shift << 8;
    Audio_PlayCue(142);
    frame = 0;
    while (frame != WORK_EFFECT->count * 20 + 72) {
        if (frame == 64) BattleEventRuntime_BeginPhaseFar(0);
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (WORK_EFFECT->mode == 1) {
            x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
            y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin_y;
            BattleFx_FetchRectangleBlitters(WORK_EFFECT->side, draw);
            y -= 24;
            if (frame > 32) y = y - (frame << 1) + 64;
            draw[0](dst, (u8 *)work + 0x2000, x, y, 40, 40);
            if (frame <= 3) draw[1](dst, (u8 *)work + 0x2000, x, y, 40, 40);
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((u32 *)gWorkSlot)[46];
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
        draw[1] = (DrawRectangle)((u32 *)gWorkSlot)[47];
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        member = 0;
        point_base = 0;
        scale_phase = frame * 3 << 9;
        tick = frame;
        do {
            actor = (s32 *)*GetBattleObjectSlotFar(WORK_EFFECT->actors[member]);
            if ((u32)tick <= 95) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera, camera + 12);
                vector.x = actor[2];
                vector.y = actor[3];
                vector.depth = actor[4];
                EffectPosition_ApplyBaseAndYOffset(
                    (s32 *)&vector, &screen);
                screen.x = anchor.x + shift;
                screen.y -= 24;
                if (tick <= 67) {
                    angle = 0;
                    scale_value = 0x2a000 - scale_phase;
                    rotation = (64 - tick) << 9;
                    point = 0;
                    do {
                        Render_ResetTransformState();
                        if (tick <= 63) {
                            scale.x = scale_value;
                            scale.y = scale_value;
                            scale.depth = scale_value;
                            SceneTransform_ApplyScale((s32 *)&scale);
                            SceneTransform_ApplyRoll(rotation);
                            SceneTransform_ApplyYaw(rotation);
                        }
                        SceneTransform_ApplyRoll(angle);
                        EffectPosition_ApplyBaseAndYOffset(
                            Data_080ee128, &vector);
                        trail = &work->points[point_base + point];
                        trail->position.x = vector.x + screen.x;
                        trail->position.y = vector.y + screen.y + 16;
                        point++; angle += 0x5555;
                    } while (point != 3);
                    point = 0;
                    do {
                        from = &work->points[point_base + point];
                        point++;
                        to = &work->points[point_base + __modsi3(point, 3)];
                        radius = 5 - tick / 16;
                        sample = 0;
                        width = radius << 1;
                        do {
                            x = from->position.x;
                            x += __divsi3(sample * (to->position.x - x), 24);
                            y = from->position.y;
                            y += __divsi3(sample * (to->position.y - y), 24);
                            x -= radius; y -= radius;
                            draw[0](dst, (u8 *)work + BattleFx6_FlareCells[radius - 1] + 0x1000, x, y, width, width);
                            sample++;
                        } while (sample != 24);
                    } while (point != 3);
                }
                if (tick > 63) {
                    draw[0](dst, work, screen.x - 24, screen.y - 24, 24, 48);
                    draw[1](dst, work, screen.x, screen.y - 24, 24, 48);
                }
            }
            scale_phase -= 0x3000; point_base += 32; member++; tick -= 8;
        } while (member != 1);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        work->transfer_pending = member;
        WaitFrames(1);
        frame++;
    }
    Scheduler_RemoveCallback(0x080cd261);
    BattleFx_EndCanvasLayer();
}
