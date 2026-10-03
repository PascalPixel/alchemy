#include "TYPES.H"
#include "INPUT.H"

extern u16 BattleFx_CyclePatternWords[];

/* ☀️'s, reading ⚓️'s held buttons from gInput. */
u16 BattleFx_GetCycledTableWord(void)
{
    struct InputState *input = &gInput;
    u16 *words = BattleFx_CyclePatternWords;

    asm volatile("" : "+l"(words)); /* FAKEMATCH: ⚓️ loads the table's address before reading the buttons */
    return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}
