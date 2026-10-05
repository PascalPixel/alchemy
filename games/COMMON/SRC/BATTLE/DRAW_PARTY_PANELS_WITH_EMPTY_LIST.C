#include "PROTO.H"
#include "TYPES.H"
void BattleLayout_HighlightPartyPanels(u16 *);
void Battle_DrawPartyPanelsWithEmptyList(void)
{
    /* FAKEMATCH: the incoming r9 chain spill preserves the ABI save/store omitted by ordinary top-level C. */
    volatile u32 frame;
    u16 data[2];
    /* FAKEMATCH: capturing the incoming static chain preserves the r9 save before either call. */
    register u32 chain asm("r9");

    /* FAKEMATCH: the zero-instruction chain read keeps r9 preserved across the callback. */
    __asm__("" : "=r"(chain));
    frame = chain;
    data[0] = 255;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
}
