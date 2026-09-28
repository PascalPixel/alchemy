/* Draft of resource_3c8 0x02008e04..0x02008e7c (120 bytes with pool),
 * SceneData_SelectTableBySceneB5ToBa; the listing keeps the rows. Remaining
 * difference: the reference loads the scene numbers 0xb5 and 0xb7..0xba from
 * its literal pool and compares registers, as link-time scene symbols do;
 * plain constants compile to cmp with an immediate. The tables keep their
 * listing addresses. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"
extern u8 Data_0200dd68[];
extern u8 Data_0200ddc8[];
extern u8 Data_0200e020[];
extern u8 Data_0200e230[];
extern u8 Data_0200e350[];
extern u8 Data_0200e548[];

/* Contiguous unnamed leaf-owner run for resource_3c8. */
s32 SceneData_SelectTableBySceneB5ToBa(void)
{
    s16 v = gGameState.scene;

    if (v == 0xb5) {
        return (s32)Data_0200dd68;
    }
    if (v == 0xb7) {
        return (s32)Data_0200e020;
    }
    if (v == 0xb8) {
        return (s32)Data_0200e230;
    }
    if (v == 0xb9) {
        return (s32)Data_0200e350;
    }
    if (v == 0xba) {
        return (s32)Data_0200e548;
    }
    return (s32)Data_0200ddc8;
}
