#include "types.h"
#define NULL ((void *)0)

/*
 * resource_39a owner at 0x02002094, 452 bytes: the overlay's pathing step.
 *
 * Complete owner: the two-stage 'push {r5, r6, r7, lr}' / high-register push
 * prologue with 'sub sp, #20', and the matching
 * 'add sp, #20 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0'.  r0 holds the
 * popped return address, so nothing is returned.  Seven pool words follow the
 * return and are data.
 *
 * Clean-room transposition of resource_399:1fa4.  The local overlay bytes
 * witness the same 424-byte instruction body, with the heading table,
 * callback pointer, and veneer addresses translated to resource_39a.  The
 * complete owner is 424 bytes of code plus its 28-byte literal pool.
 *
 * The local call map has 20 sites and 19 distinct veneer addresses.  Repeated
 * semantic imports retain separate names where this overlay uses separate
 * veneers, while the two-argument command at 0x02002164 remains distinct from
 * the three-argument marker lookup sharing veneer address 0x02004406.
 *
 * Frame map: sp+0 holds the goal marker, sp+4 the heading, and sp+8..sp+19 the
 * three-word probe position handed to the stepping imports by address.
 * The witnessed x/z assignment order plus the inline stepping wrapper leave
 * only one independent high-register-copy/ALU pair; the already-supported
 * -fthumb-high-move-before-alu mode closes that pair byte-exactly.
 */

/* In-image table at 0x02008000 + 0x2464; 0x02000240 is below the link base
 * and is a resident table whose word at byte offset 500 selects the subject. */
extern s16 Data_0200a464[];
struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};
extern struct SharedData_02000240 Data_02000240;
extern u32 Data_03001ae8;

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

/* Installed callback at 0x02008000 + 0x2014, named by its linked address. */
extern void Func_0200a014();

/* Local veneer identities witnessed by this owner's encoded BL pairs. */
struct Subject_02002094 *Func_02004498();
void Func_0200440a();
s32 Func_020043ec();
void Func_02004392();
s32 Func_02004406();
s32 Func_02004410();
void Func_02004426();
void Func_02004406_b();
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

static __inline__ void AdvanceProbe_02002094(s32 heading, s32 *probe)
{
    /* Keep this call as an inline boundary: GCC then rematerializes sp+8 for
     * argument 2 before completing the split 0x100000 constant, as in ROM. */
    Func_02004450((s32)0x100000, heading, probe);
}

void Func_02002094(void)
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
        /* The test is on heading << 16 against 0xffff0000, i.e. the signed
         * halfword -1 meaning "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_0200440a();

        /* movs r3,#0x80 / lsls r3,#12 builds the 0x80000 bias kept in fp. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = Func_020043ec((s32)*subject_id, x, z);
        /* movs r0,#0x80 / lsls r0,#13 builds 0x100000.  The probe block is
         * passed by address and is advanced by the callee. */
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
        /* Same veneer address as the marker lookup, but a two-argument
         * command with a distinct declaration. */
        Func_02004406_b(subject, 2);
        Func_02004416(subject, 48);
        Func_02004444(subject);
        subject->callback = (void *)Func_0200a014;

        goto advanceProbe;
continueProbe:
        if (Func_0200446e((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
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

advanceProbe:
        AdvanceProbe_02002094(heading, probe);
        marker = Func_020044c4_b((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
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
    /* movs r3,#0x80 / lsls r3,#7 builds 0x4000. */
    subject->state_052 = 0x4000;

tail:
    Func_02004484(10);
    Func_02004568();
}
