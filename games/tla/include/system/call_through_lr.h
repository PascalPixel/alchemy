#ifndef ALCHEMY_CALL_THROUGH_LR_H
#define ALCHEMY_CALL_THROUGH_LR_H

/// @file
/// Calls through a register the way The Lost Age makes them: the callee's
/// address goes into lr and a lone second half of a bl, with offset 0,
/// jumps there and leaves the return address in lr. The arguments sit in r0
/// to r3 and the result comes back in r0, as for any call.

#include "types.h"

// In every arity r0 to r3 are the call's arguments and the registers it
// clobbers, so each is an output; the callee's address may share a free one,
// as the game's code puts it in r3 when it can.
CAMELOT_ASM("a linkage no procedure-call standard produces: mov lr, rN and "
            "a lone bl suffix with offset 0, where the stock compiler calls "
            "__call_via_rN")
static __inline__ s32 Call_ThroughLr4(const void *function, s32 a, s32 b, s32 c,
                                      s32 d) {
  register s32 r0 __asm__("r0") = a;
  register s32 r1 __asm__("r1") = b;
  register s32 r2 __asm__("r2") = c;
  register s32 r3 __asm__("r3") = d;
  __asm__ volatile("mov\tlr, %4\n\t.2byte\t0xf800"
                   : "+l"(r0), "+l"(r1), "+l"(r2), "+l"(r3)
                   : "l"(function)
                   : "r12", "r14", "cc", "memory");
  return r0;
}

CAMELOT_ASM("as Call_ThroughLr4, with r3 free for the callee's address")
static __inline__ s32 Call_ThroughLr3(const void *function, s32 a, s32 b,
                                      s32 c) {
  register s32 r0 __asm__("r0") = a;
  register s32 r1 __asm__("r1") = b;
  register s32 r2 __asm__("r2") = c;
  register s32 r3 __asm__("r3");
  __asm__ volatile("mov\tlr, %4\n\t.2byte\t0xf800"
                   : "+l"(r0), "+l"(r1), "+l"(r2), "=l"(r3)
                   : "l"(function)
                   : "r12", "r14", "cc", "memory");
  return r0;
}

CAMELOT_ASM("as Call_ThroughLr4, with r2 and r3 free")
static __inline__ s32 Call_ThroughLr2(const void *function, s32 a, s32 b) {
  register s32 r0 __asm__("r0") = a;
  register s32 r1 __asm__("r1") = b;
  register s32 r2 __asm__("r2");
  register s32 r3 __asm__("r3");
  __asm__ volatile("mov\tlr, %4\n\t.2byte\t0xf800"
                   : "+l"(r0), "+l"(r1), "=l"(r2), "=l"(r3)
                   : "l"(function)
                   : "r12", "r14", "cc", "memory");
  return r0;
}

CAMELOT_ASM("as Call_ThroughLr4, with r1 to r3 free")
static __inline__ s32 Call_ThroughLr1(const void *function, s32 a) {
  register s32 r0 __asm__("r0") = a;
  register s32 r1 __asm__("r1");
  register s32 r2 __asm__("r2");
  register s32 r3 __asm__("r3");
  __asm__ volatile("mov\tlr, %4\n\t.2byte\t0xf800"
                   : "+l"(r0), "=l"(r1), "=l"(r2), "=l"(r3)
                   : "l"(function)
                   : "r12", "r14", "cc", "memory");
  return r0;
}

CAMELOT_ASM("as Call_ThroughLr4, with every argument register free")
static __inline__ s32 Call_ThroughLr0(const void *function) {
  register s32 r0 __asm__("r0");
  register s32 r1 __asm__("r1");
  register s32 r2 __asm__("r2");
  register s32 r3 __asm__("r3");
  __asm__ volatile("mov\tlr, %4\n\t.2byte\t0xf800"
                   : "=l"(r0), "=l"(r1), "=l"(r2), "=l"(r3)
                   : "l"(function)
                   : "r12", "r14", "cc", "memory");
  return r0;
}

// A constant the game builds as a shifted byte plus an addend, where GCC
// would load it from the literal pool: the empty asm keeps the two apart.
#define SPLIT_CONSTANT(base, addend)                                           \
  ({                                                                           \
    u32 split_constant = (base);                                               \
    __asm__("" : "+l"(split_constant));                                        \
    split_constant + (addend);                                                 \
  })

#endif
