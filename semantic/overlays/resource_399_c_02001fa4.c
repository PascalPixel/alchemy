typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
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
 * TRANSPOSED from semantic/overlays/resource_39a_c_02002094.c.  The two owners
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
 *  - all 20 calls were re-resolved with 'bun tools/overlay_call_targets.ts
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
 */

/* In-image table at 0x02008000 + 0x2430; 0x02000240 is below the link base
 * and is a resident table whose word at byte offset 500 selects the subject. */
extern s16 Data_0200a430[];
extern s16 Data_02000240[];
extern u32 Data_03001ae8;

/* The installed callback, named by its in-image address. */
extern void Func_02001f24();

/* Imports, named by the main-image address their veneer publishes.  Old-style
 * declarations are mandatory: 0x08009080 is reached with two arguments here
 * while its siblings take three or four. */
u8 *Func_0808a400();     /* subject record by table selector */
void Func_0808a018();
s32 Func_080091b0();     /* tile marker at (x, z) */
void Func_08000128();    /* advance the probe block one step along a heading */
s32 Func_080091a8();     /* tile height at (x, z) */
void Func_08009150();    /* place the subject at (x, y, z) */
void Func_08009080();
void Func_08009088();
void Func_08009158();    /* commit the placement */
void Func_080000c0();
void Func_0808a020();

void Func_02001fa4(void)
{
    u8 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 x;
    s32 z;

    subject = Func_0808a400(*(s32 *)((u8 *)Data_02000240 + 500));

    for (;;) {
        heading = Data_0200a430[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, i.e. the signed
         * halfword -1 meaning "no heading". */
        if ((s16)heading == -1) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_0808a018();

        /* movs r3,#0x80 / lsls r3,#12 builds the 0x80000 bias kept in fp. */
        x = (*(s32 *)(subject + 8) & (s32)0xfff00000) + 0x80000;
        probe[0] = x;
        probe[1] = *(s32 *)(subject + 12);
        z = (*(s32 *)(subject + 16) & (s32)0xfff00000) + 0x80000;
        probe[2] = z;

        goal = Func_080091b0((s32)subject[34], x, z);
        /* movs r0,#0x80 / lsls r0,#13 builds 0x100000.  The probe block is
         * passed by address and is advanced by the callee. */
        Func_08000128((s32)0x100000, heading, probe);

        marker = Func_080091b0((s32)subject[34], probe[0], probe[2]);
        if (marker == 255) {
            *(u16 *)(subject + 6) = (u16)heading;
            goto tail;
        }
        if (Func_080091a8((s32)subject[34], probe[0], probe[2])
                - *(s32 *)(subject + 12) > 0x80000) {
            *(u16 *)(subject + 6) = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        *(s32 *)(subject + 48) = 0x20000;
        *(s32 *)(subject + 52) = 0x1999;
        *(u16 *)(subject + 100) = 0;
        Func_08009150(subject, x, *(s32 *)(subject + 12), z);
        /* Same import as the probe above, two arguments here. */
        Func_08009080(subject, 2);
        Func_08009088(subject, 48);
        Func_08009158(subject);
        *(void **)(subject + 108) = (void *)Func_02001f24;

        goto advanceProbe;
continueProbe:
        if (Func_080091a8((s32)subject[34], probe[0], probe[2])
                - *(s32 *)(subject + 12) > 0x80000) {
            goto finishProbe;
        }
        *(s32 *)(subject + 48) = 0x20000;
        *(s32 *)(subject + 52) = 0x1999;
        x = probe[0];
        z = probe[2];
        Func_08009150(subject, probe[0], probe[1], probe[2]);
        Func_08009158(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        Func_08000128((s32)0x100000, heading, probe);
        marker = Func_080091b0((s32)subject[34], probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        *(s32 *)(subject + 48) = 0x20000;
        *(s32 *)(subject + 52) = 0x10000;
        Func_08009150(subject, x, *(s32 *)(subject + 12), z);
        Func_08009158(subject);
        Func_080000c0(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    *(void **)(subject + 108) = NULL;
    subject[90] |= 1;
    /* movs r3,#0x80 / lsls r3,#7 builds 0x4000. */
    *(s32 *)(subject + 52) = 0x4000;

tail:
    Func_080000c0(10);
    Func_0808a020();
}
