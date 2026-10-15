/* Draft (tla-en): listing 464 instructions, draft 462, common 321, differ 284.
 * Written from the listing (☀️'s BattleFx_RunParticleFieldVariant is only
 * a hint). A five-cell burst drawn on the actor's side for 30 frames;
 * at frame 18 256 particles (in gMapCellBuffer) scatter from it, drawn
 * from frame 18 on with sizes from Data_08197410.
 * Left:
 * - the native frame is 40 bytes with an unreferenced slot at sp+16
 *   between frame (sp+20) and draw (sp+12), as in 0817b9b8; canvas stays
 *   in r11 and draw and sheet are spilled. Here the frame is 36 and canvas
 *   is spilled.
 * - the heap slot base stays in r5 from the first loads to draw's load
 *   after the Func_081963ec call, as through the "heap" pointer here.
 * - the native keeps one Func_081963ec call after the two argument pairs;
 *   here each arm has its call.
 * - cross-jumping of the switch's draw calls: in the native side 0's case
 *   0 keeps its own call (its draw is reloaded into r6, the rest into r4),
 *   cases 1 and 2 of both sides share one call and cases 3 and 4 share
 *   "subs r3, #88" before it.
 * Resources 308, 243 and 329 (BattleFx_EffectPalette02) need names in
 * system/resource/directory.s when this is adopted.
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "iwram_call.h"

struct FxWork {
    u8 unknown_0000[0x700];
    u8 sheet[0x7780 - 0x700];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake_frames;
    u8 unknown_77ac[0x78e8 - 0x77ac];
    s32 transfer_pending;
};

extern char ResourceId_Data134;
extern char ResourceId_Data0F3;
extern char ResourceId_EffectPalette02;
extern const u16 Data_08197410[];

void BattleFx_BeginCanvasLayer(s32 bg_control);
void *Resource_GetTableEntry(s32 resource_id);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_08118088(s32 id, s32 value);
void Func_081180e8(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_081963ec(s32 slot, s32 value);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

#define HI(v) (((s16 *)&(v))[1])

void Func_0817c27c(struct BattleEffectArgument *effect)
{
    struct HeapSlots *heap = Ram_HeapSlots;
    struct FxWork *work = (struct FxWork *)heap->battle_fx_work[0];
    u8 *canvas = heap->battle_fx_work[1];
    struct EffectPosition pos;
    s32 frame;
    struct EffectStep *step;
    DrawRectangle draw;
    u8 *sheet = heap->unknown_64[0];
    s32 i;

    BattleFx_BeginCanvasLayer(1);
    if (effect->side == 0)
        Func_081963ec(104, 19);
    else
        Func_081963ec(104, 23);
    draw = (DrawRectangle)heap->unknown_64[1];
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0F3, work->sheet, 0, 0);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_EffectPalette02), 128);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
    for (frame = 0; frame != 52; frame++) {
        EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &pos);
        if (effect->side == 0) {
            switch (frame / 6) {
            case 0:
                draw(canvas, work->sheet, pos.x / 2 - 48, pos.y - 120, 48, 96);
                break;
            case 1:
                draw(canvas, work->sheet + 0x1200, pos.x / 2 - 38, pos.y - 120, 38, 92);
                break;
            case 2:
                draw(canvas, work->sheet + 0x1fa8, pos.x / 2 - 38, pos.y - 100, 30, 76);
                break;
            case 3:
                draw(canvas, work->sheet + 0x2890, pos.x / 2 - 64, pos.y - 88, 56, 96);
                break;
            case 4:
                draw(canvas, work->sheet + 0x3d90, pos.x / 2 - 64, pos.y - 88, 56, 88);
                break;
            }
        } else {
            switch (frame / 6) {
            case 0:
                draw(canvas, work->sheet, pos.x / 2, pos.y - 120, 48, 96);
                break;
            case 1:
                draw(canvas, work->sheet + 0x1200, pos.x / 2, pos.y - 120, 38, 92);
                break;
            case 2:
                draw(canvas, work->sheet + 0x1fa8, pos.x / 2, pos.y - 100, 30, 76);
                break;
            case 3:
                draw(canvas, work->sheet + 0x2890, pos.x / 2, pos.y - 88, 56, 96);
                break;
            case 4:
                draw(canvas, work->sheet + 0x3d90, pos.x / 2, pos.y - 88, 56, 88);
                break;
            }
        }
        if (frame == 18) {
            Func_08118088(effect->actors[0], 1);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
            Func_081180e8(134);
            work->shake_frames = 8;
            for (i = 0; i != 256; i++) {
                s32 angle;
                s32 speed;

                step = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                if (effect->side == 0)
                    step->x = (pos.x / 2 - 48) << 16;
                else
                    step->x = (pos.x / 2 + 48) << 16;
                step->y = (pos.y - (Random16() & 7)) << 16;
                angle = (Random16() & 0x7fff) + 0x4000;
                speed = Random16() & 255;
                if (effect->side == 0)
                    step->velocity_x = ((Trig_Sin(angle) * speed) >> 4) - 0x80000;
                else
                    step->velocity_x = ((Trig_Sin(angle) * speed) >> 4) + 0x80000;
                step->velocity_y = (Trig_Cos(angle) * (speed + 64)) >> 5;
                step->variant = (Random16() & 15) + 16;
            }
        }
        if (frame > 17) {
            for (i = 0; i != 256; i++) {
                step = &((struct EffectStep *)Ram_MapCellBuffer)[i];
                if (step->variant >= 0) {
                    s32 size = (step->variant >> 3) + 1;

                    draw(canvas, sheet + Data_08197410[size - 1], HI(step->x) - size / 2,
                        HI(step->y) - size, size, size * 2);
                    if (i & 1)
                        BattleFxKernels_IntegrateVector2(step, 60, -0x2000);
                    else
                        BattleFxKernels_IntegrateVector2(step, 60, 0x1000);
                    step->variant--;
                }
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
