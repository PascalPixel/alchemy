/* Draft of resource_399 0x020098c4 (SceneState_UpdateZoneFlagsFromActorZero), built with
 * games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMIRU.H.
 * Remaining difference: its three flag-set calls reach one veneer; spelled with that one name, the compiler merges their tails (a branch before the lsls that builds 0x200) where the ROM keeps them apart. The original spelled them through two per-site aliases.
 * The listing keeps these rows. */
#include "IMIRU.H"

void SceneState_UpdateZoneFlagsFromActorZero(void)
{
    T_020018c4 *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = ((T *)Engine_ActorGet(0));
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = GameFlag_IsSet(g);
    if (r != 0)
        return;
    (*(State **)&gMapWork)->unk17 = r;
    Engine_GameFlagSet(g);
    GameFlag_Clear(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)&gMapWork;
        st->unk17 = 0;
        Engine_GameFlagSet(g);
        GameFlag_Clear(h);
        return;
    }
    r = GameFlag_IsSet(h);
    if (r != 0)
        return;
    st = *(State **)&gMapWork;
    st->unk17 = 1;
    Engine_GameFlagSet(h);
    GameFlag_Clear(g);
    return;
}
