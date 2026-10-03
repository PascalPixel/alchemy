#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

/* Per variant: how long it runs, how long each column stays up, its shake. */
enum {
    TIMING_FRAMES,
    TIMING_RISE_FRAMES,
    TIMING_SHAKE
};

struct RockWallHeights {
    s32 column[4];
};

extern const u8 RockWall_Timings[][3];
extern const struct RockWallHeights RockWall_Heights;

#define COLUMN_WIDTH 40

/* Where a pixel lies in a canvas of 8x8 tiles, 32 tiles to the row. */
#define CANVAS_PIXEL(x, y) \
    (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/* Battle effect: three columns of rock, 40 pixels wide and four frames
   apart, rise out of the ground and sink back, each capped with the strip of
   ground it lifted (read back from the background tiles). A unit standing on
   a rising column is thrown into the air, and reacts when it lands. */
void BattleFx_RunRockWall(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 column;
    DrawRectangle draw;
    struct BattleCamera *camera;
    s32 x_offset;
    struct RockWallHeights heights;
    s32 point[3];
    struct EffectPosition screen;
    s32 member;
    s32 row;
    s32 col;
    s32 y;

    heap_cache = &gWorkSlot[39];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)gWorkSlot + 12 * 4);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet, (u8 *)work + 0x1e00, 1, 1);
    /* FAKEMATCH: forwarded through Call3, the copy loads the buffer address
       straight into r0, so the loop below loads its own after the call. */
    Call3((void (*)())Iwram_CopyWords, (s32)Ram_MapCellBuffer, 0x06008000, 0x8000);
    for (row = 0; row != 16; row++) {
        y = row + 96;
        for (col = 0; col != COLUMN_WIDTH; col++) {
            s32 pixel = CANVAS_PIXEL(col + 32, y);

            ((u8 *)work)[row * COLUMN_WIDTH + col] = Ram_MapCellBuffer[pixel];
        }
    }
    if (work->effect->side == 1) {
        REG_BG2X = -0x7000;
        x_offset = -112;
    } else {
        x_offset = 0;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw = (DrawRectangle)gWorkSlot[46];
    for (member = 0; member != work->effect->count; member++)
        work->particles[member].variant = 0;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    heights = RockWall_Heights;
    work->shake_frames = 128;
    Audio_PlayCue(141);

    for (frame = 0; frame != RockWall_Timings[work->effect->variant][TIMING_FRAMES]; frame++) {
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame == RockWall_Timings[work->effect->variant][TIMING_FRAMES] - 16)
            BattleEventRuntime_BeginPhaseFar(133);
        for (column = 0; column != 3; column++) {
            s32 start = column * 4 + 16;

            if ((frame & 31) + 32 == start)
                heights.column[column] += 32;
            if (frame >= start
                && frame < start + RockWall_Timings[work->effect->variant][TIMING_RISE_FRAMES]) {
                s32 height;
                s32 top;

                height = (heights.column[column] * Trig_Sin((frame - start) << 10)) >> 16;
                if (height < 0)
                    height = -height;
                top = 112 - height;
                draw(canvas, (u8 *)work + 0x1e00, column * COLUMN_WIDTH + 8, top,
                    COLUMN_WIDTH, height);
                draw(canvas, work, column * COLUMN_WIDTH + 8, 96 - height, COLUMN_WIDTH, 16);
                for (member = 0; member != work->effect->count; member++) {
                    struct MotionObject *object;

                    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
                    point[0] = object->x;
                    point[1] = object->y;
                    point[2] = object->z;
                    EffectPosition_ApplyBaseAndYOffset(point, &screen);
                    screen.x += x_offset;
                    if (screen.x >= column * COLUMN_WIDTH + 8
                        && screen.x <= column * COLUMN_WIDTH + 8 + COLUMN_WIDTH
                        && screen.y >= top) {
                        object->velocity_y = 0xc0000;
                        object->vertical_motion_strength = 0xab85;
                    }
                    if (object->y < 0)
                        ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
                }
            }
        }
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *object;
            struct EffectStep *state;

            object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            state = &work->particles[member];
            if (state->variant == 0 && object->y <= 0 && object->velocity_y < 0) {
                state->variant = 1;
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 5);
            }
        }
        Camera_ApplyShake(RockWall_Timings[work->effect->variant][TIMING_SHAKE],
            RockWall_Timings[work->effect->variant][TIMING_SHAKE]);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
