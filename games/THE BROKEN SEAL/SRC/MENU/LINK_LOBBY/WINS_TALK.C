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
void UiWork_ClearValueNameTables(void);
void UiText_DrawQuantity(s32 value, s32 digits);
s32 Engine_EventShowMessage(s32 actor, s32 mode);

extern union GameStateRows gGameState;

/* The attendant's win count: by the player's facing, total or consecutive linked wins; with none yet, the matching wait line, otherwise the count as a five-digit argument before the line. The empty case of the first branch shares the printing tail. */
s32 LinkLobby_TalkLinkedWins(s32 actor)
{
    u32 facing = Engine_ActorGet(0)->facing;
    u16 *count;
    s32 base;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.words[125], 0);
    if ((u32)(facing - 0xa001) <= 0x3ffe) {
        base = 0x297b;
        count = &gGameState.counts[342];
        if (*count == 0) {
            Engine_EventSetMessage(0x2988);
            return Engine_EventShowMessage(actor, 0);
        }
    } else {
        base = 0x297d;
        count = &gGameState.counts[345];
        if (*count == 0)
            goto none;
    }
    UiWork_ClearValueNameTables();
    UiText_DrawQuantity(*count, 5);
    Engine_EventSetMessage(base + 1);
    return Engine_EventShowMessage(actor, 0);
none:
    Engine_EventSetMessage(0x2989);
    return Engine_EventShowMessage(actor, 0);
}
