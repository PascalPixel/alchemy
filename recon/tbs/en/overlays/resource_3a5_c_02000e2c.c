/* NONMATCHING: 552 of 552 bytes, 23 differing halfwords / 23 aligned edits
 * (2026-09-26). Whole owner 02000e2c..02001054, return at 02001026 and all
 * eleven literal words at 02001028..02001050. Complete layout/pool exact.
 * Three bounded trials: phase-local actors remove the wait and both burst
 * pointer copies (45 to 26 differing halfwords); choosing an immutable total
 * before the countdown regresses to 560 bytes / 80 edits; one LandingSpot
 * record restores separate coordinate loads instead of ldmia (23 differences).
 * Retain the phase-local actors and typed coordinate record.
 * Hand-written: picks the landing spot nearest the leader for this scene
 * (Value_00000059/5a select the table), drops the leader onto it with two
 * Effect_Spawn bursts and holds the meter at gGameState+0x232 down by 5 a
 * frame for 60 frames. Binds the scene unit's calls plus gFrameCount-free
 * data: Value_00000059, Value_0000005a, Data_02000240_t. Remaining: global
 * allocation swaps the spot counter and byte offset (r6/r7); the hold-store
 * and timer-decrement scratch registers swap r2/r3; the second parameter
 * block rematerializes 0x8000 after storing scale_y instead of keeping it
 * separate from the spread literal. Further work needs a new counter or
 * parameter lifetime hypothesis, not actor/coordinate spelling sweeps. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn();
void SceneState_SetHalfwordB030(s32 value);
s32 CalculatePlanarDistance(s32 *from, s32 *to);
void OverlayObject_WaitUntilField12BelowLimit(struct FieldActor *actor, s32 limit);

struct SparkleParams {
    u8 unknown_00[8];
    s32 scale_x;
    s32 scale_y;
    s32 speed;
    s32 spread;
    u16 sprite;
    u8 unknown_1a[14];
};

struct LandingSpot {
    s32 x;
    s32 z;
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Value_00000059;
extern u8 Value_0000005a;

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02000e2c(void)
{
    struct EventWork *event;
    s32 timer;
    s32 best;
    s32 count;
    s32 *spots;
    s32 left;
    s32 offset;
    s32 *spot;
    s32 pick;
    s32 distance;
    struct FieldActor *actor;
    struct SparkleParams first;
    struct SparkleParams second;
    s16 *meter;
    u16 *hold;
    s32 zero;
    s32 hold_frames = 600;

    event = gEventWork;
    timer = 60;
    best = 0xf00000;
    Call1((void (*)())Engine_GameFlagSet, 0x200);
    SceneState_SetHalfwordB030(1);
    if (Data_02000240_t.halves[224][0] == (s32)&Value_00000059) {
        count = 3;
        spots = (s32 *)0x02009f30;
    } else if (Data_02000240_t.halves[224][0] == (s32)&Value_0000005a) {
        count = 5;
        spots = (s32 *)0x02009f48;
    } else {
        count = 2;
        spots = (s32 *)0x02009f70;
    }
    left = count;
    spot = spots;
    if (count != 0) {
        offset = 0;
        do {
            distance = CalculatePlanarDistance(&Engine_ActorGet(0)->x.fixed, spot);
            if (distance <= best) {
                best = distance;
                pick = left - count;
            }
            offset += 8;
            spot = (s32 *)((u8 *)spots + offset);
        } while (--count != 0);
    }
    pick <<= 1;
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
    {
        struct FieldActor *actor = Engine_ActorGet(0);
        struct LandingSpot *pos = (struct LandingSpot *)&spots[pick];

        Engine_ObjectSetPosition(actor, pos->x, 0, pos->z);
    }
    Engine_ActorGet(0)->velocity_y = 0x60000;
    Engine_AudioPlayCue(152);
    actor = Engine_ActorGet(0);
    OverlayObject_WaitUntilField12BelowLimit(actor, Engine_ActorGet(0)->y.fixed);
    Engine_AudioPlayCue(241);
    {
        struct FieldActor *actor = Engine_ActorGet(0);

        first.sprite = 214;
        first.scale_x = 0x8000;
        first.scale_y = 0xcccc;
        first.speed = 0x10000;
        first.spread = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0, 0x1c0000, &first);
    }
    Call3((void (*)())Engine_ActorShowEmote, 0, 0x104, 0);
    Engine_ActorSetAnimation(0, 18);
    hold = (u16 *)((u8 *)event + 0xcba);
    meter = Data_02000240_t.halves[281];
    zero = 0;
    do {
        *hold = hold_frames;
        timer--;
        if (*meter != 0) {
            *meter -= 5;
            if (*meter <= 0) {
                *meter = zero;
            } else if (timer == 0) {
                timer = 1;
            }
        }
        Engine_TaskWait(1);
    } while (timer != 0);
    {
        struct FieldActor *actor = Engine_ActorGet(0);

        second.sprite = 214;
        second.scale_y = 0xcccc;
        second.scale_x = 0x8000;
        second.speed = 0x8000;
        second.spread = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, timer, timer, 0x1c0000, &second);
    }
    Engine_AudioPlayCue(0x120);
    Engine_AudioPlayCue(152);
    Engine_ActorGet(0)->velocity_y = 0x60000;
    Engine_ActorSetAnimation(0, 1);
    Engine_EventWait(10);
    *(u16 *)((u8 *)event + 0xcba) = timer;
    SceneState_SetHalfwordB030(0);
}
