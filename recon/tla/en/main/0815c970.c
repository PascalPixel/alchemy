/* Draft of Func_0815c970, ported from The Broken Seal's
 * BattleFx_RunTwoResource (listing 260 instructions, draft 252, differ 202).
 * The logic and constants agree; the remaining rows are register
 * allocation. The native keeps effect in r8, work in r10, mode in r11, the
 * blitter in r6 and the heap base in r5 across the first two calls, spills
 * canvas and frames to the stack (28-byte frame) and leaves r7 unused;
 * here effect, work and canvas take r6, r7 and fp. Matched on the way: the
 * heap base through a local (the native keeps it in r5) and the blitter
 * load in each branch, which cross-jumps the two loader calls into one. */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
extern char ResourceId_LimePalette;

extern const u16 Data_081986d4[];
extern const u8 Data_081986e0[];
extern const u8 Data_081986c8[];
extern const s8 Data_081986ec[];
extern const u8 Data_081986ce[];

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_081434f8(void);
void Func_081963ec(s32 slot, s32 mode);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 actor, s32 a, s32 b, s32 c, s32 d);
void Func_08158ce0(s32 random_mask, u32 shake_range);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);

// The Broken Seal's BattleFx_RunTwoResource.
void Func_0815c970(struct BattleEffectArgument *effect, s32 mode)
{
    struct HeapSlots *slots;
    struct BattleEffectWork *work;
    void *canvas;
    struct MotionObject *object;
    struct EffectPosition position;
    DrawRectangle draw;
    s32 frames;
    s32 frame;
    s32 cell;

    slots = Ram_HeapSlots;
    work = (struct BattleEffectWork *)slots->battleFxWork[0];
    canvas = slots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    if (effect->side == 1) {
        Func_081963ec(104, 35);
        draw = (DrawRectangle)slots->unk64[1];
    } else {
        Func_081963ec(104, 39);
        draw = (DrawRectangle)slots->unk64[1];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
    if (mode == 0) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
        Iwram_CopyWords((void *)0x05000000, palette, 128);
    }
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    object = GetBattleObjectSlotFar(effect->actors[0])->object;
    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &position);
    if (effect->side == 0)
        REG_BG2X = (16 - position.x) << 8;
    else
        REG_BG2X = (112 - position.x) << 8;
    frames = 74;
    if (mode != 1)
        frames = 48;
    for (frame = 0; frame != frames; frame++) {
        cell = frame / 4;
        if (cell <= 5) {
            if (cell <= 3)
                draw(canvas, work->sheet + Data_081986d4[cell],
                    Data_081986e0[cell + effect->side * 6],
                    Data_081986ec[cell] + 32, Data_081986c8[cell],
                    Data_081986ce[cell]);
            else
                draw(canvas, Ram_MapCellBuffer + Data_081986d4[cell],
                    Data_081986e0[cell + effect->side * 6],
                    Data_081986ec[cell] + 32, Data_081986c8[cell],
                    Data_081986ce[cell]);
        }
        if (frame == 8) {
            if (mode == 0) {
                Func_081180e8(0x85);
                Func_08118088(effect->actors[0], 1);
            } else {
                Audio_PlayCue(0x86);
                Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
            }
            work->shakeFrames = 8;
        }
        if (mode == 1) {
            if (frame == 13) {
                object->velocityY = 0xc0000;
                object->verticalMotionStrength = 0x7851;
                object->verticalMotionPhase = 0x4000;
            }
            if (frame == 65) {
                work->shakeFrames = 4;
                Func_081180e8(0x86);
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
