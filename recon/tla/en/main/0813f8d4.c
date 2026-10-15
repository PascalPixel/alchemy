/* Near miss, differ 12 of 168 (five reordered pairs): The Broken Seal's
 * BattlePresentation_DrawStreaks, with this game's effect work, Random16's
 * values stored unmasked and Func_080150e4 for the roll. Left are ARM9
 * schedule swaps: the frame counter's load against the work pointer's
 * reload from its stack slot, gMapCellBuffer's pool load before and after
 * the zero it sits beside (twice), the tail load against the point's store,
 * and the fade's add against the head's load. Measured: the counter as a
 * read and a store, or a ++ (same 12); the seed's two stores swapped (20);
 * the point's zeros after i (48). */
#include "types.h"
#include "ram_buffer.h"
#include "system.h"

struct StreakWork
{
    s32 frame;
    s32 transferPending;
};

struct Streak
{
    s32 head;
    s32 tail;
    s32 unused08;
    s32 pitch;
    s32 yaw;
    s32 roll;
    s32 unused18;
};

struct StreakPoint
{
    s32 x;
    s32 y;
    s32 z;
};

void Render_ResetTransformState(void);
void Func_080150e4(s32 roll);
void SceneTransform_ApplyPitch(s32 pitch);
void SceneTransform_ApplyYaw(s32 yaw);
void EffectPosition_ApplyBaseAndYOffset(s32 *point, struct StreakPoint *out);
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

void Func_0813f8d4(void)
{
    struct StreakWork *work = (struct StreakWork *)Ram_HeapSlots->battleFxWork[0];
    s32 frame;
    s32 i;
    struct Streak *streak;
    struct StreakPoint point;
    struct StreakPoint head;
    struct StreakPoint tail;

    frame = (work->frame)++;
    if (frame == 0)
    {
        for (i = 0; i != 256; i++)
        {
            struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
            s32 r = Random16() & 15;

            seed->head = r + 48;
            seed->tail = r + 40;
            seed->pitch = Random16();
            seed->yaw = Random16();
            seed->roll = Random16();
        }
    }
    point.y = 0;
    point.z = 0;
    i = 0;
    for (; i != 64; i++)
    {
        streak = &((struct Streak *)Ram_MapCellBuffer)[i];
        if (frame > i / 4 && streak->head > 0)
        {
            s32 fade;

            Render_ResetTransformState();
            Func_080150e4(streak->roll);
            SceneTransform_ApplyPitch(streak->pitch);
            SceneTransform_ApplyYaw(streak->yaw);
            point.x = streak->head;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
            head.x += 64;
            head.y += 80;
            point.x = streak->tail;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &tail);
            tail.x += 64;
            tail.y += 80;
            streak->tail -= 4;
            streak->head -= 4;
            if (streak->tail < 0)
                streak->tail = 0;
            fade = -streak->tail / 2;
            Func_08143eb4(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
            Func_08143eb4(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
            Func_08143eb4(tail.x, tail.y, head.x, head.y, fade + 56);
        }
    }
    work->transferPending = 1;
}
