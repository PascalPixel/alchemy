/* NONMATCHING: resource_398 at 0x020080a0 and 0x020080f4 (84 bytes each with
 * their pools), between FIELD/BIRIBINO_DOU/EVENT_TABLE.C and REGION.C, stay
 * listing.
 *
 * Remaining difference: the reference loads the scene numbers 0x31, 0x30
 * and 0x2f from its literal pool and compares registers, as link-time scene
 * symbols do; plain constants compile to cmp with an immediate. The tables
 * keep their listing addresses.
 */

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/REGION.H"

extern u8 Data_02008c2c[];
extern u8 Data_02008c5c[];
extern u8 Data_02008cbc[];
extern u8 Data_02008c14[];
extern u8 Data_02008ea8[];
extern u8 Data_02008efc[];
extern u8 Data_02008f80[];
extern u8 Data_02008e9c[];

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0x31) {
        return (s32)Data_02008c2c;
    }
    if (selector == 0x30) {
        return (s32)Data_02008c5c;
    }
    if (selector == 0x2f) {
        return (s32)Data_02008cbc;
    }
    return (s32)Data_02008c14;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0x31) {
        return (s32)Data_02008ea8;
    }
    if (selector == 0x30) {
        return (s32)Data_02008efc;
    }
    if (selector == 0x2f) {
        return (s32)Data_02008f80;
    }
    return (s32)Data_02008e9c;
}
