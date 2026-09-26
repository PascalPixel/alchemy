/* NONMATCHING: 616 bytes, candidate 676, 254 differing halfwords, 147
 * halfword edits (2026-09-25). VinasuHeya_Func0200247c, meant for
 * FIELD/VINASU_HEYA/F_0247C.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: 254 HW structural; size +60; multi-branch push-puzzle over
 * actors 10-13 with flag 0x200+i, sprite-mask eject, z=19 swap+camera pan;
 * SettleBlocks/FLAG_301 sibling shape needs fresh FieldActor rewrite */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x020026e0: 0x0200a2a5  Local_020022a4 (overlay function)
 */

s32 OverlayObject_SpawnWithMode14();
void OverlayObject_WaitUntilIdle();
void VinasuHeya_Func020022c8();
void SceneActor_PickHighestSlotAtSameTileAndRelease();
void Engine_EventBegin();
void Engine_MapCopyCellAttributes();
s32 Engine_GameFlagIsSet();
s32 Engine_ActorGet();
void Engine_GameFlagSet();
void Engine_ObjectDispatchRelease();
void Engine_ActorSetPosition();
void Engine_ActorSetSpritePriority();
void Engine_EventEnd();
s32 Engine_CameraSetSpeed();
s32 Engine_CameraMoveTo();
void Engine_CameraWaitForMove();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void VinasuHeya_Func0200247c(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p7;
    s32 p9;
    s32 p9b;
    s32 rec;
    u8 *rec8;
    u8 *record;
    s32 r3;
    s32 none;
    s32 v8;
    s32 base11_a;
    s32 v5;
    s32 base5_0;
    s32 v7;
    s32 v0;
    s32 v3;
    s32 slot16;
    s32 slot12;
    s32 slot8;
    s32 slot20;

    slot16 = 0;
    slot12 = slot16;
    Engine_EventBegin();
    none = 0;
    slot8 = &slot20;
    base11_a = 10;
    v8 = none;
    L_020024a4:;
    rec8 = Value1(Engine_ActorGet, base11_a);
    p9 = (*(s32 *)((s32)rec8 + 8) >> 20);
    if (p9 == 13) {
        p10 = (*(s32 *)((s32)rec8 + 16) >> 20);
        if (p10 != 7) {
            goto L_0200250a;
        }
        rec = Value1(Engine_GameFlagIsSet, (0x200 + v8));
        if (rec != 0) {
            goto L_0200250a;
        }
        OverlayObject_WaitUntilIdle((s32)rec8);
        Engine_GameFlagSet((0x200 + v8));
        {
            u8 value = *(volatile u8 *)&rec8[35];
        
            rec8[35] = (u8)(value | 2);
        }
        rec8[89] = rec;
        *(u8 *)((((s32)rec8 + 89) - 4)) = rec;
        Call6(Engine_MapCopyCellAttributes, 4, 19, 1, 1, p9, p10);
    } else {
        L_0200250a:;
        if ((12 & *(u8 *)(*(s32 *)((s32)rec8 + 80) + 9)) == 12) {
            record = Value1(Engine_GameFlagIsSet, (0x200 + v8));
            if ((s32)record != 0) {
                goto L_020025b0;
            }
            v5 = 0;
            Engine_ActorSetSpritePriority(base11_a, 1);
            v7 = v5;
            *(s32 *)((s32)rec8 + 68) = v5;
            if ((*(s32 *)((s32)rec8 + 16) >> 20) <= 12) {
                record = Value4(OverlayObject_SpawnWithMode14, *(s32 *)((s32)rec8 + 8), 0, 0xe00000, 253);
                slot16 = (s32)record;
                record = Value4(OverlayObject_SpawnWithMode14, *(s32 *)((s32)rec8 + 8), 0, 0xf00000, 253);
                slot12 = (s32)record;
            }
            OverlayObject_WaitUntilIdle((s32)rec8);
            Engine_ActorSetPosition(base11_a, 0, 0);
            Engine_ObjectDispatchRelease(slot16);
            Engine_ObjectDispatchRelease(slot12);
            Engine_GameFlagSet((0x200 + v8));
            goto L_020026ca;
            L_0200257e:;
            record = Engine_ActorGet((v5 + 10));
            *(s32 *)(slot8 + 8) = *(s32 *)((s32)rec8 + 8);
            *(s32 *)(slot8 + 12) = *(s32 *)((s32)rec8 + 12);
            *(s32 *)(slot8 + 16) = *(s32 *)((s32)rec8 + 16);
            *(s32 *)((s32)rec8 + 8) = *(s32 *)((s32)record + 8);
            *(s32 *)((s32)rec8 + 12) = *(s32 *)((s32)record + 12);
            *(s32 *)((s32)rec8 + 16) = *(s32 *)((s32)record + 16);
            *(s32 *)((s32)record + 8) = *(s32 *)(slot8 + 8);
            *(s32 *)((s32)record + 12) = *(s32 *)(slot8 + 12);
            *(s32 *)((s32)record + 16) = *(s32 *)(slot8 + 16);
        } else {
            L_020025b0:;
            if ((*(s32 *)((s32)rec8 + 16) >> 20) != 19) {
                goto L_020026bc;
            }
            record = Value1(Engine_GameFlagIsSet, (0x200 + v8));
            if ((s32)record != 0) {
                goto L_020026bc;
            }
            *(s32 *)((s32)rec8 + 60) = -0x80000000;
            *(s32 *)((s32)rec8 + 20) = (s32)record;
            *(s32 *)((s32)rec8 + 40) = (s32)record;
                base5_0 = 0;
            rec8[85] = (s32)record;
            *(u16 *)(((s32)rec8 + 85) + 15) = (s32)record;
            p7 = v8;
            v7 = p7;
            if (0 < p7) {
                do {
                    record = Value1(Engine_GameFlagIsSet, (base5_0 + 0x200));
                    if ((s32)record == 0) {
                        goto L_0200257e;
                    }
                    base5_0 = (base5_0 + 1);
                } while (base5_0 < p7);
            }
        }
        record = Engine_ActorGet((v7 + 10));
        *(s32 *)((s32)record + 60) = -0x80000000;
        *(s32 *)((s32)record + 20) = 0;
        *(s32 *)((s32)record + 40) = 0;
        record[85] = 0;
        {
            s32 shown = 0;
        
            *(u16 *)(((s32)record + 85) + 15) = shown;
        }
        Value2(Engine_CameraSetSpeed, 0x30000, 0x6000);
        *(u8 *)(Engine_CameraMoveTo() + 85) = 0;
        Call4(Engine_CameraMoveTo, 0x880000, 0x80000, 0x1580000, 1);
        Engine_CameraWaitForMove();
        SceneActor_PickHighestSlotAtSameTileAndRelease((v7 + 10));
        record = Value1(Engine_ActorGet, (v7 + 10));
        if ((*(s32 *)((s32)record + 8) >> 20) == 6) {
            *(u16 *)(Engine_ActorGet(8) + 100) += 1;
            v0 = (Engine_ActorGet(9) + 100);
            v3 = (*(u16 *)(Engine_ActorGet(9) + 100) - 1);
        } else {
            *(u16 *)(Engine_ActorGet(8) + 100) = (*(u16 *)(Engine_ActorGet(8) + 100) - 1);
            v0 = (Engine_ActorGet(9) + 100);
            v3 = (*(u16 *)(Engine_ActorGet(9) + 100) + 1);
        }
        *(u16 *)(v0) = v3;
        record = Engine_ActorGet((v7 + 10));
        *(s32 *)((s32)record + 108) = 0x200a2a5;
        VinasuHeya_Func020022c8(40);
        {
            u8 *record = Engine_ActorGet((v7 + 10));
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
        Engine_GameFlagSet((v7 + 0x200));
        goto L_020026ca;
        L_020026bc:;
        v8 = (v8 + 1);
        base11_a = (base11_a + 1);
        if (v8 <= 3) {
            goto L_020024a4;
        }
    }
    L_020026ca:;
    Engine_EventEnd();
}
