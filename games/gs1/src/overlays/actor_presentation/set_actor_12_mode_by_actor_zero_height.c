#include "types.h"

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */

extern u8 *Func_02006fc6(s32 id);
extern u8 *Func_02006fd6(s32 id);
extern void Func_020070a8(s32 id, s32 arg1);
extern void Func_020070b2(s32 id, s32 arg1);

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
    if (*(s32 *)(Func_02006fc6(0) + 12) > 0x00300000) {
        {
            u8 *flag = Func_02006fd6(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Func_020070a8(12, 3);
    } else {
        Func_020070b2(12, 2);
    }
}
