#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

#include "types.h"

static __inline__ void Call1(void (*f)(), s32 a0)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
	s32 a7, s32 a8, s32 a9, s32 a10)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
	f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ s32 Value0(s32 (*f)())
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0, a1, a2, a3);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
	/* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
	return f(a0, a1, a2, a3, a4, a5, a6);
}

#endif
