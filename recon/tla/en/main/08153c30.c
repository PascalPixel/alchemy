/* Near miss of Func_08153c30, ported from The Broken Seal's
 * BattleFx_RunTargetBursts (listing 291 instructions, draft 291, differ 22):
 * ten reordered pairs, no other difference. The Lost Age loads its two
 * rectangle blitters through Func_081963ec into heap slots 104 and 188 and
 * reads the effect from its argument. Adopting needs the three resources
 * named in directory.s (SmokeSheet, TargetBurstImage, PinkBurstSheet). */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

extern char ResourceId_SmokeSheet;
extern char ResourceId_TargetBurstImage;
extern char ResourceId_PinkBurstSheet;

u32 Random16(void);
void WaitFrames(s32 frames);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_081963ec(s32 slot, s32 mode);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void SceneTransform_ApplyPosition(s32 *position);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping, s32 gravity);

// Bursts of smoke rise from each target in turn (The Broken Seal's
// BattleFx_RunTargetBursts).
void Func_08153c30(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    void *palette;
    s32 frame;
    DrawRectangle rectangle_b;
    DrawRectangle rectangle_a;
    struct EffectStep *step;
    s32 i;
    struct EffectPosition screen;
    s32 record[3];

    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage, work->sheet + 0x1b00, 0, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    Func_081963ec(104, 3);
    rectangle_a = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Func_081963ec(188, 19);
    rectangle_b = (DrawRectangle)Ram_HeapSlots->unkb8[1];

    for (i = 0; i != 64; i++) {
        s32 angle;
        s32 magnitude;

        step = &work->particles[i];
        angle = Random16() & 0xffff;
        magnitude = Random16() & 0xff;
        step->x = 0;
        step->y = 0;
        step->z = 0;
        if (i % 6 == 5) {
            step->velocityX = 0;
            step->velocityY = 0;
        } else {
            step->velocityX = (Trig_Sin(angle) * magnitude) >> 7;
            step->velocityY = (Trig_Cos(angle) * magnitude) >> 9;
        }
        step->velocityZ = 0;
        step->variant = 0;
    }

    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    for (frame = 0; frame != 96; frame++) {
        s32 member;
        s32 *camera;

        camera = Ram_HeapSlots->cameraWork;
        for (member = 0; member != effect->count; member++) {
            struct MotionObject *member_object;

            member_object = GetBattleObjectSlotFar(effect->actors[member])->object;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera, camera + 3);
            record[0] = member_object->x;
            record[1] = 160 << 13;
            record[2] = member_object->z;
            SceneTransform_ApplyPosition(record);
            if (frame >= member * 8 && frame < member * 8 + 40) {
                struct EffectStep *p;

                for (i = 0; i != 6; i++) {
                    s32 cel;

                    p = &work->particles[member * 6 + i];
                    cel = p->variant / 6;
                    if (cel > 5)
                        cel = 5;
                    EffectPosition_ApplyBaseAndYOffset((s32 *)p, &screen);
                    screen.x >>= 1;
                    if (i == 5) {
                        BattleFxKernels_IntegrateVector3(p, 62, 0x800);
                        rectangle_a(canvas, work->sheet + cel * 0x480 + 0x1b00,
                            screen.x - 12, screen.y - 36, 24, 48);
                    } else {
                        BattleFxKernels_IntegrateVector3(p, 60, 0x200);
                        rectangle_b(canvas, work->sheet + cel * 0x480,
                            screen.x - 12, screen.y - 36, 24, 48);
                    }
                    p->variant += 1;
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
