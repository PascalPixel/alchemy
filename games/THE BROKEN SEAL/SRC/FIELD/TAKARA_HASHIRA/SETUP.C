#include "TYPES.H"

void FieldScene_RedrawActorFootprint();
void Engine_MapCopyCells();
s32 Engine_GameFlagIsSet();
void Engine_MapCopyCellsTo();
void SceneActor_ApplyPlacementQueryAndTag();
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Engine_ActorGet();
void Main_0808a408();
void SceneState_ClearWord24AndObjectByte62();
void OverlayObject_SetCallbackAndMode2();
void Local_02001d84();
void Engine_EventWait();
void Engine_TaskAddCallback();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetAnimation();
void Engine_ActorSetChildValue();
void TakaraHashira_DropActorTen();
void TakaraHashira_Func020025f8();
void FieldScene_RunScene3b3_0200263c();


extern u8 Data_00000074[];
extern u8 Data_00000077[];
extern u8 Data_00000079[];
extern u8 Data_0000007a[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 TakaraHashira_SetupArea(void)
{
    u32 i;
    u8 *record;
    s32 v1;
    s32 v2;
    s32 base5_8;
    s32 v5;
    s32 v0;

    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x204;
    if (Data_02000240_t[224][0] == (s32)Data_00000074) {
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000077) {
            Call6(Engine_MapCopyCells, 32, 0, 64, 32, 0, 64);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            FieldScene_RedrawActorFootprint(12);
            FieldScene_RedrawActorFootprint(13);
            FieldScene_RedrawActorFootprint(14);
            FieldScene_RedrawActorFootprint(15);
            if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
                goto L_020029fa;
            }
            if (Value1(Engine_GameFlagIsSet, 0x200) == 0) {
                goto L_020029fa;
            }
            Call6(Engine_MapCopyCellsTo, 79, 34, 84, 24, 1, 2);
            Call6(Engine_MapCopyCellsTo, 0, 32, 32, 0, 32, 32);
            Call6(Engine_MapCopyCellsTo, 32, 32, 64, 0, 32, 32);
            SceneActor_ApplyPlacementQueryAndTag(9);
            SceneActor_ApplyPlacementQueryAndTag(10);
            SceneActor_ApplyPlacementQueryAndTag(11);
            SceneActor_ApplyPlacementQueryAndTag(12);
            SceneActor_ApplyPlacementQueryAndTag(13);
            SceneActor_ApplyPlacementQueryAndTag(14);
            SceneActor_ApplyPlacementQueryAndTag(15);
            Call6(Engine_MapCopyCellAttributes, 24, 3, 1, 1, 24, 8);
            goto L_020029fa;
        } else {
            if (Data_02000240_t[224][0] != (s32)Data_00000079) {
                goto L_02002922;
            }
            OverlayObject_CreateConfiguredObject(0x2480000, 0, 0xc80000, 223);
            if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
                *(u8 *)(Engine_ActorGet(0) + 98) = 1;
            }
            Main_0808a408(0);
            if (*(u8 *)(Engine_ActorGet(0) + 98) == 0) {
                SceneState_ClearWord24AndObjectByte62();
            }
            OverlayObject_SetCallbackAndMode2(8);
            OverlayObject_SetCallbackAndMode2(9);
            OverlayObject_SetCallbackAndMode2(10);
            OverlayObject_SetCallbackAndMode2(11);
            {
                s32 *entry = (s32 *)0x200b6d0;

                for (v1 = 0; (u32)v1 <= 3; v1++) {
                    entry[0] = 0;
                    entry[1] = 0;
                    entry[2] = 0;
                    entry[4] = v1 + 0x200;
                    entry += 5;
                }
            }
            Local_02001d84();
            Engine_EventWait(1);
            Call2(Engine_TaskAddCallback, 0x200a6b9, 0xc80);
            if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
                goto L_020029fa;
            }
            for (base5_8 = 8; (u32)base5_8 <= 11; base5_8++) {
                record = Value1(Engine_ActorGet, base5_8);
                v2 = *(s32 *)(record + 8) >> 20;
                if (v2 == 37) {
                    v0 = *(s32 *)(record + 16) >> 20;
                    if (v0 == 9) {
                        Call6(Engine_MapCopyCellAttributes, 27, 8, 1, 1, v2, v0);
                        break;
                    }
                }
            }
        }
        goto L_020029fa;
        L_02002922:;
        if (Data_02000240_t[224][0] == (s32)Data_0000007a) {
            Engine_ActorSetAnimation(10, 2);
            Engine_ActorSetChildValue(10, 6);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            v5 = 0;
            *(u8 *)(Engine_ActorGet(8) + 85) = v5;
            *(u8 *)(Engine_ActorGet(9) + 85) = v5;
            TakaraHashira_DropActorTen();
            TakaraHashira_Func020025f8(11);
            TakaraHashira_Func020025f8(12);
            TakaraHashira_Func020025f8(13);
            FieldScene_RunScene3b3_0200263c(11);
            FieldScene_RunScene3b3_0200263c(12);
            FieldScene_RunScene3b3_0200263c(13);
            record = Value1(Engine_ActorGet, 13);
            *(s32 *)((s32)record + 108) = v5;
            TakaraHashira_Func020025f8(14);
            {
                u8 *record = Engine_ActorGet(14);
                /* FAKEMATCH: retain the flag read before its merge. */
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 8);
            }
            if (Value1(Engine_GameFlagIsSet, 0x202) == 0) {
                v5 = 192;
                record = Engine_ActorGet(13);
                *(s32 *)((s32)record + 24) = (v5 << 9);
                record = Value1(Engine_ActorGet, 13);
                *(s32 *)((s32)record + 28) = (v5 << 9);
                record = Value1(Engine_ActorGet, 13);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                record = Value1(Engine_ActorGet, 14);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                Call6(Engine_MapCopyCellAttributes, 26, 12, 1, 1, 22, 16);
            }
        }
    }
    L_020029fa:;
    return 0;
}
