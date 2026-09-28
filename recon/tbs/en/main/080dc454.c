/* Draft, not exact (2026-09-28): 616 of 616 bytes, 175 differing halfwords
 * (the earlier m2c draft was 640 bytes, 271 halfwords). Rewritten from the
 * listing in the style of the matched effect family (TARGET_BURSTS.C,
 * SWIRLING_STARS.C): three sparks orbit each affected unit for 64 frames
 * from frame i * 16, drawn through the first blitter of the pair.
 * Recovered: the heap-cache and camera words derive from one gWorkSlot
 * address (the reference computes the camera word as the cache minus 108),
 * the doubled transfer_mode/transfer_value stores, the pointer form of the
 * seeding loop, the unreduced spark index member * 3 + k and the k * 0x5555
 * and k * 0x240 givs.
 * Remaining: the reference strength-reduces the unit window frame - 16 *
 * member into a stack slot (initialised from frame in the unit preheader)
 * and keeps &pos in fp and a copy in sl for the spark loop, rematerialising
 * &record (sp+76) and &center (sp+64) at each use; here &record is hoisted
 * into fp and &pos lives in sl through a pointer variable. Spelling the
 * window as a variable assigned at the top of the unit loop recovers the
 * reduced giv (620 bytes, 206 halfwords) but not the pointer allocation;
 * pointer variables for record and center, a block-local spark counter and
 * the other x_offset forms did not help. */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

extern u8 Value_000000a9;
extern u8 gWorkSlot[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);

void BattleFx_RunOrbitingSparks(void)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition center;
    struct EffectPosition pos;
    s32 frame;
    DrawRectangleFn draw[2];
    s32 member;
    s32 facing;
    s32 x_offset;
    u8 *palette;
    s32 i;
    struct EffectPosition *pos_ptr;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    facing = *(s32 *)(gWorkSlot + 12 * 4);
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000052 = 0x1010;
    palette = Resource_GetTableEntry((s32)&Value_000000a9);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    Resource_DecodeType01(palette + 128, work);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    draw[0] = (DrawRectangleFn)heap_cache[7];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    x_offset = 0;
    if (work->effect->side != 0)
        x_offset = -112;
    *(volatile s32 *)0x04000028 = x_offset << 8;
    for (i = 0; i != 64; i++) {
        struct EffectStep *step;

        step = &work->particles[i];
        step->x = 0;
        step->y = 0;
        step->velocity_x = 0;
        step->z = 4;
    }
    pos_ptr = &pos;
    for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
        for (member = 0; member != work->effect->count; member++) {
            void *member_object;

            member_object = *GetBattleObjectSlotFar(work->effect->actors[member]);
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], pos_ptr);
            pos_ptr->x += x_offset;
            if ((u32)(frame - member * 16) < 64) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                record[0] = *(s32 *)((u8 *)member_object + 8);
                record[1] = *(s32 *)((u8 *)member_object + 12);
                record[2] = *(s32 *)((u8 *)member_object + 16);
                EffectPosition_ApplyBaseAndYOffset(record, &center);
                center.x += x_offset;
                for (i = 0; i != 3; i++) {
                    struct EffectStep *spark;
                    s32 x;
                    s32 y;

                    spark = &work->particles[member * 3 + i];
                    x = pos_ptr->x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    y = pos_ptr->y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    spark->velocity_x += 0x200;
                    draw[0](canvas, (u8 *)work + i * 0x240, x - 12, y - 28, 24, 24);
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
