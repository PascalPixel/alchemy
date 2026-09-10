#include "types.h"

#define NULL ((void *)0)
#define StagedActor_RunHeadingProbeStep Func_02001fa4

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02001fa4 {
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

extern s16 Data_0200a430[];
extern struct SharedData_02000240 Data_02000240;
extern u32 Data_03001ae8;

void Func_02009f24();
struct Subject_02001fa4 *Func_02004358();
void Func_02004212();
s32 Func_0200420c();
void Func_020041ba();
s32 Func_02004226_a();
s32 Func_02004230();
void Func_0200423e();
void Func_02004216();
void Func_02004226_b();
void Func_0200425c();
s32 Func_0200428e();
void Func_02004298();
void Func_020042a6();
void Func_02004278();
s32 Func_020042e4();
void Func_020042da();
void Func_020042e8();
void Func_02004286();
void Func_020042a4();
void Func_02004370();

/* Data_0200a430 is this overlay's heading table.  Data_02000240 is a resident
 * table; its word at byte offset 500 selects the subject. */

/* The per-frame callback installed on the subject, named by its address. */

/* Imports named by their relocated call words.  Old-style declarations are
 * mandatory: one of these imports is reached with two arguments here while its
 * other sites pass three or four. */

static __inline__ void AdvanceProbe_02001fa4(s32 heading, s32 *probe)
{
    /* Keep this call behind an inline boundary: the third argument's address is
     * then rematerialized before the split 0x100000 constant is completed. */
    Func_02004278((s32)0x100000, heading, probe);
}

/*
 * The overlay's pathing step for resource_399.  The six pool words after the
 * return belong to this owner.
 *
 * Frame map: the goal marker, then the heading, then the three-word probe
 * position that the stepping imports are handed by address.  The order of the x
 * and z assignments, together with the inline stepping wrapper, is what leaves
 * a single high-register copy before the arithmetic that follows it.
 */
void StagedActor_RunHeadingProbeStep(void)
{
    struct Subject_02001fa4 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = Func_02004358(Data_02000240.selected_subject);

    for (;;) {
        heading = Data_0200a430[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Func_02004212();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = Func_0200420c((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Func_020041ba((s32)0x100000, heading, probe);

        marker = Func_02004226_a((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Func_02004230((s32)*p, probe[0], probe[2])
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
        Func_0200423e(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Func_02004216(subject, 2);
        Func_02004226_b(subject, 48);
        Func_0200425c(subject);
        subject->callback = (void *)Func_02009f24;

        goto advanceProbe;
continueProbe:
        if (Func_0200428e((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Func_02004298(subject, probe[0], probe[1], probe[2]);
        Func_020042a6(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        AdvanceProbe_02001fa4(heading, probe);
        marker = Func_020042e4((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Func_020042da(subject, x, subject->y, z);
        Func_020042e8(subject);
        Func_02004286(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Func_020042a4(10);
    Func_02004370();
}
