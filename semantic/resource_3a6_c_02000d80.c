#include "types.h"

/*
 * resource_3a6 owner at 0x02000d80, 46 bytes of code (0x02000d80-0x02000dad)
 * plus the seven pool words at 0x02000db0-0x02000dcb.
 *
 * Complete owner: `push {lr}` at 0x02000d80 and `pop {r1} / bx r1` at
 * 0x02000daa.  The popped register is the return address and r0 is live out on
 * every path, so the owner takes no arguments and returns a pointer.
 *
 * A pure selector: it reads the shared RAM table's entry 224 as an s16 and maps
 * it to one of three in-image tables.  Data_02000240 is the same shared `s16[]`
 * table resource_3ba_c_02000a3c.c documents, read there at index 225.
 *
 * The three results 0x0200a2c4, 0x0200a39c and 0x0200a234 are EVEN, so under
 * the 0x02008000 overlay link base they are data at file offsets 0x22c4,
 * 0x239c and 0x2234 rather than function pointers.
 *
 * Note the shape of the test chain: 0x5d branches to the same 0x02000da8 that
 * the chain falls through to, so 0x5d is spelled as an early exit to the
 * default rather than as a case of its own.  Writing it as a switch or as a
 * three-way if/else if/else would fold that branch away.
 */

extern s16 Data_02000240[];

extern u8 Data_0200a2c4[];
extern u8 Data_0200a39c[];
extern u8 Data_0200a234[];

u8 *Func_02000d80(void)
{
    s32 scene;

    scene = Data_02000240[224];

    if (scene != 0x5d) {
        if (scene == 0x5e) return Data_0200a2c4;
        if (scene == 0x5f) return Data_0200a39c;
    }
    return Data_0200a234;
}
