/* 2026-09-29 alchemy permute (with --symbol Func_080d59b0): score 115 on
   the permuter's scorer (19 register-only, 1 operand for the
   Value_000000a8 resource id), unchanged after 41,098 candidates in five minutes;
   the swap of the rock pointer and the floor constant stays. */
/* Draft, not exact (2026-09-29): 664 of 664 bytes, 19 differing halfwords,
 * all one register swap. Rewritten from the listing in the style of the
 * matched effect family (TARGET_BURSTS.C, SWIRLING_STARS.C): 32 rocks are
 * dropped from above the scene, the first twelve drawn from frame i * 4,
 * bouncing once they pass the floor; each affected unit is lifted with
 * ObjectGroup_UpdateMembers on frame i * 16 + 64. The bounce takes the
 * speed (vy + 1) before placing the rock on the floor and stepping z.
 * 2026-09-29: the literal buffer address is gone. The rocks now live in the
 * linker-placed gMapCellBuffer, reached through a pointer taken once at the
 * top; the seeding loop then keeps the ROM's walking pointer. Spelling the
 * symbol at each use instead gives 672 bytes.
 * Remaining: in the draw loop the ROM keeps the rock pointer in r6 and the
 * floor constant 0x5c0000 in r5; here they are swapped. Global allocation
 * ranks the pointer (50 weighted references over 69 insns) above the
 * constant (9 over 10), so the pointer takes r5 first; the ROM allocated the
 * constant first. Index-derived pointers, a separate seeding pointer, the
 * loop-header initialiser, >= 0x5c0001 and 92 << 16 leave it unchanged.
 * Blocker before adoption: the resource id 0xa8 is a literal-pool word in
 * the reference, which a link-time constant reproduces (main names
 * resource ids this way, e.g. Value_0000004f); Value_000000a8 is not yet
 * defined in CONSTANTS.LD. */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

extern u8 Value_000000a8;
extern void *gBattleFxWork[];
extern s32 gCameraWork;
extern struct EffectStep gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
void SceneTransform_ApplyPosition(s32 *position);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);

void BattleFx_RunBouncingRocks(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];

    struct EffectStep *rock;
    struct EffectStep *rocks;
    s32 i;
    s32 frame;
    DrawRectangleFn draw_b;
    DrawRectangleFn draw_a;
    s32 member;

    rocks = gMapCellBuffer;
    heap_cache = gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&Value_000000a8, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw_a = (DrawRectangleFn)heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 15, 1);
    draw_b = (DrawRectangleFn)heap_cache[8];
    for (i = 0; i != 32; i++) {
        rock = &rocks[i];
        rock->x = ((Random16() & 63) + 32) << 16;
        rock->y = -0x200000;
        rock->velocity_y = Random16() & 0;
        rock->z = Random16() & 3;
        rock->variant = Random16() & 255;
    }
    if (work->effect->side == 1)
        *(volatile s32 *)0x04000028 = -0x7000;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(142);
    for (frame = 0; frame != 148; frame++) {
        s32 facing;

        facing = gCameraWork;
        if (frame == 80)
            BattleEventRuntime_BeginPhaseFar(0);
        for (member = 0; member != work->effect->count; member++) {
            void *member_object;

            member_object = *GetBattleObjectSlotFar(work->effect->actors[member]);
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            record[0] = *(s32 *)((u8 *)member_object + 8);
            record[1] = 160 << 14;
            record[2] = *(s32 *)((u8 *)member_object + 16);
            SceneTransform_ApplyPosition(record);
            if (frame == member * 16 + 64)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
        }
        rock = rocks;
        for (i = 0; i != 12; i++, rock++) {
            if (frame > i * 4 && rock->y <= 0x7fffff) {
                s32 cel;

                cel = (rock->variant / 16) & 7;
                if (cel < 4)
                    draw_a(canvas, (u8 *)work + (cel << 10),
                        (rock->x >> 16) - 16, (rock->y >> 16) - 16, 32, 32);
                else
                    draw_b(canvas, (u8 *)work + (cel << 10) - 0x1000,
                        (rock->x >> 16) - 16, (rock->y >> 16) - 16, 32, 32);
                rock->y += rock->velocity_y;
                rock->velocity_y += 0x2000;
                rock->variant += rock->z;
                if (rock->y > 0x5c0000 && rock->velocity_y == 0) {
                    s32 speed = rock->velocity_y + 1;
                    rock->y = 0x5c0000;
                    rock->z += 4;
                    rock->velocity_y = -speed / 2;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
