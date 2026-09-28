/* Draft of LinkLobby_TalkToAttendant, resource_3cb at 0x0200906c (was
 * MENU/LINK_LOBBY/ATTENDANT_TALK.C).
 * Remaining difference: each branch's message number is a C constant GCC
 * hoists above its test; the ROM loads it after, as a link-time value.
 * The listing keeps these rows. */
#include "TYPES.H"

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
        message = 0x297f;
        break;
    case 14:
    default:
        message = 0x2982;
        break;
    }
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (Engine_GameFlagIsSet(0x304))
        step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
    Engine_EventSetMessage(message + step);
    Call2(Engine_EventOpenMessage, actor, 0);
    return Engine_EventEnd();
}
