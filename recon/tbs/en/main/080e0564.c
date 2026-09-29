/* 2026-09-29: eight minutes of permutation (alchemy permute --symbol
 * Func_080e0564): 2790 -> 2115 (72 register-only, 6 operand, 10 reordered,
 * 8 inserted, 2 deleted), kept: the cell variant is read through a local
 * cursor to the cell buffer, the sheet address is taken as &sheet[offset],
 * and the variant is decremented through pointer arithmetic. Resource
 * numbers 0x6f, 0x73 and 0x94 are still Value_ symbols. */
/* Draft, complete main:080e0564 [080e0564,080e08c0), 860 bytes.
 * Commit 914176340 matched this owner byte for byte with the spray pool
 * written as the literal EWRAM address 0x02010000. Literal RAM addresses
 * are not allowed, and the pool through its linker-placed name,
 * gMapCellBuffer, does not match: 876/860 bytes. GCC 2.96 folds a constant
 * pool base plus the variant offset into one pool word (0x02010018), but
 * with a symbol it loads gMapCellBuffer and adds 24 in the loop setup, and
 * the extra register shifts the allocation of the frame, start and spout
 * loops. The same happens in REEL_INIT_TITLE.C. Needs a named form of the
 * pool that the compiler folds like a constant, or Pascal's ruling on it.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"

extern u8 gBattleFxWork[];
extern struct EffectStep gMapCellBuffer[];
extern u16 ParticleStreams_CellOffsets[];
extern u8 Value_0000006f;
extern u8 Value_00000073;
extern u8 Value_00000094;

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Audio_PlayCue(s32 cue);

#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: a swinging beam sweeps across the field for 80 frames
   while ten spouts start four frames apart from frame 16, each rising 12
   pixels a frame; as each starts it throws sixteen droplets into the spray
   pool, shakes every target and, for odd spouts, plays a cue. Droplets
   fall under gravity and shrink as they age. */
void BattleFx_RunSpoutBursts(struct BattleEffectArgument *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangleFn draw[2];
    u8 *sheet;
    s32 angle;
    struct EffectStep *spout;
    struct EffectStep *drop;
    s32 i;
    s32 j;
    s32 start;
    s32 x;
    s32 y;
    s32 spin;
    s32 speed;
    s32 size;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    BattleEffect_LoadWork(46, 7, 7, 11, 2);
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw[0] = heap_cache[7];
    draw[1] = heap_cache[8];
    Resource_LoadAndDecompress((s32)&Value_00000073, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000094, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_0000006f, (u8 *)work + 0x2f8, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 32; i++) {
        spout = &work->particles[i];
        spout->x = Random16() & 63;
        spout->y = 104;
    }
    for (i = 0; i != 512; i++)
        gMapCellBuffer[i].variant = -1;
    Audio_PlayCue(141);
    for (frame = 0, angle = 0x8000; frame != 96; frame++) {
        if (frame <= 79) {
            x = ((Trig_Sin(angle) * 3) << 3) >> 16;
            y = ((64 - frame * 2) * Trig_Cos(angle)) >> 16;
            draw[1](canvas, work, x + 22, y + 29, 20, 38);
        }
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(133);
        for (i = 0, start = 16, spout = work->particles; i != 10; start += 4, spout++, i++) {
            if (frame >= start) {
                draw[0](canvas, (u8 *)work + 0x9e0, spout->x - 17, spout->y - 32, 34, 65);
                if (frame == start) {
                    for (j = 0, drop = &gMapCellBuffer[i * 32]; j != 16; j++) {
                        spin = (Random16() & 0x7fff) + 0x4000;
                        speed = (Random16() & 0x1ff) + 256;
                        drop->x = spout->x << 16;
                        drop->y = (spout->y + 16) << 16;
                        drop->velocity_x = (Trig_Sin(spin) * speed) >> 7;
                        drop->velocity_y = (Trig_Cos(spin) * speed) >> 6;
                        drop->variant = (Random16() & 15) + 32;
                        drop++;
                    }
                    if (i & 1)
                        Audio_PlayCue(133);
                    work->shake_frames = 4;
                    for (j = 0; j != work->effect->count; j++) {
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
                    }
                }
                spout->y -= 12;
            }
        }
        for (i = 0; i != 512; i++) {
            if (gMapCellBuffer[i].variant != -1) {
                struct EffectStep *cells = gMapCellBuffer;

                size = cells[i].variant / 16 + 2;
                draw[1](canvas, &sheet[ParticleStreams_CellOffsets[size - 1]], HI(gMapCellBuffer[i].x) - size / 2, HI(gMapCellBuffer[i].y) - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(&gMapCellBuffer[i], 62, 0x2000);
                (gMapCellBuffer + i)->variant--;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        angle -= 0x800;
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
