/* NONMATCHING: rejected H3 candidate 876/reference 860 bytes, 411 differing
 * halfwords / 244 normalized edits (2026-09-27). A separate s16 angle
 * preserves behavior but rotates the first-loop counter into r8 and hoists
 * a shifted angle induction variable in the last loop. It fails the admitted
 * counter r4 / early work r5 / state r8 boundary. Complete diff read.
 * Checkpoint this negative model, then restore H2; close the angle-type axis.
 * Best admitted H2 candidate 864/reference 860 bytes, 347 differing
 * halfwords / 157 normalized edits (2026-09-27). A scoped early PuzzleWork
 * pointer admits early work in r5, signed choice in r6, complete in r9 and
 * previous in fp. The initial previous spill disappears; frame drops to 8.
 * Residual: the shrink decrement is hoisted across TaskWait and spilled,
 * angle truncation lifetime, work/state store order and count truncation.
 * Complete normalized diff read; no C or alignment credit.
 * H1 candidate 872/reference 860 bytes, 357 differing
 * halfwords / 177 normalized edits (2026-09-27). Separating the late random
 * choice from the initial signed choice removes its stack spill and keeps
 * the initial signed choice in r5. Prediction of a four-byte frame failed:
 * previous still spills at sp+8, complete owns fp rather than r9, and the
 * shared zero still crosses the scene branches. Full diff and pools read.
 * Historical baseline: 860 bytes, candidate 876, 392 differing halfwords,
 * 182 aligned edits (2026-09-26). Complete own-ROM state-machine recovery
 * fixes work-state polling, indexed work stores and the shared input/angle
 * lifetime. Pointer-table ownership regressed to 199 edits, byte next-state
 * retention to 194, and shared next-state/scale lifetime to 242; retained
 * the best reconstruction. Remaining: initial saved-register lifetimes,
 * work store scheduling and the final angle-step constant lifetime.
 *
 * 2026-09-27 callback-family audit: exact CALLBACK_SCHEDULER.C and
 * FIELD_EVENT.H both give add/remove an s32 return; the old Call2/Call1
 * wrappers erased it through void function pointers. Transfer the canonical
 * Task_AddCallback/Task_RemoveCallback interface used by exact
 * VINASU_CHOJO/SCRIPTED_PRESENTATION.C, binding this overlay's exact
 * SceneEffect_UpdateStateMachine (02000f94, runtime 02008f94).
 * One trial: 876/392/182, candidate binary identical to the retained baseline.
 * Full normalized diff confirms the 12-byte frame versus reference 4,
 * next-state spills, work-store ordering and angle lifetime remain unchanged.
 * Retain the corrected interface; no follow-up without a new source fact.
 * No state, zero, loop or counter model changed; zero new DONE bytes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct PuzzleState {
    u8 state;
    s8 next;
};

struct PuzzleWork {
    s16 values[6];
};

enum { WORK_SHOWN, WORK_DELAY, WORK_UNUSED, WORK_ANGLE, WORK_SIZE, WORK_PHASE };

extern struct PuzzleState Data_02001001;
extern u8 Data_02001000;
extern struct PuzzleWork *Data_0200bf6c;
extern u8 Value_ffffcccd;
extern u8 Value_fffff400;

void SceneActor_SetPositionFromTransformedBase(s32 id, s32 x, s32 angle);
void SceneEffect_UpdateStateMachine(void);
void ArutamiraDou_ReleaseWallBurst(void);
s32 Main_030003ac(s32 dividend, s32 divisor);
void Engine_EventResetEffectCounter(void);

/* FAKEMATCH: wrappers keep call constants in word-mode argument pseudos. */
static __inline__ void Call1(void (*fn)(), s32 value)
{
    fn(value);
}

static __inline__ void Call3(void (*fn)(), s32 left, s32 middle, s32 right)
{
    fn(left, middle, right);
}

void FieldScene_RunStatefulSequence(s32 action)
{
    s32 complete = 0;
    s32 i;
    struct FieldActor *actor;
    u32 state;
    u32 previous;
    s32 next;
    s32 scale;
    s32 x;
    s16 angle;
    struct PuzzleWork *work;

    Engine_EventBegin();
    Engine_EventResetEffectCounter();
    Call1(Engine_EventSetMessage, 0x21db);
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
    for (i = 0; i <= 4; i++) {
        actor = Engine_ActorGet(i + 11);
        actor->update = 0;
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
    }
    state = Data_02001001.state;
    {
        struct PuzzleWork *work = Data_0200bf6c;

        next = Data_02001001.next;
        previous = (u8)Data_02001001.next;
        work->values[WORK_ANGLE] = Engine_MathDivide(next << 16, 5) + 0x4000;
    }
    if ((s8)state == 0) {
        if (action == 16) {
            state = 1;
            Engine_AudioPlayCue(110);
        } else {
            Engine_AudioPlayCue(114);
        }
        Data_02001000 = 0;
    } else if ((s8)state == 1) {
        if (action == 16) {
            Engine_AudioPlayCue(110);
        } else if (action == 20) {
            state = 2;
            Engine_AudioPlayCue(110);
            Engine_TaskWait(30);
            angle = Data_0200bf6c->values[WORK_ANGLE];
            for (i = 0; i <= 4; i++) {
                s32 id = i + 11;

                Call3(SceneActor_SetPositionFromTransformedBase, id, 0x180000, (u16)angle);
                Engine_AudioPlayCue(151);
                actor = Engine_ActorGet(id);
                actor->scale_x = 0;
                scale = 0x6666;
                do {
                    actor->scale_y = scale;
                    actor->scale_x = scale;
                    Engine_TaskWait(1);
                    scale += 0xc00;
                } while (actor->scale_x <= 0xffff);
                angle += (s32)&Value_ffffcccd;
            }
            Engine_TaskWait(30);
            complete = 1;
        } else {
            Engine_AudioPlayCue(114);
            state = 0;
        }
    } else if ((s8)state == 2) {
        if (action != next + 16) {
            state = 0;
            Engine_AudioPlayCue(114);
            Engine_TaskWait(30);
            for (i = 0; i <= 4; i++) {
                s32 id = i + 11;

                actor = Engine_ActorGet(id);
                Engine_AudioPlayCue(151);
                scale = actor->scale_x;
                if (scale > 0x6666) {
                    do {
                        actor->scale_y = scale;
                        actor->scale_x = scale;
                        Engine_TaskWait(1);
                        scale += (s32)&Value_fffff400;
                    } while (actor->scale_x > 0x6666);
                }
                Engine_ActorSetPosition(id, 0, 0);
            }
        } else {
            Engine_AudioPlayCue(110);
            complete = 1;
            Engine_TaskWait(30);
        }
    }
    Data_02001001.state = state;
    if (complete != 0) {
        u32 count = Data_02001000 + 1;
        s32 choice;

        Data_02001000 = count;
        choice = Main_030003ac((s8)(((u32)(Engine_RandomNext() << 2) >> 16)
                                + previous + 1) + 5, 5);
        Data_02001001.next = choice;
        work = Data_0200bf6c;
        work->values[WORK_SHOWN] = 0;
        work->values[WORK_DELAY] = 0;
        work->values[WORK_SIZE] = 0x200;
        work->values[WORK_PHASE] = 0x3000;
        Task_AddCallback(SceneEffect_UpdateStateMachine, TASK_PRIORITY_SCENE);
        if ((u8)count <= 2) {
            while (Data_0200bf6c->values[WORK_SHOWN] != 99)
                Engine_TaskWait(1);
            Engine_TaskWait(10);
            Engine_AudioPlayCue(110);
        } else {
            Data_02001001.state = 99;
            while (Data_0200bf6c->values[WORK_SHOWN] != 2)
                Engine_TaskWait(1);
            work = Data_0200bf6c;
            work->values[WORK_SHOWN] = 2;
            work->values[WORK_DELAY] = 0;
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
            Engine_EventWait(20);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Data_0200bf6c->values[WORK_SHOWN] = 99;
            Engine_AudioPlayCue(190);
            x = 0x180000;
            angle = Data_0200bf6c->values[WORK_ANGLE];
            do {
                for (i = 0; i <= 4; i++) {
                    s32 id = i + 11;

                    actor = Engine_ActorGet(id);
                    actor->scale_x -= 16;
                    actor->scale_y -= 16;
                    SceneActor_SetPositionFromTransformedBase(id, x, (u16)angle);
                    angle += (s32)&Value_ffffcccd;
                }
                x += (s32)&Value_ffffcccd;
                angle += 0xc00;
                Engine_TaskWait(1);
            } while (x > 0);
            for (i = 0; i <= 4; i++)
                Engine_ActorSetPosition(i + 11, 0, 0);
            ArutamiraDou_ReleaseWallBurst();
            Engine_AudioPlayCue(80);
        }
        Task_RemoveCallback(SceneEffect_UpdateStateMachine);
    }
}
