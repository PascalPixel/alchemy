#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

/// @file
/// The call layer of the scene files that have not yet moved to
/// event_inline.h: Call1 to Call11 call a routine with that many arguments,
/// Value0 to Value4 return its int result.
///
/// It is a habit of the whole game written once, as event_inline.h is: a layer,
/// not a compiler-steering device. Each call passes its arguments through the
/// inline's own parameters, so where a scene calls a routine again with the
/// same constant, the constant is built again for each call, as the ROM shows.

#include "types.h"

static __inline__ void Call1(void (*f)(), s32 a0) { f(a0); }

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2) {
  f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                             s32 a4, s32 a5) {
  f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                             s32 a4, s32 a5, s32 a6) {
  f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                              s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
                              s32 a10) {
  f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ s32 Value0(s32 (*f)()) { return f(); }

static __inline__ s32 Value1(s32 (*f)(), s32 a0) { return f(a0); }

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) { return f(a0, a1); }

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2) {
  return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  return f(a0, a1, a2, a3);
}

#endif
