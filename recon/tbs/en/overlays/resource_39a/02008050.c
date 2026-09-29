/* NONMATCHING: resource_39a at 0x02008050 (156 bytes with its pool), the
 * second entry veneer's table selector (the slot other field overlays fill
 * with their entrances), between FIELD/COMMON/IMIRU_FUCHIN/OPENING.C and
 * HOOKS.C, stays listing.
 *
 * Remaining difference: the reference loads the scene numbers 0x34, 0x3e,
 * 0x3f, 0x40, 0x41, 0x42 and 0x43 from its literal pool and compares
 * registers, as link-time scene numbers do; plain constants compile to cmp
 * with an immediate. With the seven numbers as link-time values this body
 * compiles to the reference exactly. The tables it returns (0x0200a4bc,
 * 0x0200a504, 0x0200a5f4, 0x0200a63c, 0x0200a6cc, 0x0200a744, 0x0200a7bc and
 * the default 0x0200a48c) also need labels on their listing rows.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

extern s32 gImiruFuchinEntrances[];
extern s32 gImiruFuchinRoom1Entrances[];
extern s32 gImiruFuchinRoom2Entrances[];
extern s32 gImiruFuchinRoom3Entrances[];
extern s32 gImiruFuchinRoom4Entrances[];
extern s32 gImiruFuchinRoom5Entrances[];
extern s32 gImiruFuchinRoom6Entrances[];
extern s32 gImiruFuchinRoom7Entrances[];

s32 *SceneData_SelectTableBySceneId(void)
{
    s16 scene = gGameState.scene;

    if (scene == 0x34) {
        return gImiruFuchinRoom1Entrances;
    }
    if (scene == 0x3e) {
        return gImiruFuchinRoom2Entrances;
    }
    if (scene == 0x3f) {
        return gImiruFuchinRoom3Entrances;
    }
    if (scene == 0x40) {
        return gImiruFuchinRoom4Entrances;
    }
    if (scene == 0x41) {
        return gImiruFuchinRoom5Entrances;
    }
    if (scene == 0x42) {
        return gImiruFuchinRoom6Entrances;
    }
    if (scene == 0x43) {
        return gImiruFuchinRoom7Entrances;
    }
    return gImiruFuchinEntrances;
}
