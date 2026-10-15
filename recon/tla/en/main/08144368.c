/* Draft, differ 123 of 294: The Broken Seal's BattleFx_RunMemberOrbit as ⚓️ has
   it: the effect argument read directly, the spiral sheet (resource 0x173)
   unpacked into the work after the particles, two blitters from
   Func_081963ec(104, 0x13) and (188, 0x1f), both callbacks at 0xc80 and
   Func_0814cd48 for the members' timers. The instructions are the native's;
   what is left is allocation: the native keeps the effect argument in r9, the
   point array's address in r10 and takes the screen position's address in
   the member loop (add r5, sp, #44, then r11 for the four cels), in a 68-byte
   frame; here the screen address is hoisted and spilled (72 bytes) and the
   argument takes r11. */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "fixed_math.h"
#include "transform.h"
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
    u8 unknown_7788[0x160];
    s32 transferPending;
};

struct BattleObjectSlot
{
    struct ObjectRuntime *object;
};

extern char ResourceId_SpiralSheet;

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_08152474(void);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);

void Func_08144368(struct BattleEffectArgument *effect)
{
    struct CanvasEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    s32 point[3];
    struct EffectPosition screen;
    s32 member;
    s32 offset_x;
    s32 frame;

    work = (struct CanvasEffectWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
    Func_081963ec(104, 0x13);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Func_081963ec(188, 0x1f);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unknown_b8[1];
    Scheduler_AddOrUpdateCallback(Func_08152474, 0xc80);
    work->transferMode = 2;
    work->transferValue = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    if (effect->side == 1)
    {
        REG_BG2X = -0x6800;
        offset_x = -112;
    }
    else
    {
        offset_x = 0;
    }
    for (frame = 0; frame != (effect->count * 16) + 48; frame++)
    {
        s32 *camera;
        s32 *row;
        s32 i;

        // The background waves sideways line by line.
        camera = Ram_HeapSlots->camera_work;
        row = work->bg2X;
        if (effect->side == 0)
        {
            s32 angle;
            s32 base;

            for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++)
            {
                *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
                angle += 1024;
            }
        }
        else
        {
            s32 angle;

            for (i = 0, angle = frame << 10; i != 160; i++)
            {
                *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
                angle += 1024;
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera, camera + 3);
        member = 0;
        if (effect->count != 0)
        {
            while (member != effect->count)
            {
                struct ObjectRuntime *obj;

                obj = GetBattleObjectSlotFar(effect->actors[member])->object;
                if (frame > member * 16 && frame < (member * 16) + 60)
                {
                    s32 spin;

                    if (frame == (member * 16) + 32)
                    {
                        Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
                    }
                    point[0] = obj->x;
                    point[1] = 40 << 16;
                    point[2] = obj->z;
                    EffectPosition_ApplyBaseAndYOffset(point, &screen);

                    // The spiral (32x32): 4 cels, the blitter switching every 16 frames.
                    for (i = 0; i != 4; i++)
                    {
                        s32 x;
                        s32 y;
                        s32 half;

                        spin = (frame << 9) + (i << 14);
                        x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offset_x;
                        y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
                        half = frame / 16;
                        draw[half & 1](canvas, work->sheet + (((frame / 4) - (half * 4)) << 10),
                            x - 16, y - 16, 32, 32);
                    }
                }
                member++;
            }
        }
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Scheduler_RemoveCallback(Func_08152474);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
