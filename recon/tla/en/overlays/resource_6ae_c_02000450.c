/* Draft: differ 16 (listing 32 instructions, draft 28). The native adds the
 * difference of two pool words, 0x121 - 0x10e, to the halfword at party
 * 0x1fa: two linker symbols of one table, not yet identified (O3), so the
 * draft folds them into 19. It also reloads the party address after the
 * calls where the draft keeps it in r5. */
#include "../../../../games/tla/src/field/overlay_6ae/ov6ae.h"

s32 GameFlag_Test(s32 flag);
void Func_020057d4(s32 a);
void Func_020055dc(s32 a, s32 b);

s32 Func_02000450(void)
{
    if (gPartyState.entrance == 1 && GameFlag_Test(0xa3d) == 0)
        Func_020057d4(1);
    Func_020055dc(gPartyState.map + (0x121 - 0x10e), 0x5000000);
    return 0;
}
