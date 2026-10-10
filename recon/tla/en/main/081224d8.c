/* Pin-free trial (tla-en): listing 28 instructions, draft 23, common 21, differ 9.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "types.h"
#include "btlsys.h"
extern u8 Data_03001e74[];

s32 Battle_SelectWeightedIndex(u8 *weights)
{
    s32 value;
    s32 total;
    s32 result;
    s32 index;
    /* FAKEMATCH: Retain the native incoming-r9 chain spill before argument setup and Random16; ordinary top-level C omits that ABI save/store. */
    volatile u32 frame;
    /* FAKEMATCH: Plain top-level C omits the native r9 save; capture the incoming chain before Random16. */
    u32 chain;

    /* FAKEMATCH: Read the native chain register without an instruction so GCC preserves r9. */
    __asm__("" : "=r"(chain));
    frame = chain;
    {
        /* FAKEMATCH: An unconstrained argument copy precedes the native chain spill; retain incoming r0 through that spill and hand it back. */
        u8 *arg = weights;
        /* FAKEMATCH: The r0/chain tie keeps the spill before the argument copy without extending either pin over Random16. */
        __asm__("" : "+r"(arg) : "r"(chain));
        weights = arg;
    }
    value = BtlRndFar() & 0xFF;
    total = weights[0];
    result = 0;
    index = 0;
    if (value >= total) {
loop:
        index++;
        if (index <= 7) {
            total += weights[index];
            if (value < total)
                result = index;
            else
                goto loop;
        }
    }
    return result;
}
