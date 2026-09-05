#include "types.h"

#define NULL ((void *)0)

/*
 * resource_399 owner at 0x02001fa4, 452 bytes: the overlay's pathing step.
 *
 * Complete owner: the two-stage 'push {r5, r6, r7, lr}' / high-register push
 * prologue with 'sub sp, #20', and the matching
 * 'add sp, #20 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0'.  r0 holds the
 * popped return address, so nothing is returned.  Six pool words follow the
 * return and are data.
 *
 * TRANSPOSED from games/gs1/semantic/overlays/resource_39a_c_02002094.c.  The two owners
 * are the same routine shared verbatim: over all 226 halfwords they differ in
 * 22 places, 20 of which are the halves of the ten BL pairs and two of which are
 * the low halves of pool words.
 *
 * What was changed:
 *  - the heading table 0x0200a464 becomes 0x0200a430 (file offset 0x2430 under
 *    the 0x02008000 link base);
 *  - the installed per-frame callback 0x0200a015 becomes 0x02009f25, i.e. this
 *    overlay's own routine at file offset 0x1f24 plus the Thumb bit.  That
 *    offset does open a function ('push {r5, lr}', 0xb520), which is an
 *    independent check on the base and on the pointer's meaning;
 *  - all 20 calls were re-resolved with 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 *    resource_399 1fa4': 20 sites over 11 distinct veneers, the same targets
 *    with the same multiplicities and in the same site order as resource_39a.
 *    The 39a source predates the corrected 'bl' rule and named the imports by
 *    their (wrongly decoded) displacements, which both split single imports
 *    across many names and merged two distinct ones under Func_02004406.  The
 *    corrected resolution is used here and it groups the calls sensibly: the
 *    three marker lookups are one import (0x080091b0), the two height probes
 *    another (0x080091a8), the three placements a third (0x08009150), and the
 *    2-argument site the 39a source shared with the 3-argument marker lookup is
 *    a genuinely different import (0x08009080).
 *
 * Frame map: sp+0 holds the goal marker, sp+4 the heading, and sp+8..sp+19 the
 * three-word probe position handed to the stepping imports by address.
 * The witnessed x/z assignment order plus the inline stepping wrapper leave
 * only one independent high-register-copy/ALU pair; the already-supported
 * -fthumb-high-move-before-alu mode closes that pair byte-exactly.
 */

/* In-image table at 0x02008000 + 0x2430; 0x02000240 is below the link base
 * and is a resident table whose word at byte offset 500 selects the subject. */
extern s16 Data_0200a430[];
struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};
extern struct SharedData_02000240 Data_02000240;
extern u32 Data_03001ae8;

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

/* The installed callback, named by its in-image address. */
extern void Func_02009f24();

/* Imports, named by the main-image address their veneer publishes.  Old-style
 * declarations are mandatory: 0x08009080 is reached with two arguments here
 * while its siblings take three or four. */
struct Subject_02001fa4 *Func_02004358();
void Func_02004212();
s32 Func_0200420c();
void Func_020041ba();
s32 Func_02004226();
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

static __inline__ void AdvanceProbe_02001fa4(s32 heading, s32 *probe)
{
    /* Keep this call as an inline boundary: GCC then rematerializes sp+8 for
     * argument 2 before completing the split 0x100000 constant, as in ROM. */
    Func_02004278((s32)0x100000, heading, probe);
}
                         /* subject record by table selector */

                         /* tile marker at (x, z) */
                         /* advance the probe block one step along a heading */
                         /* tile height at (x, z) */
                         /* place the subject at (x, y, z) */

                         /* commit the placement */

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
        /* The test is on heading << 16 against 0xffff0000, i.e. the signed
         * halfword -1 meaning "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_02004212();

        /* movs r3,#0x80 / lsls r3,#12 builds the 0x80000 bias kept in fp. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = Func_0200420c((s32)*p, x, z);
        /* movs r0,#0x80 / lsls r0,#13 builds 0x100000.  The probe block is
         * passed by address and is advanced by the callee. */
        Func_020041ba((s32)0x100000, heading, probe);

        marker = Func_02004226((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Func_02004230((s32)*p, probe[0], probe[2])
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
    /* movs r3,#0x80 / lsls r3,#7 builds 0x4000. */
    subject->state_052 = 0x4000;

tail:
    Func_020042a4(10);
    Func_02004370();
}
