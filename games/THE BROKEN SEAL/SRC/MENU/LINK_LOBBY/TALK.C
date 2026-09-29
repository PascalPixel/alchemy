#include "TYPES.H"
extern u8 MsgLobbyTryingGetAway[];
extern u8 MsgLobbyCantWaitLets[];
extern u8 MsgLobbyGoingFightAlone[];
extern u8 MsgLobbyKnewLostBecause[];
extern u8 MsgLobbyNotFaultLost[];
extern u8 MsgLobbyWonWithoutWell[];
extern u8 MsgLobbyWonToldCountOn[];

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

s32 LinkLobby_PartyContains(s32 actor);
void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

extern union GameStateRows gGameState;

/* FAKEMATCH: flag tests through an inline wrapper keep each constant
   flag number its own load instead of one shared register. */
static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

/* A lobby actor's line, one per actor from each base: with flag 0x304 set, by flag 0x305 and the actor's own flag 0x2f0 + actor; otherwise by what LinkLobby_PartyContains reports for actor 0 and for this actor. */
s32 LinkLobby_TalkByProgress(s32 actor)
{
    s32 base = (s32)MsgLobbyCantWaitLets;
    s32 lead = LinkLobby_PartyContains(0);
    s32 own = LinkLobby_PartyContains(actor);

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (Value1(Engine_GameFlagIsSet, 0x304)) {

        Value1(Engine_GameFlagIsSet, 0x2f0);
        base = Engine_GameFlagIsSet(actor + 0x2f0);
        if (Value1(Engine_GameFlagIsSet, 0x305)) {
            if (base)
                base = (s32)MsgLobbyWonToldCountOn;
            else
                base = (s32)MsgLobbyWonWithoutWell;
        } else {
            if (base)
                base = (s32)MsgLobbyNotFaultLost;
            else
                base = (s32)MsgLobbyKnewLostBecause;
        }
    } else if (lead != 0) {
        if (own == 0)
            base = (s32)MsgLobbyGoingFightAlone;
    } else {
        base = (s32)MsgLobbyTryingGetAway;
    }
    Engine_EventSetMessage(base + actor - 1);
    Engine_EventShowMessage(actor, 0);
    return Engine_EventEnd();
}
