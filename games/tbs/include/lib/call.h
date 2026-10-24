#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

/// @file
/// Call2, a call through an unprototyped function pointer.

#include "types.h"

/// Calls `f` with two integer arguments and returns its result. LuckyDice_Run
/// decodes its board and sprite pictures through it.
static __inline__ s32 Call2(s32 (*f)(), s32 a0, s32 a1) {
  /* FAKEMATCH(wrapper): the arguments pass through the inline's parameters, so
   * a constant one is loaded again at each call; as the declared calls, 9 of
   * 2046 instructions differ in tbs-en (LuckyDice_Run). */
  return f(a0, a1);
}

#endif
