/* Draft: listing 434 instructions, draft 425, differ 411 (the alignment drifts from the first
 * spill). Written from the listing: the bursts at the three frames of gData_08199d94, their
 * flattened model drawn through Func_08196958/Func_08196a7c, and the particles blitted twice per
 * frame. The native frame (84) holds effect at sp+60, work 56, canvas 52, frame 48, an unused
 * slot at 44, blit 40, graphics 36, the vertices 32, the model 28, the texture's and the
 * position's addresses at 24 and 20, the speed (120 + 20 * j) at 16 and the burst's particle
 * offset at 12; the texture is at sp+64 and the position at sp+72. The slot above blit is the
 * blitter pair's second word (a two-word array is one DImode pseudo, as in Func_08182898 and
 * Func_0818c7b8; 463 to 411). Here graphics and frame still take other slots, so the
 * declaration order is what to look at next. */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect_work.h"

typedef s32 (*DrawRectangle)(void *canvas, void *source, s32 x, s32 y, s32 width, s32 height);

struct ModelTexture {
    u32 width:8;
    u32 height:8;
    u32 unk02:16;
    void *pixels;
};

struct Model {
    s32 kind;
    s32 unk04;
    const void *mesh;
    void *vertices;
    struct ModelTexture *texture;
    s32 alpha;
};

extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;
extern struct EffectStep gData_02014000[];
extern const u8 gData_08199364[];
extern const u8 gData_08199d94[];
extern const u8 gData_08199210[];
extern const u16 gData_08197410[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_08157530(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);
void Func_081963ec(s32 slot, s32 value);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Func_081969f8(s32 count);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_081180e8(s32 cue);
void Audio_PlayCue(s32 cue);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Render_ResetTransformState(void);
void Func_080151e4(s32, s32, s32);
void Func_08015160(s32, s32, s32);
void Func_080150e4(s32);
void SceneTransform_ApplyPitch(s32);
void Func_0801521c(s32);
void Func_08196958(const void *, void *, s32);
void Func_08196a7c(struct Model *);
void BattleFxKernels_IntegrateVector2(void *particle, s32 mask, s32 gravity);
void MemFree(void *block);
void Func_08158ce0(s32, s32);
void Func_081434f8(void);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

void Func_0818b868(struct BattleEffectArgument *effect)
{
    u8 *work = Ram_HeapSlots->battleFxWork[0];
    void *canvas = Ram_HeapSlots->battleFxWork[1];
    u8 *graphics = Ram_HeapSlots->unk64[0];
    DrawRectangle blit[2];
    struct ModelTexture texture;
    struct EffectPosition position;
    struct Model *model;
    void *vertices;
    struct EffectStep *step;
    s32 frame;
    s32 i;
    s32 j;
    s32 speed;

    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    Func_08157530(effect->actor, effect->actors[0], 8, 80);
    WaitFrames(8);
    Func_081963ec(104, 23);
    blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
    for (frame = 0; frame != 50; frame++)
    {
        if (frame == 0)
        {
            for (i = 0; i != 512; i++)
                gData_02014000[i].variant = -1;
        }
        vertices = Runtime_BumpAllocateAlternatePool(32);
        model = Func_081969f8(1);
        EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
        texture.width = 7;
        texture.height = 7;
        texture.pixels = Ram_MapCellBuffer;
        model->mesh = gData_08199364;
        model->kind = 7;
        model->texture = &texture;
        model->vertices = vertices;
        speed = 120;
        for (j = 0; j != 3; j++)
        {
            struct EffectStep *anchor = &((struct EffectStep *)work)[j];

            if (frame == gData_08199d94[j])
            {
                step = &gData_02014000[j * 128];
                for (i = 0; i != 32; i++)
                {
                    s32 angle;
                    s32 force;

                    step->x = (position.x / 2) << 16;
                    step->y = (position.y - 24) << 16;
                    angle = Random16();
                    force = (Random16() & 255) + 32;
                    step->velocityX = (Trig_Sin(angle) * force) >> 8;
                    step->velocityY = (Trig_Cos(angle) * force) >> 6;
                    if (effect->side == 0)
                        step->velocityX += -0x8000;
                    else
                        step->velocityX += 0x8000;
                    anchor->x = position.x;
                    anchor->y = position.y - 24;
                    step->variant = (Random16() & 7) + 16;
                    step++;
                }
                if (j == 2)
                    Func_081180e8(134);
                else
                    Audio_PlayCue(134);
                BattleFx_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000, 0, speed);
                Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
                *(s32 *)(work + 0x77a8) = 8;
            }
            if (frame >= gData_08199d94[j] && frame < gData_08199d94[j] + 16)
            {
                s32 t = frame - gData_08199d94[j];
                s32 alpha = 64 - t * 8;
                s32 scale = t * 10192 + 0x4000;

                if (alpha > 0)
                    alpha = 0;
                if (alpha > -64)
                {
                    model->alpha = alpha;
                    Render_ResetTransformState();
                    Func_080151e4(0x8000, 0x10000, 0x8000);
                    Func_08015160((anchor->x - 128) << 16, (anchor->y - 64) << 16, 0);
                    Func_080150e4((j << 14) + 0x2000);
                    SceneTransform_ApplyPitch(-0x1000);
                    Func_0801521c(scale);
                    Func_08196958(gData_08199210, vertices, 4);
                    Func_08196a7c(model);
                }
            }
            speed += 20;
        }
        step = gData_02014000;
        for (i = 0; i != 512; i++)
        {
            s32 size = step->variant;

            if (size >= 0)
            {
                size = (size >> 2) + 3;
                blit[0](canvas, graphics + gData_08197410[size - 1],
                    *(s16 *)((u8 *)&step->x + 2) - size / 2, *(s16 *)((u8 *)&step->y + 2) - size,
                    size, size * 2);
                BattleFxKernels_IntegrateVector2(step, 62, 0);
                blit[0](canvas, graphics + gData_08197410[size - 1],
                    *(s16 *)((u8 *)&step->x + 2) - size / 2, *(s16 *)((u8 *)&step->y + 2) - size,
                    size, size * 2);
                BattleFxKernels_IntegrateVector2(step, 62, 0);
                step->variant--;
            }
            step++;
        }
        MemFree(model);
        MemFree(vertices);
        Func_08158ce0(8, 8);
        Func_081434f8();
        *(s32 *)(work + 0x78e8) = 1;
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(104);
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
