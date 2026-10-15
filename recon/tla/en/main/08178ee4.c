/* Draft (tla-en): listing 525 instructions, draft 525, common 488, differ 74.
 * Written from the listing (☀️'s BattleFx_RunCounterReveal is only a
 * hint). A large sprite assembled from gMapCellBuffer cells sways on a
 * sine path over 108 frames, faded in and out through BLDALPHA, with two
 * blitters (heap slots 104 and 188) chosen by the effect's side.
 * Devices already in (each a one-pass block): one before the heap loads so
 * effect's spill comes first, as the native; one between BG2PA's pinned
 * 0x100 and its store so the pool load comes before the address.
 * Left:
 * - the native loads gMapCellBuffer into r6 before BattleFx_BeginTiledCanvas
 *   (here after it).
 * - both Func_081963ec calls set r1 before r0 in the native; sched2 breaks
 *   the tie by RTL order here (the two moves are symmetric in the
 *   dependence lists).
 * - the stores of the blitters' stack arguments and the side index's
 *   "lsls r1, r1, #2" are one row apart in several calls, and the
 *   halfword pool loads of the 16 in the BLDALPHA fades come one row
 *   earlier in the native.
 * Resource 220 needs its name in system/resource/directory.s when this is
 * adopted. The cell addresses are gMapCellBuffer offsets (the listing's
 * pool names them Data_020124de, Data_02011f20, Data_02012e6e and
 * Data_02014e9d).
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "iwram_call.h"

struct FxWork {
    struct EffectStep step;
    u8 unk001c[0x7780 - 0x1c];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x20];
    s32 shakeFrames;
    u8 unk77ac[0x78e8 - 0x77ac];
    s32 transferPending;
};

extern char ResourceId_Data0DC;
extern const u16 Data_081993d8[];
extern const u8 Data_081993d0[];
extern const u8 Data_081993d4[];
extern const u16 Data_081993f0[];
extern const u8 Data_081993e8[];
extern const u8 Data_081993e0[];

void BattleFx_BeginTiledCanvas(s32 bg_control);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_081963ec(s32 slot, s32 value);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void BattleBackground_LoadFar(s32, s32, s32);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

#define HI(v) (((s16 *)&(v))[1])

void Func_08178ee4(struct BattleEffectArgument *effect)
{
    struct HeapSlots *heap;
    struct FxWork *work;
    u8 *canvas;
    u8 *cells;
    DrawRectangle blit[2];
    struct EffectPosition pos;
    s32 frame;
    s32 x;
    s32 y;

    do { } while (0);
    heap = Ram_HeapSlots;
    work = (struct FxWork *)heap->battleFxWork[0];
    canvas = heap->battleFxWork[1];
    cells = Ram_MapCellBuffer;
    BattleFx_BeginTiledCanvas(0x2001);
    {
        register u16 scale __asm__("r3") = 0x100;

        do { } while (0);
        REG_BG2PA = scale;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_Data0DC, cells, 1, 1);
    Func_081963ec(104, 35);
    blit[0] = (DrawRectangle)heap->unk64[1];
    Func_081963ec(188, 39);
    blit[1] = ((DrawRectangle *)heap)[47];
    work->transferMode = 1;
    work->transferValue = 0;
    Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &pos);
    for (frame = 0; frame != 108; frame++) {
        if (frame == 0) {
            if (effect->side == 0)
                work->step.x = (pos.x + 40) << 16;
            else
                work->step.x = (pos.x - 168) << 16;
            work->step.y = -100 << 16;
            if (effect->side == 0)
                work->step.velocityX = -6 << 16;
            else
                work->step.velocityX = 6 << 16;
            work->step.velocityY = 8 << 16;
            REG_BLDALPHA = 0x1000;
        }
        if (frame <= 31)
            BattleFxKernels_IntegrateVector2(&work->step, 60, 0);
        if (frame <= 32) {
            s32 level = frame / 2;

            REG_BLDALPHA = ((16 - level / 2) << 8) | level;
        }
        if (frame > 91) {
            s32 level = 108 - frame;

            REG_BLDALPHA = ((16 - level / 2) << 8) | level;
            BattleFxKernels_IntegrateVector2(&work->step, 60, -0x8000);
        }
        x = HI(work->step.x) + ((Trig_Sin(frame << 8) << 3) >> 16);
        y = HI(work->step.y) + ((Trig_Cos(frame << 8) << 3) >> 16);
        REG_BG2X = -x << 8;
        if (effect->side == 0) {
            blit[effect->side](canvas, Ram_MapCellBuffer, 25, y + 2, 96, 83);
            if ((u32)frame <= 47)
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x24de, 31, y + 22, 48, 51);
            else
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x1f20, 64, y + 16, 42, 35);
        } else {
            blit[effect->side](canvas, Ram_MapCellBuffer, 7, y + 2, 96, 83);
            if ((u32)frame <= 47)
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x24de, 49, y + 22, 48, 51);
            else
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x1f20, 22, y + 16, 42, 35);
        }
        if ((u32)(frame - 28) <= 11) {
            s32 cell = (frame - 28) / 3;

            if (effect->side == 0) {
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x2e6e + Data_081993d8[cell],
                    46 - Data_081993d0[cell] / 2, y - Data_081993d4[cell] / 2 + 61, Data_081993d0[cell],
                    Data_081993d4[cell]);
            } else {
                blit[effect->side](canvas, Ram_MapCellBuffer + 0x2e6e + Data_081993d8[cell],
                    Data_081993d0[cell] / 2 - Data_081993d0[cell] + 82, y - Data_081993d4[cell] / 2 + 61,
                    Data_081993d0[cell], Data_081993d4[cell]);
            }
        }
        if (frame > 39) {
            if (frame <= 47) {
                if (effect->side == 0)
                    blit[effect->side](canvas, Ram_MapCellBuffer + 0x4e9d, 30, y + 46, 32, 29);
                else
                    blit[effect->side](canvas, Ram_MapCellBuffer + 0x4e9d, 66, y + 46, 32, 29);
            } else {
                s32 cell = (frame - 56) / 6;

                if (cell < 0)
                    cell = 0;
                if (cell <= 7) {
                    if (effect->side == 0)
                        blit[effect->side](canvas, Ram_MapCellBuffer + 0x4e9d + Data_081993f0[cell], 63,
                            y + Data_081993e8[cell] - 8, 32, Data_081993e0[cell]);
                    else
                        blit[effect->side](canvas, Ram_MapCellBuffer + 0x4e9d + Data_081993f0[cell], 33,
                            y + Data_081993e8[cell] - 8, 32, Data_081993e0[cell]);
                }
            }
        }
        if (frame == 80) {
            work->shakeFrames = 6;
            Func_081180e8(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
        }
        if (frame == 30) {
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
            work->shakeFrames = 3;
            Audio_PlayCue(212);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
        }
        Func_08158ce0(4, 4);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)Func_08143000);
    BattleBackground_LoadFar(1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670), 24);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
