/* Draft, one instruction out of place: before the first palette copy the
   ROM loads the copy routine address before it copies the palette pointer
   to r1 (a direct call does that, but then the two copies share 0x05000000
   in a register); passing the routine as a wrapper parameter fixes the order
   but makes the slot pointer take r5 instead of r8. */
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

extern u8 gBattleFxWork[];
extern struct BattleCamera *gCameraWork;

#define gSkulls ((struct EffectStep *)Ram_MapCellBuffer)

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyPitch(s32 angle);
u32 Resource_DecodeType01(const void *source, void *destination);

/* A resource that opens with a 64-colour palette. */
struct PaletteBlock {
    u16 colors[64];
};

static __inline__ void CopyPalette(void *destination, const void *source)
{
    /* FAKEMATCH: the two palette copies share the routine's address but
       build the palette address again at each call, which only an inlined
       constant argument compiles to; a direct call keeps it in a register. */
    Iwram_CopyWords(destination, source, 128);
}

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

/* Battle effect: eight skulls for each target start scattered around it and
   close in, turning with the frame, while the background sways. */
void Unnamed_080ce4e8(struct BattleEffectArgument *effect)
{
    struct EffectPosition position;
    s32 point[3];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    struct BlitterPair draw;
    struct BattleCamera *camera;
    struct PaletteBlock *palette;
    s32 i;
    s32 k;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    CopyPalette((void *)0x05000000, palette++);
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyPalette((void *)0x05000000, palette);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw.upper = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw.lower = heap_cache[8];
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (i = 0; i != 512; i++) {
        gSkulls[i].x = ((Random16() & 255) - 127) << 15;
        gSkulls[i].y = ((Random16() & 255) - 127) << 15;
        gSkulls[i].z = ((Random16() & 255) - 127) << 15;
    }
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 32 + 96; frame++) {
        s32 *row;

        camera = gCameraWork;
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(0);
        row = work->bg2_x;
        if (work->effect->side == 0) {
            for (i = 0; i != 160; i++)
                *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
        } else {
            for (i = 0; i != 160; i++)
                *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
        }
        for (k = 0; k != work->effect->count; k++) {
            struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            point[0] = target->x;
            point[1] = 0x140000;
            point[2] = target->z;
            SceneTransform_ApplyPosition(point);
            if (frame > k * 32) {
                SceneTransform_ApplyPitch(frame << 9);
                if (frame == k * 32 + 32)
                    ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 32);
                for (i = 0; i != 8; i++) {
                    struct EffectStep *skull = &gSkulls[k * 64 + i];

                    if (frame > (k * 8 + i) * 4) {
                        s32 x = (skull->x >> 8) * (skull->x >> 8);
                        s32 y = (skull->y >> 8) * (skull->y >> 8);
                        s32 z = (skull->z >> 8) * (skull->z >> 8);
                        s32 distance = Iwram_Sqrt(x + y + z) >> 8;

                        if (distance != 0) {
                            EffectPosition_ApplyBaseAndYOffset((s32 *)skull, &position);
                            position.x >>= 1;
                            draw.upper(canvas, work->sheet + i % 3 * 576,
                                position.x - 12, position.y - 12, 24, 24);
                            skull->x -= skull->x / distance;
                            skull->y -= skull->y / distance;
                            skull->z -= skull->z / distance;
                            skull->variant++;
                        }
                    }
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
