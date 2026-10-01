/* NONMATCHING: Japanese Tolbi proximity flag, 2026-10-01.
 * The complete callback retains the localized scene-record layout.
 * Japanese moves its widening flag from ea4 to f34 (two pool-byte differences).
 */
#include "games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H"

s32 SceneActor_UpdatePartnerProximity(u8 *self)
{
    u8 **globals = (u8 **)gWindowWork;
    u8 *scene = globals[0];
    u8 *work = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * The branch must stay two calls: as a conditional expression the
     * selector folds into arithmetic on the bit instead.
     */
    if ((*flags & 1) != 0) {
        partner = Object_GetById(17);
    } else {
        partner = Object_GetById(16);
    }
    if (SceneActor_UpdatePlayerProximity(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Object_GetById(0);

    /*
     * Widen the test when the scene counter at work + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_UpdatePlayerProximity(self, player, range, force);
    return 0;
}
