#include "YAMA.H"

/*
 * Compare record 0's field at +12 against a signed threshold and set record
 * 12's mode accordingly; the taller branch also sets bit 1 of record 11's
 * byte at +35.  Both offsets are named by position only and their roles are
 * unverified; +35 is read-modify-written as a flags byte.  The threshold is
 * kept as the value 0x00300000 the code builds, in no assumed fixed-point
 * unit.
 */
void SceneActor_SetActor12ModeByActorZeroHeight(void)
{
    extern u32 Data_03001e40;

    if (*(s32 *)((u8 *)Engine_ActorGet(0) + 12) > 0x00300000) {
        {
            u8 *flag = (u8 *)Engine_ActorGet(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Actor_SetSpritePriority(12, 3);
    } else {
        Actor_SetSpritePriority(12, 2);
    }
}

void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Actor_Get(no);
    record[89] &= 0xfe;

    SetMapCellCollision(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}
