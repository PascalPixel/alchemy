#include "TYPES.H"
#include "SCENE.H"

/* Battle presentation keeps its own copy of the BG0 vertical offset reset. */
void BattlePresentation_ClearBg0VerticalOffset(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}

void Graphics_ClearBg0Vofs(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}
