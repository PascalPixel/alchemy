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
s32 Main_08077148(void);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

extern union GameStateRows gGameStateRows;
extern u8 Data_0000298c[], Data_0000298d[], Data_0000298e[];

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

/* A lobby actor who alternates lines: face the selected subject; with flag 0x204 clear, set it and say line 0x298d (Main_08077148 at most 3) or 0x298c; otherwise clear it and say 0x298e. */
s32 LinkLobby_TalkAlternating(s32 actor)
{
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameStateRows.words[125], 0);
    if (!Value1(Engine_GameFlagIsSet, 0x204)) {
        if (Main_08077148() <= 3)
            message = (s32)Data_0000298d;
        else
            message = (s32)Data_0000298c;
        Call1(Engine_GameFlagSet, 0x204);
    } else {
        message = (s32)Data_0000298e;
        Call1(Engine_GameFlagClear, 0x204);
    }
    Engine_EventSetMessage(message);
    Call2(Engine_EventOpenMessage, actor, 0);
    return Engine_EventEnd();
}
