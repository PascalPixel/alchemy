/* Draft of resource_3c8 0x0200af8c..0x0200b00c (128 bytes with pool),
 * SceneData_SelectTableBySceneId; the listing keeps the rows. Remaining
 * difference: the reference loads the scene numbers 0xb5..0xba from its
 * literal pool and compares registers, as link-time scene symbols do; plain
 * constants compile to cmp with an immediate. The tables keep their listing
 * addresses. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"
extern u8 Data_0200ee44[];
extern u8 Data_0200ef1c[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];

u8 *SceneData_SelectTableBySceneId(void)
{
    extern u8 Data_02000240[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == 0xb5) {
        return Data_0200ee44;
    }
    if (v == 0xb6) {
        return Data_0200ef1c;
    }
    if (v == 0xb7) {
        return Data_0200f120;
    }
    if (v == 0xb8) {
        return Data_0200f300;
    }
    if (v == 0xb9) {
        return Data_0200f3b4;
    }
    if (v == 0xba) {
        return Data_0200f4f8;
    }
    return Data_0200ef1c;
}
