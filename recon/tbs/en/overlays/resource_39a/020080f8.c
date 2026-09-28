/* NONMATCHING: resource_39a at 0x020080f8 (136 bytes with its pool), an
 * exported table selector between FIELD/COMMON/IMIRU_FUCHIN/HOOKS.C and
 * TRANSITION.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers from its literal
 * pool and compares registers, as link-time scene symbols do; plain constants
 * compile to cmp with an immediate. The tables keep their listing addresses.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a9bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a9ec;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200aa4c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200aac4;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200ab3c;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200ab9c;
    }
    return (s32)Data_0200a9a4;
}
