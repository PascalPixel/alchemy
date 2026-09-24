#include "TYPES.H"

void Local_0200288c();
void Engine_EventBegin();
s32 ColossoLogRollingStage_RunStateInteraction();
void Engine_EventSetMessage();
void Local_02000188();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_EventShowMessage();
void Local_020001b4();
s32 Local_02003640();
s32 Engine_ActorFaceDirection();
void Local_020001c8();
void Engine_ActorSetSpeed();
void ColossoLogRollingStage_PositionScaledObject();
void Engine_TaskWait();
void Local_02000b30();
void Engine_ActorShowEmote();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void ColossoLogRollingStage_InitializeStateInteraction();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();


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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Colosso log stage: unless the stage is already cleared, show the
 * introduction, wait up to 240 frames for the log to settle and hand over to
 * the stage. */
void KorosseoMaruta_RunStageIntro(s32 a0)
{
    s32 p8;
    s32 rec8;

    if (Data_02000240_t[225][0] == 2) {
        Local_0200288c();
    } else {
        Engine_EventBegin();
        rec8 = Value2(ColossoLogRollingStage_RunStateInteraction, a0, 3);
        if (rec8 == 0) {
            p8 = *(s32 *)0x03001ebc;
            Call1(Engine_EventSetMessage, 0x20bb);
            Local_02000188();
            Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
            Call4(Engine_CameraMoveTo, 0x2680000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            Local_020001b4();
            Engine_EventWait(60);
            Engine_EventShowMessage(a0, 0);
            Value3(Local_02003640, 0, 0x1f8, 200);
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            Local_020001c8();
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionScaledObject(0, 0x2a8, 200);
            if (*(s16 *)(p8 + 0x182) != 5) {
                do {
                    Engine_TaskWait(1);
                    if (++rec8 > 239) {
                        break;
                    }
                } while (*(s16 *)(p8 + 0x182) != 5);
            }
            Local_02000b30();
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            Call3(Engine_ActorShowEmote, 0, 0x103, 60);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 3);
            {
                /* FAKEMATCH: the slot pointer and word temporary order the address before the zero */
                u8 *slot = (u8 *)(p8 + 0x182);
                s32 shown = 0;

                *(u16 *)slot = shown;
            }
        } else {
            if (rec8 == 1) {
                Call1(Engine_EventSetMessage, 0x20ba);
                Engine_EventShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec8, a0, 3);
        Engine_EventEnd();
    }
}
