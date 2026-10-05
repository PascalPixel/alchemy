#include "TYPES.H"
#include "INPUT.H"

extern u16 BattleFx_CyclePatternWords[];

u16 BattleFx_GetCycledTableWord(void)
{
    struct InputState *input = &gInput;
    u16 *words = BattleFx_CyclePatternWords;

    /* FAKEMATCH: ⚓️ loads the table's address before reading the buttons */
    asm volatile("" : "+l"(words));
    return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

extern u16 BattleFx_EightWayHeadingWords[];

u16 BattleFx_GetEightWayHeadingWord(void)
{
    struct InputState *input = &gInput;
    u16 *words = BattleFx_EightWayHeadingWords;

    /* FAKEMATCH: ⚓️ loads the table's address before reading the buttons */
    asm volatile("" : "+l"(words));
    return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}
