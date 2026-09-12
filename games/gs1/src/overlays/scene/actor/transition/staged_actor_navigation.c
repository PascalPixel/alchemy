#include "types.h"

#define NULL ((void *)0)
#define SceneActor_StepSubjectAlongHeading Func_02002094

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02002094 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 Data_0200a464[];
extern struct SharedData_02000240 Data_02000240;
extern u32 Data_03001ae8;

void Func_0200a014();
struct Subject_02002094 *Func_02004498();
void Func_0200440a();
s32 Func_020043ec();
void Func_02004392();
s32 Func_02004406();
s32 Func_02004410();
void Func_02004426();
void Func_02004406_a();
void Func_02004416();
void Func_02004444();
s32 Func_0200446e();
void Func_02004480();
void Func_0200448e();
void Func_02004450();
s32 Func_020044c4_b();
void Func_020044c2();
void Func_020044d0();
void Func_02004466();
void Func_02004484();
void Func_02004568();

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */

static __inline__ void AdvanceProbe_02002094(s32 heading, s32 *probe)
{
    /*
     * Keep this call behind an inline boundary: it is what makes sp+8 be
     * rematerialized for argument 2 before the split 0x100000 constant is
     * completed.
     */
    Func_02004450((s32)0x100000, heading, probe);
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
    struct Subject_02002094 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = Func_02004498(Data_02000240.selected_subject);

    for (;;) {
        heading = Data_0200a464[(Data_03001ae8 >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_0200440a();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Func_020043ec((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Func_02004392((s32)0x100000, heading, probe);

        marker = Func_02004406((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Func_02004410((s32)*subject_id, probe[0], probe[2])
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
        Func_02004426(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Func_02004406_a(subject, 2);
        Func_02004416(subject, 48);
        Func_02004444(subject);
        subject->callback = (void *)Func_0200a014;

        goto advance_probe;
continue_probe:
        if (Func_0200446e((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Func_02004480(subject, probe[0], probe[1], probe[2]);
        Func_0200448e(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe_02002094(heading, probe);
        marker = Func_020044c4_b((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Func_020044c2(subject, x, subject->y, z);
        Func_020044d0(subject);
        Func_02004466(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Func_02004484(10);
    Func_02004568();
}
