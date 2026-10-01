#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern DrawRectangle gWorkSlot[];
extern struct BattleCamera *gCameraWork;

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
u32 Resource_DecodeType01(const void *source, void *destination);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void Audio_PlayCue(s32 cue);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

extern u16 ParticleStreams_CellOffsets[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect opening shared by the djinn effects: thirty-two sparks spin
   out of the acting unit and are pulled back in while the gathering point
   travels to the place the anchor picks; a djinn of the kind's colour then
   appears there, its screen position goes back to the caller, and a burst of
   sixty-four motes flies out of it. Kinds above three run twenty frames
   longer and fade the blend out at the end. */
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side,
    s32 anchor, s32 *out_x, s32 *out_y)
{
    s32 base[3];
    s32 goal[3];
    s32 step[3];
    s32 point[3];
    struct EffectPosition position;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    void *sheet;
    s32 total;
    struct MotionObject *target;
    struct MotionObject *actor;
    u8 *palette;
    s32 resource;
    s32 frame;
    s32 i;
    s32 size;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    /* FAKEMATCH: the canvas size sits in a local set here, well ahead of
       the two clears that use it; as a literal at the calls GCC keeps it in
       one register across them, where the ROM builds it at each call. */
    size = 0x4000;
    work->fade_frames = 24;
    work->fade_step = 0;
    if (kind > 3) {
        kind -= 4;
        total = 84;
    } else {
        total = 64;
    }
    switch (kind) {
    case 0:
        resource = (s32)&ResourceId_VenusDjinnSmallSheet;
        break;
    case 1:
        resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
        break;
    case 2:
        resource = (s32)&ResourceId_MarsDjinnSmallSheet;
        break;
    default:
        resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
        break;
    }
    palette = Resource_GetTableEntry(resource);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    Resource_DecodeType01(palette + 128, work);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    if (side == 1) {
        BattleEffect_LoadWork(46, 7, 7, 7, 3);
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 3, 3);
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
    }
    draw[0] = gWorkSlot[46];
    draw[1] = gWorkSlot[47];
    actor = GetBattleObjectSlotFar(work->effect->actor)->object;
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 64; i++) {
        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = (Random16() & 255) + 128;

        spark->x = 0;
        spark->y = ((Random16() & 31) + 20) << 16;
        spark->z = 0;
        spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
        spark->velocity_y = 0;
        spark->velocity_z = (Trig_Cos(angle) * speed) >> 5;
        spark->variant = 0;
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    base[0] = actor->x;
    base[1] = 0;
    base[2] = actor->z;
    switch (anchor) {
    case 0:
        goal[0] = target->x;
        goal[1] = 0x3c0000;
        goal[2] = target->z;
        break;
    case 1:
        goal[0] = target->x;
        goal[1] = 0x3c0000;
        goal[2] = 0;
        break;
    case 2:
        goal[0] = actor->x;
        goal[1] = 0x3c0000;
        goal[2] = actor->z;
        break;
    case 3:
        goal[0] = actor->x;
        goal[1] = 0x3c0000;
        goal[2] = 0;
        break;
    case 4:
        goal[0] = 0;
        goal[1] = 0x3c0000;
        goal[2] = 0;
        break;
    }
    step[0] = (goal[0] - base[0]) / 40;
    step[1] = (goal[1] - base[1]) / 40;
    step[2] = (goal[2] - base[2]) / 40;
    for (frame = 0; frame != total; frame++) {
        struct BattleCamera *camera = gCameraWork;

        if (frame > 75)
            *(volatile u16 *)0x04000052 = (168 - frame * 2) | 0x1000;
        if (frame == 8)
            Audio_PlayCue(212);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame >= 6 && frame <= 45) {
            base[0] += step[0];
            base[1] += step[1];
            base[2] += step[2];
        }
        SceneTransform_ApplyPosition(base);
        if (frame == 0)
            ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
        if (frame == 24)
            ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 32; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            if (frame >= i / 8 && spark->variant == 0) {
                s32 size;

                Graphics_SaveTransferWorkOnce();
                switch (i & 3) {
                case 0:
                    SceneTransform_ApplyYaw(frame * (i * 32 + 256));
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
                    break;
                case 2:
                    SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
                    SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
                    break;
                }
                EffectPosition_ApplyBaseAndYOffset(&spark->x, &position);
                position.x >>= 1;
                Graphics_RestoreTransferWork();
                if (position.depth < 250)
                    position.depth = 250;
                if (position.depth > 634)
                    position.depth = 634;
                size = 8 - (position.depth - 250) / 64;
                draw[1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(spark, 60, 0);
                if (frame >= i / 8 + 24) {
                    s32 pull_x = -spark->x >> 7;
                    s32 pull_y = -spark->y >> 7;
                    s32 pull_z = -spark->z >> 7;

                    spark->velocity_x += pull_x;
                    spark->velocity_y += pull_y;
                    spark->velocity_z += pull_z;
                    spark->velocity_x = spark->velocity_x * 62 / 64;
                    spark->velocity_y = spark->velocity_y * 62 / 64;
                    spark->velocity_z = spark->velocity_z * 62 / 64;
                    if ((pull_x > -2048 && pull_x < 2048) && (pull_z > -2048 && pull_z < 2048))
                        spark->variant = -1;
                }
            }
        }
        if (frame >= 54 && frame <= 69) {
            point[0] = Trig_Sin(frame << 10) << 2;
            point[1] = 0;
            point[2] = 0;
            EffectPosition_ApplyBaseAndYOffset(point, &position);
            *out_x = position.x;
            *out_y = position.y;
            position.x >>= 1;
            draw[0](canvas, work, position.x - 10, position.y - 20, 20, 40);
        }
        if (frame == 64) {
            for (i = 0; i != 64; i++) {
                struct EffectStep *mote = &work->particles[i];
                s32 angle = Random16() & 0xffff;
                s32 speed = (Random16() & 255) + 128;

                mote->x = *out_x << 15;
                mote->y = *out_y << 16;
                mote->velocity_x = (Trig_Sin(angle) * speed) >> 6;
                mote->velocity_y = (Trig_Cos(angle) * speed) >> 5;
                mote->variant = (Random16() & 15) + 8;
            }
        }
        if (frame > 63) {
            for (i = 0; i != 64; i++) {
                struct EffectStep *mote = &work->particles[i];

                if (mote->variant >= 0) {
                    s32 size = (mote->variant >> 3) + 2;

                    draw[0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                        HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
                    EffectStep_AdvanceWithGravity2D(mote, 60, 0);
                    mote->variant--;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
    Iwram_ClearWords((void *)0x06004000, size);
    Iwram_ClearWords(canvas, size);
    *(volatile u16 *)0x04000052 = 0x1010;
}
