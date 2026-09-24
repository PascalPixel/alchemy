/* Draft, not exact (2026-09-24): candidate=380 reference=396, 168 differing halfwords.
   Hand-written from the assembly; control flow, calls and constants follow the
   reference. Residual: the reference keeps the work pointer on the stack (all of
   r5-r7, r8-fp are taken), addresses the two projected positions through
   registers rebuilt before each projection call (r4 spilled to sp+4, r7), and
   keeps the streak pointer in r8 and the 0xffff mask in r6. */
#include "TYPES.H"
#include "EFFECT_STEP.H"

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

void Func_080cc960(void)
{
    u8 *work = *(u8 **)0x03001eec;
    s32 frame;
    s32 i;
    struct Streak *streak;
    struct StreakPoint point;
    struct EffectPosition head;
    struct EffectPosition tail;

    frame = (*(s32 *)(work + 0x778c))++;
    if (frame == 0) {
        streak = (struct Streak *)0x02010000;
        for (i = 0; i != 256; i++) {
            s32 r = Random16() & 15;

            streak->head = r + 48;
            streak->tail = r + 40;
            streak->pitch = Random16() & 0xffff;
            streak->yaw = Random16() & 0xffff;
            streak->roll = Random16() & 0xffff;
            streak++;
        }
    }
    point.y = 0;
    point.z = 0;
    streak = (struct Streak *)0x02010000;
    for (i = 0; i != 64; i++) {
        if (frame > i / 4 && streak->head > 0) {
            s32 fade;
            s32 color;

            Render_ResetTransformState();
            SceneTransform_ApplyRoll(streak->roll);
            SceneTransform_ApplyPitch(streak->pitch);
            SceneTransform_ApplyYaw(streak->yaw);
            point.distance = streak->head;
            EffectPosition_ApplyBaseAndYOffset((s32)&point, &head);
            head.x += 64;
            head.y += 80;
            point.distance = streak->tail;
            EffectPosition_ApplyBaseAndYOffset((s32)&point, &tail);
            tail.x += 64;
            tail.y += 80;
            streak->tail -= 4;
            streak->head -= 4;
            if (streak->tail < 0)
                streak->tail = 0;
            fade = -streak->tail / 2;
            color = fade + 48;
            Func_080cde90(tail.x - 1, tail.y, head.x - 1, head.y, color);
            Func_080cde90(tail.x, tail.y - 1, head.x, head.y - 1, color);
            Func_080cde90(tail.x, tail.y, head.x, head.y, fade + 56);
        }
        streak++;
    }
    *(s32 *)(work + 0x7824) = 1;
}
