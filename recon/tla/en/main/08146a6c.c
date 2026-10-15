/* Near miss, differ 32 of 410: The Broken Seal's BattleFx_RunSpinningStars as ⚓️
   has it: the effect argument read directly, the rune palette (resource 0x13a)
   copied and resource 0x134 unpacked into the sheet slot, one blitter from
   Func_081963ec(104, 0x13) loaded before the transfer settings, the callback
   at 0xc80, Func_081496c8 for the phase palette, byte radii and a fixed
   2-pixel size. The two tables are not const: as const arrays loop.c hoists
   the cell offset out of the draw loops and spills it (differ 182). Same
   frame, registers and instructions; what is left is order at eleven rows,
   each a load (spill reload, work slot, sine operand) the native issues one
   instruction earlier than the constant or store beside it. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "fixed_math.h"
#include "object_runtime.h"

// The battle effect work as the canvas effects use it.
struct CanvasEffectWork
{
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    s32 bg2X[160];
    u8 unk7300[0x480];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x20];
    s32 shakeFrames;
    u8 unk77ac[0x13c];
    s32 transferPending;
};

struct BattleObjectSlot
{
    struct ObjectRuntime *object;
};

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern u8 Data_0819793c[];
extern u16 Data_08197410[];
extern u8 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081434f8(void);
void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase);
void Func_08158ce0(s32 x, s32 y);
void Func_081180e8(s32 phase);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
void Audio_PlayCue(s32 cue);
void *Resource_GetTableEntry(s32 id);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);

void Func_08146a6c(struct BattleEffectArgument *effect)
{
    struct CanvasEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    s32 *camera;
    struct ObjectRuntime *source;
    struct ObjectRuntime *target;
    struct EffectStep *point;
    struct EffectStep *step;
    struct EffectPosition pos;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    work = (struct CanvasEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    graphics = Ram_HeapSlots->unk64[0];
    camera = Ram_HeapSlots->cameraWork;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    Func_081963ec(104, 0x13);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    work->transferMode = 2;
    work->transferValue = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    source = GetBattleObjectSlotFar(effect->actor)->object;
    target = GetBattleObjectSlotFar(effect->actors[0])->object;
    for (i = 0, point = work->particles; i != 8; i++)
    {
        s32 delay = i * 8;

        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->velocityX = (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
        point->velocityY = (target->y - point->y + 0x140000) / 12;
        point->velocityZ = (target->z - point->z) / 12;
        point->variant = (Random16() & 15) + delay;
        point++;
    }

    size = 2;
    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 128; frame++)
    {
        Func_081496c8(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            Func_081180e8(134);
        for (i = 0; i != 8; i++)
        {
            step = &work->particles[i];
            if (frame >= step->variant)
            {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera, camera + 3);
                EffectPosition_ApplyBaseAndYOffset((s32 *)step, &pos);
                pos.x >>= 1;
                if (pos.x >= -8 && pos.x <= 127)
                {
                    if (pos.y <= 127)
                    {
                        if (pos.y >= -8)
                        {
                            for (j = 0; j != 10; j++)
                            {
                                trails[i * 10 + j].velocityX =
                                    pos.x
                                    + ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11))
                                           * Data_0819793c[j & 1])
                                            / 2
                                        >> 16);
                                trails[i * 10 + j].velocityY =
                                    pos.y
                                    - (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11))
                                            * Data_0819793c[j & 1]
                                        >> 16);
                            }
                            for (j = 0; j != 10; j++)
                            {
                                struct EffectStep *from = &trails[j + i * 10];
                                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                                for (k = 0; k != 12; k++)
                                {
                                    s32 x = from->velocityX
                                            + (to->velocityX - from->velocityX) * k / 12;
                                    s32 y = from->velocityY
                                            + (to->velocityY - from->velocityY) * k / 12;

                                    draw[0](canvas, graphics + Data_08197410[size - 1],
                                        x - size / 2, y - size, size, size * 2);
                                }
                            }
                        }
                    }
                }
                if (step->y <= 0x1dffff)
                {
                    step->velocityY = -step->velocityY;
                    step->velocityX /= 2;
                    step->velocityZ /= 2;
                    work->shakeFrames = 4;
                    Audio_PlayCue(134);
                    for (j = 0; j != effect->count; j++)
                        Func_0814cd48(effect->actors[j], 7, 5, j, 8);
                }
                step->x += step->velocityX;
                step->y += step->velocityY;
                step->z += step->velocityZ;
            }
        }
        Func_08158ce0(4, 4);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
