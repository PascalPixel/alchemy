/* Draft, not exact (2026-09-26): candidate=396 reference=396,
   9 differing halfwords, 8 aligned edits; equal block topology, frame 48.
   Fresh baseline with shared projection header: 412/396 bytes, 188 differing
   halfwords, 73 aligned edits. H1 splits the first-frame seed cursor from
   the draw cursor. This recovers seed r5, mask r6, draw cursor r8 and removes
   twelve extra bytes; the work spill and both projection slots already match.
   H1 witness 5283d5f32: 400 bytes, 146 differing halfwords, 49 aligned edits.
   H2 initializes point y/z from the draw index's zero. This removes the
   duplicate zero instruction and restores the full extent and pool position.
   H2 witness a1b8a7aa8: 396 bytes, 35 differing halfwords, 31 aligned edits.
   H3 indexes both arrays by i instead of retaining cursors. Strength
   reduction now initializes seed after mask, gives the exact seed loop,
   restores all projection argument/register choices, and fixes pool order.
   Residual is confined to the draw preheader at 080cc9c8..080cc9d6
   (point address/zero/index/array-base scheduling) and 080cca7a..080cca7e
   (color+48 before rather than after the two coordinate decrements).
   The exact next owner PREPARE_SCENE.C schedules this no-argument callback
   at 0xc80 after resetting work+778c. Extent is [080cc960,080ccaec).
   No bytes adopted. Three structural hypotheses used; stop before any
   further setup-order spelling trials. No complete match is claimed. */
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
        for (i = 0; i != 256; i++) {
            struct Streak *seed = &((struct Streak *)0x02010000)[i];
            s32 r = Random16() & 15;

            seed->head = r + 48;
            seed->tail = r + 40;
            seed->pitch = Random16() & 0xffff;
            seed->yaw = Random16() & 0xffff;
            seed->roll = Random16() & 0xffff;
        }
    }
    i = 0;
    point.y = i;
    point.z = i;
    for (; i != 64; i++) {
        streak = &((struct Streak *)0x02010000)[i];
        if (frame > i / 4 && streak->head > 0) {
            s32 fade;
            s32 color;

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
            color = fade + 48;
            Func_080cde90(tail.x - 1, tail.y, head.x - 1, head.y, color);
            Func_080cde90(tail.x, tail.y - 1, head.x, head.y - 1, color);
            Func_080cde90(tail.x, tail.y, head.x, head.y, fade + 56);
        }
    }
    *(s32 *)(work + 0x7824) = 1;
}
