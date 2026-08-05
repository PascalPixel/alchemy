#include "types.h"

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d owner 0x02002eb8..0x02002f17 (0x60 = 96 bytes).
 *
 * Reviewed saved-link setup helper. Every service call and constant comes
 * directly from the local disassembly; it returns through pop/bx at
 * 0x02002f12..0x02002f14 and carries no pool tail.
 */











extern void Func_02006594();
extern void Func_020065c2();
extern void Func_020065ec();
extern void Func_02006668();
extern void Func_02006678();
extern void Func_02006680();
extern void Func_020060fc();
extern void Func_020066b6();
extern void Func_020066d4();
extern void Func_020065f0();
void Func_02002eb8(void) {
    Func_02006594();
    Func_020065c2(0, 0x8000, 0x4000);
    Func_020065ec(0, 0x68, 0x98);
    Func_02006668(0, 0x4000, 0x3C);
    Func_02006678(0x11, 0);
    Func_02006680(0x12, 0);
    Func_020060fc();
    Func_020066b6(-1, -1, -1, 0);
    Func_020066d4(1);
    Func_020065f0();
}
