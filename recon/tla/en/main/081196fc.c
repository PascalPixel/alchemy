/*
 * Draft: BattleCamera_Create: allocates heap slot 48's 0x4c-byte camera and
 * sets its defaults. Score 23: the listing stores the 12..20 vector through a
 * temporary pointer first and builds 0x2800/0x5000 inline.
 */
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "RUNTIME_MEM.H"

struct SaveSlotRecord {
    u8 unknown_00[0x1c];
    u8 used;
    u8 unknown_1d[0x23];
};

struct SaveWork {
    u8 unknown_0000[0x303c];
    struct SaveSlotRecord slots[3];
};


void BattleCamera_Create(void)
{
    s32 *camera = Runtime_AllocateBlock(48, 0x4c);

    camera[3] = 0;
    camera[4] = 0x400000;
    camera[5] = 0;
    camera[1] = 0x5a0000;
    ((s16 *)camera)[27] = 0x2800;
    ((s16 *)camera)[26] = 0x5000;
    camera[0] = 0;
    camera[2] = 0x400000;
    camera[8] = 0x1000000;
}
