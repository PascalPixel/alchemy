/* Draft of LinkLobby_TalkByProgress, resource_3cb at 0x02008f8c (was
 * MENU/LINK_LOBBY/PROGRESS_TALK.C).
 * Remaining difference: each branch's message number is a C constant GCC
 * hoists above its test; the ROM loads it after, as a link-time value.
 * The listing keeps these rows. */
#include "TYPES.H"

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

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

/* A lobby actor's line, one per actor from each base: with flag 0x304 set, by flag 0x305 and the actor's own flag 0x2f0 + actor; otherwise by what LinkLobby_PartyContains reports for actor 0 and for this actor. */
s32 LinkLobby_TalkByProgress(s32 actor)
{
    s32 base = 0x294e;
    s32 lead = LinkLobby_PartyContains(0);
    s32 own = LinkLobby_PartyContains(actor);

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if (Value1(Engine_GameFlagIsSet, 0x304)) {
        s32 seen;

        Value1(Engine_GameFlagIsSet, 0x2f0);
        seen = Engine_GameFlagIsSet(actor + 0x2f0);
        if (Value1(Engine_GameFlagIsSet, 0x305)) {
            if (seen)
                base = 0x2967;
            else
                base = 0x296c;
        } else {
            if (seen)
                base = 0x2971;
            else
                base = 0x2976;
        }
    } else if (lead != 0) {
        if (own == 0)
            base = 0x2953;
    } else {
        base = 0x2958;
    }
    Engine_EventSetMessage(base + actor - 1);
    Engine_EventShowMessage(actor, 0);
    return Engine_EventEnd();
}
