#include "TYPES.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

s32 Local_02000f30(s32 actor);
void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

extern union GameStateRows gGameStateRows;
extern u8 Data_0000294e[], Data_00002953[], Data_00002958[];
extern u8 Data_00002967[], Data_0000296c[], Data_00002971[], Data_00002976[];

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

/* A lobby actor's line, one per actor from each base: with flag 0x304 set, by flag 0x305 and the actor's own flag 0x2f0 + actor; otherwise by what Local_02000f30 reports for actor 0 and for this actor. */
s32 LinkLobby_TalkByProgress(s32 actor)
{
    s32 base = (s32)Data_0000294e;
    s32 lead = Local_02000f30(0);
    s32 own = Local_02000f30(actor);

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameStateRows.words[125], 0);
    if (Value1(Engine_GameFlagIsSet, 0x304)) {
        s32 seen;

        Value1(Engine_GameFlagIsSet, 0x2f0);
        seen = Engine_GameFlagIsSet(actor + 0x2f0);
        if (Value1(Engine_GameFlagIsSet, 0x305)) {
            if (seen)
                base = (s32)Data_00002967;
            else
                base = (s32)Data_0000296c;
        } else {
            if (seen)
                base = (s32)Data_00002971;
            else
                base = (s32)Data_00002976;
        }
    } else if (lead != 0) {
        if (own == 0)
            base = (s32)Data_00002953;
    } else {
        base = (s32)Data_00002958;
    }
    Engine_EventSetMessage(base + actor - 1);
    Engine_EventShowMessage(actor, 0);
    return Engine_EventEnd();
}
