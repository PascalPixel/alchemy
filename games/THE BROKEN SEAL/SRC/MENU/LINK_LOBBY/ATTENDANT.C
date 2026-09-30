#include "TYPES.H"
#include "CALL.H"
extern u8 MsgLobbyBattleArenaOld[];
extern u8 MsgLobbyChangeOrderParty[];
extern u8 MsgLobbyThreeAlliesFight[];
extern u8 MsgLobbyThreeAlliesFightLinked[];
extern u8 MsgLobbyThreeAlliesFightLinkedFinals[];
extern u8 MsgLobbyTravelingWarrior[];

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 mode);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
s32 Party_CountActiveOwners(void);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
/* FAKEMATCH: Engine_EventEnd is declared returning s32 so each handler
 * returns the last callee's r0, as the battle application does, and
 * the game state is read through a word view for the selected actor. */
s32 Engine_EventEnd(void);

extern union GameStateRows gGameState;

/* The three lobby regulars face the leader and speak a line that moves on
 * with the lobby's progress flags. */
s32 LinkLobby_TalkToAttendant(s32 actor)
{
    s32 message;
    s32 step = 0;

    Engine_EventBegin();
    switch (actor) {
    case 12:
        message = (s32)MsgLobbyTravelingWarrior;
        break;
    case 13:
        message = (s32)MsgLobbyThreeAlliesFight;
        break;
    case 14:
    default:
        message = (s32)MsgLobbyBattleArenaOld;
        break;
    }
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (Engine_GameFlagIsSet(0x304))
        step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
    Engine_EventSetMessage(message + step);
    Engine_EventOpenMessage(actor, 0);
    return Engine_EventEnd();
}

/* The attendant alternates between explaining linked finals and the party
 * order. */
s32 LinkLobby_TalkAlternating(s32 actor)
{
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (!Value1(Engine_GameFlagIsSet, 0x204)) {
        if (Party_CountActiveOwners() <= 3)
            message = (s32)MsgLobbyThreeAlliesFightLinkedFinals;
        else
            message = (s32)MsgLobbyThreeAlliesFightLinked;
        Engine_GameFlagSet(0x204);
    } else {
        message = (s32)MsgLobbyChangeOrderParty;
        Engine_GameFlagClear(0x204);
    }
    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(actor, 0);
    return Engine_EventEnd();
}
