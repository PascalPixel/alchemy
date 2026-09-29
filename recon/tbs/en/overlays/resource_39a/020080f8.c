/* NONMATCHING: resource_39a at 0x020080f8 (136 bytes with its pool), the
 * fourth entry veneer's table selector (the slot other field overlays fill
 * with their placements), between FIELD/COMMON/IMIRU_FUCHIN/HOOKS.C and
 * TRANSITION.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers 0x34, 0x3e,
 * 0x3f, 0x40, 0x41 and 0x43 from its literal pool and compares registers, as
 * link-time scene numbers do; plain constants compile to cmp with an
 * immediate. With the six numbers as link-time values this body compiles to
 * the reference exactly. The tables it returns (0x0200a9bc, 0x0200a9ec,
 * 0x0200aa4c, 0x0200aac4, 0x0200ab3c, 0x0200ab9c and the default 0x0200a9a4)
 * also need labels on their listing rows.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

extern s32 gImiruFuchinPlacements[];
extern s32 gImiruFuchinRoom1Placements[];
extern s32 gImiruFuchinRoom2Placements[];
extern s32 gImiruFuchinRoom3Placements[];
extern s32 gImiruFuchinRoom4Placements[];
extern s32 gImiruFuchinRoom5Placements[];
extern s32 gImiruFuchinRoom7Placements[];

s32 *SceneData_SelectDataByRuntimeSelector(void)
{
    s16 scene = gGameState.scene;

    if (scene == 0x34) {
        return gImiruFuchinRoom1Placements;
    }
    if (scene == 0x3e) {
        return gImiruFuchinRoom2Placements;
    }
    if (scene == 0x3f) {
        return gImiruFuchinRoom3Placements;
    }
    if (scene == 0x40) {
        return gImiruFuchinRoom4Placements;
    }
    if (scene == 0x41) {
        return gImiruFuchinRoom5Placements;
    }
    if (scene == 0x43) {
        return gImiruFuchinRoom7Placements;
    }
    return gImiruFuchinPlacements;
}
