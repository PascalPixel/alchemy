/* Draft of resource_3c2 0x020081d4..0x0200821c (72 bytes with pool),
 * Scene_RunActorCueBranch; the listing keeps the rows. Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Actor_unk10_2,
 * Actor_Apply11, Actor_Apply12, Actor_unk11_2, Actor_unk12_2, Actor_unk13_2,
 * ...). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/SUHARA.H"
extern u8 MsgSuharaSupposeFolkBlown[];

void Scene_RunActorCueBranch(s32 obj)
{
    s32 cue = (s32)MsgSuharaSupposeFolkBlown;
    Actor_unk10_2(cue);
    Actor_Apply11(obj, 0);
    if (Actor_Apply12(ACTOR_PARTY_LEADER, 0) == 0) {
        Actor_unk11_2(10);
        Actor_unk12_2(cue + 1);
    } else {
        Actor_unk13_2(cue + 2);
    }
    Actor_Apply13(obj, 0);
}
