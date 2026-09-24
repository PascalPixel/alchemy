#include "TYPES.H"

/*
 * Raribero house: a face-toward cue branch. If the party faces the door
 * (north) the cue line plays; otherwise a flag picks one of the two default
 * lines.
 */

u16 *Func_020014b8(s32);
s32 Func_020014ac(s32);
void Func_020015c6(s32, s32);
void Func_02001556(s32);
void Func_02001566(s32);
void Func_0200156e(s32, s32);
void Func_0200157e(s32, s32);

void Dialogue_HandleFacingCueBranch(s32 no)
{
    u16 party_facing = (Func_020014b8(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Func_020015c6(11, no);
    } else if (Func_020014ac(0x9a7)) {
        Func_02001556(0x28f6);
        Func_0200156e(no, 0);
    } else {
        Func_02001566(0x26eb);
        Func_0200157e(no, 0);
    }
}
