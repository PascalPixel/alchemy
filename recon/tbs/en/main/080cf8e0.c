/* Draft: native 670 instructions, draft 654, common 441, differ 442.
 * Stubborn (2026-10-06): register allocation drifts from the mode chain on
 * (native mode-test reloads rotate through r4, the draft's through r0-r3
 * only). Native recomputes each burst address from k * 4 hoisted to a stack
 * slot; the draft strength-reduces it to a pointer stepped by 28.
 * Shared heap slots and hardware declarations preserve the original 484 score.
 * An explicit per-target delay (8 frames) improves 484 -> 444 and restores
 * the native 80-byte frame; no unused local or new FAKEMATCH device is needed.
 * Remaining: loader branch merging, burst-index strength reduction, and
 * allocation/reloads while drawing shards. The four SevenMode tables still
 * need native-pool labels before adoption.
 * Trials: direct palette calls 550; pointer/array burst forms 484/495;
 * an explicit burst-group index 486 (442 with the per-target delay);
 * burst pointer stepping 446; i + k * 32 444;
 * (k * 4) * 8 and k << 2 << 3 444; a k * 4 local 581; draw-pair shape 484;
 * draw cursor 473.
 */
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"

extern s8 SevenMode_Records[];
extern u8 SevenMode_ShardWidths[];
extern u8 SevenMode_ShardHeights[];
extern u16 SevenMode_ShardOffsets[];

#define gShots ((struct EffectStep *)Ram_MapCellBuffer)
#define gBursts ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

typedef s32 (*WordCopy)(void *, const void *, s32);

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the palette copies share the routine's address but build
       the palette address again at each call, which only an inlined constant
       argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    s32 total;
    struct BattleCamera *camera;
    struct MotionObject *source;
    u8 *palette;
    s32 i;
    s32 height;
    s32 delay;
    s32 group;

    heap_cache = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x3e80);
    if (mode == 0)
        palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
    else if (mode == 1)
        palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
    else if (mode == 2)
        palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    else if (mode == 3)
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    else if (mode == 4)
        palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    else if (mode == 6)
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);

    for (i = 0; i != 1024; i++)
        gShots[i].variant = -1;
    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    height = 0x40000;
    for (k = 0; k != work->effect->count; k++) {
        struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

        for (i = 0; i != 16; i++) {
            struct EffectStep *shot = &gShots[k * 16 + i];

            shot->x = source->x;
            shot->y = height;
            shot->z = source->z;
            shot->velocity_x = (target->x - source->x) >> 4;
            shot->velocity_y = 0x40000;
            shot->velocity_z = (target->z - source->z) >> 4;
            shot->variant = 0;
        }
    }
    for (i = 0; i != 256; i++)
        gBursts[i].variant = -1;

    if (effect->side == 0) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    } else {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (SevenMode_Records[mode * 2] == 0)
        total = work->effect->count * 8 + 72;
    else
        total = work->effect->count * 8 + 56;
    AudioCommand_PlayFar(103);

    for (frame = 0; frame != total; frame++) {
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        for (k = 0, delay = 0, group = 0; k != work->effect->count; k++, delay += 8, group += 32) {
            if (frame >= delay) {
                struct EffectStep *shot = &gShots[k * 16];

                if (frame == delay + 17) {
                    ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 16);
                    BattleEventRuntime_BeginPhaseFar(133);
                }
                if (shot->variant >= 0) {
                    s32 step = (frame - delay) / 3;

                    if (step > 9)
                        step = 9;
                    EffectPosition_ApplyBaseAndYOffset((s32 *)shot, &position);
                    position.x >>= 1;
                    if (step > 4)
                        draw[0](canvas, work->sheet + step * 768, position.x - 16, position.y - 12, 32, 24);
                    else
                        draw[0](canvas, work->sheet + step * 768, position.x - 12, position.y - 16, 24, 32);
                    if (shot->variant == 0)
                        EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
                    if (shot->y < 0) {
                        s32 count;

                        shot->y = 0;
                        shot->variant = 1;
                        count = 4;
                        if (SevenMode_Records[mode * 2] != 0)
                            count = 16;
                        for (i = 0; i != count; i++) {
                            struct EffectStep *burst = &gBursts[i + group];

                            burst->x = shot->x;
                            burst->y = shot->y;
                            burst->z = shot->z;
                            if (SevenMode_Records[mode * 2] == 0) {
                                burst->velocity_x = ((Random16() & 63) - 32) << 11;
                                burst->velocity_y = 0;
                                burst->velocity_z = ((Random16() & 63) - 32) << 11;
                            } else {
                                burst->velocity_x = ((Random16() & 63) - 32) << 13;
                                burst->velocity_y = ((Random16() & 31) + 32) << 12;
                                burst->velocity_z = ((Random16() & 63) - 32) << 13;
                            }
                            burst->variant = 0;
                        }
                    }
                }
            }
        }
        for (i = 0; i != 256; i++) {
            struct EffectStep *burst = &gBursts[i];

            if ((u32)burst->variant <= 44 && burst->y >= 0) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)burst, &position);
                position.x >>= 1;
                if (SevenMode_Records[mode * 2] == 0) {
                    draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + 0x3e80,
                        position.x - 12, position.y - 24, 24, 48);
                } else {
                    s32 picture = burst->variant / 5;
                    s32 flip;

                    if (i & 1)
                        picture += 9;
                    flip = effect->side;
                    if (burst->velocity_x > 0)
                        flip ^= 1;
                    draw[flip](canvas, work->sheet + SevenMode_ShardOffsets[picture] + 0x1e00,
                        position.x - SevenMode_ShardWidths[picture] / 2,
                        position.y - SevenMode_ShardHeights[picture] / 2,
                        SevenMode_ShardWidths[picture], SevenMode_ShardHeights[picture]);
                }
                if (SevenMode_Records[mode * 2] == 0)
                    EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
                else
                    EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
                burst->variant++;
            }
        }
        Camera_ApplyShake(2, 2);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
