/* NONMATCHING: 612 bytes, candidate 652, 313 differing halfwords, 213
 * halfword edits (2026-09-25). KuupuappuHeya_Func020048c8, meant for
 * FIELD/KUUPUAPPU_HEYA/F_048C8.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: decompile first pass of KuupuappuHeya push/snap
 * interaction (FindEntryAtPosition + SnapToNearestStop); far from exact
 * WALL: structural-topology: decompile far; needs hand rewrite from inspect
 * --asm */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x02004b20: 0x03001ebc  SceneWorkCell
 *   0x02004b24: 0xfd92f005
 *   0x02004b28: 0x0200e4f8
 */

s32 SceneData_FindEntryAtPosition();
s32 KuupuappuHeya_SnapToNearestStop();
s32 SceneActor_CheckTileFreeOfKinds();
void SceneActor_ApplyScaledBytePairPosition();
s32 Math_Atan2();
void Engine_ObjectSetAnimation();
s32 Engine_RandomNext();
s32 Engine_ActorLookup();
void Engine_ActorSetAttachedEffect();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void KuupuappuHeya_Func020048c8(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 rec;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 record;
    s32 value;
    s32 r5;
    s32 none;
    s32 v9;
    s32 v3;
    s32 slot2;

    rec2 = Engine_ActorLookup(0);
    v9 = none;
    p11 = *(s32 *)0x03001ebc;
    Value1(Engine_ActorLookup, 2);
    none = 0;
    rec4 = Value1(SceneData_FindEntryAtPosition, (none + 8));
    if (rec4 != 0) {
        if (*(s32 *)(none + 56) == -0x80000000) {
            *(u16 *)0x00000002 = *(s16 *)(rec2 + 6);
            value = Math_Atan2((*(s32 *)(none + 16) - *(s32 *)(rec2 + 16)), (*(s32 *)(none + 8) - *(s32 *)(rec2 + 8)));
            if (*(s16 *)((0x19c + p11)) > 0) {
                if (((((*(s32 *)(none + 8) - *(s32 *)(rec2 + 8)) >> 16) * ((*(s32 *)(none + 8) - *(s32 *)(rec2 + 8)) >> 16)) + (((*(s32 *)(none + 16) - *(s32 *)(rec2 + 16)) >> 16) * ((*(s32 *)(none + 16) - *(s32 *)(rec2 + 16)) >> 16))) > 0x190) {
                    goto L_0200497e;
                }
                if ((((*(u16 *)0x00000002 - ((u32)(((value << 16) >> 16) << 16) >> 16)) << 16) >> 16) <= -0x26d0ffb) {
                    goto L_0200497e;
                }
                if ((((*(u16 *)0x00000002 - ((u32)(((value << 16) >> 16) << 16) >> 16)) << 16) >> 16) >= 0x1000) {
                    goto L_0200497e;
                }
            } else {
                L_0200497e:;
                if (((((*(s32 *)(none + 8) - *(s32 *)(rec2 + 8)) >> 16) * ((*(s32 *)(none + 8) - *(s32 *)(rec2 + 8)) >> 16)) + (((*(s32 *)(none + 16) - *(s32 *)(rec2 + 16)) >> 16) * ((*(s32 *)(none + 16) - *(s32 *)(rec2 + 16)) >> 16))) > 64) {
                    *(u16 *)0x00000002 = *(s16 *)(none + 6);
                }
            }
            rec7 = KuupuappuHeya_SnapToNearestStop(rec4, 2);
            if (Value0(SceneActor_CheckTileFreeOfKinds) == 0) {
                SceneActor_ApplyScaledBytePairPosition(none, rec7);
                Engine_ObjectSetAnimation(none, 2);
            } else {
                Engine_ObjectSetAnimation(none, 1);
            }
        }
    }
    rec = Value1(Engine_ActorLookup, 24);
    rec4 = Value1(SceneData_FindEntryAtPosition, (rec + 8));
    if (rec4 != 0) {
        v9 = none;
        if (*(s32 *)(rec + 56) == -0x80000000) {
            value = Engine_RandomNext();
            *(u16 *)(&slot2) = (((u32)((((((u32)(value << 1) >> 16) << 1) + ((u32)(value << 1) >> 16)) << 29) + -0x30000000) >> 16) + *(u16 *)(rec + 6));
            rec7 = KuupuappuHeya_SnapToNearestStop(rec4, &slot2);
            if (Value0(SceneActor_CheckTileFreeOfKinds) != 0) {
                *(u16 *)(&slot2) = (*(u16 *)(rec + 6) + 0x8000);
                rec7 = KuupuappuHeya_SnapToNearestStop(rec4, &slot2);
                if (Value0(SceneActor_CheckTileFreeOfKinds) == 0) {
                    Engine_ActorSetAttachedEffect(24, 2);
                    goto L_02004a3c;
                }
                Engine_ObjectSetAnimation(rec, 4);
                v9 = 1;
            } else {
                L_02004a3c:;
                SceneActor_ApplyScaledBytePairPosition(rec, r5);
                Engine_ObjectSetAnimation(rec, 2);
                v9 = none;
            }
        }
    }
    rec = Value1(Engine_ActorLookup, 25);
    rec4 = Value1(SceneData_FindEntryAtPosition, (rec + 8));
    if (rec4 != 0) {
        if (*(s32 *)(rec + 56) == -0x80000000) {
            value = Value0(Engine_RandomNext);
            *(u16 *)(&slot2) = (((u32)((((((u32)((value << 1) + value) >> 16) << 1) + ((u32)((value << 1) + value) >> 16)) << 28) + -0x30000000) >> 16) + *(u16 *)(rec + 6));
            rec7 = KuupuappuHeya_SnapToNearestStop(rec4, &slot2);
            if (Value0(SceneActor_CheckTileFreeOfKinds) != 0) {
                *(u16 *)(&slot2) = (*(u16 *)(rec + 6) + 0x8000);
                rec7 = KuupuappuHeya_SnapToNearestStop(rec4, &slot2);
                if (Value0(SceneActor_CheckTileFreeOfKinds) == 0) {
                    Engine_ActorSetAttachedEffect(25, 2);
                    goto L_02004ad2;
                }
                Engine_ObjectSetAnimation(rec, 4);
                v9 = (v9 + 2);
            } else {
                L_02004ad2:;
                SceneActor_ApplyScaledBytePairPosition(rec, r5);
                Engine_ObjectSetAnimation(rec, 2);
            }
        }
    }
    if (v9 != 0) {
        v3 = (*(u16 *)0x0200e4f8 + 1);
        *(u16 *)0x0200e4f8 += 1;
        if ((u32)(v3 << 16) <= 0x1d0000) {
            goto L_02004b0e;
        }
        *(u16 *)((0x182 + p11)) = (v9 + 200);
    } else {
        *(u16 *)0x0200e4f8 = v9;
    }
    L_02004b0e:;
    p8 = (v3 << 16);
    p9 = none + 8;
    p10 = &slot2;
}
