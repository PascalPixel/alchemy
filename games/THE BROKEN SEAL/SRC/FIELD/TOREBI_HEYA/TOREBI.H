/* Declarations shared by the Tolbi house scene's sources (resource_3b6). */
#ifndef TOREBI_H
#define TOREBI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"



struct Ent { s32 a; u16 b; u16 c; };

extern u8 Data_02008bb4[];
extern u8 Data_02008dac[];
extern u8 Data_02008e00[];
extern u8 Data_02009040[];
extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];
extern struct Ent Data_02009dcc[];


/* Contiguous unnamed leaf-owner run for resource_3b6. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */

/* FAKEMATCH: call sites spelled through these wrappers, the SceneInit_
 * and Scene_ ones included, pass their constants straight into the
 * argument registers; a direct call precomputes a costly constant into a
 * pseudo that the compiler then shares with later uses in the block. A
 * value-returning call also sets r0 last of its arguments. The old-style
 * declarations vary by call site across this overlay. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void SceneInit_Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 SceneInit_Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void SceneInit_Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void SceneInit_Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Scene_Call1(void (*f)(s32), s32 a) { f(a); }

static __inline__ s32 Scene_Value2(s32 (*f)(s32, s32), s32 a, s32 b) { return f(a, b); }

static __inline__ void Scene_Call3(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c) { f(a, b, c); }

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

#endif
