/* 2026-09-29 alchemy permute: score 2367 to 985 on the permuter's scorer
   (0 is exact); remaining 16 register-only, 4 operand, 2 reordered, 5
   inserted, 2 deleted. Kept rewrites: 19x reorder local declarations, 14x
   reorder independent statements, 13x introduce a temporary, 12x swap
   commutative operands, 10x add a same-width cast, 10x drop a same-width
   cast, 6x remove a temporary, 6x pointer arithmetic or indexing, 6x split
   or join a compound assignment, 6x toggle register, 5x test truth or
   compare with zero, 4x change loop form. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
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

#include "RESOURCE_IDS.H"
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
    struct EffectPosition pos;
    register struct EffectPosition center;
    s32 record[3];
    s32 frame;
    DrawRectangleFn draw[2];
    s32 member;
    s32 facing;
    register u8 *palette;
    s32 x_offset;
    s32 i;
    register u8 *tmp5;
    struct EffectPosition *pos_ptr;
    s32 tmp4;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    facing = *(s32 *)(gWorkSlot + 12 * 4);
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000052 = (u32)0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    tmp5 = palette + 128;
    Resource_DecodeType01(tmp5, work);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    draw[0] = (DrawRectangleFn)heap_cache[7];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    tmp4 = work->effect->side;
    x_offset = 0;
    if (tmp4)
        x_offset = -112;
    *(volatile s32 *)0x04000028 = x_offset << 8;
    for (i = 0; i != 64; i++) {
        struct EffectStep *tmp6;
        struct EffectStep *step;
        tmp6 = &*((struct EffectStep *)work->particles + i);
        step = tmp6;
        step->x = 0;
        step->y = 0;
        step->velocity_x = 0;
        step->z = 4;
    }
    frame = 0;
    pos_ptr = &pos;
    while (frame != work->effect[0].count * 16 + 64) {
        for (member = 0; member != work->effect->count; member += 1) {
            void *member_object;
            register u32 tmp3;
            member_object = GetBattleObjectSlotFar(work->effect->actors[member])[0];
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], pos_ptr);
            tmp3 = frame - 16 * member;
            pos_ptr->x += x_offset;
            if (64 > tmp3) {
                register s32 *tmp;
                u8 *tmp2;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, 12 + facing);
                tmp = (s32 *)(16 + (u8 *)member_object);
                record[0] = *(s32 *)(8 + (u8 *)member_object);
                tmp2 = (u8 *)member_object + 12;
                record[1] = *(s32 *)tmp2;
                record[2] = *tmp;
                EffectPosition_ApplyBaseAndYOffset(record, &center);
                center.x += x_offset;
                i = 0;
                while (i != 3) {
                    register s32 x;
                    struct EffectStep *spark;
                    s32 y;
                    s32 tmp8;
                    spark = &work->particles[member * 3 + i];
                    x = pos_ptr->x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    y = pos_ptr->y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    spark->velocity_x += 0x200;
                    tmp8 = y - 28;
                    draw[0](canvas, (u8 *)work + 0x240 * i, x - 12, tmp8, 24, 24);
                    ++i;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
