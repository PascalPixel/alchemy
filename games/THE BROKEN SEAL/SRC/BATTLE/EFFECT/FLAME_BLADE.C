#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Three tables of 340 sparks fill the map cell buffer. */
#define SPARK ((struct EffectStep *)Ram_MapCellBuffer)

extern void *gBattleFxWork[];
extern u16 ParticleStreams_CellOffsets[];
/* The strike column of each of the three strikes, for either side. */
extern const u8 FlameBlade_StrikeColumns[];
/* The flash cell for each third of a flash's life. */
extern const u8 FlameBlade_FlashCells[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 value);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void **GetBattleObjectSlotFar(s32 unit);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode, s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void BattleFx_StepPaletteToResource(s32 resource_id);
void Audio_PlayCue(s32 cue);

/* Battle effect: a flame blade slides down beside the acting unit, then two
   strikes eight frames apart each throw twelve flashes and 256 sparks and
   make the affected units react six frames later. */
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect)
{
    void **heap;
    void **p;
    struct BattleEffectWork *work;
    void *canvas;
    s32 n;
    void *sheet;
    void *object;
    DrawRectangle routine[2];
    s32 frame;
    s32 i;

    /* The first two heap slots are read through a walking pointer; the
       base itself serves the sheet. */
    heap = gBattleFxWork;
    p = heap;
    work = *p++;
    canvas = *p;
    sheet = heap[2];

    object = *GetBattleObjectSlotFar(effect->actor);
    effect->variant = 1;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    BattleFx_FetchRectangleBlitters(work->effect->side, routine);
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 48);

    Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, (u8 *)work + (128 << 6), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

    n = 0;
    do {
        i = 0;
        do {
            struct EffectStep *flash = &work->particles[n * 16 + i];

            s32 scale = i * 2;
            s32 ang = Random16() & 0xFFFF;

            flash->x = Trig_Sin(ang) * scale;
            flash->y = -(Trig_Cos(ang) * scale);
            flash->variant = i / 2 + 25;
            i++;
        } while (i != 16);

        i = 0;
        do {
            struct EffectStep *spark = &SPARK[n * 340 + i];

            s32 mag;
            s32 ang;

            /* FAKEMATCH: the mask is loaded into the magnitude before the
               random word is taken in, as the native code does; masking the
               call's result keeps the mask in a scratch register. */
            mag = 0x1FF;
            mag &= Random16();
            ang = Random16() & 0xFFFF;
            spark->x = FlameBlade_StrikeColumns[n + work->effect->side * 3] << 16;
            spark->y = 176 << 15;
            mag += 32;
            spark->velocity_x = (Trig_Sin(ang) * mag) >> 6;
            spark->velocity_y = -(Trig_Cos(ang) * mag * 2) >> 6;
            spark->variant = (Random16() & 7) + 32;
            i++;
        } while (i != 170 << 1);
        n++;
    } while (n != 3);

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    frame = 0;
    do {
        if (frame == 4) {
            Audio_PlayCue(212);
        }
        if (frame == 8) {
            work->shake_frames = frame;
        }
        if (frame == 18) {
            Audio_PlayCue(145);
        }
        if (frame == 40) {
            BattleEventRuntime_BeginPhaseFar(134);
        }

        /* The blade: in from the top for ten frames, held, then sinking. */
        if (frame <= 39) {
            s32 high;
            s32 x;
            s32 top;

            high = 128;
            if (work->effect->side == 1) {
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
            if (top + 128 > 104) {
                high = high - top - 24;
            }
            if (high > 0) {
                routine[0](canvas, work, x - 32, top, 64, high);
            }
        }

        if (frame > 16) {
            BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
        }

        n = 0;
        do {
            s32 start = n * 8 + 16;

            if (frame == start) {
                work->shake_frames = 12;
            }
            if (frame >= start) {
                if (frame < start + 2) {
                    routine[0](canvas, (u8 *)work + (128 << 6),
                        FlameBlade_StrikeColumns[n + work->effect->side * 3] - 16, 56, 32, 64);
                }
                i = 0;
                do {
                    struct EffectStep *flash = &work->particles[n * 16 + i];

                    s32 x;
                    s32 life;
                    s32 y;

                    y = HI(flash->y);
                    x = HI(flash->x) + FlameBlade_StrikeColumns[n + work->effect->side * 3];
                    life = flash->variant;
                    if ((u32)life <= 17) {
                        routine[0](canvas,
                            (u8 *)work + (FlameBlade_FlashCells[life / 3] << 11) + (128 << 6),
                            x - 16, y + 56, 32, 64);
                    }
                    life = flash->variant;
                    flash->variant = life > 0 ? life - 1 : -1;
                    i++;
                } while (i != 12);
            }

            if (frame > start + 5) {
                i = 0;
                do {
                    struct EffectStep *spark = &SPARK[n * 340 + i];

                    if (spark->variant > 0) {
                        s32 x;
                        s32 y;
                        s32 size;

                        EffectStep_AdvanceWithGravity2D(spark, 64, 128 << 5);
                        spark->variant--;
                        y = spark->y;
                        if (y > (216 << 15)) {
                            spark->velocity_y = -spark->velocity_y / 2;
                        } else {
                            x = spark->x;
                            if ((u32)x <= 0x007EFFFF && y >= 0) {
                                u8 *cell;
                                s32 lane;
                                s32 py = y >> 16;
                                s32 px = x >> 16;

                                size = spark->variant / 5 + 1;
                                lane = i & 1;
                                cell = (u8 *)sheet + ParticleStreams_CellOffsets[size - 1];
                                px -= size / 2;
                                py -= size;
                                routine[lane](canvas, cell, px, py, size, size * 2);
                            }
                        }
                    }
                    i++;
                } while (i != 128 << 1);
            }

            for (i = 0; i != work->effect->count; i++) {
                if (frame == start + 6) {
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
                }
            }
            n++;
        } while (n != 2);

        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 80);

    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
