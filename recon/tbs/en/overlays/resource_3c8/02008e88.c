/* Draft of resource_3c8 0x02008e88..0x02008f1c (148 bytes with pool),
 * SceneData_SelectTableBySceneAndApply; the listing keeps the rows. Remaining
 * difference: the reference loads the scene numbers 0xb5..0xba from its
 * literal pool and compares registers, as link-time scene symbols do; plain
 * constants compile to cmp with an immediate. The tables keep their listing
 * addresses. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    extern s16 Data_02000240[];

    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = Data_02000240;
    v = tbl[224];
    if (v == 0xb5) {
        return Data_0200e904;
    }
    if (v == 0xb6) {
        ret = Data_0200e9c4;
    } else if (v == 0xb7) {
        ret = Data_0200eb74;
    } else if (v == 0xb8) {
        ret = Data_0200ec04;
    } else if (v == 0xb9) {
        ret = Data_0200ec64;
    } else if (v == 0xba) {
        ret = Data_0200ecf4;
    } else {
        goto no_match;
    }
    SceneEvents_UpdateInView(ret);
    return ret;

no_match:
    return Data_0200e8ec;
}
