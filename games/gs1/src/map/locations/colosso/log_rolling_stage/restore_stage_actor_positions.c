#include "colosso_log_rolling_stage.h"

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence) -- declared/named as the literal per-site
 * targets, not the shared ultimate-destination symbol. */
s32 Func_02006f98();           /* Func_080770e0 veneer #1 */
s32 Func_02006fa2();           /* Func_080770e0 veneer #2 */
void Func_02007058();          /* Func_0808a0f0 veneer #1 */
s32 Func_02006fc0();           /* Func_080770e0 veneer #3 */
s32 Func_02006fca();           /* Func_080770e0 veneer #4 */
void Func_0200707c();          /* Func_0808a0f0 veneer #2 */
s32 Func_02006fe4();           /* Func_080770e0 veneer #5 */
s32 Func_02006fee();           /* Func_080770e0 veneer #6 */
void Func_020070a0();          /* Func_0808a0f0 veneer #3 */

void ColossoLogRollingStage_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Func_02006f98(896);
        s32 z = Func_02006fa2(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_02007058(1, x, z);
    }
    {
        s32 x = Func_02006fc0(912);
        s32 z = Func_02006fca(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_0200707c(2, x, z);
    }
    {
        s32 x = Func_02006fe4(928);
        s32 z = Func_02006fee(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_020070a0(3, x, z);
    }
}
