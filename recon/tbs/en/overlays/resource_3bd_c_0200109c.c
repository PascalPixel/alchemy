/* NONMATCHING: 860 bytes, candidate 868, 380 differing halfwords, 251
 * halfword edits (2026-09-25). FieldScene_RunStatefulSequence, meant for
 * FIELD/ARUTAMIRA_DOU/F_0109C.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Uses the loaded -0xc00 literal, typed actor records, a
 * declared input parameter and the IWRAM remainder import; remaining
 * differences are structural.
 * WALL: State-machine control flow and actor-loop register lifetimes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneActor_SetPositionFromTransformedBase();
void ArutamiraDou_Func02003ad4();
s32 Main_030003ac(s32 dividend, s32 divisor);
void Engine_EventResetEffectCounter(void);

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunStatefulSequence(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p8;
    s32 p8b;
    struct FieldActor *rec7;
    s32 record;
    s32 value;
    s32 none;
    s32 v4;
    s32 v8;
    s32 v7;
    s32 base6_6666;
    s32 v9;
    s32 v6;
    s32 base7_2001001;
    s32 base6_200bf6c;
    s32 base8_180000;
    s32 v2;
    s32 slot0;
    u8 *p5;
    s32 p6;
    u8 p5b;

    none = 0;
    Engine_EventBegin();
    Engine_EventResetEffectCounter();
    Call1(Engine_EventSetMessage, 0x21db);
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
    none = 0;
    v4 = none;
    do {
        slot0 = v4;
        rec7 = Engine_ActorGet((v4 + 11));
        v4 = slot0;
        v4 = (v4 + 1);
        rec7->update = 0;
        rec7->scale_x = 0x10000;
        rec7->scale_y = 0x10000;
    } while (v4 <= 4);
    p8 = *(u8 *)0x02001001;
    p5 = (u8 *)*(s32 *)0x0200bf6c;
    p11 = *(u8 *)0x02001002;
    record = Engine_MathDivide((*(s8 *)(0x2001001 + 1) << 16), 5);
    *(u16 *)((s32)p5 + 6) = (record + 0x4000);
    if (((s32)((s32)p8 << 24) >> 24) == 0) {
        if (a0 == 16) {
            Engine_AudioPlayCue(110);
            v8 = 1;
        } else {
            Engine_AudioPlayCue(114);
            v8 = p8;
        }
        *(u8 *)0x02001000 = 0;
        v9 = none;
    } else {
        if (((s32)((s32)p8 << 24) >> 24) == 1) {
            if (a0 == 16) {
                Engine_AudioPlayCue(110);
                v8 = p8;
                v9 = none;
                goto L_02001256;
            }
            v9 = none;
            if (a0 == 20) {
                v8 = 2;
                Engine_AudioPlayCue(110);
                Engine_TaskWait(30);
                v4 = 0;
                v7 = *(s16 *)(*(s32 *)0x0200bf6c + 6);
                do {
                    v7 = (v7 << 16);
                    slot0 = v4;
                    Call3(SceneActor_SetPositionFromTransformedBase, (v4 + 11), 0x180000, ((u32)v7 >> 16));
                    Engine_AudioPlayCue(151);
                    rec7 = Engine_ActorGet((v4 + 11));
                    rec7->scale_x = 0;
                    base6_6666 = 0x6666;
                    v4 = slot0;
                    do {
                        rec7->scale_y = base6_6666;
                        rec7->scale_x = base6_6666;
                        slot0 = v4;
                        Engine_TaskWait(1);
                        base6_6666 = (base6_6666 + 0xc00);
                        v4 = slot0;
                    } while (rec7->scale_x <= 0xffff);
                    v4 = (v4 + 1);
                    v7 = (((((u32)v7 >> 16) + -0x3333) << 16) >> 16);
                } while (v4 <= 4);
                Engine_TaskWait(30);
                v9 = 1;
                v9 = 1;
                goto L_02001256;
            }
            Engine_AudioPlayCue(114);
            none = 0;
            v8 = none;
        } else {
            v8 = p8;
            v9 = none;
            if (((s32)((s32)p8 << 24) >> 24) == 2) {
                if (a0 != (*(s8 *)(0x2001001 + 1) + 16)) {
                    none = 0;
                    v8 = none;
                    Engine_AudioPlayCue(114);
                    Engine_TaskWait(30);
                    v4 = 0;
                    do {
                        slot0 = v4;
                        rec7 = Engine_ActorGet(v4 + 11);
                        Engine_AudioPlayCue(151);
                        p6 = rec7->scale_x;
                        v4 = slot0;
                        if ((s32)p6 > 0x6666) {
                            v6 = (s32)p6;
                            do {
                                rec7->scale_y = v6;
                                rec7->scale_x = v6;
                                slot0 = v4;
                                Engine_TaskWait(1);
                                v6 = (v6 - 0xc00);
                                v4 = slot0;
                            } while (rec7->scale_x > 0x6666);
                        }
                        slot0 = v4;
                        Engine_ActorSetPosition((v4 + 11), 0, 0);
                        v4 = slot0;
                        v4 = (v4 + 1);
                        a0 = (v4 + 11);
                    } while (v4 <= 4);
                    v9 = none;
                } else {
                    Engine_AudioPlayCue(110);
                    v9 = 1;
                    Engine_TaskWait(30);
                    v8 = p8;
                }
            }
        }
    }
    L_02001256:;
    base7_2001001 = 0x2001001;
    *(u8 *)base7_2001001 = v8;
    if (v9 == 0) {
    } else {
        p5b = *(u8 *)0x02001000;
        *(u8 *)0x02001000 = ((s32)p5b + 1);
        value = Engine_RandomNext();
        record = Main_030003ac((((s32)((s32)((((u32)(value << 2) >> 16) + p11) + 1) << 24) >> 24) + 5), 5);
        base6_200bf6c = 0x200bf6c;
        *(u8 *)(base7_2001001 + 1) = record;
        {
            u16 *target = (u16 *)(*(s32 *)base6_200bf6c);
            s32 shown = 0;

            *target = shown;
        }
        {
            s32 target = *(s32 *)base6_200bf6c;
            s32 shown = 0;

            *(u16 *)(target + 2) = shown;
        }
        {
            s32 target = *(s32 *)base6_200bf6c;
            s32 shown = 0x200;

            *(u16 *)(target + 8) = shown;
        }
        {
            s32 target = *(s32 *)base6_200bf6c;
            s32 shown = 0x3000;

            *(u16 *)(target + 10) = shown;
        }
        Call2(Engine_TaskAddCallback, 0x2008f95, 0xc80);
        if ((u32)((u32)(s32)((s32)((s32)p5b + 1) << 24) >> 24) <= 2) {
            if (*(s16 *)(*(s32 *)base6_200bf6c) != 99) {
                for (i = 0; (i >> 16) != 99; i += 0x10000) {
                    Engine_TaskWait(1);
                }
            }
            Engine_TaskWait(10);
            Engine_AudioPlayCue(110);
        } else {
            *(u8 *)base7_2001001 = 99;
            if (*(s16 *)(*(s32 *)base6_200bf6c) != 2) {
                for (i = 0; (i >> 16) != 2; i += 0x10000) {
                    Engine_TaskWait(1);
                }
            }
            {
                u16 *target = (u16 *)(*(s32 *)base6_200bf6c);
                s32 shown = 2;

                *target = shown;
            }
            {
                s32 target = *(s32 *)base6_200bf6c;
                s32 shown = 0;

                *(u16 *)(target + 2) = shown;
            }
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
            Engine_EventWait(20);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            {
                u16 *target = (u16 *)(*(s32 *)base6_200bf6c);
                s32 shown = 99;

                *target = shown;
            }
            Engine_AudioPlayCue(190);
            base8_180000 = 0x180000;
            v7 = *(s16 *)(*(s32 *)base6_200bf6c + 6);
            do {
                v4 = 0;
                do {
                    slot0 = v4;
                    rec7 = Engine_ActorGet(v4 + 11);
                    rec7->scale_x = rec7->scale_x - 16;
                    rec7->scale_y = rec7->scale_y - 16;
                    SceneActor_SetPositionFromTransformedBase((v4 + 11), base8_180000, ((u32)(v7 << 16) >> 16));
                    v2 = -0x3333;
                    v4 = slot0;
                    v4 = (v4 + 1);
                    v7 = (((((u32)(v7 << 16) >> 16) + -0x3333) << 16) >> 16);
                } while (v4 <= 4);
                base8_180000 = (base8_180000 + v2);
                v7 = (((((u32)(v7 << 16) >> 16) + 0xc00) << 16) >> 16);
                Engine_TaskWait(1);
            } while (base8_180000 > 0);
            v4 = 0;
            do {
                slot0 = v4;
                Engine_ActorSetPosition((v4 + 11), 0, 0);
                v4 = slot0;
                v4 = (v4 + 1);
            } while (v4 <= 4);
            ArutamiraDou_Func02003ad4();
            Engine_AudioPlayCue(80);
        }
        Call1(Engine_TaskRemoveCallback, 0x2008f95);
    }
    p8b = v9;
    p10 = *(s8 *)(0x2001001 + 1);
}
