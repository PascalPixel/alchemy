/* DRAFT: OrbitingSparks unused-blitter-slot audit. Complete 616-byte extent,
 * including both literal pools, is now uncredited in all six editions.
 * The consumed scalar keeps all instructions/registers/order; score 30:
 * ten stack operands and the projected-position offset differ. Frame 84
 * instead of 88; the caller and the local record sizes remain unchanged.
 *
 * Own EN ROM at 080dc454 stores draw at sp+36 and never touches sp+40.
 * The frame counter is at 44, canvas at 48, pos at 52, center at 64 and
 * record at 76. No callee is given the unused word or a blitter-pair address.
 * Only heap slot 46 is built and released. The single draw call site loads
 * the saved heap[7] pointer. The callback gets a function and priority only.
 * BattleEffect_LoadWork allocates its own heap block without a local pointer.
 *
 * All three coordinate buffers have real consumers: record feeds
 * EffectPosition_ApplyBaseAndYOffset, center receives its three-word result,
 * and pos receives EffectPosition_ApplyStepAndYOffset. Both routes end at
 * Render_ProjectPoint/resident IwramTransformVector's three-word contract.
 * center's result is adjusted and then discarded by the native function;
 * that native computation stays, unlike inventing a second unused blitter.
 * Neighbor RunTargetBursts really creates and consumes two blitters; it
 * remains credited in the existing module and proves nothing about this one.
 *
 * T0 scalar: 310; T1 one-element array: 310; T2 consumed canvas/draw cache:92.
 * T3 aggregate record/center/pos:1591; T4 center/pos pair:1546;
 * T5 buffers scoped to member loop:310. No reserved member was introduced.
 * T6-T10 final setup order (draw, mode, transfer value):
 * 021:400; 102:255; 120:30; 201:395; 210:270. Retain T8 order120.
 * T11 pos/center pair with T8 setup:499.
 * T12-T17 aggregate field order (record, center, pos), all consumed:
 * 012:1447; 021:1447; 102:1403; 120:1402; 201:499; 210:499.
 * Some aggregates naturally use 88 bytes but add a live pointer spill and
 * change the register/instruction layout. They do not recover the native gap.
 * T18-T21 after ordinary grouping failed: measured r9 pin for the live work
 * pointer, native r9 versus those aggregates' r11, scores5439/5439/5420/5420
 * for orders012/021/102/120. Those trials add no unread data, but worsen the
 * match and are not retained. Existing ordering/register tags stay unchanged.
 *
 * T22-T24 consumed draw/cell-size descriptor: mutable 2581; const whole
 * object 2528; const stride member 2528. The stride controls actual source
 * cell offsets, but each adds metadata loads/stores absent natively.
 * T25-T30 consumed render binding (draw/canvas/work), field orders
 * 012:8393; 021:8393; 102:8408; 120:8408; 201:8373; 210:8373.
 * T31 draw/canvas pair under retained setup:86 (six stack operands, one
 * offset, one reorder); frame stays84 and canvas moves to40, not native48.
 * Retain the simpler scalar body; no new steering device was introduced.
 *
 * 2026-10-04 lead: the unused word (sp+40) sits between the slots reload
 * gives at its start (canvas, frame) and the later spills (draw downward),
 * exactly where reload1.c's first pass calls setup_save_areas. That routine
 * reserves a word for every call-used register (r0-r4, ip, lr here) that
 * global alloc gave a call-crossing pseudo, and the word stays when that
 * pseudo is spilled later. A source shape whose allocation briefly puts a
 * call-crossing value in a call-used register would explain the frame
 * without any unread data. alchemy permute (18,666 candidates) found none.
 * No producer/callee evidence supports the original draw[2] reservation.
 * Keep this source-local near match until the frame has an honest model;
 * do not restore padding solely to keep the previous percentage.
 */
#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "MOTION_OBJECT.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"

extern u8 gBattleFxWork[];


void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);

/* Three sparks orbit each affected unit for 64 frames from frame i * 16,
   drawn through the single loaded blitter. */
void BattleFx_RunOrbitingSparks(void)
{
    /* FAKEMATCH: the existing relative heap-cell transport preserves load and literal ordering; independent typed slot loads change those instructions. */
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition center;
    struct EffectPosition pos;
    s32 frame;
    DrawRectangleFn draw;
    s32 member;
    struct BattleCamera *camera;
    u8 *palette;
    s32 x_offset;
    s32 i;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * sizeof(void *));
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000052 = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    Resource_DecodeType01(palette + 128, work);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw = (DrawRectangleFn)heap_cache[7];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 0)
        x_offset = 0;
    else
        x_offset = -112;
    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer */
    do {
        *(volatile s32 *)0x04000028 = x_offset << 8;
    } while (0);
    for (i = 0; i != 64; i++) {
        struct EffectStep *step = &work->particles[i];

        step->x = 0;
        step->y = 0;
        step->velocity_x = 0;
        step->z = 4;
    }
    for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *member_object;
            u32 window;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &pos);
            window = frame - member * 16;
            pos.x += x_offset;
            if (window < 64) {
                /* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp */
                register s32 *rec asm("r0");

                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                rec = record;
                rec[0] = member_object->x;
                rec[1] = member_object->y;
                rec[2] = member_object->z;
                EffectPosition_ApplyBaseAndYOffset(rec, &center);
                /* FAKEMATCH: sched2 barrier, so this store precedes the spark counter as in the reference */
                do {
                    center.x += x_offset;
                } while (0);
                for (i = 0; i != 3; i++) {
                    struct EffectStep *spark = &work->particles[member * 3 + i];
                    s32 x;
                    s32 y;

                    x = pos.x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    y = pos.y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    spark->velocity_x += 0x200;
                    draw(canvas, (u8 *)work + i * 0x240, x - 12, y - 28, 24, 24);
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
