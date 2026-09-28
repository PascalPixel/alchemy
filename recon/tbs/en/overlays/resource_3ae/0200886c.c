/* Draft of resource_3ae 0x0200886c (FieldScene_RunScene3ae_0200086c), built with
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x6b and 0x70 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "KAREI.H"

extern u8 Data_0000006b[];
extern u8 Data_00000070[];
extern u8 Data_0000006c[];
extern s16 Data_02000240_t[][1];
void Func_020011fc();

s32 FieldScene_RunScene3ae_0200086c(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 90) {
        GameFlag_Set(0x950);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000006b) {
        FieldScene_RunScene3ae_020008cc();
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000070) {
            Func_020011fc();
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000006c) {
                SceneState_SetRuntimeWord448To521AndSend303();
            }
        }
    }
    return 0;
}
