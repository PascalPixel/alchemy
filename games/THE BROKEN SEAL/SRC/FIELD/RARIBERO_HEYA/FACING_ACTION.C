#include "TYPES.H"

/*
 * Raribero house: a face-toward action. If the party faces the door (north)
 * the door line plays; otherwise a flag picks one of the two default lines.
 */

u16 *Func_02001450(s32);
s32 Func_0200143c(s32);
void Func_02001554(s32);
void Func_020014e6(s32);
void Func_02001504(s32);
void Func_020014fe(s32, s32);
void Func_0200151c(s32, s32);

void Dialogue_HandleFacingAction(s32 no)
{
    u16 party_facing = (Func_02001450(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Func_02001554(no);
    } else if (Func_0200143c(0x9a7)) {
        Func_020014e6(0x28fc);
        Func_020014fe(no, 0);
    } else {
        Func_02001504(0x26f6);
        Func_0200151c(no, 0);
    }
}
