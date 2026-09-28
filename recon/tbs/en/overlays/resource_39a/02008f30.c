/* NONMATCHING: resource_39a at 0x02008f30 (156 bytes with its pool), an
 * exported table selector between FIELD/COMMON/IMIRU_FUCHIN/TRANSITION.C and
 * LAYOUT.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers from its literal
 * pool and compares registers, as link-time scene symbols do; plain constants
 * compile to cmp with an immediate. The tables keep their listing addresses.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200abd8;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200ac08;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200ad1c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200ae24;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200b058;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200b130;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200b184;
    }
    return (s32)Data_0200abcc;
}
