#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

#include "types.h"
#include "system/call_through_lr.h"

static __inline__ void Call1(void (*f)(), s32 a0) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                             s32 a4, s32 a5) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                             s32 a4, s32 a5, s32 a6) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3,
                              s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
                              s32 a10) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants. */
  f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ s32 Value0(s32 (*f)()) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants and keeps the result argument in r0. */
  return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants and keeps the result argument in r0. */
  return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants and keeps the result argument in r0. */
  return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants and keeps the result argument in r0. */
  return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing
   * direct-call constants and keeps the result argument in r0. */
  return f(a0, a1, a2, a3);
}

// The same calls where the game makes them through lr (call_through_lr.h):
// where GCC calls through a register, as when it shares a callee's address.
static __inline__ void CallLr0(void (*f)()) {
  Call_ThroughLr0((const void *)f);
}

static __inline__ void CallLr1(void (*f)(), s32 a0) {
  Call_ThroughLr1((const void *)f, a0);
}

static __inline__ void CallLr2(void (*f)(), s32 a0, s32 a1) {
  Call_ThroughLr2((const void *)f, a0, a1);
}

static __inline__ void CallLr3(void (*f)(), s32 a0, s32 a1, s32 a2) {
  Call_ThroughLr3((const void *)f, a0, a1, a2);
}

static __inline__ void CallLr4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  Call_ThroughLr4((const void *)f, a0, a1, a2, a3);
}

static __inline__ s32 ValueLr0(s32 (*f)()) {
  return Call_ThroughLr0((const void *)f);
}

static __inline__ s32 ValueLr1(s32 (*f)(), s32 a0) {
  return Call_ThroughLr1((const void *)f, a0);
}

static __inline__ s32 ValueLr2(s32 (*f)(), s32 a0, s32 a1) {
  return Call_ThroughLr2((const void *)f, a0, a1);
}

static __inline__ s32 ValueLr3(s32 (*f)(), s32 a0, s32 a1, s32 a2) {
  return Call_ThroughLr3((const void *)f, a0, a1, a2);
}

static __inline__ s32 ValueLr4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) {
  return Call_ThroughLr4((const void *)f, a0, a1, a2, a3);
}

#endif
