#include "types.h"

/*
 * resource_3bb owner at 0x02002450, 1,264 bytes. This is the independently
 * address-mapped sibling of resource_3bc:0x02002ee8 and the callback
 * installed by resource_3bb_c_02002940.c at 0x0200a451.  It starts with the
 * saved-register/frame prologue at 0x02002450, returns through the single
 * unwind at 0x02002904-0x02002914, and includes the alignment halfword and
 * ten-word trailing pool at 0x02002916-0x0200293f.  The next owner begins at
 * 0x02002940.
 *
 * The long body has one queue-consumption loop (the command values 0x1000,
 * 0x2000, 0x3000, 0x4000, 0x7fff and -1 select the sibling state cells),
 * three fixed-point interpolation passes, and a mode-selected sprite-output
 * tail.  The inline literal runs at 0x020025b0-0x020026e3 are branch-skipped
 * data, not separate owners.  The 10 resolved calls are retained in their
 * machine order: the -1 queue arm removes the callback and releases its
 * palette slot; the three interpolation passes use the relocated quotient
 * helper; the packed state is normalized once; and each of the four output
 * modes submits a record through Func_080001e8.
 *
 * The addresses below are the established resource_3bb state cells and the
 * two hardware/global queues visible in the pool.  The reconstruction keeps
 * the original field widths and direct memory effects but uses ordinary C
 * loops and expressions throughout; no inline assembly, fixed-register
 * binding, or translator escape is needed.
 */





/* This import is used with the four historical arities below. */


extern void Func_020062f0(void *callback);
extern void Func_0200634a(s32 slot);
extern s32 Func_02006322(s32 delta, s32 duration);
extern s32 Func_020063be(s32 delta, s32 duration);
extern s32 Func_0200641e(s32 delta, s32 duration);
extern s32 Func_020064d6(s32 packed);
extern void Func_0200659a();
extern void Func_02006616();
extern void Func_02006674();
extern void Func_020066c8();
void Func_02002450(void)
{
    volatile s16 *paletteSlot = (volatile s16 *)0x0200ca1c;
    volatile s16 *stateCell = (volatile s16 *)0x0200cc3c;
    volatile u16 *paletteTable = (volatile u16 *)0x03001b10;
    volatile u32 *queueCell = (volatile u32 *)0x0200cc40;
    volatile u16 *commandA = (volatile u16 *)0x0200cc0c;
    volatile u16 *commandB = (volatile u16 *)0x0200cc10;
    volatile u16 *commandC = (volatile u16 *)0x0200cc18;
    volatile u16 *commandD = (volatile u16 *)0x0200cc34;
    volatile u16 *commandE = (volatile u16 *)0x0200cc38;
    volatile u16 *commandF = (volatile u16 *)0x0200cc90;
    volatile u16 *commandG = (volatile u16 *)0x0200cc98;
    volatile u16 *countA = (volatile u16 *)0x0200cc9c;
    volatile u16 *countB = (volatile u16 *)0x0200cc1c;
    volatile u16 *durationA = (volatile u16 *)0x0200cbf4;
    volatile u16 *durationB = (volatile u16 *)0x0200cc48;
    volatile u16 *durationC = (volatile u16 *)0x0200cc0c;
    volatile u32 *outputWords = (volatile u32 *)0x0200cc60;
    volatile u16 *spriteCount = (volatile u16 *)0x02002090;
    volatile u16 *regIme = (volatile u16 *)0x04000208;
    u16 *queue;
    u32 *write;
    s32 tile;
    s32 phase;
    s32 first;
    s32 second;
    s32 third;
    s32 packed;
    s32 i;

    tile = paletteTable[((s32)*paletteSlot << 2) / 2 + 1] >> 5;
    phase = *stateCell;
    write = (u32 *)outputWords;

    /* A zero phase drains commands until one of the state arms takes over. */
    if (phase == 0) {
        queue = (u16 *)(*queueCell);
        for (;;) {
            s32 command = (s16)*queue++;

            *queueCell = (u32)queue;

            if (command == -1) {
                Func_020062f0((void *)0x0200a451);
                Func_0200634a(*paletteSlot);
                return;
            }

            if (command == 0x4000) {
                *commandB = (u16)((s16)queue[0] << 8);
                *commandG = queue[1];
                *commandA = (u16)phase;
                queue += 2;
                *queueCell = (u32)queue;
            } else if (command == 0x3000) {
                *(volatile u16 *)0x0200cc90 = *commandG;
                *commandG = queue[0];
                *commandA = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200cc1c = (u16)phase;
            } else if (command == 0x1000) {
                *(volatile u16 *)0x0200cc08 = *commandC;
                *commandC = queue[0];
                *durationA = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200cc9c = (u16)phase;
            } else if (command == 0x2000) {
                *commandE = *commandD;
                *commandD = queue[0];
                *durationB = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200cc24 = (u16)phase;
            } else if (command == 0x7fff) {
                *stateCell = (u16)queue[0];
                queue++;
                *queueCell = (u32)queue;
                break;
            } else {
                /* Unknown commands consume one halfword and are retried. */
                continue;
            }

            phase = *stateCell;
            if (phase != 0)
                break;
        }
        phase = *stateCell;
    }

    /* Interpolate the three state channels, preserving signed 16-bit fields. */
    phase = (s16)(phase - 1);
    *stateCell = (u16)phase;

    if (*durationA == 0) {
        first = (s16)*commandC;
    } else {
        s32 progress = (s16)(++*countA);
        s32 target = (s16)*commandC;
        s32 start = (s16)*(volatile u16 *)0x0200cc08;

        first = start + Func_02006322(progress * (target - start),
                                      (s16)*durationA);
        if (progress >= (s16)*durationA)
            *durationA = 0;
    }

    if (*durationB == 0) {
        second = (s16)*commandD;
    } else {
        s32 progress = (s16)(++*countA);
        s32 target = (s16)*commandD;
        s32 start = (s16)*commandE;

        second = start + Func_020063be(progress * (target - start),
                                       (s16)*durationB);
        if (progress >= (s16)*durationB)
            *durationB = 0;
    }

    if (*durationC == 0) {
        third = (s16)*commandG;
    } else {
        s32 progress = (s16)(++*countB);
        s32 target = (s16)*commandG;
        s32 start = (s16)*commandF;

        third = start + Func_0200641e(progress * (target - start),
                                      (s16)*durationC);
        if (progress >= (s16)*durationC)
            *durationC = 0;
    }

    packed = (first & 0xffff) | ((second & 0xffff) << 16);
    packed = Func_020064d6(packed);
    *(volatile s32 *)0x0200cc10 += third;

    /* Mode 1 emits four records. */
    if (*(volatile s16 *)0x0200cc30 == 1) {
        for (i = 0; i <= 3; i++) {
            s32 y = third + ((first * (i * 32 - 48)) >> 8);

            write[0] = 0;
            write[1] = ((u32)(y + 0x58) & 0x1ff) << 16;
            write[1] |= 0x80004038u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)(tile + i * 8);
            if (y + 0x98 <= 0x12f)
                Func_0200659a((void *)write, 0xec, write + 3, 0x80004000);
            write += 3;
        }
    /* Mode 3 emits two narrower records. */
    } else if (*(volatile s16 *)0x0200cc30 == 3) {
        for (i = 0; i <= 1; i++) {
            s32 y = third + ((first * (i * 32 - 16)) >> 8);

            write[0] = 0;
            write[1] = ((u32)(y + 0x58) & 0x1ff) << 16;
            write[1] |= 0x80004030u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)(tile + i * 8);
            if (y + 0x98 <= 0x12f)
                Func_02006616((void *)write, 0xec, write + 3, 0x80004000);
            write += 3;
        }
    /* Mode 4 emits a single record if its 0x98/0x12f bounds permit it. */
    } else if (*(volatile s16 *)0x0200cc30 == 4) {
        s32 y = third;

        if (y + 0x78 < 0x130) {
            write[0] = 0;
            write[1] = ((u32)(y + 0x38) & 0x1ff) << 16;
            write[1] |= 0x80004030u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)tile;
            Func_02006674((void *)write, 0xec, write + 3);
            write += 3;
        }
    /* All other modes use the single centered record arm. */
    } else {
        write[0] = 0;
        write[1] = ((u32)(third + 0x98) & 0x1ff) << 16;
        write[1] |= 0x80000030u | ((u32)packed << 25);
        write[2] = 0x00f40000u | (u32)tile;
        Func_020066c8((void *)write, 0xec);
        write += 3;
    }

    /* Two interrupt-masked records are appended to the shared sprite queue. */
    {
        u16 savedIme = *regIme;
        u16 count = *spriteCount;

        *regIme = (u16)(u32)regIme;
        if (count <= 31) {
            u32 *entry = (u32 *)((u8 *)spriteCount + 4 + count * 12);

            *spriteCount = (u16)(count + 1);
            entry[0] = 0xfc << 6;
            entry[1] = 0x04000050;
            entry[2] = 0x80000;
        }
        *regIme = savedIme;
    }

    {
        u16 savedIme = *regIme;
        u16 count = *spriteCount;

        *regIme = (u16)(u32)regIme;
        if (count <= 31) {
            u32 *entry = (u32 *)((u8 *)spriteCount + 4 + count * 12);
            s32 depth = 16 - second;

            *spriteCount = (u16)(count + 1);
            entry[0] = ((u32)depth << 8) | (u32)second;
            entry[1] = 0x04000052;
            entry[2] = 0x80000;
        }
        *regIme = savedIme;
    }
}
