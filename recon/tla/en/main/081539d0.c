/* Near miss of Func_081539d0, ported from The Broken Seal's
 * BattleFx_RunSwirlingStars (listing 272 instructions, draft 272, differ 14):
 * seven reordered pairs, each the native placing a load one row earlier
 * (the particle base before the rectangle call, the second loop's particle
 * base, the cell offset table, the camera word) or a use of a loaded word
 * one row later (frame's copy to sl after the record stores, the negations
 * of the star's coordinates after the stores of the previous sums). All
 * follow from a load latency of 3 in sched2. Tried: the particle pointer set
 * before the rectangle call (differ 32, r5 and r6 trade). */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "ram_buffer.h"

extern char ResourceId_StarDotSheet;
extern u16 gData_08197410[];

u32 Random16(void);
void WaitFrames(s32 frames);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_08144aac(s32 flip, DrawRectangle *out);
void Func_080150e4(s32 angle);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Render_ResetTransformState(void);
void Transform_SetLookAt(void *eye, void *pos);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Transform_Push(void);
void Transform_Pop(void);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping, s32 gravity);

#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

void Func_081539d0(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition pos;
    DrawRectangle draw[2];
    struct EffectStep *star;
    s32 i;
    s32 frame;

    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
    Func_08144aac(effect->side ^ 1, draw);
    for (i = 0; i != 256; i++) {
        star = &PARTICLES[i];
        star->x = ((Random16() & 0xff) - 127) << 16;
        star->y = ((Random16() & 0xff) - 127) << 16;
        star->z = ((Random16() & 0xff) - 127) << 16;
        star->velocityX = 0;
        star->velocityY = 0;
        star->velocityZ = 0;
        star->variant = 0;
    }
    work->transferMode = 1;
    work->transferValue = 0;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    record[0] = 0;
    record[1] = 160 << 15;
    record[2] = 0;
    for (frame = 0; frame != 160; frame++) {
        s32 *camera;

        camera = Ram_HeapSlots->cameraWork;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(record);
        star = PARTICLES;
        for (i = 0; i != 64; i++, star++) {
            if (frame > i / 4 && star->variant == 0) {
                s32 size;

                Transform_Push();
                switch (i & 3) {
                case 0:
                    SceneTransform_ApplyYaw(frame * (i + 32) * 8);
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    break;
                case 2:
                    Func_080150e4(-frame * (i + 32) * 8);
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    Func_080150e4(-frame * (i + 32) * 8);
                    break;
                }
                EffectPosition_ApplyBaseAndYOffset((s32 *)star, &pos);
                pos.x >>= 1;
                Transform_Pop();
                if (pos.depth < 250)
                    pos.depth = 250;
                if (pos.depth > 0x27a)
                    pos.depth = 0x27a;
                size = 9 - (pos.depth - 250) / 64;
                draw[0](canvas,
                    work->sheet + ((i & 3) * 770 + gData_08197410[size - 1]),
                    pos.x - size / 2, pos.y - size, size, size * 2);
                BattleFxKernels_IntegrateVector3(star, 60, 0);
                if (frame > i / 4 + 30) {
                    s32 dx = -star->x >> 8;
                    s32 dy = -star->y >> 8;
                    s32 dz = -star->z >> 8;

                    star->velocityX += dx;
                    star->velocityY += dy;
                    star->velocityZ += dz;
                }
            }
        }
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
