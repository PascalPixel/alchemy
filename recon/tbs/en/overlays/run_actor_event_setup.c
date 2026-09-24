#include "TYPES.H"

void Main_0808a5e0();
void Engine_ActorSetAnimation();
void Engine_MapCopyCellAttributes();
void ConfigureAndPlaceActorFourteen();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
s32 Engine_GameFlagClear();
void FieldScene_RunScene3a2SequenceA();
u8 * Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void ActorPresentation_PrepareActorFourteenWithCallback();
void FieldScene_SetSlot15Byte89AndRunStep();
void Engine_GameFlagSet();
void Engine_MapCopyCellsTo();
s32 Engine_ActorEnableActionCallback();
void Scene_RunActorExchange();


extern u8 Data_0000004a[];
extern u8 Data_00000058[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* NONMATCHING: 744 of 744 bytes, 2 halfword edits (2026-09-24). The reference
 * loads the Data_02000240 base after the event-work store; here it is
 * scheduled first. A do-while wrap, volatile spellings and a local for the
 * area id did not move it. */
s32 Func_020011b0(void)
{
    u32 i;
    u8 *record;

    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x100;
    if (Data_02000240_t[224][0] == (s32)Data_00000058) {
        Main_0808a5e0(169);
        Engine_ActorSetAnimation(11, 5);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetAnimation(14, 2);
        Call6(Engine_MapCopyCellAttributes, 21, 9, 1, 1, 21, 73);
        ConfigureAndPlaceActorFourteen();
        if (Value1(Engine_GameFlagIsSet, 0x8b2) != 0) {
            Call3(Engine_ActorSetPosition, 13, 0x880000, 0x1000000);
            Engine_ActorFaceDirection(13, 0, 0);
        }
        if (Data_02000240_t[225][0] == 2) {
            Call1(Engine_GameFlagClear, 0x12f);
            goto L_02001456;
        }
        if (Data_02000240_t[225][0] != 3) {
            goto L_02001456;
        }
        if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
            goto L_02001456;
        }
        FieldScene_RunScene3a2SequenceA();
    } else {
        if (Data_02000240_t[224][0] != (s32)Data_0000004a) {
        } else {
            record = Engine_ActorGet(14);
            Engine_ActorSetSpriteFlags((s32)record, 0);
            {
                u8 *record = Engine_ActorGet(14);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
            if (Value1(Engine_GameFlagIsSet, 0x200) != 0) {
                Engine_ActorSetAnimation(14, 5);
                ActorPresentation_PrepareActorFourteenWithCallback();
            }
            if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
                Engine_ActorSetAnimation(15, 4);
                FieldScene_SetSlot15Byte89AndRunStep();
            }
            if ((u32)(((u16)Data_02000240_t[225][0] - 4) << 16) <= 0x10000) {
                Call1(Engine_GameFlagClear, 0x12f);
            }
            if (Value1(Engine_GameFlagIsSet, 0x89a) == 0) {
                if (Value1(Engine_GameFlagIsSet, 0x895) == 0) {
                    if (Value1(Engine_GameFlagIsSet, 0x8b2) == 0) {
                        Engine_ActorSetPosition(10, 0, 0);
                    }
                }
            }
            if (Value1(Engine_GameFlagIsSet, 0x8b2) == 0) {
                if (Value1(Engine_GameFlagIsSet, 0x895) != 0) {
                    if (Data_02000240_t[225][0] == 2) {
                        Engine_ActorSetPosition(11, 0, 0);
                        Call1(Engine_GameFlagSet, 0x8b2);
                        Call1(Engine_GameFlagSet, 0x8b3);
                        Engine_ActorSetPosition(10, 0, 0);
                    }
                }
            }
            if (Value1(Engine_GameFlagIsSet, 0x8b2) != 0) {
                Call6(Engine_MapCopyCellsTo, 54, 21, 53, 21, 1, 2);
                Call6(Engine_MapCopyCellAttributes, 18, 20, 1, 3, 17, 21);
                Call6(Engine_MapCopyCellsTo, 44, 18, 43, 17, 1, 1);
                Call6(Engine_MapCopyCellAttributes, 8, 17, 1, 1, 7, 17);
            }
            if (Value1(Engine_GameFlagIsSet, 0x895) != 0) {
                if (Value1(Engine_GameFlagIsSet, 0x8b2) == 0) {
                    Engine_ActorSetPosition(12, 0, 0);
                    Engine_ActorSetPosition(13, 0, 0);
                    Call3(Engine_ActorSetPosition, 8, 0xc00000, 0x1080000);
                    Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
                    Call3(Engine_ActorSetPosition, 10, 0xb80000, 0x1300000);
                    Call3(Engine_ActorFaceDirection, 8, 0x5000, 0);
                    Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
                    Value2(Engine_ActorEnableActionCallback, 9, 0x2009730);
                    record = Engine_ActorGet(9);
                    *(s32 *)((s32)record + 24) = -0x10000;
                }
            }
            if (Value1(Engine_GameFlagIsSet, 0x8b2) == 0) {
                Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
                Value2(Engine_ActorEnableActionCallback, 9, 0x2009730);
                record = Engine_ActorGet(9);
                *(s32 *)((s32)record + 24) = -0x10000;
            }
            if (Data_02000240_t[225][0] == 5) {
                if (Value1(Engine_GameFlagIsSet, 0x8b1) == 0) {
                    if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
                        if (Value1(Engine_GameFlagIsSet, 0x8b2) == 0) {
                            Scene_RunActorExchange();
                        }
                    }
                }
            }
        }
    }
    L_02001456:;
    return 0;
}
