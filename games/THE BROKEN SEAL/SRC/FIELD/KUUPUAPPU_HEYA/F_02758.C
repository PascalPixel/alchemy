#include "TYPES.H"

u8 *Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
void Engine_ActorEnableActionCallback();
void OverlayObject_ConfigureObject22WithResource17();
void Engine_MapCopyCellAttributes();
void Engine_TaskAddCallback();
void Engine_EventWait();
void Engine_MapCopyCellsTo();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
void Engine_ActorSetAnimation();
void KuupuappuHeya_RunVaultEvent();
void Engine_EventEnd();
void SceneState_SetWord1c0To209AndRun();
void Engine_AudioPlayCue();
void Engine_EventSetMessage();
void SceneActor_SetModeZeroAndValue();
void Main_080091b8();
void FieldScene_RunLateSequence();
void Engine_EventRequestExit();
void RunEventScript01();
void RunDialoguePromptScene();

extern s16 Data_02000240_t[][1];

struct ActorMode {
    u8 pad[100];
    u16 mode;
};

struct ActorFlags {
    u8 pad[89];
    u8 flags;
};

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers instead of sharing them across calls. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Place the room's actors and map cells for the current story state. */
s32 KuupuappuHeya_Func02002758(void)
{
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    switch (Data_02000240_t[225][0]) {
    case 5:
        {
            s32 zero = 0;

            Engine_ActorGet(8)[85] = zero;
            *(s32 *)(Engine_ActorGet(8) + 12) = zero;
            *(s32 *)(Engine_ActorGet(8) + 20) = zero;
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x850) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 2, 0x1780000, 0x1780000);
            Call3((void (*)())Engine_ActorFaceDirection, 2, 0x4000, 0);
        }
        if (Engine_GameFlagIsSet(0x856) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 2, 0, 0);
        }
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 16, 0x2180000, 0x1d00000);
            Call2((void (*)())Engine_ActorEnableActionCallback, 16, 1);
            Call3((void (*)())Engine_ActorFaceDirection, 16, 0x5000, 0);
        }
        break;
    case 7:
    case 11:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 18, 0x2380000, 0x2880000);
            Call2((void (*)())Engine_ActorEnableActionCallback, 18, 1);
            Call3((void (*)())Engine_ActorFaceDirection, 18, 0x4000, 0);
            OverlayObject_ConfigureObject22WithResource17(231, 0x2380000, 0x100000, 0x2a00000);
            Call2((void (*)())Engine_TaskAddCallback, 0x200a71d, 0xc80);
        } else if (Engine_GameFlagIsSet(0x853) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 18, 0, 0);
        }
        break;
    case 12:
        if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x852) != 0
            && Engine_GameFlagIsSet(0x853) == 0 && Engine_GameFlagIsSet(0x300) != 0) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 14, 45, 3, 1, 14, 44);
            Call2((void (*)())Engine_TaskAddCallback, 0x200aba1, 0xc80);
            break;
        }
        if (Engine_GameFlagIsSet(0x856) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 25, 0x780000, 0x2b80000);
            Call3((void (*)())Engine_ActorFaceDirection, 25, 0x8000, 0);
        }
        if (Engine_GameFlagIsSet(0x852) != 0) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 14, 45, 3, 1, 14, 44);
            if (Engine_GameFlagIsSet(0x853) == 0) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 14, 50, 3, 1, 14, 44);
                break;
            }
            ((struct ActorMode *)Engine_ActorGet(24))->mode = 5;
            ((struct ActorMode *)Engine_ActorGet(25))->mode = 4;
            Call2((void (*)())Engine_TaskAddCallback, 0x200aba1, 0xc80);
        }
        break;
    case 13:
    case 14:
        Engine_EventWait(2);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 35, 20, 2, 10);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 95, 20, 2, 10);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 35, 80, 2, 10);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 46, 21, 4, 8);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 46, 81, 4, 8);
        ((struct ActorFlags *)Engine_ActorGet(26))->flags |= 4;
        Engine_ActorSetSpriteFlags(Engine_ActorGet(26), 0);
        if (Engine_GameFlagIsSet(0x859) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 26, 0x2a40000, 0x19b0000);
            Call6((void (*)())Engine_MapCopyCellAttributes, 101, 24, 3, 4, 41, 24);
        }
        break;
    case 15:
        Engine_EventWait(2);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 44, 21, 2, 8);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 44, 81, 2, 8);
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call2((void (*)())Engine_ActorSetAnimation, 15, 2);
            Call2((void (*)())Engine_ActorSetAnimation, 16, 2);
            Engine_EventWait(1);
            Call2((void (*)())Engine_ActorSetAnimation, 17, 2);
            break;
        }
        Call3((void (*)())Engine_ActorSetPosition, 8, 0x3380000, 0x1c80000);
        if (Engine_GameFlagIsSet(0x854) != 0) {
            KuupuappuHeya_RunVaultEvent();
            Engine_EventEnd();
            break;
        }
        Call2((void (*)())Engine_ActorSetAnimation, 8, 7);
        SceneState_SetWord1c0To209AndRun();
        Engine_AudioPlayCue(17);
        Engine_EventSetMessage(0x12c3);
        Call2((void (*)())SceneActor_SetModeZeroAndValue, 8, 10);
        Engine_EventEnd();
        break;
    case 16:
        Call6((void (*)())Main_080091b8, 54, 2, 2, 8, 44, 21);
        Call6((void (*)())Main_080091b8, 54, 2, 2, 8, 44, 81);
        Call3((void (*)())Engine_ActorSetPosition, 8, 0x3380000, 0x1c80000);
        FieldScene_RunLateSequence();
        Engine_EventRequestExit(16);
        break;
    case 17:
        Call6((void (*)())Main_080091b8, 54, 2, 2, 8, 44, 21);
        Call6((void (*)())Main_080091b8, 54, 2, 2, 8, 44, 81);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Call3((void (*)())Engine_ActorSetPosition, 8, 0x3380000, 0x1c80000);
            RunEventScript01();
            RunDialoguePromptScene();
            break;
        }
        Call2((void (*)())Engine_ActorSetAnimation, 15, 2);
        Call2((void (*)())Engine_ActorSetAnimation, 16, 2);
        Engine_EventWait(1);
        Call2((void (*)())Engine_ActorSetAnimation, 17, 2);
        break;
    }
    return 0;
}
