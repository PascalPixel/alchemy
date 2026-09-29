/* Draft of resource_3cb 0x0200906c (LinkLobby_TalkToAttendant): it matches
 * the ROM byte for byte now that the messages it loads from the literal pool
 * have catalogue names (MsgLobbyBattleArenaOld, MsgLobbyThreeAlliesFight).
 * The listing keeps these rows until the draft is adopted. */
#include "TYPES.H"
extern u8 MsgLobbyBattleArenaOld[];
extern u8 MsgLobbyThreeAlliesFight[];

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 mode);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

extern union GameStateRows gGameState;

static __inline__ s32 Call2(s32 (*f)(s32, s32), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Link lobby attendants: face the selected subject and say the actor's line, advanced by flags 0x304 and 0x305. */
s32 LinkLobby_TalkToAttendant(s32 actor)
{
    s32 message;
    s32 step = 0;

    Engine_EventBegin();
    switch (actor) {
    case 12:
        message = 0x2985;
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
