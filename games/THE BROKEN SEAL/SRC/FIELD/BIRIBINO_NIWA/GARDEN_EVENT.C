#include "TYPES.H"
extern u8 MsgBiribinoLordMccoyDifficult[];
extern u8 MsgBiribinoLordOnlyMeet[];
extern u8 MsgBiribinoMasterQuiteCranky[];
extern u8 MsgBiribinoWhere[];
extern struct EventWork *gEventWork;

/* Main-image code reached through the overlay's import veneers, declared
 * old-style: the scene passes its arguments as plain words. */
s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Engine_EventBegin();
void Engine_EventEnd();
void Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
u8 *Engine_ActorGet();
void Engine_ActorSetSpeed();
void Engine_ActorShowEmote();
void Engine_ActorWalkToAndWait();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
s32 Engine_ActorSetPosition();
void Engine_ObjectSetTargetAndCallback();
void FieldScene_OpenGate();

/* The guard's action table, laid out after the code. */
extern u8 BiribinoNiwa_GuardScript[];

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments. */

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void BiribinoNiwa_RunGardenEvent(void)
{
    u8 *record;

    Engine_EventBegin();
    if (Value1(Engine_GameFlagIsSet, 0x84a) != 0) {
        if (Value1(Engine_GameFlagIsSet, 0x304) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x201) == 0) {
                Call1(Engine_EventSetMessage, (s32)MsgBiribinoWhere);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Call3(Engine_ActorShowEmote, 12, 0x107, 40);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Engine_ActorRunRepeatedMotion(12, 2);
                Call1(Engine_GameFlagSet, 0x201);
            }
            Call1(Engine_EventSetMessage, (s32)MsgBiribinoMasterQuiteCranky);
            Engine_EventShowMessage(12, 0);
            goto L_0200041c;
        }
        Call1(Engine_EventSetMessage, (s32)MsgBiribinoLordMccoyDifficult);
        Engine_EventShowMessage(12, 0);
    } else {
        Call1(Engine_EventSetMessage, (s32)MsgBiribinoLordOnlyMeet);
        Value2(Engine_EventOpenMessage, 12, 0);
        if (Value2(Engine_EventChooseYesNo, 0, 0) != 0) {
            goto L_02000408;
        }
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
        Engine_EventShowMessageAndWait(12, 0, 10);
        record = Engine_ActorGet(0);
        if (*(s32 *)((s32)record + 16) <= 0x10dffff) {
            Call3(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
            Call3(Engine_ActorWalkToAndWait, 0, 0x15a, 0x112);
            Call3(Engine_ActorWalkToAndWait, 0, 0x148, 0x11a);
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        }
        Call3(Engine_ActorFaceDirection, 11, 0x1000, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x7000, 20);
        Call3(Engine_ActorShowEmote, 11, 0x102, 20);
        Engine_ActorStartRepeatedMotion(11, 1);
        Call3((void (*)())Engine_EventShowMessageAndWait, 11, 0, 10);
        Call3(Engine_ActorShowEmote, 12, 0x108, 60);
        Engine_ActorStartRepeatedMotion(12, 1);
        Engine_EventShowMessageAndWait(12, 0, 20);
        Engine_ActorSetAnimation(11, 3);
        Engine_ActorSetAnimationAndWait(12, 3);
        Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
        Engine_ActorStartRepeatedMotion(11, 1);
        Engine_EventShowMessageAndWait(11, 0, 20);
        Call3(Engine_ActorFaceDirection, 11, 0xf000, 0);
        Call3(Engine_ActorSetSpeed, 12, 0x10000, 0x8000);
        *(u8 *)(Engine_ActorGet(12) + 90) &= 254;
        Call3(Engine_ActorWalkToAndWait, 12, 0x15a, 0x107);
        Engine_EventWait(1);
        {
            u8 *record = Engine_ActorGet(12);
            /* FAKEMATCH: the flag byte is read through a volatile access. */
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
        Call3(Engine_ActorSetSpeed, 11, 0x9999, 0x4ccc);
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 0x107);
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 252);
        Call3(Engine_ActorFaceDirection, 11, 0xc000, 10);
        FieldScene_OpenGate();
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 246);
        Value3(Engine_ActorSetPosition, 11, 0, 0);
        /* FAKEMATCH: the do/while keeps this call after the argument setup
         * of the previous one. */
        do {
            Call1(Engine_GameFlagSet, 0x84a);
        } while (0);
    }
    Call3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)BiribinoNiwa_GuardScript);
    goto L_0200041c;
    L_02000408:;
    Engine_EventShowMessage(12, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x3000, 10);
    L_0200041c:;
    Engine_EventEnd();
}
