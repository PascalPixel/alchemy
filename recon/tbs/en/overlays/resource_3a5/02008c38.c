/* Draft of resource_3a5 0x02008c38, from
 * games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SCENE_ID_SELECTED_ACTOR_SCENE.C.
 * Remaining difference: the ROM loads desert scenes 0x59-0x5c from the
 * literal pool, as link-time scene symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "RAMAKAN.H"

void FieldScene_RunScene3a5_02000c38(void)
{
    extern u8 Data_02000240[];

    Actor_RunRepeatedMotion(8, 2);
    Func_02002a54((s32)Data_0000005b, 5);
    do {
        Data_02000240[0x22b] = 3;
    } while (0);
    Func_02002a5e(53, 5);
}

