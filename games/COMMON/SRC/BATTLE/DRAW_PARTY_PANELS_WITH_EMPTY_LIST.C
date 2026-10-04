/* Complete current-source and own-ROM audit, 2026-10-02.
 * Ordinary TBS flags; all six complete 44-byte functions are exact, with
 * resolved call identities and one real public definition.
 * EN trials first: plain shape emitted 32 bytes (42 linked differences);
 * nested source matched but retained two names; direct nested export lacked
 * a public binding; chain-after differed at 3 bytes; chain-before was exact.
 * All private trial assemblies were reproduced from their current sources.
 */
#include "TYPES.H"
void BattleLayout_HighlightPartyPanels(u16 *);
void BattlePres_SetActorModesFar(u16 *,s32);
/* The native callback keeps the incoming nested-function chain in r9. */
void Battle_DrawPartyPanelsWithEmptyList(void)
{
    /* FAKEMATCH: Retain the native incoming-r9 chain spill before the calls; ordinary top-level C omits that ABI save/store. */
    volatile u32 frame;
    u16 data[2];
    /* FAKEMATCH: Plain top-level C omits the native r9 save; capture the incoming static chain before either call. */
    register u32 chain asm("r9");

    /* FAKEMATCH: Read the native chain register without an instruction so GCC preserves r9 as the callback requires. */
    __asm__("" : "=r"(chain));
    frame = chain;
    data[0] = 255;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
}
