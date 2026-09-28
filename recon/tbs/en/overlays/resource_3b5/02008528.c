/* Draft of FieldScene_RunScene3b5_02000528, resource_3b5 at 0x02008528, built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H.
 * Remaining difference: the ROM loads message 0x1fa0 from its literal pool, as a
 * link-time value; as a C constant GCC builds it with mov and lsl (30 bytes).
 * The listing keeps these rows. */
#include "MACHI.H"

void FieldScene_RunScene3b5_02000528(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage(0x1fa0);
    Call3(Engine_ActorFaceDirection, 25, 0xc000, 0);
    Engine_EventShowMessage(25, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
    Engine_EventShowMessage(25, 0);
    Engine_EventEnd();
}
