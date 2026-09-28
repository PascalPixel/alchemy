/* Turning and stepping an actor along the heading the held direction gives. */
#include "IMIRU_FUCHIN.H"

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = gImiruFuchinKeyHeadings[(*(u32 *)gKeysHeld >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetAnimation(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetAnimation(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

/*
 * Pathing step for resource_39a.  r0 holds the popped return address, so
 * nothing is returned, and the seven pool words after the return belong to
 * the owner.  Frame: sp+0 is the goal marker, sp+4 the heading, and
 * sp+8..sp+19 the three-word probe position handed to the stepping imports by
 * address.  The x and z assignment order and the inline stepping wrapper are
 * what reproduce the reference; do not reorder or respell them.
 */
void SceneActor_StepSubjectAlongHeading(void)
{

    struct PathSubject *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = ObjectTable_Get(gGameState.selected_actor);

    for (;;) {
        heading = gImiruFuchinHeadings[(*(u32 *)gKeysHeld >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Event_Begin();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = GetMapCellCollision((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Object_SetPosition(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Object_SetAnimation(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Object_CommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advance_probe;
continue_probe:
        if (Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Object_SetPosition(subject, probe[0], probe[1], probe[2]);
        Object_CommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe(heading, probe);
        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Object_SetPosition(subject, x, subject->y, z);
        Object_CommitPosition(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
