/* NONMATCHING: resource_39a at 0x02008050 (156 bytes with its pool), the
 * placement selector the entry veneers export, between
 * FIELD/COMMON/IMIRU_FUCHIN/OPENING.C and HOOKS.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers from its literal
 * pool and compares registers, as link-time scene symbols do; plain constants
 * compile to cmp with an immediate. The tables keep their listing addresses.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a4bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a504;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200a5f4;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200a63c;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200a6cc;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200a744;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200a7bc;
    }
    return (s32)Data_0200a48c;
}
