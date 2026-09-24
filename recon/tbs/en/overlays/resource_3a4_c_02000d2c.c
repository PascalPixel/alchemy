/* NONMATCHING: 408 of 404 bytes, 141 halfword edits (2026-09-24).
 * Hand-written from the resolved disassembly: three goto loops swing the
 * sprite angle (+30) up past 0x8fff, back below 0x7000 and settle it to
 * 0xc000 while Engine_MathCos/Sin move the actor. The limits and the settle
 * step share one variable (r8 in the reference) and 0x8000 is a variable
 * (fp). Remaining: cse reuses loop 2's 0x80000 increment as loop 3's initial
 * accumulator (a spilled pseudo, sp 8 instead of 4), and gcse carries acc >>
 * 16 around loop 3's back edge; the reference materialises both afresh. */
#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_AudioPlayCue();
void Engine_WorkSetValuesIfNonNegative();
void Engine_EventWait();
s32 Engine_MathCos();
void Engine_TaskWait();
s32 Engine_MathSin();
void FieldScene_RunSharedSetPiece();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02000d2c(void)
{
    u8 *actor;
    u8 *sprite;
    s32 x;
    s32 z;
    u32 acc;
    u32 angle;
    u32 limit;
    u32 half;
    s32 c;
    s32 s;

    actor = (u8 *)Value1(Engine_ActorGet, 10);
    sprite = *(u8 **)(actor + 80);
    x = *(s32 *)(actor + 8);
    z = *(s32 *)(actor + 12);
    Engine_EventBegin();
    Engine_AudioPlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x10000, 0x10000);
    Engine_EventWait(10);
    Call1(Engine_AudioPlayCue, 0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    acc = 0;
    limit = 0x8fff;
rise:
    acc += 0x80000;
    *(u16 *)(sprite + 30) += acc >> 16;
    c = Engine_MathCos(*(u16 *)(sprite + 30) + 0x4000);
    *(s32 *)(actor + 8) = (c << 4) + x;
    angle = *(u16 *)(sprite + 30);
    if (angle <= limit) {
        Engine_TaskWait(1);
        goto rise;
    }
    acc = 0;
    limit = 0x7000;
fall:
    acc += 0x80000;
    *(u16 *)(sprite + 30) = angle - (acc >> 16);
    c = Engine_MathCos(*(u16 *)(sprite + 30) + 0x4000);
    *(s32 *)(actor + 8) = (c << 4) + x;
    angle = *(u16 *)(sprite + 30);
    if (angle > limit) {
        Engine_TaskWait(1);
        angle = *(u16 *)(sprite + 30);
        goto fall;
    }
    half = 0x8000;
    acc = 0x80000;
settle:
    acc = ((acc >> 16) + (acc >> 19)) << 16;
    limit = acc >> 16;
    *(u16 *)(sprite + 30) = limit + angle;
    c = Engine_MathCos(*(u16 *)(sprite + 30) + 0x4000);
    s = Engine_MathSin(*(u16 *)(sprite + 30) + half);
    *(s32 *)(actor + 8) = (c << 4) + x;
    if (*(u16 *)(sprite + 30) > half) {
        *(s32 *)(actor + 12) = z - (s << 3);
    }
    if ((s32)(*(u16 *)(sprite + 30) + limit) <= 0xbfff) {
        Engine_TaskWait(1);
        angle = *(u16 *)(sprite + 30);
        goto settle;
    }
    Engine_TaskWait(1);
    {
        s32 shown = 0xc000;

        *(u16 *)(sprite + 30) = shown;
    }
    Engine_AudioPlayCue(183);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_EventWait(20);
    Call1(Engine_AudioPlayCue, 0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    FieldScene_RunSharedSetPiece(5);
    Engine_EventEnd();
}
