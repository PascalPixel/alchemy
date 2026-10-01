/* NONMATCHING: 2026-10-01 brief Wave2 AdvanceProbe_02001fa4 plain-source attempt.
 * Removing this one source device changes StagedActor_RunHeadingProbeStep.
 * First remaining difference: StagedActor_RunHeadingProbeStep: add	r2, sp, #8 => lsl	r0, r0, #13 (215/215 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMIRU.H"
#include "CALL.H"

s32 __umodsi3();

void ActorPresentation_ApplyTableA5ecToActorNine(void)
;

void SceneState_StoreTable96adToWork(void)
;

void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work)
;

void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
;

void FieldScene_RunScene399SequenceA(void)
;

/* The flag setter is called through a void function type, as the three
 * call sites discard its result and the compiler keeps their tails apart. */
void SceneState_UpdateZoneFlagsFromActorZero(void)
;

void SceneState_UpdateActor11WithFlag203(void)
;

void SceneActor_SetActorZeroFacingC000AndRun(void)
;

/* Runs a short fixed sequence of two calls, one 3-argument call passing a
 * fixed-point-looking pair of constants, one 3-argument call passing
 * (0, 232, 204), and a final call, in that order. */
void RunEventScript02(void)
;

/* Runs a long scripted sequence of position, animation, and timing calls
 * against actor records 0, 3, 19, and 20, with a scene phase word at
 * offset 0x1c0 of the shared scene work record set at the start and near
 * the end. */
void FieldScene_RunThreeActorChoreography(void)
;

void SceneActor_TurnTowardTableAngle(s32 z)
;

/* Draft context: these removed shared adapters isolate this one attempted device. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}
#else
static inline void Task_Wait(s32 frames)
{
    WaitFrames(frames);
}
#endif

static inline void Object_SetAnimation(struct FieldActor *object, s32 animation)
{
    Object_SetMode(object, animation);
}

void StagedActor_RunHeadingProbeStep(void)
{
    extern struct SharedData_02000240 Data_02000240;

    struct Subject_02001fa4 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = ObjectTable_Get(Data_02000240.selected_subject);

    for (;;) {
        heading = ImiruMura_SwayHeadings[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Event_Begin();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = GetMapCellCollision((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*p, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before the step above. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Object_SetAnimation(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Engine_ObjectCommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advanceProbe;
continueProbe:
        if (Map_GetTerrainHeight((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Engine_ObjectSetPosition(subject, probe[0], probe[1], probe[2]);
        Engine_ObjectCommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        Vector_AddPolarOffset((s32)0x100000, heading, probe);
        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Engine_ObjectCommitPosition(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
