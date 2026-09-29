/* Draft of resource_3c0 0x02008dc8, built with
 * games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H.
 * Remaining difference: the ROM loads scene 0xa5 from its literal pool as a
 * link-time value; GCC compares an immediate.
 * The listing keeps these rows. */
#include "SABAKU.H"

s32 FieldScene_RunScene3c0SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    if (gGameState.scene == 0xa5) {
        *(u8 *)(Engine_ActorGet(14) + 35) = 2;
        v5 = 0;
        *(u8 *)(Engine_ActorGet(14) + 85) = v5;
        Actor_SetPosition(14, 0xf80000, 0x2c80000);
        Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
        MapObject_SetPosition(100, -1, -1);
        BattleFx_EmitRandomParticle();
        Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
        record = Value2(Engine_TaskAddCallback, (s32)EncounterPalette_Pulse, 0xc80);
        return record;
    }
    return a0;
}
