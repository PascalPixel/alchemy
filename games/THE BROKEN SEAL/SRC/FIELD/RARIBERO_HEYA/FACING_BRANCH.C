#include "TYPES.H"

/*
 * Raribero house: a face-toward branch. If the party faces the door (north)
 * the door line plays; otherwise a flag picks one of the two default lines.
 */


u16 *Func_02001378(s32);
s32 Func_0200136c(s32);
void Func_02001476(s32, s32);
void Func_02001416(s32);
void Func_02001426(s32);
void Func_0200142e(s32, s32);
void Func_0200143e(s32, s32);

void Dialogue_HandleFacingBranch(s32 no)
{
    u16 party_facing = (Func_02001378(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Func_02001476(33, no);
    } else if (Func_0200136c(0x9a7)) {
        Func_02001416(0x28f2);
        Func_0200142e(no, 0);
    } else {
        Func_02001426(0x26e7);
        Func_0200143e(no, 0);
    }
}
