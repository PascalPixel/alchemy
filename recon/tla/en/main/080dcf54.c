/* Near miss: score 60. ⚓️ loads the table's address straight after
   gInput's, before reading the held buttons; this draft loads it after the
   shift. A table pointer temporary, volatile input and 30 s of permuting
   did not move it. */
#include "TYPES.H"

/* The controller state the engine refreshes each frame. */
struct InputState {
    u32 held;
    u32 pressed;
};

extern struct InputState gInput;
extern u16 BattleFx_CyclePatternWords[];

/* ☀️'s, reading ⚓️'s held buttons from gInput. */
u16 BattleFx_GetCycledTableWord(void)
{
    return BattleFx_CyclePatternWords[(gInput.held >> 4) & 15];
}
