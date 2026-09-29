#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];

/*
 * Frame callback that BattlePresentation_PrepareScene schedules at 0xc80.
 * On its first frame it seeds 256 streaks with a random length and a random
 * roll, pitch and yaw; every frame after that it draws the first 64 (one more
 * every four frames) as three lines from tail to head, both projected around
 * the screen point (64, 80), and pulls each streak four units closer to that
 * point. The line colour brightens as the tail reaches the centre.
 */

u32 Random16(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080cde90(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

struct Streak {
    s32 head;
    s32 tail;
    s32 unused_08;
    s32 pitch;
    s32 yaw;
    s32 roll;
    s32 unused_18;
};

struct StreakPoint {
    s32 distance;
    s32 y;
    s32 z;
};

void BattlePresentation_DrawStreaks(void)
{
    u8 *work = *(u8 **)gBattleFxWork;
    s32 frame;
    s32 i;
    struct Streak *streak;
    struct StreakPoint point;
    struct EffectPosition head;
    struct EffectPosition tail;

    frame = (*(s32 *)(work + 0x778c))++;
    if (frame == 0) {
        for (i = 0; i != 256; i++) {
            struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
            s32 r = Random16() & 15;

            seed->head = r + 48;
            seed->tail = r + 40;
            seed->pitch = Random16() & 0xffff;
            seed->yaw = Random16() & 0xffff;
            seed->roll = Random16() & 0xffff;
        }
    }
    point.y = 0;
    point.z = 0;
    i = 0;
    for (; i != 64; i++) {
        streak = &((struct Streak *)Ram_MapCellBuffer)[i];
        if (frame > i / 4 && streak->head > 0) {
            s32 fade;

            Render_ResetTransformState();
            SceneTransform_ApplyRoll(streak->roll);
            SceneTransform_ApplyPitch(streak->pitch);
            SceneTransform_ApplyYaw(streak->yaw);
            point.distance = streak->head;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
            head.x += 64;
            head.y += 80;
            point.distance = streak->tail;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &tail);
            tail.x += 64;
            tail.y += 80;
            streak->tail -= 4;
            streak->head -= 4;
            if (streak->tail < 0)
                streak->tail = 0;
            fade = -streak->tail / 2;
            Func_080cde90(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
            Func_080cde90(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
            Func_080cde90(tail.x, tail.y, head.x, head.y, fade + 56);
        }
    }
    *(s32 *)(work + 0x7824) = 1;
}
