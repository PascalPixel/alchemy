#include "TYPES.H"

extern u8 Data_0200ac00[];

s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
void Engine_GameFlagSet();
void Engine_EventRequestExit();
void SceneState_SetValues352_365_2116_2117_40();
s32 Engine_ActorGet();
void Engine_MapCopyCellsTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void Main_0808a2c0();
void Main_0808a2c8();
s32 Engine_MapCopyCellAttributes();
void Main_0808a168();
void Scene_RunExtendedActorSequence();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAnimation();
void HaidiaIe_RunScene015B4();
void HaidiaIe_RunScriptScene();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Main_0808a2e0();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 HaidiaIe_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 v5;

    if (Value1(Engine_GameFlagIsSet, 0x90b) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (Value1(Engine_GameFlagIsSet, 0x90c) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0, 0);
    }
    if (Value1(Engine_GameFlagIsSet, 0x90d) != 0) {
        Engine_ActorSetPosition(10, 0, 0);
    }
    switch (Data_02000240_t[225][0]) {
    case 98:
        Engine_GameFlagSet(32);
        Engine_EventRequestExit(50);
        return 0;
    case 99:
        SceneState_SetValues352_365_2116_2117_40();
        return 0;
    case 97:
        HaidiaIe_RunScene015B4();
        return 0;
    }
    v5 = 192;
    record = Engine_ActorGet(8);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1(Engine_ActorGet, 9);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1(Engine_ActorGet, 10);
    *(s32 *)(record + 28) = (v5 << 9);
    if (Value1(Engine_GameFlagIsSet, 0x87a) != 0) {
        Call6(Engine_MapCopyCellsTo, 97, 2, 80, 5, 2, 2);
        Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
            Main_0808a2c0();
            Main_0808a2c8();
            Call6(Engine_MapCopyCellAttributes, 21, 38, 1, 1, 18, 41);
            record = Value1(Engine_GameFlagIsSet, 0x840);
            if (record == 0) {
                goto L_02000aaa;
            }
            Engine_ActorSetPosition(17, 0, 0);
            Engine_ActorSetPosition(18, 0, 0);
            Call3(Main_0808a168, 19, 0x10000, (s32)Data_0200ac00);
        } else {
            if (Value1(Engine_GameFlagIsSet, 0x815) != 0) {
                Call3(Engine_ActorSetPosition, 16, 0xb40000, 0x2380000);
                Call6(Engine_MapCopyCellsTo, 92, 2, 80, 5, 2, 2);
                Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
        }
        L_02000aaa:;
        if (Data_02000240_t[225][0] == 12) {
            Scene_RunExtendedActorSequence();
        } else {
            if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
                record = Engine_ActorGet(20);
                *(s32 *)(record + 24) = 0x4ccc;
                *(s32 *)(record + 28) = 0x4ccc;
                record = Engine_ActorGet(20);
                Engine_ActorSetSpriteFlags(record, 0);
                record = Engine_ActorGet(21);
                *(s32 *)(record + 24) = 0x9999;
                *(s32 *)(record + 28) = 0x9999;
                Engine_ActorSetAnimation(13, 5);
            } else {
                if (Value1(Engine_GameFlagIsSet, 0x815) != 0) {
                    Call3(Engine_ActorSetPosition, 21, 0x14b0000, 0xf90000);
                    record = Engine_ActorGet(21);
                    Engine_ActorSetSpriteFlags(record, 0);
                }
            }
            if (Value1(Engine_GameFlagIsSet, 0x840) != 0) {
                Engine_ActorSetPosition(26, 0, 0);
                Engine_ActorSetPosition(22, 0, 0);
            }
            if (Data_02000240_t[225][0] == 19) {
                HaidiaIe_RunScene015B4();
            } else {
                if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
                    record = Value1(Engine_GameFlagIsSet, 0x842);
                    if (record == 0) {
                        goto L_02000b6a;
                    }
                    HaidiaIe_RunScriptScene();
                } else {
                    L_02000b6a:;
                    if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
                        Engine_EventOpenScreen();
                        Engine_EventWaitForScreen();
                        Main_0808a2e0();
                    }
                }
            }
        }
    }
    L_02000b80:;
    return 0;
}
