/* NONMATCHING: resource_398 at 0x02008040 (84 bytes with its pool), between
 * FIELD/BIRIBINO_DOU/HIDE_ACTOR.C and EVENT_TABLE.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers 0x31, 0x30
 * and 0x2f from its literal pool and compares registers, as link-time scene
 * symbols do; plain constants compile to cmp with an immediate. The tables
 * keep their listing addresses.
 */

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/REGION.H"

extern u8 Data_020089ec[];
extern u8 Data_02008a64[];
extern u8 Data_02008b24[];
extern u8 Data_020089bc[];

s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0x31) {
        return (s32)Data_020089ec;
    }
    if (selector == 0x30) {
        return (s32)Data_02008a64;
    }
    if (selector == 0x2f) {
        return (s32)Data_02008b24;
    }
    return (s32)Data_020089bc;
}
