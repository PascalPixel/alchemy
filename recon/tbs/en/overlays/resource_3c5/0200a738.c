/* Draft of resource_3c5 0x0200a738..0x0200a7a0 (104 bytes with pool),
 * SceneActor_RunSlotZeroFacingCheck; the listing keeps the rows. Remaining
 * difference: the reference loads the game state's base and the offset 498
 * separately, as indexing a byte array of unknown extent does; the struct
 * view folds them into one pool word, and this local pointer view keeps the
 * size but allocates differently (30 bytes differ from +0x0). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

void SceneActor_RunSlotZeroFacingCheck(void)
{
    struct Obj *p = Actor_Get(ACTOR_PARTY_LEADER);
    s32 x = BabiIriguchi_FindActorAhead();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;
    u8 *state = (u8 *)&gGameState;

    if (state[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = battle_owner_69();
        }
        if (m == 0x4000) {
            r = FieldEffect_UpdateGridPlacement();
        }
    }
    if (r != 0) {
        if (state[498] != 1) {
            SceneActor_PushObjectAheadIfLevel();
        }
    }
}
