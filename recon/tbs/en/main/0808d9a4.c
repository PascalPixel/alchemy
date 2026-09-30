/* 2026-09-29: eight minutes of permutation (--function Func_0808d9a4)
 * reached 2985 from 3650 through 26 rewrites; not kept, since the owner's
 * topology still differs (below) and message 0x970 and 0x1000 are still
 * spelled as Data_ symbols. */
/* 2026-09-29: callees carry the build's names; alchemy permute (--function
 * Func_0808d9a4) scores 3650, from 4630. */
#include "TYPES.H"

/* NONMATCHING: 1044-byte complete extent. The old draft mistook the
 * _call_via_r3 callback for a two-argument service and omitted the 0x976
 * condition-failure message; both are corrected from the listing.
 * Baseline: 389 normalized instructions versus 396. Correct callback and
 * message flow gives 392; holding the message kind from descriptor lookup
 * gives 383. A typed flags/condition/action union gives identical code.
 * Topology still differs: base-plus-500 loads fold to absolute addresses,
 * the action-table index folds into its pool address, and saved pointer
 * and condition registers differ. No byte-match claim or adoption. */

extern u8 Data_00000970[];
extern u8 Data_00001000[];
extern u8 gGameState[];
extern u16 Data_02000240_t[][2];
void WaitFrames();
void Object_SetMode();
void ObjectDispatch_ReleaseFar();
s32 UiText_ShowPositionedMessageAndWaitFar();
void UiWork_PushValueSlotFar();
void UiWindow_CreateWithLayoutBoundsFar();
void UiWork_FinalizeAndReleaseBlock16Far();
s32 PartyInventory_AddFar();
s32 GameFlag_TestFar();
void GameFlag_SetBitFar();
void GameFlag_ClearBitFar();
void Party_AdjustSixDigitCounterAFar();
s32 BattleFx_GetWeightedResult();
void BattleFx_SelectBattleCue();
void BattleParty_ApplyDrain();
s32 GameFlag_IsConditionActive();
s32 BattleFx_FindDescriptor();
void EffectRuntime_SetMode5AndPlayCue();
void EffectRuntime_SetMode7AndLaunch();
void EffectRuntime_SetMode4AndPlayCue();
void EffectRuntime_SetMode2();
s32 EffectRuntime_GetCurrentObject();
void EffectRuntime_ClearCurrentFlags();
s32 BattleFx_StartRandomParticleEmitter();
void Object_DestroyIfPresent();
void EffectRuntime_PrepareRisingObject();
void Battle_WaitMode0();
void Battle_InitializeRenderObject();
void Battle_Reset();
void BattleFx_FinishAction();
void BattleFx_PlayQueuedSound();
void Audio_PlayCue();

struct ActionDescriptor {
    s32 flags;
    u16 value_flags;
    s16 condition;
    union {
        u32 raw;
        u16 message;
        void (*callback)(s32 object_id);
    } action;
};

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

s32 Func_0808d9a4(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 p8b;
    struct ActionDescriptor *rec;
    s32 rec3;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v2;
    s32 base2_2000240;
    s32 v10;
    s32 base5_968;
    s32 v5;
    u8 *p6;
    u8 *p5;

    p8 = a0;
    p6 = *(s32 *)((s16 *)gGameState + 250);
    i = p8 - 242;
    if (i <= 5) {
        Battle_InitializeRenderObject();
        p8b = *(u8 *)(0x0809e680 + i);
        ((void (*)())UiText_ShowPositionedMessageAndWaitFar)((0x928 + p8b), 1);
        ((void (*)())UiText_ShowPositionedMessageAndWaitFar)((0x948 + p8b), 1);
    } else {
        rec = (struct ActionDescriptor *)Value2(BattleFx_FindDescriptor, 3, p8);
        if (rec == 0) {
        } else {
            v5 = ((rec->flags >> 4) & 31);
            p10 = rec->condition;
            if ((0x400 & rec->value_flags) == 0) {
                if (v5 == 0) {
                    goto L_0808da2c;
                }
                Battle_InitializeRenderObject();
                Value2(UiText_ShowPositionedMessageAndWaitFar, (v5 + 0x928), 1);
                Call1(GameFlag_SetBitFar, 0x142);
            } else {
                L_0808da2c:;
                Call1(GameFlag_ClearBitFar, 0x142);
            }
            if ((0xf000000 & (s32)rec->action.raw) == 0) {
                if ((-0x100000 & (s32)rec->action.raw) != 0x400000) {
                    goto L_0808da9a;
                }
            } else {
                if (Value1(GameFlag_IsConditionActive, p10) != 0) {
                    rec->action.callback(
                        *(s32 *)((s16 *)gGameState + 250));
                }
                if (Value1(GameFlag_TestFar, 0x142) == 0) {
                    goto L_0808dd6a;
                }
                ((void (*)())UiText_ShowPositionedMessageAndWaitFar)((v5 + 0x948), 1);
                goto L_0808dd6a;
            }
            if (Value1(GameFlag_IsConditionActive, p10) != 0) {
                ((void (*)())UiText_ShowPositionedMessageAndWaitFar)(rec->action.message, 1);
            } else {
                ((void (*)())UiText_ShowPositionedMessageAndWaitFar)(0x976, 1);
            }
            goto L_0808dd6a;
            L_0808da9a:;
            Battle_Reset();
            if (Value1(GameFlag_IsConditionActive, p10) == 0) {
            } else {
                v2 = 1;
                if ((0xf0000 & (s32)rec->action.raw) == 0x10000) {
                    v2 = 1;
                    if ((s32)p6 <= 7) {
                        v2 = 0;
                    }
                }
                if (v2 == 0) {
                } else {
                    if ((rec->flags & 0x1ff) == 19) {
                        EffectRuntime_SetMode4AndPlayCue(p8);
                    }
                    if ((-0x100000 & (s32)rec->action.raw) == 0x300000) {
                        if ((rec->flags & 0x1ff) == 19) {
                            EffectRuntime_SetMode2(p8);
                        }
                        rec8 = Value1(EffectRuntime_GetCurrentObject, p8);
                        EffectRuntime_PrepareRisingObject();
                        Audio_PlayCue(83);
                        UiWork_PushValueSlotFar(rec->action.message, 5);
                        Value2(UiText_ShowPositionedMessageAndWaitFar, (s32)Data_00000970, 3);
                        Call2(BattleParty_ApplyDrain, 0x3e7, 0);
                        UiWindow_CreateWithLayoutBoundsFar(1);
                        Audio_PlayCue(126);
                        ((void (*)())UiText_ShowPositionedMessageAndWaitFar)(((s32)Data_00000970 + 1), 1);
                        UiWork_FinalizeAndReleaseBlock16Far();
                        Object_SetMode(rec8, 2);
                        Audio_PlayCue(246);
                        Battle_WaitMode0(30);
                        ((void (*)())UiText_ShowPositionedMessageAndWaitFar)(((s32)Data_00000970 + 2), 1);
                        EffectRuntime_ClearCurrentFlags(p8);
                        if (p10 == -1) {
                            goto L_0808dd50;
                        }
                        GameFlag_SetBitFar(p10);
                        goto L_0808dd50;
                    }
                    v10 = p10;
                    if ((-0x100000 & (s32)rec->action.raw) == 0x500000) {
                        p5 = *(s32 *)0x03001ebc;
                        if ((rec->flags & 0x1ff) == 19) {
                            EffectRuntime_SetMode7AndLaunch(p8);
                        }
                        if (p10 != -1) {
                            p10b = ((s32)p10 | (s32)Data_00001000);
                            Data_02000240_t[141][0] = p10b;
                        }
                        record = Value2(BattleFx_GetWeightedResult, 99, rec->action.message);
                        *(u16 *)(((s32)p5 + 0x17c)) = record;
                        base2_2000240 = (s32)gGameState;
                        *(u8 *)((base2_2000240 + 0x22b)) = 2;
                        BattleFx_SelectBattleCue(99, rec->action.message);
                        Audio_PlayCue(*(s16 *)(0x200042e));
                        goto L_0808dd3e;
                        v10 = p10b;
                    }
                    if ((-0x100000 & (s32)rec->action.raw) == 0x200000) {
                        rec7 = BattleFx_StartRandomParticleEmitter(*(s32 *)0x02000434, 0);
                        WaitFrames(30);
                        if ((rec->flags & 0x1ff) == 19) {
                            EffectRuntime_SetMode2(p8);
                        }
                        EffectRuntime_PrepareRisingObject(rec7);
                        Audio_PlayCue(83);
                        UiWork_PushValueSlotFar(rec->action.message, 5);
                        Call2(UiText_ShowPositionedMessageAndWaitFar, 0x969, 3);
                        Party_AdjustSixDigitCounterAFar(rec->action.message);
                        if (v10 != -1) {
                            GameFlag_SetBitFar(v10);
                        }
                        ObjectDispatch_ReleaseFar(rec7);
                        goto L_0808dd50;
                    }
                    rec3 = Value2(BattleFx_StartRandomParticleEmitter, *(s32 *)0x02000434, ((s32)rec->action.raw & 0xfff));
                    WaitFrames(30);
                    rec8 = PartyInventory_AddFar(rec->action.message);
                    v5 = 0xffff;
                    if (rec8 == -1) {
                        UiWork_PushValueSlotFar(((s32)rec->action.raw & 0xfff), 2);
                        base5_968 = 0x968;
                        v5 = (base5_968 + 4);
                        Value2(UiText_ShowPositionedMessageAndWaitFar, base5_968, 1);
                        ((void (*)())UiText_ShowPositionedMessageAndWaitFar)((base5_968 + 4), 1);
                        Object_DestroyIfPresent(rec3);
                        if ((rec->flags & 0x1ff) != 19) {
                            goto L_0808dd50;
                        }
                        EffectRuntime_SetMode5AndPlayCue(p8);
                        goto L_0808dd50;
                    }
                    if ((rec->flags & 0x1ff) == 19) {
                        EffectRuntime_SetMode2(p8);
                    }
                    EffectRuntime_PrepareRisingObject(rec3);
                    Audio_PlayCue(83);
                    UiWork_PushValueSlotFar(((s32)rec->action.raw & v5), 2);
                    if (rec8 == *(s32 *)0x02000434) {
                        Call2(UiText_ShowPositionedMessageAndWaitFar, 0x96a, 3);
                    } else {
                        UiWork_PushValueSlotFar(rec8, 1);
                        Call2(UiText_ShowPositionedMessageAndWaitFar, 0x96b, 3);
                    }
                    if (v10 != -1) {
                        GameFlag_SetBitFar(v10);
                    }
                    ObjectDispatch_ReleaseFar(rec3);
                    goto L_0808dd50;
                }
                L_0808dd3e:;
                Call2(UiText_ShowPositionedMessageAndWaitFar, 0x973, 1);
                goto L_0808dd50;
                v5 = 0x2000240;
            }
            ((void (*)())UiText_ShowPositionedMessageAndWaitFar)((v5 + 0x948), 1);
            L_0808dd50:;
            BattleFx_FinishAction();
            BattleFx_PlayQueuedSound();
            goto L_0808dd6a;
        }
        Call2(UiText_ShowPositionedMessageAndWaitFar, 0x92d, 1);
        Call2(UiText_ShowPositionedMessageAndWaitFar, 0x94d, 1);
        L_0808dd6a:;
        Call1(GameFlag_ClearBitFar, 0x142);
    }
    return 0;
}
