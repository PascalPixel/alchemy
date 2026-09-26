/* NONMATCHING: candidate 468 of 468 bytes, 15 differing halfwords,
 * 9 aligned edits (2026-09-26). The common message and open-message tails
 * now agree, including all branch destinations and literal words.
 * Remaining: flag 0x300 is shared in r5 across calls instead of
 * rematerialized in r0. Message selection and the halfword store now agree.
 * Service declarations retain their registered return types. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Local_0200008c(s32 mode);
void Local_02000128(s32 mode);
u32 State_RunQueryWithInterruptMasterSaved(void);
void Main_080770e8(s32 counter, s32 value);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00002930;

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(s32 (*f)(s32, s32), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call1(void (*f)(s32), s32 a0)
{
    f(a0);
}

s32 Func_02000b94(void)
{
    s32 answer;
    s32 msg;
    s32 line;

    msg = (s32)&Data_00002930;
    Engine_EventBegin();
    Engine_ActorFaceActor(8, Data_02000240_t.words[125], 0);
    if (Local_0200008c(0) == 0) {
        Engine_TaskWait(1);
    }
    if (Local_0200008c(0) == 0) {
        Local_02000128(5);
        State_RunQueryWithInterruptMasterSaved();
        if (!Value1(Engine_GameFlagIsSet, 0x173)) {
            Engine_EventSetMessage(msg + 5);
            Engine_EventOpenMessage(8, 0);
            answer = Engine_UiWorkWaitThenFinalizeCapacity(0, 0);
            if (answer == 0) {
                Main_080770e8(1000, 0);
                Engine_GameFlagSet(0x173);
                Engine_GameFlagClear(0x172);
                Engine_GameFlagClear(0x16c);
                Engine_GameFlagSet(0x202);
                /* FAKEMATCH: Select the shared-tail message before its answer store. */
                line = msg + 7;
                Data_02000240_t.halves[341][0] = answer;
                goto message;
            } else {
                Call1(Engine_GameFlagClear, 0x173);
                Engine_GameFlagSet(0x16c);
                Local_02000128(0);
                line = msg + 6;
                goto message;
            }
        } else {
            line = msg + 3;
            goto message;
        }
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x173)) {
            Local_02000128(0);
            Engine_EventSetMessage(0x293d);
            Engine_EventOpenMessage(8, 0);
            Engine_GameFlagClear(0x202);
            Call1(Engine_GameFlagClear, 0x173);
        }
        if (Engine_GameFlagIsSet(0x202)) {
            line = msg + 3;
message:
            Engine_EventSetMessage(line);
open_message:
            Call2(Engine_EventOpenMessage, 8, 0);
            goto done;
        } else if (!Engine_GameFlagIsSet(0x201) && !Engine_GameFlagIsSet(0x300)) {
            Engine_EventSetMessage(msg);
            Call2(Engine_EventOpenMessage, 8, 0);
            Engine_GameFlagSet(0x300);
        } else {
            Engine_GameFlagSet(0x300);
            if (Engine_GameFlagIsSet(0x201)) {
                Engine_EventSetMessage(msg + 2);
            } else {
                Engine_EventSetMessage(msg + 1);
            }
            Engine_EventOpenMessage(8, 0);
            if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
                if (Local_0200008c(0)) {
                    Engine_GameFlagSet(0x16c);
                    Engine_GameFlagSet(0x172);
                    if (Engine_GameFlagIsSet(0x201)) {
                        Engine_EventSetMessage(msg + 3);
                    } else {
                        Engine_EventSetMessage(msg + 4);
                    }
                    Local_02000128(1);
                    Engine_GameFlagSet(0x202);
                    goto open_message;
                } else {
                    Engine_GameFlagSet(0x205);
                }
            } else {
                Engine_EventSetMessage(msg);
                Call2(Engine_EventOpenMessage, 8, 0);
            }
        }
    }
done:
    Engine_EventEnd();
}
