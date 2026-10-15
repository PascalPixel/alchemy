/* Near miss, differ 44 of 222: The Broken Seal's BattleFx_RunTargetMarkers as ⚓️
   has it: the effect argument is read directly (no work->effect), the
   particles sit at the head of the work with the sheet after them, the blitter
   comes from Func_081963ec(104, 0x23) and the transfer callback runs at 0xc80.
   The frame slots and every instruction match; what is left is order: the
   native loads the work slot before it sets the first argument, loads each
   pool word (0x100, -0x7000) ahead of the register address, sets r1 before r0
   for Func_081963ec, and reloads its spills one row earlier in the marker
   loop. Tried: the blitter flags in a local first (no change). */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_efx.h"

// The battle effect work as this effect uses it.
struct MarkerWork
{
    struct EffectStep particles[64];
    u8 sheet[0x7080];
    s32 transferMode;
    s32 transferValue;
    u8 unknown_7788[0x160];
    s32 transferPending;
};

extern char ResourceId_FlameColumnSheet;

void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081180e8(s32 phase);
void Func_081963ec(s32 slot, s32 flags);

void Func_0814b160(struct BattleEffectArgument *efx)
{
    void *canvas;
    struct MarkerWork *work;
    DrawRectangle draw[2];
    struct EffectStep *marker;
    s32 i;
    s32 frame;
    s32 start;
    s32 cel;
    s32 y;
    struct EffectPosition pos;

    canvas = Ram_HeapSlots->battle_fx_work[1];
    work = (struct MarkerWork *)Ram_HeapSlots->battle_fx_work[0];
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1, 1);
    if (efx->side == 1)
    {
        REG_BG2X = -0x7000;
    }
    Func_081963ec(104, 0x23);
    work->transferMode = 1;
    work->transferValue = 0;
    draw[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    for (i = 0; i != efx->count; i++)
    {
        work->particles[i].variant = Random16() & 63;
    }
    frame = 0;
    while (frame != (efx->count << 5) + 32)
    {
        if (frame == 32)
        {
            Func_081180e8(0);
        }
        i = 0;
        if (efx->count != 0)
        {
            marker = work->particles;
            do
            {
                start = i << 4;
                if (frame == start)
                {
                    Audio_PlayCue(143);
                }
                if (frame >= start && frame < start + 72)
                {
                    EffectPosition_ApplyStepAndYOffset(efx->actors[i], &pos);
                    if (efx->side == 1)
                    {
                        pos.x -= 112;
                    }
                    y = pos.y;
                    pos.y = y - 16;
                    draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
                    if (frame >= start)
                    {
                        cel = ((frame - start) + marker->variant) / 6 % 9;
                        draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
                    }
                }
                marker++;
                i++;
            } while (i != efx->count);
        }
        work->transferPending = 1;
        WaitFrames(1);
        frame += 1;
    }
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
