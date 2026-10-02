/* Complete current-source and own-ROM audit, 2026-10-02.
 * Ordinary TBS flags; all six complete 62-byte functions are exact, with the
 * Random16 call resolved and one real public definition. The following two
 * bytes stay in their separate uncredited scaffold; no linker rows change.
 * EN trials first: plain shape emitted 46 bytes (60 linked differences);
 * chain-only and r0-memory each differed at 4 bytes; frame-order emitted 64
 * bytes with 53 differences; r0-order and weights-order emitted 64 bytes
 * with 55 differences; the short r0/chain handoff was exact.
 * All private trial assemblies were reproduced from their current sources.
 */
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
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
