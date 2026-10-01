/* NONMATCHING byte-publication transfer: 852/860 bytes, 339 differing
 * halfwords / 97 aligned edits (2026-09-27). Keep the four-byte frame,
 * arithmetic recovery and newly admitted publication boundaries. Explicit
 * byte stores, following WORLD_MAP/LINKED_EFFECTS.C, remove the spurious
 * zero retained across the random/remainder calls and publish the choice
 * before reloading work. A separate u8 completed snapshot after work setup
 * restores count truncation before the callback and cmp #2 afterwards.
 * These local boundaries improve despite 95 -> 97 aggregate aligned edits.
 * Remaining: early work/state load ordering, first angle lifetime, count
 * address derivation, work scratch registers and final angle increment.
 * Bounded negatives: early u8 count truncates before its store (852/340/99);
 * subtracting one from the state address restores the count address but
 * lacks valid object ownership (852/339/96), so that diagnostic is rejected.
 * A nested owning record aligns phase to +4, not +1: invalid layout.
 * A flat count/state/next record restores offsets 0/1/2 but gives 860/299/113,
 * retaining another base in r8 and disrupting work stores. Reject it too.
 * Restoring Value_ffffcccd only in the first ring gives 856/338/99, without
 * the required retained step or shifted-angle lifetime. No further sweep.
 * All complete normalized differences read; zero new exact credit.
 *
 * Previous Astra arithmetic recovery: 860/860 bytes, 259 differing
 * halfwords / 95 aligned edits (2026-09-27), with the correct four-byte
 * frame and equal branch topology. Replacing the decompiler's address-valued
 * -0xc00 with ordinary subtraction removes its call-crossing spill and
 * restores the complete prologue and opening actor loop (104 edits).
 * Ordinary -0x3333 arithmetic also restores the final loop's reload rather
 * than a saved address constant (95 edits). Keep these arithmetic facts;
 * the ROM's complete literal pool remains part of the acceptance gate.
 * Remaining: initial state/work load order; first angle snapshot lifetime;
 * completion-count truncation, state/work publication order and shared zero;
 * final angle increment. No DONE or alignment credit.
 * Bounded transfer results: six named work fields are binary-identical to
 * old H2 (864/347/157). An early initialized eight-bit count record gives
 * 864/348/154 but truncates before the count store, unlike the reference.
 * On the new arithmetic model, a phase-local s32 zero and a late initialized
 * eight-bit count record are each binary-identical (860/259/95).
 * A signed post-call angle shift gives 848/413/228: it moves action to r8,
 * the loop counter to r7 and removes the required stack slot. Rejected.
 * Full normalized differences inspected; stop these field/zero/count/angle
 * transfers. Cross-edition check: all six editions share the same 648
 * unmasked core bytes, so they supply no alternative core for this owner.
 *
 * Previous canonical H2 candidate 864/reference 860 bytes, 347 differing
 * halfwords / 157 normalized edits. Restored after rejected H3 e00832ddf;
 * this session's three structural trials are exhausted. No declaration or
 * angle-type sweep follows without new producer/consumer evidence.
 * Rejected H3 candidate 876/reference 860 bytes, 411 differing
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
 * ArutamiraDou_SpinActorWheel (02000f94, runtime 02008f94).
 * One trial: 876/392/182, candidate binary identical to the retained baseline.
 * Full normalized diff confirms the 12-byte frame versus reference 4,
 * next-state spills, work-store ordering and angle lifetime remain unchanged.
 * Retain the corrected interface; no follow-up without a new source fact.
 * No state, zero, loop or counter model changed; zero new DONE bytes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
    return Engine_TaskAddCallback(callback, priority);
}

static inline s32 Task_RemoveCallback(void (*callback)(void))
{
    return Engine_TaskRemoveCallback(callback);
}

extern u8 MsgArutamiraRotatedRock[];

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
extern struct PuzzleWork *ArutamiraDou_ClearTarget;

void SceneActor_SetPositionFromTransformedBase(s32 id, s32 x, s32 angle);
void ArutamiraDou_SpinActorWheel(void);
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
    struct PuzzleWork *work;

    Engine_EventBegin();
    Engine_EventResetEffectCounter();
    Call1(Engine_EventSetMessage, (s32)MsgArutamiraRotatedRock);
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
    for (i = 0; i <= 4; i++) {
        actor = Object_GetById(i + 11);
        actor->update = 0;
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
    }
    state = Data_02001001.state;
    {
        struct PuzzleWork *work = ArutamiraDou_ClearTarget;

        next = Data_02001001.next;
        previous = (u8)Data_02001001.next;
        work->values[WORK_ANGLE] = __divsi3(next << 16, 5) + 0x4000;
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
            action = ArutamiraDou_ClearTarget->values[WORK_ANGLE];
            for (i = 0; i <= 4; i++) {
                s32 id = i + 11;

                action <<= 16;
                Call3(SceneActor_SetPositionFromTransformedBase, id, 0x180000, (u32)action >> 16);
                Engine_AudioPlayCue(151);
                actor = Object_GetById(id);
                actor->scale_x = 0;
                scale = 0x6666;
                do {
                    actor->scale_y = scale;
                    actor->scale_x = scale;
                    Engine_TaskWait(1);
                    scale += 0xc00;
                } while (actor->scale_x <= 0xffff);
                action = ((s32)((((u32)action >> 16) - 0x3333) << 16)) >> 16;
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

                actor = Object_GetById(id);
                Engine_AudioPlayCue(151);
                scale = actor->scale_x;
                if (scale > 0x6666) {
                    do {
                        actor->scale_y = scale;
                        actor->scale_x = scale;
                        Engine_TaskWait(1);
                        scale -= 0xc00;
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
    /* FAKEMATCH: byte publication avoids a shared bit-field zero. */
    *(u8 *)&Data_02001001.state = state;
    if (complete != 0) {
        u32 count = Data_02001000 + 1;
        s32 choice;
        u8 completed;

        Data_02001000 = count;
        choice = Main_030003ac((s8)(((u32)(Engine_RandomNext() << 2) >> 16)
                                + previous + 1) + 5, 5);
        *(s8 *)&Data_02001001.next = choice;
        work = ArutamiraDou_ClearTarget;
        work->values[WORK_SHOWN] = 0;
        work->values[WORK_DELAY] = 0;
        work->values[WORK_SIZE] = 0x200;
        work->values[WORK_PHASE] = 0x3000;
        completed = count;
        Task_AddCallback(ArutamiraDou_SpinActorWheel, TASK_PRIORITY_SCENE);
        if (completed <= 2) {
            while (ArutamiraDou_ClearTarget->values[WORK_SHOWN] != 99)
                Engine_TaskWait(1);
            Engine_TaskWait(10);
            Engine_AudioPlayCue(110);
        } else {
            Data_02001001.state = 99;
            while (ArutamiraDou_ClearTarget->values[WORK_SHOWN] != 2)
                Engine_TaskWait(1);
            work = ArutamiraDou_ClearTarget;
            work->values[WORK_SHOWN] = 2;
            work->values[WORK_DELAY] = 0;
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
            Engine_EventWait(20);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            ArutamiraDou_ClearTarget->values[WORK_SHOWN] = 99;
            Engine_AudioPlayCue(190);
            x = 0x180000;
            action = ArutamiraDou_ClearTarget->values[WORK_ANGLE];
            do {
                for (i = 0; i <= 4; i++) {
                    s32 id = i + 11;

                    actor = Object_GetById(id);
                    actor->scale_x -= 16;
                    actor->scale_y -= 16;
                    SceneActor_SetPositionFromTransformedBase(id, x, (u32)(action << 16) >> 16);
                    action = ((s32)((((u32)(action << 16) >> 16) - 0x3333) << 16)) >> 16;
                }
                x -= 0x3333;
                action = ((s32)((((u32)(action << 16) >> 16) + 0xc00) << 16)) >> 16;
                Engine_TaskWait(1);
            } while (x > 0);
            for (i = 0; i <= 4; i++)
                Engine_ActorSetPosition(i + 11, 0, 0);
            ArutamiraDou_ReleaseWallBurst();
            Engine_AudioPlayCue(80);
        }
        Task_RemoveCallback(ArutamiraDou_SpinActorWheel);
    }
}
