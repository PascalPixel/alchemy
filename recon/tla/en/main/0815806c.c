/* Draft of Func_0815806c, ported from The Broken Seal's
 * BattleFx_RunMarsDjinnPuffs (listing 421 instructions, draft 421,
 * differ 68): the canvas and effect work trade registers (r9 and fp)
 * through the whole function, and the effect work is read before the
 * canvas where the native reads the canvas first; the rest are reordered
 * loads. */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "resource.h"

extern char ResourceId_Data134;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;
extern u16 Data_08197410[];
extern u8 Data_08197467[];
extern u8 Data_0819746d[];
extern u8 Data_08197473[];
extern u16 Data_0819747a[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_081434f8(void);
void Func_08144aac(s32 flip, DrawRectangle *out);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value);
void Func_08158ce0(s32 random_mask, u32 shake_range);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);

#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define HI(v) (((s16 *)&(v))[1])

void Func_0815806c(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    u8 *sheet;
    DrawRectangle callbacks[2];
    DrawRectangle *draw;
    struct EffectStep *step;
    struct EffectStep *mote;
    s32 member;
    s32 frame;
    s32 i;
    s32 curtain_y;

    canvas = Ram_HeapSlots->battleFxWork[1];
    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    sheet = Ram_HeapSlots->unk64[0];
    BattleFx_BeginCanvasLayer(0);
    draw = callbacks;
    Func_08144aac(0, draw);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320, 1, 1);
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    member = 0;
    step = work->particles;
    do {
        s32 angle = member << 11;

        step->x = (Trig_Sin(angle) * 24) >> 16;
        step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
        if (member & 1)
            step->x = 32 - step->x;
        else
            step->x += 32;
        step->variant = -(member * 2);
        for (i = 0; i != 16; i++) {
            mote = &PARTICLES[member * 16 + i];
            mote->x = (((Random16() & 15) + step->x) - 8) << 16;
            mote->y = ((Random16() & 7) + 96) << 16;
            mote->velocityX = ((Random16() & 127) - 64) << 11;
            mote->velocityY = ((Random16() & 127) - 64) << 10;
            mote->z = Random16() & 0xffff;
            mote->velocityZ = Random16() & 0xffff;
        }
        step++;
        member++;
    } while (member != 9);

    Audio_PlayCue(0x88);
    frame = 0;
    curtain_y = -172;
    do {
        if (frame == 56)
            Func_081180e8(0x85);
        if (frame <= 23)
            callbacks[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40, 40);
        if (frame == 20)
            Iwram_CopyWords(
                (void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
        if (frame >= 20 && frame <= 31) {
            if (frame > 23)
                draw[1](canvas, work->sheet, 146 - frame * 4, curtain_y, 20, 40);
            else
                callbacks[0](canvas, work->sheet, 50, 20, 20, 40);
        }
        if (frame == 32) {
            Audio_PlayCue(0x91);
            work->shakeFrames = 8;
            Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
        }
        if (frame > 31) {
            member = 0;
            step = work->particles;
            do {
                if (step->variant >= 0 && step->variant <= 47) {
                    s32 cell = step->variant / 8;
                    u32 width;

                    callbacks[0](canvas, work->sheet + Data_0819747a[cell],
                        step->x - ((width = Data_08197467[cell]) >> 1),
                        step->y + Data_08197473[cell], width, Data_0819746d[cell]);
                }
                member++;
                step->variant++;
                step++;
            } while (member != 9);
        }
        step = PARTICLES;
        member = 0;
        do {
            if (frame >= (member / 16) * 2 + 40) {
                s32 size;
                s32 x;
                s32 angle;

                size = (member & 1) + 3;
                x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
                draw[1](canvas, sheet + Data_08197410[size - 1], x - ((u32)size >> 1),
                    HI(step->y) - size, size, size * 2);
                BattleFxKernels_IntegrateVector2(step, 64, -0x2000);
                angle = step->z;
                step->z = angle + 0x800;
                if (step->z > 0xffff)
                    step->z = angle + 0x800 - 0xffff;
            }
            member++;
            step++;
        } while (member != 144);

        if (frame == 38) {
            for (member = 0; member != effect->count; member++) {
                Func_0814cd48(effect->actors[member], 7, 5, member, 16);
                Func_08118088(effect->actors[member], 6);
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
        curtain_y += 8;
        frame++;
    } while (frame != 112);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
