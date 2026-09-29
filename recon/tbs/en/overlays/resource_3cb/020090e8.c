/* Draft of LinkLobby_TalkAlternating, resource_3cb at 0x020090e8 (was
 * MENU/LINK_LOBBY/ALTERNATE_TALK.C).
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
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
s32 Party_CountActiveOwners(void);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

extern union GameStateRows gGameState;

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

/* A lobby actor who alternates lines: face the selected subject; with flag 0x204 clear, set it and say line 0x298d (Party_CountActiveOwners at most 3) or 0x298c; otherwise clear it and say 0x298e. */
s32 LinkLobby_TalkAlternating(s32 actor)
{
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (!Value1(Engine_GameFlagIsSet, 0x204)) {
        if (Party_CountActiveOwners() <= 3)
            message = 0x298d;
        else
            message = 0x298c;
        Call1(Engine_GameFlagSet, 0x204);
    } else {
        message = 0x298e;
        Call1(Engine_GameFlagClear, 0x204);
    }
    Engine_EventSetMessage(message);
    Call2(Engine_EventOpenMessage, actor, 0);
    return Engine_EventEnd();
}
