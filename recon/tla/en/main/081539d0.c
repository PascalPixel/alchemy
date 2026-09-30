#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
extern u8 gBattleFxWork[];
extern u8 gCameraWork[];

/*
 * Battle effect: a cloud of 256 stars scattered through a cube around the
 * scene centre (only the first 64 are shown). For 160 frames star i appears
 * from frame i / 4, turned by a yaw, pitch or roll (or both of the last two,
 * by i & 3) that grows with the frame, and is drawn from one of four sheets
 * at a size of 1..9 pixels by depth. Thirty frames after it appears each
 * star is pulled back towards the centre.
 */

/* Resource id the reference loads from its literal pool. */
extern const u16 ParticleStreams_CellOffsets[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);

void BattleFx_RunSwirlingStars(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition pos;
    DrawRectangleFn draw[2];
    struct EffectStep *star;
    s32 i;
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work, 1, 1);
    BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, (u32 *)draw);
    for (i = 0; i != 256; i++) {
        star = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        star->x = ((Random16() & 0xff) - 127) << 16;
        star->y = ((Random16() & 0xff) - 127) << 16;
        star->z = ((Random16() & 0xff) - 127) << 16;
        star->velocity_x = 0;
        star->velocity_y = 0;
        star->velocity_z = 0;
        star->variant = 0;
    }
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    record[0] = 0;
    record[1] = 160 << 15;
    record[2] = 0;
    for (frame = 0; frame != 160; frame++) {
        s32 facing;

        facing = *(s32 *)gCameraWork;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        SceneTransform_ApplyPosition(record);
        star = (struct EffectStep *)Ram_MapCellBuffer;
        for (i = 0; i != 64; i++, star++) {
            if (frame > i / 4 && star->variant == 0) {
                s32 size;

                Graphics_SaveTransferWorkOnce();
                switch (i & 3) {
                case 0:
                    SceneTransform_ApplyYaw(frame * (i + 32) * 8);
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    break;
                case 2:
                    SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
                    break;
                }
                EffectPosition_ApplyBaseAndYOffset((s32 *)star, &pos);
                pos.x >>= 1;
                Graphics_RestoreTransferWork();
                if (pos.depth < 250)
                    pos.depth = 250;
                if (pos.depth > 0x27a)
                    pos.depth = 0x27a;
                size = 9 - (pos.depth - 250) / 64;
                draw[0](canvas,
                    (u8 *)work + ((i & 3) * 770 + ParticleStreams_CellOffsets[size - 1]),
                    pos.x - size / 2, pos.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(star, 60, 0);
                if (frame > i / 4 + 30) {
                    s32 dx = -star->x >> 8;
                    s32 dy = -star->y >> 8;
                    s32 dz = -star->z >> 8;

                    star->velocity_x += dx;
                    star->velocity_y += dy;
                    star->velocity_z += dz;
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
