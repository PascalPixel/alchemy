/* Near miss, differ 36 of 480: The Broken Seal's BattleFx_RunRingBolts as ⚓️ has
   it: the effect argument read directly, the rune palette and resource 0x134
   as in Func_08146a6c, Object_SetMode for the caster's children, one blitter
   from Func_081963ec(104, 0x13), the callback at 0xc80, the target thrown with
   Object_ResetMotion and Object_SetPosition, Func_081180e8 for the last bolt.
   The tables are not const (see Func_08146a6c). Same frame, registers and
   instructions; what is left is order: at each of about sixteen rows the native
   issues a memory access (the argument's spill, a work slot, a spill reload or
   a field read) one instruction ahead of the constant or move beside it. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "object_runtime.h"

// The battle effect work as the canvas effects use it.
struct CanvasEffectWork
{
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    s32 bg2X[160];
    u8 unknown_7300[0x480];
    s32 transferMode;
    s32 transferValue;
    u8 unknown_7788[0x20];
    s32 shakeFrames;
    u8 unknown_77ac[0x13c];
    s32 transferPending;
};

struct BattleObjectSlot
{
    struct ObjectRuntime *object;
};

struct CameraView
{
    u8 unknown_00[0x36];
    u16 yaw;
};

struct Vector3
{
    s32 x;
    s32 y;
    s32 z;
};

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern s32 Data_08197924[];
extern u16 Data_08197410[];
extern u8 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081434f8(void);
void Func_08158ce0(s32 x, s32 y);
void Func_081180e8(s32 phase);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
void Func_080150e4(s32 angle);
void Func_080151ac(struct Vector3 *scale);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPosition(void *position);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *object, s32 value);
void Audio_PlayCue(s32 cue);
void *Resource_GetTableEntry(s32 id);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);

void Func_08146638(struct BattleEffectArgument *efx)
{
    struct EffectPosition pos;
    struct EffectPosition base;
    struct Vector3 scale;
    struct CanvasEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    s32 *camera;
    struct ObjectRuntime *caster;
    struct ObjectRuntime *source;
    struct ObjectRuntime *target;
    struct EffectStep *point;
    struct EffectStep *step;
    s32 start;
    s32 peak;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    work = (struct CanvasEffectWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    graphics = Ram_HeapSlots->unknown_64[0];
    camera = Ram_HeapSlots->camera_work;
    caster = GetBattleObjectSlotFar(efx->actor)->object;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    Object_SetMode(caster, 2);
    ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
    Func_081963ec(104, 0x13);
    work->transferMode = 2;
    work->transferValue = 75;
    draw[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    source = GetBattleObjectSlotFar(efx->actor)->object;
    target = GetBattleObjectSlotFar(efx->actors[0])->object;
    for (i = 0, point = work->particles; i != 3; i++)
    {
        point->x = source->x;
        point->y = source->y + 0x280000;
        point->z = source->z;
        if (i == 0)
            point->velocity_x = (target->x - point->x) / 12;
        else
            point->velocity_x = (target->x * 2 - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x280000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = 0;
        point++;
    }

    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 60; frame++)
    {
        if (frame <= 47)
        {
            struct CameraView *view = (struct CameraView *)camera;
            s32 speed;

            if (frame <= 39)
                speed = 128;
            else
                speed = 0x300 - frame * 16;
            if (efx->side == 0)
                view->yaw -= speed;
            else
                view->yaw += speed;
        }
        for (i = 0; i != 3; i++)
        {
            start = i * 12;
            if (frame >= start)
            {
                step = &work->particles[i];
                size = (frame - start) / 4 + 2;
                if (size > 10)
                    size = 10;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera, camera + 3);
                SceneTransform_ApplyPosition(step);
                peak = 0;
                for (j = 0; j != 10; j++)
                {
                    s32 depth;

                    Graphics_SaveTransferWorkOnce();
                    Func_080150e4((frame - start) << 10);
                    SceneTransform_ApplyYaw(0x4000);
                    scale.x = ((frame - start) << 12) + 0x1000;
                    if (scale.x > 0x10000)
                        scale.x = 0x10000;
                    scale.y = scale.x;
                    scale.z = scale.x;
                    Func_080151ac(&scale);
                    Func_080150e4(j * 0x199a);
                    depth = EffectPosition_ApplyBaseAndYOffset(&Data_08197924[(j & 1) * 3], &pos);
                    if (peak < depth)
                        peak = depth;
                    pos.x >>= 1;
                    trails[i * 10 + j].velocity_x = pos.x + base.x;
                    trails[i * 10 + j].velocity_y = pos.y + base.y;
                    trails[i * 10 + j].velocity_x = pos.x;
                    trails[i * 10 + j].velocity_y = pos.y;
                    Graphics_RestoreTransferWork();
                }
                if (peak <= 399999)
                {
                    for (j = 0; j != 10; j++)
                    {
                        struct EffectStep *from =
                            &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
                        struct EffectStep *to =
                            &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

                        for (k = 0; k != 16; k++)
                        {
                            s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 16;
                            s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 16;

                            draw[0](canvas, graphics + Data_08197410[size - 1],
                                x - size / 2, y - size, size, size * 2);
                        }
                    }
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
                if (frame == start + i + 10)
                {
                    target->acceleration = 0x20000;
                    target->speed_limit = 0x80000;
                    target->velocity_y = 0x50000;
                    target->gravity = 0xab85;
                    target->action_flags = 0;
                    Object_ResetMotion(target);
                    if (target->x < 0)
                        Object_SetPosition(target, target->x - 0x280000, 0, target->z);
                    else
                        Object_SetPosition(target, target->x + 0x280000, 0, target->z);
                    if (i == 2)
                    {
                        Func_081180e8(134);
                    }
                    else
                    {
                        Audio_PlayCue(134);
                        Func_0814cd48(efx->actors[0], 7, 5, 0, 8);
                    }
                    work->shakeFrames = 4;
                }
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
