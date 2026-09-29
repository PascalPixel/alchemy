/* 2026-09-29 alchemy permute: score 10599 to 9104 on the permuter's scorer
   (0 is exact); remaining 109 register-only, 19 stack-only, 66 operand, 59
   reordered, 13 inserted, 22 deleted. Kept rewrites: 25x reorder
   independent statements, 21x swap commutative operands, 17x introduce a
   temporary, 15x reorder local declarations, 12x change loop form, 12x
   pointer arithmetic or indexing, 11x split or join a compound assignment,
   9x move an assignment into or out of a condition, 9x test truth or
   compare with zero, 7x add a same-width cast, 3x remove a temporary, 3x
   drop a same-width cast, 2x invert an if/else, 1x toggle register.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* NONMATCHING: shared callee return types audited on 2026-09-26.
 * 1180 of 1204 bytes, 564 differing halfwords, 339 aligned edits.
 * Canonical declarations are retained; the remaining source model is not exact. */
#include "types.h"
#include "BATTLE_EFX.H"

extern u8 Data_00000073[];
extern u8 Data_0000007d[];
extern u8 Data_00000089[];
s32 Func_0800231c();
s32 Trig_Sin();
void Func_08002dd8();
s32 Resource_GetTableEntry();
void Func_080030f8();
void Func_080041d8();
void Scheduler_RemoveCallback();
s32 Random16();
void Render_ResetTransformState();
void Graphics_PrepareTransferInIwramWork();
s32 Resource_DecodeType01();
void _call_via_r3();
void Func_080072f4();
void Func_08009080();
void Func_08009140();
void Func_08009150();
void Func_080b5088();
s32 GetBattleObjectSlotFar();
void Func_080b50e8();
void ObjectGroup_TickMemberTimers();
void BattleFx_BeginCanvasLayer();
void BattleFx_EndCanvasLayer();
void ObjectGroup_UpdateMembers();
void Camera_ApplyShake();
void EffectPosition_ApplyAlternateStepAndYOffset();
void Func_080f9010();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_080ce034(s32 a0)
{
    u32 i;
    u8 *p10;
    s32 p10b;
    s32 p11;
    s32 p9;
    s32 p9b;
    s32 p9c;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 r13;
    s32 r8;
    s32 v1;
    s32 base6_3001eec;
    s32 none;
    s32 v8;
    s32 v3;
    s32 base6_2010000;
    s32 v7;
    s32 v11;
    s32 v12;
    s32 slot36;
    register s32 v0;
    s32 slot32;
    s32 slot28;
    s32 slot40;
    s32 slot8;
    s32 slot24;
    s32 slot12;
    s32 slot20;
    u8 *p5;
    u8 *p4;
    s32 slot16;
    u8 slot48[12];
    s32 tmp10;

    base6_3001eec = 0x3001eec;
    v1 = *(s32 *)0x3001eec;
    slot36 = v1;
    slot32 = *(s32 *)(0x3001eec + 4);
    slot28 = ((s32 *)(base6_3001eec + 8))[0];
    *(s32 *)(slot36 + 0x7828) = a0;
    BattleFx_BeginCanvasLayer(0);
    record = Resource_GetTableEntry((s32)Data_00000073);
    ((void (*)())Resource_DecodeType01)(record, slot28);
    rec7 = Value1(Resource_GetTableEntry, (s32)Data_0000007d);
    tmp10 = (s32)0x3001388;
    Call4(_call_via_r3, 0x5000000, rec7, 128, tmp10);
    Value2(Resource_DecodeType01, rec7 + 128, slot36);
    ((void (*)())BattleEffect_LoadWork)(46, 7, 7, 3, 2);
    slot40 = *(s32 *)(base6_3001eec + 28);
    slot8 = r13 + 40;
    ((void (*)())BattleEffect_LoadWork)(47, 7, 7, 7, 2);
    *(s32 *)(slot8 + 4) = *(s32 *)(base6_3001eec + 32);
    *(s32 *)(slot36 + 0x7780) = 2;
    ((s32 *)(slot36 + 0x7784))[(u32)0] = 75;
    Call2(Func_080041d8, 0x80cd261, 0x480);
    none = 0;
    v8 = none;
    v3 = 0x2010018;
    while (1) {
        *(s32 *)v3 = 0;
        v8 += (u32)1;
        v3 += 28;
        if (v8 == 0x400)
            break;
    }
    record = GetBattleObjectSlotFar(*(s32 *)(*(s32 *)(slot36 + 0x7828) + 8));
    slot20 = -0xf0000;
    p10 = *(s32 *)record;
    slot16 = slot36 + 0x7828;
    record = GetBattleObjectSlotFar(*(s16 *)(*(s32 *)(slot36 + 0x7828) + 36));
    slot24 = *(s32 *)record;
    if (0 >= ((s32 *)((s32)p10 + 8))[0]) {
        slot20 = 0xf0000;
    }
    slot12 = 48 + r13;
    none = 0;
    v7 = r8;
    v11 = none;
L_080ce130:
    ;
    p5 = *(s32 *)0x03001e80;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((s32)p5, (s32)p5 + 12);
    if (v11 <= 17) {
        if (v11 != 0) {
            goto L_080ce164;
        }
    }
    EffectPosition_ApplyAlternateStepAndYOffset(*(s32 *)(*(s32 *)slot16 + 8), slot12);
    *(s32 *)slot12 = (*(s32 *)slot12 + ((u32)*(s32 *)slot12 >> 31)) >> 1;
L_080ce164:
    ;
    if ((u32)(v11 - 2) <= 1) {
        Func_080072f4(slot32, slot36, *(s32 *)slot12 - 16, *(s32 *)(slot12 + 4) - 64, 32, 64);
    }
    if ((u32)(v11 - 4) <= 11) {
        s32 tmp;
        tmp = v11 - 4;
        p9 = ((tmp + ((u32)(v11 - 4) >> 31)) >> 1) << 11;
        none = 0;
        v8 = none;
        v7 = slot48;
        while (1 != 0) {
            s32 tmp11;
            record = Trig_Sin(v8 << 12);
            p5 = ((s32 *)v7)[0];
            record = Func_0800231c((s32)(v8 << 12));
            tmp11 = (s32)p5 + ((record * v11) >> 16);
            Call6(Func_080072f4, slot32, p9 + slot36, tmp11 - 16, *(s32 *)(v7 + 4) + ((v11 * record) >> 16) - v11 - 64, 32, 64);
            v8 = v8 + 1;
            if (v8 == 16)
                break;
        }
    }
    if (v11 == 4) {
        s32 *tmp3;
        s32 tmp8;
        s32 *tmp9;
        s32 tmp14;
        s32 tmp5;
        *(s32 *)((s32)p10 + 40) = 0x140000;
        tmp3 = (s32 *)(72 + (s32)p10);
        *(s32 *)((s32)p10 + 52) = 0x10000;
        tmp9 = (s32 *)((s32)p10 + 48);
        *tmp9 = 0x30000;
        tmp3[0] = 0xab85;
        p10[90] = 0;
        *(u8 *)((s32)p10 + 90 - 2) = 0;
        tmp14 = (s32)p10;
        tmp8 = tmp14 + 8;
        tmp5 = (s32)p10 + 8;
        Func_08009150((s32)p10, (*(s32 *)tmp5 << 1) + *(s32 *)tmp8, 0, *(s32 *)((s32)p10 + 16));
        Func_08009080((s32)p10, 2);
        *(s32 *)(0x77a8 + slot36) = v11;
        Func_080f9010(136);
    }
    if (v11 == 16) {
        s32 tmp7;
        rec7 = Value1(Resource_GetTableEntry, (s32)Data_00000089);
        Call4(_call_via_r3, 0x5000000, (s32)rec7, 128, 0x3001388);
        ((void (*)())Resource_DecodeType01)(rec7 + 128, slot36);
        *(s32 *)((s32)p10 + 72) = 0;
        tmp7 = 36 + (s32)p10;
        *(s32 *)tmp7 = 0;
        *(s32 *)((s32)p10 + 40) = 0;
        *(s32 *)((s32)p10 + 16) = *(s32 *)(16 + slot24);
        Func_08009140((s32)p10);
    }
    if (17 >= v11) {
    } else {
        if (*(s32 *)(12 + (s32)p10) > 0) {
            *(s32 *)((s32)p10 + 8) += slot20;
            *(s32 *)((s32)p10 + 12) += -0x80000;
            if (*(s32 *)(*(s32 *)slot16 + 4) != 0 == 0) {
                Func_080072f4(slot32, slot36, *(s32 *)slot12 - 20, *(s32 *)(slot12 + 4) - 52, 40, 64);
                *(s32 *)slot12 -= 8;
            } else {
                p4 = *(s32 *)(slot8 + 4);
                Call6(Func_080072f4, slot32, slot36, *(s32 *)slot12 - 26, ((s32 *)(slot12 + 4))[0] - (s32)52, 40, 64);
                *(s32 *)(slot12 + 4) += 8;
            }
        }
        if (*(s32 *)(12 + (s32)p10) < 0) {
            *(s32 *)((s32)p10 + 12) = 0;
            none = 0;
            v8 = none;
            p9b = slot48;
            v7 = 0x2010000;
            do {
                s32 tmp12;
                record = Random16();
                rec8 = Random16();
                *(s32 *)v7 = *(s32 *)p9b << 16;
                *(s32 *)(4 + v7) = (*(s32 *)(p9b + 4) - 24) << 16;
                record = Trig_Sin(rec8 & 0xffff);
                *(s32 *)(v7 + 8) = (((0x3ff & record) + 32) * record) >> 6;
                record = Func_0800231c(rec8 & 0xffff);
                *(s32 *)(v7 + 16) = -((((0x3ff & record) + 32) * record) << 1) >> 6;
                record = Random16();
                tmp12 = 7 & record;
                v8 = v8 + 1;
                *(s32 *)(v7 + 24) = tmp12 + 32;
                v7 = v7 + 28;
            } while (0x100 != v8);
            *(s32 *)(slot36 + 0x77a8) = 8;
            Func_080b50e8(145);
            Func_080b5088(*(s16 *)(*(s32 *)slot16 + 36), 4);
            ObjectGroup_UpdateMembers(*(s16 *)(*(s32 *)slot16 + 36), 7, 5, 0, 8);
        }
    }
    base6_2010000 = 0x2010000;
    none = 0;
    v8 = none;
    if (1 != 0) {
        do {
            if ((s32)(p4 = *(s32 *)(base6_2010000 + 24)) > 0) {
                s32 tmp13;
                v12 = *(s32 *)base6_2010000 + *(s32 *)(base6_2010000 + 8);
                ((s32 *)base6_2010000)[0] = ((s32 *)base6_2010000)[0] + *(s32 *)(base6_2010000 + 8);
                *(s32 *)(base6_2010000 + 24) = (s32)p4 - 1;
                v7 = ((s32 *)(base6_2010000 + 4))[0] + *(s32 *)(base6_2010000 + 16);
                *(s32 *)(base6_2010000 + 4) += ((s32 *)(base6_2010000 + 16))[0];
                tmp13 = ((*(s32 *)(base6_2010000 + 8) << 3) - *(s32 *)(base6_2010000 + 8)) << 3;
                *(s32 *)(base6_2010000 + 8) = tmp13 / 64;
                v3 = (((*(s32 *)(base6_2010000 + 16) << 3) - *(s32 *)(base6_2010000 + 16)) << 3) / 64 + 0x2000;
                *(s32 *)(base6_2010000 + 16) = (((*(s32 *)(base6_2010000 + 16) << 3) - *(s32 *)(base6_2010000 + 16)) << 3) / 64 + 0x2000;
                if (v7 > 0x700000) {
                    *(s32 *)(base6_2010000 + 16) = (-v3 + ((u32)-v3 >> 31)) >> 1;
                } else {
                    if ((u32)v12 <= 0x7effff) {
                        if (v7 >= 0) {
                            s32 tmp6;
                            s32 tmp2;
                            v0 = (s32)p4 - 1;
                            if ((s32)p4 - 1 < 0) {
                                s32 tmp4;
                                tmp4 = (s32)p4;
                                v0 = tmp4 + 6;
                            }
                            p4 = *(s32 *)(slot8 + ((1 & v8) << 2));
                            tmp2 = v0 >> 3;
                            tmp6 = tmp2 + 1;
                            Call6(Func_080072f4, slot32, slot28 + *(u16 *)(0x080ede48 + ((((v0 >> 3) + 1) << 1) - 2)), (v12 >> 16) - ((1 + ((u32)((v0 >> 3) + 1) >> 31) + (v0 >> 3)) >> 1), (v7 >> 16) - tmp6, (v0 >> 3) + 1, ((v0 >> 3) + 1) << 1);
                        }
                    }
                }
            }
            v8 = v8 + 1;
            base6_2010000 += 28;
            if (v8 == 0x100)
                break;
        } while (1 != 0);
    }
    Value2(Camera_ApplyShake, 16, 16);
    ObjectGroup_TickMemberTimers();
    ((s32 *)(slot36 + 0x7824))[0] = 1;
    Func_080030f8(1);
    v11++;
    if (88 != v11) {
        goto L_080ce130;
    }
    Call1(Scheduler_RemoveCallback, 0x80cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    p9c = rec7 + 128;
    BattleFx_EndCanvasLayer();
    p10b = base6_2010000;
    p11 = v7;
    v11 = p11;
}
