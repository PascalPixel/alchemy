#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

/* Sixteen columns grow and retract in staggered windows. Five image cells
 * cycle with frame and column, using the alternating cached blitters.
 * Position-query outputs are separate from the two three-word coordinates.
 */

typedef struct Column {
    s32 x;
    s32 unused[6];
} Column;
extern u8 gWorkSlot[];
extern s8 RisingColumns_ColumnOffsets[];
extern u8 Value_000000a6;
void BattleFx_BeginCanvasLayer(s32);
void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);
void EffectPosition_ApplyStepAndYOffset(s32, s32 *);
void Audio_PlayCue(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
s32 Math_Mod(s32, s32);
void Camera_ApplyShake(s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);
#define WORK_EFFECT ((struct BattleEffectArgument *)work->effect)

void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect)
{
    u32 *cache, *entry;
    struct BattleEffectWork *work;
    void *dst;
    DrawRectangle draw[2];
    s32 origin_x, origin_y;
    s32 first[3], last[3];
    s32 i, frame, cell, height, offset, middle;
    Column *column;

    cache = (u32 *)(gWorkSlot + 39 * 4);
    entry = cache;
    work = (struct BattleEffectWork *)*entry++;
    dst = (void *)*entry;
    WORK_EFFECT = effect;
    BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
    BattleFx_BeginCanvasLayer(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&Value_000000a6, work, 1, 1);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[0], first);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[WORK_EFFECT->count - 1], last);
    middle = first[0];
    middle += (last[0] - middle) / 2;
    first[0] = middle;
    *(s32 *)0x04000028 = (64 - first[0]) << 8;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)cache[8];
    column = (Column *)((u8 *)work + 0x7080);
    i = 0;
    do {
        column[i].x = RisingColumns_ColumnOffsets[i] + 64;
        i++;
    } while (i != 16);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);
    frame = 0;
    do {
        if (frame == 32) {
            Audio_PlayCue(143);
            for (i = 0; i != WORK_EFFECT->count; i++)
                ObjectGroup_UpdateMembers(WORK_EFFECT->actors[i], 7, 5, i, 16);
        }
        i = 0;
        column = (Column *)((u8 *)work + 0x7080);
        do {
            if (frame == i * 4 + 5)
                *(s32 *)((u8 *)work + 0x77a8) = 2;
            offset = i * 2;
            if (frame > offset + 4) {
                cell = Math_Mod(frame / 4 + i, 5);
                if (frame < offset + 32) {
                    height = (frame - offset) * 4 - 16;
                    if (height > 32) height = 32;
                } else {
                    height = 160 - (frame - offset) * 4;
                }
                if (height > 0)
                    draw[i & 1](dst, (u8 *)work + (cell << 10), column->x - 16,
                        (i & 7) - height + 104, 32, height);
            }
            i++;
            column++;
        } while (i != 16);
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 70);
    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
