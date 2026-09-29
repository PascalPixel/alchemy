/* NONMATCHING: resource_39a at 0x02008f30 (156 bytes with its pool), the
 * fifth entry veneer's table selector (the slot other field overlays fill
 * with their events; the cave's trigger handlers are installed there),
 * between FIELD/COMMON/IMIRU_FUCHIN/TRANSITION.C and LAYOUT.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers 0x34, 0x3e,
 * 0x3f, 0x40, 0x41, 0x42 and 0x43 from its literal pool and compares
 * registers, as link-time scene numbers do; plain constants compile to cmp
 * with an immediate. With the seven numbers as link-time values this body
 * compiles to the reference exactly. The tables it returns (0x0200abd8,
 * 0x0200ac08, 0x0200ad1c, 0x0200ae24, 0x0200b058, 0x0200b130, 0x0200b184 and
 * the default 0x0200abcc) also need labels on their listing rows.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

extern s32 gImiruFuchinEvents[];
extern s32 gImiruFuchinRoom1Events[];
extern s32 gImiruFuchinRoom2Events[];
extern s32 gImiruFuchinRoom3Events[];
extern s32 gImiruFuchinRoom4Events[];
extern s32 gImiruFuchinRoom5Events[];
extern s32 gImiruFuchinRoom6Events[];
extern s32 gImiruFuchinRoom7Events[];

s32 *SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 scene = gGameState.scene;

    if (scene == 0x34) {
        return gImiruFuchinRoom1Events;
    }
    if (scene == 0x3e) {
        return gImiruFuchinRoom2Events;
    }
    if (scene == 0x3f) {
        return gImiruFuchinRoom3Events;
    }
    if (scene == 0x40) {
        return gImiruFuchinRoom4Events;
    }
    if (scene == 0x41) {
        return gImiruFuchinRoom5Events;
    }
    if (scene == 0x42) {
        return gImiruFuchinRoom6Events;
    }
    if (scene == 0x43) {
        return gImiruFuchinRoom7Events;
    }
    return gImiruFuchinEvents;
}
