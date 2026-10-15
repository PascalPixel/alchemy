/* Draft, differ 26 of 427: The Broken Seal's BattleFx_RunFireBurstShards,
 * ported: the effect stays an argument, the camera comes from its heap
 * slot (0x30), the blitters from Func_081963ec(104, 3, 7 or 19) and slot
 * 0x68, the canvas fill through IwramFillWords, resources 0x159, 0x125
 * and 0x134, callback Func_08143000 at 0xc80. Frame declared before the
 * sheet and camera gives the native's spill slots (62 to 26). Everything
 * left is reordered rows where the native puts a load or a store ahead of
 * an independent constant or copy (the pooled map cell address and 0xff
 * after the heap reads, the work's reload before 0x7780 and before the
 * blitter's frame, the particle's x before the third random's mask, the
 * second size store before the halved size), the same kind as
 * Func_08164b2c's, where a load costing 3 and a store 2 give the native
 * order (not worked through here). */
#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "graphics.h"
#include "transform.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

extern char ResourceId_FireStreakSheet;
extern char ResourceId_FireBurstSheet;
extern char ResourceId_Data134;
extern const u16 gData_08197410[];

struct EffectCamera {
    s32 eye[3];
    s32 pos[3];
};

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081180e8(s32 phase);
void Audio_PlayCue(s32 cue);
void Func_081963ec(s32 slot, s32 kind);
void Func_08144aac(s32 side, DrawRectangle *draw);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void BattleFx_RunNoEffectFrames(s32 frames);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping, s32 gravity);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);

void Func_081661dc(struct BattleEffectArgument *effect)
{
    struct BattleFxWork *work;
    void *canvas;
    s32 frame;
    u8 *cells;
    struct EffectCamera *view;
    struct MotionObject *slot;
    struct EffectStep *shard;
    struct EffectPosition position;
    struct EffectPosition origin;
    struct EffectPosition out;
    DrawRectangle routine[2];
    s32 i;
    s32 size;
    u8 *burst;

    work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    cells = Ram_HeapSlots->unk64[0];
    view = Ram_HeapSlots->cameraWork;
    burst = Ram_MapCellBuffer;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
    WaitFrames(1);
    slot = GetBattleObjectSlotFar(effect->actors[0])->object;

    i = 0;
    do {
        struct EffectStep *seed = &work->particles[i];

        seed->x = slot->x;
        seed->y = slot->y;
        seed->z = slot->z;
        seed->velocityX = (Random16() & 255) << 11;
        seed->velocityY = ((Random16() & 255) - 127) << 12;
        seed->velocityZ = ((Random16() & 255) - 127) << 12;
        if (seed->x > 0)
            seed->velocityX = -seed->velocityX;
        seed->variant = i / 4 * 2 + 16;
        i++;
    } while (i != 64);

    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &origin);
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    work->transferMode = 2;
    work->transferValue = 75;

    for (frame = 0; frame != 64; frame++) {
        if (frame == 8)
            Func_081180e8(134);
        if (effect->variant != 0 && frame == 8)
            Audio_PlayCue(212);
        EffectPosition_ApplyStepAndYOffset(effect->actor, &position);
        if (frame >= 6 && frame <= 11) {
            if (effect->side == 0)
                Func_081963ec(104, 3);
            else
                Func_081963ec(104, 7);
            routine[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
            if (effect->side == 0)
                routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2 - 24, position.y - 24, 48, 72);
            else
                routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2, position.y - 24, 48, 72);
            Runtime_ReleaseHeapBlock(104);
        }
        if (frame >= 16 && frame <= 47) {
            s32 step = (frame - 16) / 2;
            s32 strip;

            Func_081963ec(104, 19);
            routine[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
            if (step > 2)
                step = 2;
            if (effect->variant == 0)
                strip = 0;
            else
                strip = 0x2580;
            routine[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80, origin.x / 2 - 20, origin.y - 48, 40, 80);
            BattleFx_RunNoEffectFrames(10000);
            Runtime_ReleaseHeapBlock(104);
        }
        if (frame == 8)
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        Render_ResetTransformState();
        Transform_SetLookAt(view->eye, view->pos);
        if (frame > 3) {
            Func_08144aac(effect->side, routine);
            for (i = 0; i != 64; i++) {
                s32 life;

                shard = &work->particles[i];
                life = shard->variant;
                if (life > 0) {
                    EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &out);
                    out.x >>= 1;
                    out.y = out.y + origin.y - 112;
                    size = (life >> 3) + 2;
                    routine[(i / 2) & 1](canvas, cells + gData_08197410[size - 1], out.x - size / 2, out.y - size, size, size * 2);
                    BattleFxKernels_IntegrateVector3(shard, 60, -0x400);
                    shard->variant--;
                }
            }
            Runtime_ReleaseHeapBlock(188);
            Runtime_ReleaseHeapBlock(104);
        }
        if (frame == 8) {
            Func_08118088(effect->actors[0], 4);
            work->shakeFrames = 4;
        }
        if (frame == 6)
            Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
        if (frame == 14)
            Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
        Func_08158ce0(16, 16);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
