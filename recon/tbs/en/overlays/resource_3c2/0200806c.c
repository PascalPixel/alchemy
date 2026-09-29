/* Draft of resource_3c2 0x0200806c..0x02008100 (148 bytes with pool),
 * Dialogue_HandleFacingChoice; the listing keeps the rows. Remaining
 * difference: the reference loads message 0x261c from its literal pool as a
 * link-time value and derives the answers from it; the integer message is
 * scheduled differently (156 bytes, 52 differ from +0x0). The Actor_ call
 * names are provisional; DIALOGUE.C's facing branches name the same imports. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/SUHARA.H"

void Dialogue_HandleFacingChoice(s32 no)
{
    u16 facing = (Actor_Run(ACTOR_PARTY_LEADER)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Actor_Apply(31, no);
    } else if (Actor_unk2(0x96f)) {
        s32 msg = 0x261c;
        Actor_Do(msg);
        Actor_Apply2(no, 0);
        if (Actor_Apply3(ACTOR_PARTY_LEADER, 0) == 0) {
            Actor_unk2_2(10);
            Actor_unk3_2(msg + 1);
        } else {
            Actor_unk4_2(msg + 2);
        }
        Actor_Apply4(no, 0);
    } else {
        Actor_unk5_2(0x25cf);
        Actor_Apply5(no, 0);
    }
}
