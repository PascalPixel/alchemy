// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:CgbOscOff at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Register addresses and control flow were checked against this ROM region.
#include "types.h"

void Func_080fada0(u8 channel)
{
    volatile u8 *control;

    switch (channel) {
    case 1:
        control = (volatile u8 *)0x04000063;
        *control = 8;
        control += 2;
        break;
    case 2:
        control = (volatile u8 *)0x04000069;
        *control = 8;
        control += 4;
        break;
    case 3:
        *(volatile u8 *)0x04000070 = 0;
        return;
    default:
        control = (volatile u8 *)0x04000079;
        *control = 8;
        control += 4;
        break;
    }

    *control = 0x80;
}
