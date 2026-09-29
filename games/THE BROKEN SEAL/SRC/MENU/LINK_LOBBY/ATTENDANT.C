#include "TYPES.H"
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
s32 Engine_EventEnd(void);

extern union GameStateRows gGameState;

/* FAKEMATCH: calls spelled through these wrappers pass their constants
 * straight into the argument registers. */
static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(s32), s32 a0)
{
    f(a0);
}

static __inline__ s32 Call2(s32 (*f)(s32, s32), s32 a0, s32 a1)
{
    return f(a0, a1);
}

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
    Call2(Engine_EventOpenMessage, actor, 0);
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
        Call1(Engine_GameFlagSet, 0x204);
    } else {
        message = (s32)MsgLobbyChangeOrderParty;
        Call1(Engine_GameFlagClear, 0x204);
    }
    Engine_EventSetMessage(message);
    Call2(Engine_EventOpenMessage, actor, 0);
    return Engine_EventEnd();
}
