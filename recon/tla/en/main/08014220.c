/*
 * Draft: ResourceTable_CountFreeBlocks does not yet match; 4 halfwords differ from ☀️'s C, first at +0x2 (ldr r1, [pc, #24]).
 * Links as recon/tla/raw/08014220.s.
 */
#include "TYPES.H"

extern u8 ResourceBlockOwners[];

s32 ResourceTable_CountFreeBlocks(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 free_count = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}
