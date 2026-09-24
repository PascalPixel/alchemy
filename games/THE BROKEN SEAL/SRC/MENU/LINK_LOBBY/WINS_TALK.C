#include "TYPES.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    u16 counts[512];
    s32 words[256];
};

struct LobbyActor {
    u8 unknown_00[6];
    u16 facing;
};

struct LobbyActor *Engine_ActorGet(s32 actor);
void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_EventSetMessage(s32 message);
void Main_08015118(void);
void Main_08015120(s32 value, s32 digits);
s32 Engine_EventShowMessage(s32 actor, s32 mode);

extern union GameStateRows gGameStateRows;
extern u8 Data_0000297b[], Data_0000297d[], Data_00002988[], Data_00002989[];

/* The attendant's win count: by the player's facing, total or consecutive linked wins; with none yet, the matching wait line, otherwise the count as a five-digit argument before the line. The empty case of the first branch shares the printing tail. */
s32 LinkLobby_TalkLinkedWins(s32 actor)
{
    u32 facing = Engine_ActorGet(0)->facing;
    u16 *count;
    s32 base;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameStateRows.words[125], 0);
    if ((u32)(facing - 0xa001) <= 0x3ffe) {
        base = (s32)Data_0000297b;
        count = &gGameStateRows.counts[342];
        if (*count == 0) {
            Engine_EventSetMessage((s32)Data_00002988);
            return Engine_EventShowMessage(actor, 0);
        }
    } else {
        base = (s32)Data_0000297d;
        count = &gGameStateRows.counts[345];
        if (*count == 0)
            goto none;
    }
    Main_08015118();
    Main_08015120(*count, 5);
    Engine_EventSetMessage(base + 1);
    return Engine_EventShowMessage(actor, 0);
none:
    Engine_EventSetMessage((s32)Data_00002989);
    return Engine_EventShowMessage(actor, 0);
}
