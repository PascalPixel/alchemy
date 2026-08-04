#include "types.h"

/*
 * resource_3ba command/interpolation renderer at 0x020021b8, complete
 * 1,264-byte owner through its interior literal runs, sole return, alignment
 * and ten-word trailing pool before 0x020026a8. Control flow is the reviewed
 * resource_3bb:2450/resource_3bc:2ee8 family; every state address and all ten
 * calls are independently resolved for this overlay.
 */

extern void Func_080000d8(void *callback);
extern void Func_080001b8(s32 slot);
extern s32 Func_03000380(s32 delta, s32 duration);
extern s32 Func_080001e0(s32 packed);
/* This import is used with the four historical arities below. */
extern void Func_080001e8();

void Func_020021b8(void)
{
    volatile s16 *paletteSlot = (volatile s16 *)0x0200c57c;
    volatile s16 *stateCell = (volatile s16 *)0x0200c79c;
    volatile u16 *paletteTable = (volatile u16 *)0x03001b10;
    volatile u32 *queueCell = (volatile u32 *)0x0200c7a0;
    volatile u16 *commandA = (volatile u16 *)0x0200c76c;
    volatile u16 *commandB = (volatile u16 *)0x0200c770;
    volatile u16 *commandC = (volatile u16 *)0x0200c778;
    volatile u16 *commandD = (volatile u16 *)0x0200c794;
    volatile u16 *commandE = (volatile u16 *)0x0200c798;
    volatile u16 *commandF = (volatile u16 *)0x0200c7f0;
    volatile u16 *commandG = (volatile u16 *)0x0200c7f8;
    volatile u16 *countA = (volatile u16 *)0x0200c7fc;
    volatile u16 *countB = (volatile u16 *)0x0200c77c;
    volatile u16 *durationA = (volatile u16 *)0x0200c754;
    volatile u16 *durationB = (volatile u16 *)0x0200c7a8;
    volatile u16 *durationC = (volatile u16 *)0x0200c76c;
    volatile u32 *outputWords = (volatile u32 *)0x0200c7c0;
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
                Func_080000d8((void *)0x0200a1b9);
                Func_080001b8(*paletteSlot);
                return;
            }

            if (command == 0x4000) {
                *commandB = (u16)((s16)queue[0] << 8);
                *commandG = queue[1];
                *commandA = (u16)phase;
                queue += 2;
                *queueCell = (u32)queue;
            } else if (command == 0x3000) {
                *(volatile u16 *)0x0200c7f0 = *commandG;
                *commandG = queue[0];
                *commandA = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200c77c = (u16)phase;
            } else if (command == 0x1000) {
                *(volatile u16 *)0x0200c768 = *commandC;
                *commandC = queue[0];
                *durationA = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200c7fc = (u16)phase;
            } else if (command == 0x2000) {
                *commandE = *commandD;
                *commandD = queue[0];
                *durationB = queue[1];
                queue += 2;
                *queueCell = (u32)queue;
                *(volatile u16 *)0x0200c784 = (u16)phase;
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
        s32 start = (s16)*(volatile u16 *)0x0200c768;

        first = start + Func_03000380(progress * (target - start),
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

        second = start + Func_03000380(progress * (target - start),
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

        third = start + Func_03000380(progress * (target - start),
                                      (s16)*durationC);
        if (progress >= (s16)*durationC)
            *durationC = 0;
    }

    packed = (first & 0xffff) | ((second & 0xffff) << 16);
    packed = Func_080001e0(packed);
    *(volatile s32 *)0x0200c770 += third;

    /* Mode 1 emits four records. */
    if (*(volatile s16 *)0x0200c790 == 1) {
        for (i = 0; i <= 3; i++) {
            s32 y = third + ((first * (i * 32 - 48)) >> 8);

            write[0] = 0;
            write[1] = ((u32)(y + 0x58) & 0x1ff) << 16;
            write[1] |= 0x80004038u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)(tile + i * 8);
            if (y + 0x98 <= 0x12f)
                Func_080001e8((void *)write, 0xec, write + 3, 0x80004000);
            write += 3;
        }
    /* Mode 3 emits two narrower records. */
    } else if (*(volatile s16 *)0x0200c790 == 3) {
        for (i = 0; i <= 1; i++) {
            s32 y = third + ((first * (i * 32 - 16)) >> 8);

            write[0] = 0;
            write[1] = ((u32)(y + 0x58) & 0x1ff) << 16;
            write[1] |= 0x80004030u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)(tile + i * 8);
            if (y + 0x98 <= 0x12f)
                Func_080001e8((void *)write, 0xec, write + 3, 0x80004000);
            write += 3;
        }
    /* Mode 4 emits a single record if its 0x98/0x12f bounds permit it. */
    } else if (*(volatile s16 *)0x0200c790 == 4) {
        s32 y = third;

        if (y + 0x78 < 0x130) {
            write[0] = 0;
            write[1] = ((u32)(y + 0x38) & 0x1ff) << 16;
            write[1] |= 0x80004030u | ((u32)packed << 25);
            write[2] = 0x00f40000u | (u32)tile;
            Func_080001e8((void *)write, 0xec, write + 3);
            write += 3;
        }
    /* All other modes use the single centered record arm. */
    } else {
        write[0] = 0;
        write[1] = ((u32)(third + 0x98) & 0x1ff) << 16;
        write[1] |= 0x80000030u | ((u32)packed << 25);
        write[2] = 0x00f40000u | (u32)tile;
        Func_080001e8((void *)write, 0xec);
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
