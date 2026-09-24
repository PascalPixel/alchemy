#include "TYPES.H"

/*
 * Raribero house: choose which dialogue branch to play. If the party faces
 * the door (north) the alternate line for this door runs; otherwise a flag
 * decides between the two default lines.
 */

u16 *Func_020013e4(s32);
s32 Func_020013d8(s32);
void Func_020014e2(s32, s32);
void Func_02001482(s32);
void Func_02001492(s32);
void Func_0200149a(s32, s32);
void Func_020014aa(s32, s32);

void Dialogue_HandleAlternateFacingBranch(s32 no)
{
    u16 party_facing = (Func_020013e4(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Func_020014e2(34, no);
    } else if (Func_020013d8(0x9a7)) {
        Func_02001482(0x28f4);
        Func_0200149a(no, 0);
    } else {
        Func_02001492(0x26e9);
        Func_020014aa(no, 0);
    }
}
