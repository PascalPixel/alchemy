#include "types.h"

/*
 * Camera-shift beat for overlay resource_3a6.  Each Func_ name spells the
 * overlay's own relocated call word, not a runtime address.
 */

extern void Func_02003616();
extern void Func_0200309c();
extern void Func_020030a6();
extern void Func_02003568();
extern void Func_020034f6();
extern void Func_0200304e();
extern void Func_02003640();
/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void FieldScene_RunShiftAndSetFlag301(void)
{
    Func_02003616(241);
    Func_0200309c(11, 112, 0);
    Func_020030a6(11, 80, 0);
    Func_02003568(0x301);
    Func_020034f6(2);
    Func_0200304e();
    Func_02003640(0x121);
}
