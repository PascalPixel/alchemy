/* Draft, differ 44 of 620: The Broken Seal's BattleFx_RunFlameBlade,
 * ported: the strike columns are placed from the first target's position
 * (half its x less 24, or 92 on side 1), the user's mode is set to 2
 * (Object_SetMode), the effect stays an argument, each target is also
 * flashed when its column starts, resources 0x117, 0x13e, 0x134 and 0x184,
 * callback Func_08143000 at 0xc80. Everything left is reordered rows where
 * the native puts a load (mostly the work's reload from sp+52, the pooled
 * tables) ahead of an independent constant or shift: the kind
 * Func_08164b2c's header traces to a load costing 3 and a store 2. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])
#define FLASH_SHEET (64 * 128)

extern char ResourceId_FlameBladeSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data134;
extern char ResourceId_BlastSheet;
extern u8 gData_08198a92[];
extern u8 gData_08198a98[];
extern u16 gData_08197410[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_08144aac(s32 side, DrawRectangle *draw);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 value);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase);
void Func_0815f0a0(s32 resource);
void Audio_PlayCue(s32 cue);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08118088(s32 actor, s32 variant);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);

void Func_081665a4(struct BattleEffectArgument *effect)
{
    struct BattleFxWork *work;
    void *canvas;
    s32 col;
    u8 *sheet;
    struct MotionObject *user;
    DrawRectangle blit[2];
    struct EffectPosition pos;
    s32 baseX;
    s32 frame;
    s32 i;

    work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    sheet = Ram_HeapSlots->unk64[0];

    user = GetBattleObjectSlotFar(effect->actor)->object;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    Func_08144aac(effect->side, blit);
    Object_SetMode(user, 2);
    ObjectDispatch_ApplyValueToChildrenFar(user, 48);
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    if (effect->side == 0)
        baseX = pos.x / 2 - 24;
    else
        baseX = pos.x / 2 - 92;

    Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + FLASH_SHEET, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

    col = 0;
    do {
        i = 0;
        do {
            struct EffectStep *flash = &work->particles[col * 16 + i];
            s32 radius = i * 2;
            s32 angle = Random16() & 0xffff;

            flash->x = Trig_Sin(angle) * radius;
            flash->y = -(Trig_Cos(angle) * radius);
            flash->variant = i / 2 + 25;
            i++;
        } while (i != 16);

        i = 0;
        do {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[col * 340 + i];
            s32 speed;
            s32 angle;

            speed = (Random16() & 0x1ff) + 32;
            angle = Random16() & 0xffff;
            spark->x = (gData_08198a92[col + effect->side * 3] + baseX) << 16;
            spark->y = 88 << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
            spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
            spark->variant = (Random16() & 7) + 32;
            i++;
        } while (i != 340);
        col++;
    } while (col != 3);

    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    frame = 0;
    do {
        if (frame == 4)
            Audio_PlayCue(212);
        if (frame == 8)
            work->shakeFrames = frame;
        if (frame == 18)
            Audio_PlayCue(145);
        if (frame == 40)
            Func_081180e8(134);

        if (frame <= 39) {
            s32 height;
            s32 x;
            s32 top;

            height = 128;
            if (effect->side == 1) {
                if (frame <= 9) {
                    x = frame * 10 - 8;
                    top = frame * 16 - 128;
                } else if (frame > 20) {
                    x = frame + 62;
                    top = frame * 2 - 24;
                } else {
                    x = 82;
                    top = 16;
                }
            } else {
                if (frame <= 9) {
                    x = 128 - frame * 10;
                    top = frame * 16 - 128;
                } else if (frame > 20) {
                    x = 58 - frame;
                    top = frame * 2 - 24;
                } else {
                    x = 38;
                    top = 16;
                }
            }
            if (top + 128 > 104)
                height = height - top - 24;
            if (height > 0)
                blit[0](canvas, work->sheet, x + baseX - 32, top, 64, height);
        }

        if (frame > 16)
            Func_0815f0a0((s32)&ResourceId_BlastSheet);

        col = 0;
        do {
            s32 start = col * 8 + 16;

            if (frame == start)
                work->shakeFrames = 12;
            if (frame >= start) {
                if (frame < start + 2)
                    blit[0](canvas, work->sheet + FLASH_SHEET, gData_08198a92[col + effect->side * 3] + baseX - 16, 56, 32, 64);
                i = 0;
                do {
                    struct EffectStep *flash = &work->particles[col * 16 + i];
                    s32 x;
                    s32 life;
                    s32 y;

                    y = HI(flash->y);
                    x = HI(flash->x) + gData_08198a92[col + effect->side * 3] + baseX;
                    life = flash->variant;
                    if (life >= 0 && life <= 17)
                        blit[0](canvas, work->sheet + (gData_08198a98[life / 3] << 11) + FLASH_SHEET, x - 16, y + 56, 32, 64);
                    life = flash->variant;
                    flash->variant = life > 0 ? life - 1 : -1;
                    i++;
                } while (i != 12);
            }

            if (frame > start + 5) {
                i = 0;
                do {
                    struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[col * 340 + i];

                    if (spark->variant > 0) {
                        s32 x;
                        s32 y;
                        s32 size;

                        BattleFxKernels_IntegrateVector2(spark, 64, 0x1000);
                        spark->variant--;
                        y = spark->y;
                        if (y > (108 << 16)) {
                            spark->velocityY = -spark->velocityY / 2;
                        } else {
                            x = spark->x;
                            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                                u8 *cel;
                                s32 lane;
                                s32 py = y >> 16;
                                s32 px = x >> 16;

                                size = spark->variant / 5 + 1;
                                lane = i & 1;
                                cel = sheet + gData_08197410[size - 1];
                                px -= size / 2;
                                py -= size;
                                blit[lane](canvas, cel, px, py, size, size * 2);
                            }
                        }
                    }
                    i++;
                } while (i != 256);
            }

            for (i = 0; i != effect->count; i++) {
                if (frame == start)
                    Func_0814cd48(effect->actors[i], 7, 5, i, 10);
                if (frame == start + 6) {
                    Func_0814cd48(effect->actors[i], 7, 5, i, 10);
                    Func_08118088(effect->actors[i], 4);
                }
            }
            col++;
        } while (col != 2);

        Func_08158ce0(16, 16);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 80);

    ObjectDispatch_ApplyValueToChildrenFar(user, 16);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
