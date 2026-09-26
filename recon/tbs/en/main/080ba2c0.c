/* NONMATCHING: 708 bytes, 28 aligned edits. A volatile saved input adds
 * 16 bytes and 89 edits; a volatile input parameter adds four bytes and
 * 51 edits. Neither reproduces the initial argument reload without changing
 * the frame. Retain the shared source and record the reload as unresolved. */
#define RunBattlePresentation Func_080ba2c0
#define BATTLE_SEQUENCE_OWNER RunBattlePresentation
#define BATTLE_SEQUENCE_CALLBACK ((void *)0x080bd899)
#include "../../ja/main/080b12c0.c"
