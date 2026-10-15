/* Near miss, differ 66 of 429: The Broken Seal's BattleFx_RunTornado as ⚓️ has
   it: the effect argument read directly, the tornado sheet (resource 0x192)
   unpacked after the particles, blitters from Func_081963ec(104, 0x13) and
   (188, 0x17), the callback at 0xc80, the shape, glint offset and size tables
   as Data_081979ae, Data_08197486, Data_08197492 and Data_08197498 (not const),
   Func_081180e8(133) and Func_08118088 for the struck members, and the release
   before the callback is removed. Same frame and registers; what is left is
   order, each a load (pool word, work slot, argument) the native issues one
   instruction earlier, and the literal pools that move with them. */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "fixed_math.h"

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

extern char ResourceId_TornadoSheet;
extern u8 Data_081979ae[];
extern u16 Data_08197486[];
extern u8 Data_08197492[];
extern u8 Data_08197498[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_081434f8(void);
void Func_08158ce0(s32 x, s32 y);
void Func_081180e8(s32 phase);
void Func_08118088(s32 id, s32 variant);
void Func_081963ec(s32 slot, s32 flags);
void Func_0814cd48(s32 id, s32 objectValue, s32 groupValue, s32 stateSlot, s32 stateValue);
void Audio_PlayCue(s32 cue);

void Func_08148e9c(struct BattleEffectArgument *efx)
{
    DrawRectangle draw[2];
    struct CanvasEffectWork *work;
    void *canvas;
    s32 wave;
    struct EffectStep *point;
    s32 frame;
    s32 x;
    s32 y;
    s32 segment;
    s32 cell;
    s32 i;
    s32 j;

    work = (struct CanvasEffectWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
    Func_081963ec(104, 0x13);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Func_081963ec(188, 0x17);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unknown_b8[1];
    for (j = 0; j != 16; j++)
    {
        point = &work->particles[j];
        point->x = Random16() & 31;
        point->y = (Random16() & 63) + 16;
        point->variant = -(Random16() & 15);
    }
    work->transferMode = 2;
    work->transferValue = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    REG_BLDALPHA = 0x1000;
    WaitFrames(1);
    Audio_PlayCue(141);

    for (frame = 0; frame != 80; frame++)
    {
        wave = Trig_Sin(frame << 10) << 4;
        if (frame == 32)
            Func_081180e8(133);
        for (i = 0; i != 7; i++)
        {
            if (frame == i * 8 + 16)
                Iwram_FillWords(canvas, 0x4000, 0x08080808);
        }
        if (efx->side == 1)
            wave += 0x200000;
        else
            wave -= 0x200000;
        if (frame <= 16)
            REG_BLDALPHA = frame | 0x1000;
        if (frame > 63)
            REG_BLDALPHA = (0x4f - frame) | 0x1000;
        for (i = 0; i != Data_081979ae[efx->variant * 3]; i++)
        {
            x = ((Data_081979ae[efx->variant * 3 + 1] * Trig_Sin((frame << 11) + i * 0x4000) + wave)
                    >> 16)
                + 40;
            y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
            segment = (frame / 2) % 3;
            draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
            draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40, 32);
            draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40, 32);
            for (j = 0; j != 4; j++)
            {
                point = &work->particles[i * 4 + j];
                if (point->variant >= 0)
                {
                    cell = point->variant / 2 + (j / 2) * 3;
                    draw[j & 1](canvas, work->sheet + Data_08197486[cell], point->x + x,
                        point->y + y, Data_08197492[cell], Data_08197498[cell]);
                }
                point->variant++;
                if (point->variant == 6)
                {
                    point->x = Random16() & 31;
                    point->y = (Random16() & 63) + 16;
                    point->variant = 0;
                }
            }
        }
        for (j = 0; j != efx->count; j++)
        {
            for (i = 0; i != 7; i++)
            {
                if (frame == i * 8 + j * 3 + 16)
                {
                    Func_0814cd48(efx->actors[j], 7, 5, j, 4);
                    Func_08118088(efx->actors[j], 6);
                }
            }
        }
        work->shakeFrames = 1;
        Func_08158ce0(Data_081979ae[efx->variant * 3 + 2], Data_081979ae[efx->variant * 3 + 2] * 2);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
