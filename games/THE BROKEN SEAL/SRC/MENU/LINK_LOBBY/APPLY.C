/* Handle battle applications and reopen the lobby attendant's dialogue. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgLobbyOpponentArrived[];
extern u8 MsgLobbyPleaseSpeakWhen[];

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
u32 State_RunQueryWithInterruptMasterSaved(void);
u32 GameFlag_SetByte(u32 flag, u8 value);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

/* FAKEMATCH: Typed inline calls preserve call-local constants and argument order. */
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

/* FAKEMATCH: The unused callback result keeps the last callee's r0 at return. */
s32 LinkLobby_RunBattleApplication(void)
{
    s32 answer;
    s32 msg;
    s32 line;

    msg = (s32)MsgLobbyPleaseSpeakWhen;
    Engine_EventBegin();
    Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
    if (LinkLobby_PeerSlotMatches(0) == 0) {
        Engine_TaskWait(1);
    }
    if (LinkLobby_PeerSlotMatches(0) == 0) {
        LinkLobby_WriteSlotValue(5);
        State_RunQueryWithInterruptMasterSaved();
        if (!Value1(Engine_GameFlagIsSet, 0x173)) {
            Engine_EventSetMessage(msg + 5);
            Engine_EventOpenMessage(8, 0);
            answer = Engine_EventChooseYesNo(0, 0);
            if (answer == 0) {
                GameFlag_SetByte(1000, 0);
                Engine_GameFlagSet(0x173);
                Engine_GameFlagClear(0x172);
                Engine_GameFlagClear(0x16c);
                Engine_GameFlagSet(0x202);
                /* FAKEMATCH: Select the shared-tail message before its answer store. */
                line = msg + 7;
                (*(union GameStateRows *)&gGameState).halves[341][0] = answer;
                goto message;
            } else {
                Call1(Engine_GameFlagClear, 0x173);
                Engine_GameFlagSet(0x16c);
                LinkLobby_WriteSlotValue(0);
                line = msg + 6;
                goto message;
            }
        } else {
            line = msg + 3;
            goto message;
        }
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x173)) {
            LinkLobby_WriteSlotValue(0);
            Engine_EventSetMessage((s32)MsgLobbyOpponentArrived);
            Engine_EventOpenMessage(8, 0);
            Engine_GameFlagClear(0x202);
            Call1(Engine_GameFlagClear, 0x173);
        }
        if (Engine_GameFlagIsSet(0x202)) {
            line = msg + 3;
            /* FAKEMATCH: the first branch's answers jump into this block's
             * message tail, and an accepted challenge back into its opening,
             * which keeps the reference's single copy of each. */
message:
            Engine_EventSetMessage(line);
open_message:
            Call2(Engine_EventOpenMessage, 8, 0);
            goto done;
        } else if (!Engine_GameFlagIsSet(0x201) && !Value1(Engine_GameFlagIsSet, 0x300)) {
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
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                if (LinkLobby_PeerSlotMatches(0)) {
                    Engine_GameFlagSet(0x16c);
                    Engine_GameFlagSet(0x172);
                    if (Engine_GameFlagIsSet(0x201)) {
                        Engine_EventSetMessage(msg + 3);
                    } else {
                        Engine_EventSetMessage(msg + 4);
                    }
                    LinkLobby_WriteSlotValue(1);
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
