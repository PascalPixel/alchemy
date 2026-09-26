/* NONMATCHING: 708 bytes, 28 aligned edits. A volatile saved input adds
 * 16 bytes and 89 edits; a volatile input parameter adds four bytes and
 * 51 edits. Neither reproduces the initial argument reload without changing
 * the frame. A one-load volatile pointer view of saved_input keeps the 108-
 * byte frame but has 80 edits; the same view of input adds a second pointer
 * slot (112-byte frame, 49 edits). The CSE dump keeps the original r0 input
 * available for the first byte load, and postreload therefore omits the ROM's
 * reload from sp+12. Retain the shared source; these scoped-memory hypotheses
 * did not close the reload or the subsequent angle-constant scheduling. */
#define RunBattlePresentation Func_080ba2c0
#define BATTLE_SEQUENCE_OWNER RunBattlePresentation
#define BATTLE_SEQUENCE_CALLBACK ((void *)0x080bd899)
#include "../../ja/main/080b12c0.c"
