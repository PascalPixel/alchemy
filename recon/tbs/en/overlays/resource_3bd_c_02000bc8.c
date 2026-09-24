/* NONMATCHING: 96 of 100 bytes, 18 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 49 halfwords: the reference loads the map id twice, once zero-extended into r1 for the second test and once sign-extended for the first; ours shares one load A u16 counts[] view of the row still folds (s16)map into the first compare's ldrsh (gcse); a volatile read keeps two loads but folds the base into one pool constant and extends right after the load. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows gGameStateRows;
extern u8 Data_00000092[], Data_00000097[];

void Local_02000bc8(void)
{
    u16 map = gGameStateRows.halves[224][0];

    if (gGameStateRows.halves[224][0] == (s32)Data_00000092)
    {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    }
    if ((s16)map == (s32)Data_00000097) {
        Engine_ActorSetChildValue(16, 1);
        Engine_ActorSetChildValue(17, 4);
        Engine_ActorSetChildValue(18, 11);
        Engine_ActorSetChildValue(19, 2);
        Engine_ActorSetChildValue(20, 3);
    }
}
