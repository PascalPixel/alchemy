/* Draft of resource_37f 0x, from
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.C.
 * Remaining difference: the ROM loads scene or message numbers from the
 * literal pool, as link-time symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "SORU.H"

s32 SceneData_SelectFormationTable(void)
{
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000013) {
        return (s32)Data_0200a2e4;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000010) {
            if (Data_02000240_t[225][0] >= lo) {
                if (Data_02000240_t[225][0] > 13) {
                    if (Data_02000240_t[225][0] > 16) {
                        goto L_02000128;
                    }
                    return (s32)Data_0200a524;
                }
                return (s32)Data_0200a41c;
            }
            L_02000128:;
            return (s32)Data_0200a32c;
        } else {
        }
    }
    L_0200012e:;
    return (s32)Data_0200a2d8;
}

