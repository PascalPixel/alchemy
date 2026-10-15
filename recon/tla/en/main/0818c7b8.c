/* Near miss: listing 303 instructions, draft 297, differ 162. The statements, the two jump
 * tables and their cross-jumped cases match (the second switch lists case 1 before case 0, as the
 * native's block order shows); what is left is allocation. The native spills effect to sp+8
 * (frame 24: effect below the 12-byte position at sp+12) and pushes r7 without using it, so
 * effect was given r7 and lost it to reload; blit is r6, canvas r8, work r10, frame r9, mode r11.
 * Here effect ranks last (11 refs over 264 insns against canvas's 21) and keeps r11, so there is
 * no spill and the frame is 20. Measured: effect made addressable with (void)&effect: no change
 * (addressof gives the register back); referenced from an inline nested function: effect moves to
 * sp+8 but is reached through a pointer in r11, differ 179; a pointer to position taken once: 161,
 * taken in the loop: 183. Resource ids 0x10b, 0x10c and 0x150 need names in directory.s.
 * permute (10 min): nothing below 162 by differ. */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect_work.h"

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width, s32 height);

struct EffectPosition;

extern char ResourceId_GreyCrescentSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_MarsDjinnSheet;
extern u8 Data_02014000[];

void BattleFx_BeginTiledCanvas(s32 mode);
void Func_08196404(s32 slot, s32 a1, s32 a2, s32 a3, s32 a4);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 cue);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_08158ce0(s32, s32);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

void Func_0818c7b8(struct BattleEffectArgument *effect, s32 mode)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];
    CanvasBlitFunc blit;
    s32 kernel;
    s32 frame;
    s32 position[3];

    BattleFx_BeginTiledCanvas(0);
    if (mode == 0)
    {
        kernel = 3;
        if (effect->side != 0)
            kernel = 7;
    }
    else
    {
        kernel = 11;
        if (effect->side != 0)
            kernel = 15;
    }
    Func_08196404(104, 7, 7, kernel, 1);
    blit = (CanvasBlitFunc)Ram_HeapSlots->unknown_64[1];
    Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet, Ram_MapCellBuffer, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work + 0x700, 1, 0);
    if (mode == 0)
    {
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
        REG_BLDALPHA = 0x810;
        *(s32 *)(work + 0x7780) = 2;
        *(s32 *)(work + 0x7784) = 50;
    }
    else
    {
        REG_BLDALPHA = 0x1010;
        *(s32 *)(work + 0x7780) = 2;
        *(s32 *)(work + 0x7784) = 75;
    }
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8, 0x40000);
    for (frame = 0; frame != 32; frame++)
    {
        if (frame == 0)
            EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], (struct EffectPosition *)position);
        if (frame == 8)
            Audio_PlayCue(212);
        if (frame == 12)
        {
            if (mode == 1)
                Func_081180e8(144);
            else
                Func_081180e8(134);
            BattleFx_StartActorMotion(effect->actors[0], 1, 0x40000, 0x100000, 0x40000, 250);
            *(s32 *)(work + 0x77a8) = 4;
        }
        if (frame > 7)
        {
            s32 step = (frame - 8) / 3;
            s32 y = mode != 0 ? 16 : 0;
            s32 x;

            if (effect->side == 0)
                x = position[0] / 2 - 48;
            else
                x = position[0] / 2 - 80;
            if (mode == 0)
            {
                switch (step)
                {
                case 0:
                    blit(canvas, work + 0x700, x, y, 120, 120);
                    break;
                case 1:
                    Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
                    break;
                case 2:
                    blit(canvas, Ram_MapCellBuffer, x - 4, y, 128, 128);
                    break;
                case 3:
                    blit(canvas, Data_02014000, x - 4, y, 128, 128);
                    break;
                case 4:
                    blit(canvas, work + 0x3f40, x, y, 120, 120);
                    break;
                }
            }
            else
            {
                switch (step)
                {
                case 1:
                    Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
                    break;
                case 0:
                    blit(canvas, work + 0x700, x, y, 120, 120);
                    break;
                case 2:
                    blit(canvas, Ram_MapCellBuffer, x - 4, y - 8, 128, 128);
                    break;
                case 3:
                    blit(canvas, Data_02014000, x - 4, y - 8, 128, 128);
                    break;
                case 4:
                    blit(canvas, work + 0x3f40, x, y, 120, 120);
                    break;
                }
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        *(s32 *)(work + 0x78e8) = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
