/* Draft (tla-en): listing 436 instructions, draft 436, common 392, differ 88.
 * Written from the listing (☀️'s BattleFx_RunJupiterDjinnFall is only a
 * hint). The pink star burst: 32 smoke puffs rise from the caster for 24
 * frames, the caster rises and leans to the target, and at frame 32 the
 * target bursts into 16 pink stars for 24 frames.
 * Left:
 * - the native frame is 60 bytes with an unreferenced slot at sp+20
 *   between frame (sp+24) and draw (sp+16); 0817c27c has the same unused
 *   slot between the same two. Here "sparks" (&steps[32], set once before
 *   the loop) takes that slot, but it is stored and loaded where the
 *   native rebuilds 0x380 + r9 at each loop. Without it the frame is 56
 *   and every stack offset moves (differ 132). Tried for the slot: the
 *   step or source pointer, angle or speed at function scope, a particles
 *   pointer, sparks set in each block.
 * - after BattleFx_BeginCanvasLayer, REG_BLDALPHA's 0x1010 is loaded from
 *   the pool before the register address: a u16 pinned to r3 with a
 *   one-pass block fixes it (the empty asm of f18284c.c instead moves the
 *   halfword pool by four estimated bytes). Another one-pass block before
 *   the heap loads puts effect's spill first, as the native does.
 * - the native sets Func_081963ec's 3 before its 104.
 * - in the frame-32 block the native schedules the 0x380 + r9 of the
 *   burst loop between the divisions and loads target->x after the
 *   division's 100 (each load comes one row later after a store).
 * Resources 302 (BattleFx_PinkStarSheet) and 353 (BattleFx_SmokeSheet)
 * need their names in system/resource/directory.s when this is adopted.
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "motobj.h"
#include "battle_session.h"
#include "iwram_call.h"

struct FxWork {
    struct EffectStep steps[48];
    u8 unk0540[0x700 - 0x540];
    u8 sheet[0x7780 - 0x700];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x20];
    s32 shakeFrames;
    u8 unk77ac[0x78e8 - 0x77ac];
    s32 transferPending;
};

extern char ResourceId_PinkStarSheet;
extern char ResourceId_SmokeSheet;

void BattleFx_BeginCanvasLayer(s32 bg_control);
void *Resource_GetTableEntry(s32 resourceId);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_081963ec(s32 slot, s32 value);
void Func_0817b970(s32 id, s32 priority);
void Func_08157530(s32 x, s32 y, s32 width, s32 height);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration, s32 speedLimit, s32 velocityY, s32 percent);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

#define HI(v) (((s16 *)&(v))[1])

void Func_0817b9b8(struct BattleEffectArgument *effect)
{
    struct FxWork *work;
    u8 *canvas;
    struct EffectPosition pos;
    struct EffectPosition burst;
    s32 frame;
    struct EffectStep *sparks;
    DrawRectangle draw;
    struct MotionObject *target;
    struct MotionObject *caster;
    u8 *battle;
    struct EffectStep *step;
    s32 i;

    do { } while (0);
    work = (struct FxWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(0);
    {
        register u16 alpha __asm__("r3") = 0x1010;

        do { } while (0);
        REG_BLDALPHA = alpha;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet, work->sheet + 0x3e80, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
    Func_081963ec(104, 3);
    draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
    work->transferMode = 2;
    work->transferValue = 50;
    Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
    target = GetBattleObjectSlotFar(effect->actors[0])->object;
    caster = GetBattleObjectSlotFar(effect->actor)->object;
    battle = (u8 *)Ram_HeapSlots->battleWork;
    battle[0x866] = 1;
    sparks = &work->steps[32];
    for (frame = 0; frame != 60; frame++) {
        Func_0817b970(effect->actor, 3);
        if (frame == 0) {
            EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &pos);
            for (i = 0, step = work->steps; i != 32; i++, step++) {
                s32 angle;
                s32 speed;

                step->variant = 0;
                step->x = (pos.x / 2) << 16;
                step->y = (pos.y - 8) << 16;
                angle = Random16();
                speed = (Random16() & 255) + 127;
                step->velocityX = (Trig_Sin(angle) * speed) >> 5;
                step->velocityY = (Trig_Cos(angle) * speed) >> 5;
            }
            Audio_PlayCue(136);
        }
        if (frame >= 0) {
            for (i = 0, step = work->steps; i != 24; i++, step++) {
                if (frame >= i / 8 && step->variant <= 23) {
                    draw(canvas, work->sheet + (step->variant / 4) * 0x480, HI(step->x) - 12,
                        HI(step->y) - 24, 24, 48);
                    BattleFxKernels_IntegrateVector2(step, 48, -0x2000);
                    step->variant++;
                }
            }
            if (frame == 0) {
                caster->z = target->z;
                caster->x = target->x * 160 / 100;
                caster->angle = target->angle;
            }
        }
        if (frame == 16) {
            Func_08157530(effect->actor, effect->actors[0], 16, 120);
            caster->velocityY = 0x70000;
        }
        if (frame == 32) {
            Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_PinkStarSheet), 128);
            caster->z = target->z;
            caster->x = target->x * 120 / 100;
            target->x = target->x * 80 / 100;
            caster->angle = target->angle;
            EffectPosition_ApplyStepAndYOffset(effect->actors[0], &burst);
            for (i = 0, step = sparks; i != 16; i++, step++) {
                s32 angle;
                s32 speed;

                step->variant = (Random16() & 15) + 16;
                step->x = (burst.x / 2) << 16;
                step->y = (burst.y - 8) << 16;
                angle = (Random16() & 0x7fff) + 0x4000;
                speed = (Random16() & 255) + 127;
                step->velocityX = (Trig_Sin(angle) * speed) >> 7;
                step->velocityY = (Trig_Cos(angle) * speed) >> 5;
            }
            work->shakeFrames = 4;
            Func_081180e8(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
            BattleFx_StartActorMotion(effect->actors[0], 1, 0x80000, 0x140000, 0x60000, 10);
        }
        if ((u32)(frame - 32) <= 23) {
            for (i = 0, step = sparks; i != 16; i++, step++) {
                if (step->variant >= 0) {
                    draw(canvas, work->sheet + 0x3e80 + (((frame + i) / 4) % 6) * 256, HI(step->x) - 8,
                        HI(step->y) - 8, 16, 16);
                    BattleFxKernels_IntegrateVector2(step, 56, 0);
                    step->variant--;
                }
            }
        }
        Func_08158ce0(4, 4);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    battle[0x866] = 0;
    Scheduler_RemoveCallback((s32)Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
