/* Draft, not exact: score 5782, 187 instructions differ (764 against 782).
   Same frame and stack slots. The reference keeps the address of the timing
   table in r9 outside the mote loop and loads it again after every inner
   loop and call; here that register goes to nothing and every use loads the
   address from the pool. What follows from it: r6 against r5 for the seed
   pointer, and the argument order of two draw calls. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);

extern u16 BattleFx_PuffCells[];
extern u8 BattleFx_PuffSizes[];
extern u8 Data_080eea62[];
extern u8 Data_080eea88[];
extern u8 Data_080eea91[];
extern u8 Data_080eea99[];
extern u16 Data_080eeaa2[];
extern u16 Data_080eeab2[];
extern u8 Data_080eeab8[];
extern u8 Data_080eeabb[];
extern u8 Data_080eeac3[];
extern u16 Data_080eeacc[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Per variant: how many motes circle, how many columns rise, and the frame
   on which the motes fly up. */
enum {
    RISE_MOTES,
    RISE_COLUMNS,
    RISE_FRAME
};

/* Battle effect: motes circle down around the middle of the screen while
   columns rise one by one; on the variant's frame the motes shoot upwards,
   embers scatter, and puffs open over the band the motes and embers span. */
void BattleEffect_RunRisingMotes(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    void *sheet;
    DrawRectangle *draw;
    s32 count;
    s32 ymin;
    s32 ymax;
    s32 record[3];
    struct EffectPosition screen;
    s32 point[3];
    DrawRectangle callbacks[2];
    struct EffectStep *mote;
    s32 i;

    heap_cache = &gWorkSlot[39];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
    BattleFx_FetchRectangleBlitters(0, draw = callbacks);

    mote = work->particles;
    for (i = 0; i != 64; i++) {
        mote->x = Random16() & 0xffff;
        mote->z = (Random16() & 63) + 56;
        mote->y = ((Random16() & 31) - 64) << 16;
        mote++;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 1)
        REG_BG2X = -0x7000;

    for (frame = 0; frame != Data_080eea88[work->effect->variant * 3 + RISE_FRAME] + 75; frame++) {
        ymin = 0x780000;
        ymax = 0;
        if (frame == Data_080eea88[work->effect->variant * 3 + RISE_FRAME] + 11)
            BattleEventRuntime_BeginPhaseFar(132);
        point[0] = 0;
        point[1] = 0;
        point[2] = 0x2000000;
        Render_ResetTransformState();
        SceneTransform_ApplyPosition(point);
        if (frame >= 36 && frame < 64 && (frame & 3) == 0)
            Audio_PlayCue(115);
        if (frame == 85)
            Audio_PlayCue(136);
        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 4 + 40)
                ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
        }

        count = 16;
        if (frame < Data_080eea88[work->effect->variant * 3 + RISE_FRAME])
            count = Data_080eea88[work->effect->variant * 3 + RISE_MOTES];
        if (frame < Data_080eea88[work->effect->variant * 3 + RISE_FRAME] + 35) {
            for (i = 0; i != count; i++) {
                if (frame > i) {
                    s32 slot = i % 8;

                    mote = &work->particles[i];
                    if (mote->y < (48 - i / 2) << 16 && mote->y > -0x300000) {
                        struct EffectPosition *pos = &screen;
                        u32 width;
                        u32 height;

                        record[0] = mote->z * Trig_Sin(mote->x);
                        record[1] = mote->y;
                        record[2] = mote->z * Trig_Cos(mote->x);
                        EffectPosition_ApplyBaseAndYOffset(record, pos);
                        pos->x = (pos->x >> 17) + 64;
                        pos->y = HI(pos->y) + 60;
                        draw[1](canvas, (u8 *)work + Data_080eeaa2[slot],
                            pos->x - ((width = Data_080eea91[slot]) >> 1),
                            pos->y - ((height = Data_080eea99[slot]) >> 1),
                            width, height);
                    }
                    if (frame < Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
                        if (frame > i + 16) {
                            if (mote->z > 4)
                                mote->z -= 2;
                            if (mote->y < 0x300000)
                                mote->y += 0x50000;
                            mote->x += 0x200;
                        }
                    } else {
                        mote->z += 8;
                        mote->y -= (i % 5 + 2) << 16;
                        if (ymin > mote->y)
                            ymin = mote->y;
                        if (ymax < mote->y)
                            ymax = mote->y;
                    }
                }
            }
        }
        ymin += 0x400000;
        ymax += 0x400000;

        if (frame < Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
            for (i = 0; i != Data_080eea88[work->effect->variant * 3 + RISE_COLUMNS]; i++) {
                if (i < (frame - 36) / 3) {
                    s32 image = i % 3;
                    u32 height;

                    if (frame >= Data_080eea88[work->effect->variant * 3 + RISE_FRAME] - 7) {
                        draw[1](canvas, (u8 *)work + Data_080eeab2[image],
                            Data_080eea62[i * 2],
                            Data_080eea62[i * 2 + 1] - (height = Data_080eeab8[image]),
                            32, height);
                    } else {
                        callbacks[0](canvas, (u8 *)work + Data_080eeab2[image],
                            Data_080eea62[i * 2],
                            Data_080eea62[i * 2 + 1] - (height = Data_080eeab8[image]),
                            32, height);
                    }
                }
            }
        }

        if (frame == Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
            for (i = 0; i != 32; i++) {
                struct EffectStep *ember = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                ember->x = (Random16() & 127) << 16;
                ember->y = ((Random16() & 15) + 80) << 16;
                ember->z = ((Random16() & 63) - 32) << 12;
                ember->velocity_y = ((-Random16() & 15) - 16) << 13;
                ember->variant = (Random16() & 15) + 16;
            }
        }
        if (frame >= Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
            for (i = 0; i != 24; i++) {
                struct EffectStep *ember = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                if (ember->variant >= 0) {
                    s32 slot = i % 8;

                    draw[1](canvas, (u8 *)work + Data_080eeacc[slot],
                        HI(ember->x), HI(ember->y),
                        Data_080eeabb[slot], Data_080eeac3[slot]);
                    ember->x += ember->velocity_x;
                    ember->y += ember->velocity_y;
                    ember->variant--;
                }
                if (ymin > ember->y)
                    ymin = ember->y;
                if (ymax < ember->y)
                    ymax = ember->y;
            }
        }
        ymin >>= 16;
        ymax >>= 16;
        if (ymax <= ymin)
            ymax = ymin + 1;

        if (frame == Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
            for (i = 0; i != 32; i++) {
                struct EffectStep *puff = &work->particles[i];

                puff->velocity_x = (Random16() & 127) << 16;
                if (ymax == ymin)
                    puff->velocity_y = ymin << 16;
                else
                    puff->velocity_y = (Random16() % (u32)(ymax - ymin) + ymin) << 16;
                puff->variant = (Random16() & 15) + 20;
            }
        }
        if (frame >= Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) {
            s32 drop = (frame - Data_080eea88[work->effect->variant * 3 + RISE_FRAME]) / 2;

            for (i = 0; i != 32; i++) {
                struct EffectStep *puff = &work->particles[i];

                if (puff->variant >= 0 && puff->variant <= 17) {
                    s32 image = (17 - puff->variant) / 2;
                    u32 size;

                    draw[1](canvas, (u8 *)sheet + BattleFx_PuffCells[image],
                        HI(puff->velocity_x) - ((size = BattleFx_PuffSizes[image]) >> 1),
                        HI(puff->velocity_y) - (size >> 1) - drop, size, size);
                }
                puff->variant--;
                if ((puff->variant == -1 || puff->variant == 17)
                    && frame < Data_080eea88[work->effect->variant * 3 + RISE_FRAME] + 35) {
                    puff->variant = 17;
                    puff->velocity_x = (Random16() & 127) << 16;
                    puff->velocity_y = (Random16() % (u32)(ymax - ymin) + ymin) << 16;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
