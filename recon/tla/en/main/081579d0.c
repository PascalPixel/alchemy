/* Near miss of Func_081579d0, ported from The Broken Seal's
 * BattleFx_RunJupiterDjinnFall (listing 354 instructions, draft 354,
 * differ 32): about a dozen reordered pairs, nearly all a load placed one
 * row earlier by the native (heap words, pool constants, stack reloads), and
 * one branch over a block two bytes longer. */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "ram_buffer.h"

extern char ResourceId_Data134;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_FlameballSheet;
extern const u16 Data_08197410[];

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

void Func_081579d0(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    u8 *sheet;
    DrawRectangle blit[2];
    DrawRectangle *draw;
    struct EffectStep *ball;
    struct EffectStep *mote;
    s32 i;
    s32 j;
    s32 angle;

    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    sheet = Ram_HeapSlots->unk64[0];
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;
    draw = blit;
    Func_08144aac(0, draw);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet, work->sheet + 0x320, 1, 0);
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    for (i = 0; i != 32; i++) {
        ball = &work->particles[i];
        ball->x = (Random16() & 63) + 64;
        ball->y = (Random16() & 63) - 80;
    }
    for (i = 0; i != 512; i++)
        PARTICLES[i].variant = -1;
    Audio_PlayCue(171);
    frame = 0;
    angle = 0x8000;
    do {
        if (frame == 56)
            Func_081180e8(133);

        if (frame <= 95) {
            s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
            s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

            blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
        }

        for (i = 0; i != 8; i++) {
            ball = &work->particles[i];
            if (frame >= i * 4 + 8 && ball->y <= 95) {
                blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40, 64);
                ball->x -= 6;
                ball->y += 12;
                if (ball->y > 95) {
                    for (j = 0; j != 32; j++) {
                        s32 dir;
                        s32 speed;

                        mote = &PARTICLES[i * 32 + j];
                        dir = Random16() & 0xffff;
                        speed = (Random16() & 0x1ff) + 256;
                        mote->x = ball->x << 16;
                        mote->y = ball->y << 16;
                        mote->velocityX = (Trig_Sin(dir) * speed) >> 7;
                        mote->velocityY = (Trig_Cos(dir) * speed) >> 6;
                        mote->variant = (Random16() & 15) + 32;
                    }
                    Audio_PlayCue(133);
                    work->shakeFrames = 4;
                    for (j = 0; j != effect->count; j++) {
                        Func_0814cd48(effect->actors[j], 7, 5, j, 6);
                        Func_08118088(effect->actors[j], 6);
                    }
                }
            }
        }

        for (i = 0; i != 512; i++) {
            mote = &PARTICLES[i];
            if (mote->variant != -1) {
                s32 size = mote->variant / 16 + 1;

                draw[1](canvas, sheet + Data_08197410[size - 1],
                    HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
                BattleFxKernels_IntegrateVector2(mote, 62, 0x2000);
                mote->variant--;
            }
        }
        Func_08158ce0(4, 4);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
        angle -= 0x800;
        frame++;
    } while (frame != 96);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
