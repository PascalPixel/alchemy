#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

/* Picks one of eight entries by weight: a random byte falls into the
   running total of the weights, and the entry whose share it lands in is
   chosen. A roll past all eight picks the first. */
s32 Battle_SelectWeightedIndex(u8 *weights)
{
    s32 value;
    s32 total;
    s32 result;
    s32 index;
    /* FAKEMATCH: Retain the native incoming-r9 chain spill before argument setup and Random16; ordinary top-level C omits that ABI save/store. */
    volatile u32 frame;
    /* FAKEMATCH: Plain top-level C omits the native r9 save; capture the incoming chain before Random16. */
    register u32 chain asm("r9");

    /* FAKEMATCH: Read the native chain register without an instruction so GCC preserves r9. */
    __asm__("" : "=r"(chain));
    frame = chain;
    {
        /* FAKEMATCH: An unconstrained argument copy precedes the native chain spill; retain incoming r0 through that spill and hand it back. */
        register u8 *arg asm("r0") = weights;
        /* FAKEMATCH: The r0/chain tie keeps the spill before the argument copy without extending either pin over Random16. */
        __asm__("" : "+r"(arg) : "r"(chain));
        weights = arg;
    }
    value = BattleRandom16Far() & 0xFF;
    total = weights[0];
    result = 0;
    index = 0;
    if (value >= total) {
        while (++index <= 7) {
            total += weights[index];
            if (value < total) {
                result = index;
                break;
            }
        }
    }
    return result;
}
