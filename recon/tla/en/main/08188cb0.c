/* Near miss: listing 529 instructions, draft 529, differ 18. Statements, allocation and frame
 * all match (effect r10, work r11, canvas at sp+16, the two blitters at sp+20, the position at
 * sp+28 and its address in r9, frame and frame - 8 spilled); what is left is sched2 order in five
 * places: the frame loop's preheader (the native clears frame and builds -8 before it starts the
 * position's address), the second side's cell blit (the native loads biasX's table before 0x700
 * and stores the width before it negates biasX), the spark spawn's entry test (the native builds
 * -16 before it loads the variant) and the spark height test (the native loads y before 0x680000).
 * Each is the native issuing a store or a load sooner than agscc's arm9tdmi costs rank it.
 * Measured: the BLDALPHA and BG2PA stores take the f18284c.c blend pin (each differ 2 without);
 * the timer clear through &sparks->variant, i before the pixel pointer, the pixels indexed and
 * the sparks as &sparks[i] (as in BattleFx_RunFireSwirlCharge) are the source's forms (38 to 22);
 * a one-pass block on y fixes the height test but moves the IntegrateVector2 arguments (18); an
 * asm barrier on y relays out the function (105). */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect_work.h"

typedef s32 (*DrawRectangle)(void *canvas, void *source, s32 x, s32 y, s32 width, s32 height);

extern char ResourceId_Data123;
extern char ResourceId_Data12E;
extern char ResourceId_Data16D;
extern char ResourceId_BlueFlameSheet;
extern const u16 Data_08199d40[];
extern const s8 Data_08199d4e[];
extern const s8 Data_08199d55[];
extern const u8 Data_08199d31[];
extern const u8 Data_08199d38[];

void BattleFx_BeginCanvasLayer(s32 mode);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_08144aac(s32 mode, DrawRectangle *draw);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Func_081180e8(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08118088(s32, s32);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFxKernels_IntegrateVector2(void *particle, s32 mask, s32 gravity);
void Func_08158ce0(s32, s32);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

void Func_08188cb0(struct BattleEffectArgument *effect)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    void *canvas = Ram_HeapSlots->battle_fx_work[1];
    DrawRectangle draw[2];
    struct EffectPosition position;
    struct EffectStep *sparks;
    struct EffectStep *spark;
    s32 *life;
    s32 frame;
    s32 i;

    BattleFx_BeginCanvasLayer(1);
    {
        register u16 blend __asm__("r3") = 0x1010;

        __asm__("" : "+l"(blend) : : "r2");
        REG_BLDALPHA = blend;
    }
    if (effect->variant == 0)
    {
        register u16 scale __asm__("r3") = 0x100;

        REG_BG2PA = scale;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_Data123, work + 0x700, 1, 1);
    if (effect->variant == 0)
    {
        Resource_LoadAndDecompress((s32)&ResourceId_Data12E, work + 0x4580, 1, 0);
        *(s32 *)(work + 0x7784) = 50;
    }
    else
    {
        u8 *pixels = work + 0x700;

        for (i = 0; i != 16000; i++)
        {
            if (pixels[i] != 0)
            {
                pixels[i] = pixels[i] * 2 + 32;
                if (pixels[i] > 63)
                    pixels[i] = 63;
            }
        }
        Resource_LoadAndDecompress((s32)&ResourceId_Data16D, work + 0x4580, 1, 1);
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
        *(s32 *)(work + 0x7784) = 75;
    }
    *(s32 *)(work + 0x7780) = 2;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    Func_08144aac(effect->side, draw);
    for (i = 0, life = &((struct EffectStep *)work)->variant; i != 64; i++)
    {
        *life = 0;
        life += 7;
    }
    sparks = (struct EffectStep *)work;
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x70000);
    for (frame = 0; frame != 70; frame++)
    {
        if (effect->variant == 0)
        {
            EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &position);
            if (effect->side == 0)
                REG_BG2X = (80 - position.x) << 8;
            else
                REG_BG2X = (32 - position.x) << 8;
            if (frame >= 8 && frame <= 23)
            {
                s32 cell = (frame - 8) / 2;

                if (cell > 6)
                    cell = 6;
                if (effect->side == 0)
                {
                    draw[0](canvas, work + Data_08199d40[cell] + 0x700,
                        Data_08199d4e[cell] + 30, Data_08199d55[cell] + position.y - 60,
                        Data_08199d31[cell], Data_08199d38[cell]);
                }
                else
                {
                    draw[0](canvas, work + Data_08199d40[cell] + 0x700,
                        -Data_08199d4e[cell] - Data_08199d31[cell] + 98,
                        Data_08199d55[cell] + position.y - 60, Data_08199d31[cell],
                        Data_08199d38[cell]);
                }
            }
        }
        else
        {
            EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
            if (frame >= 8 && frame <= 23)
            {
                s32 cell = (frame - 8) / 2;

                if (cell > 6)
                    cell = 6;
                if (effect->side == 0)
                {
                    draw[0](canvas, work + Data_08199d40[cell] + 0x700,
                        position.x / 2 + Data_08199d4e[cell] - 16,
                        Data_08199d55[cell] + position.y - 92, Data_08199d31[cell],
                        Data_08199d38[cell]);
                }
                else
                {
                    draw[0](canvas, work + Data_08199d40[cell] + 0x700,
                        position.x / 2 - Data_08199d4e[cell] - Data_08199d31[cell] + 16,
                        Data_08199d55[cell] + position.y - 92, Data_08199d31[cell],
                        Data_08199d38[cell]);
                }
            }
        }
        if (frame == 18)
        {
            EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
            Func_081180e8(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
            Func_08118088(effect->actors[0], 6);
            *(s32 *)(work + 0x77a8) = 4;
            for (i = 0; i != effect->variant * 16 + 16; i++)
            {
                s32 force = (Random16() & 255) + 256;
                s32 angle = Random16() & 0xffff;

                spark = &sparks[i];
                if (effect->variant == 0)
                    spark->x = 0x400000;
                else
                    spark->x = (position.x / 2) << 16;
                spark->y = 0x500000;
                spark->velocity_x = (Trig_Sin(angle) * force) >> 7;
                spark->velocity_y = -(Trig_Cos(angle) * force) >> 6;
                spark->variant = (Random16() & 15) + 16;
            }
        }
        for (i = 0; i != 64; i++)
        {
            spark = &sparks[i];
            if (spark->variant > 0)
            {
                spark->variant--;
                BattleFxKernels_IntegrateVector2(spark, 60, 0);
                if (spark->y > 0x680000)
                {
                    spark->velocity_y = -spark->velocity_y / 2;
                }
                else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0)
                {
                    s32 sx = spark->x >> 16;
                    s32 sy = spark->y >> 16;

                    if (effect->variant == 0)
                        draw[0](canvas, work + (((frame + i) / 4 % 6) << 8) + 0x4580, sx - 8, sy - 8, 16, 16);
                    else
                        draw[0](canvas, work + i % 3 * 576 + 0x4580, sx - 12, sy - 12, 24, 24);
                }
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        *(s32 *)(work + 0x78e8) = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
