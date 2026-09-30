#include "TYPES.H"
extern u8 MsgLobbyCantWaitSee[];
extern u8 MsgLobbyWonNumberConsecutive[];

extern u8 gGameState[];

void Engine_EventBegin();
void Engine_ActorFaceActor();
void UiText_DrawQuantity();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
s32 Engine_EventEnd();

/* Link lobby: face the speaker and report the stored count, or the empty
 * message when there is none. */
s32 LinkLobby_ShowCountMessage(s32 id)
{
    u8 *gs;

    Engine_EventBegin();
    gs = gGameState;
    Engine_ActorFaceActor(id, *(s32 *)(gs + 500), 0);
    if (*(u16 *)(gs + 680) != 0) {
        UiText_DrawQuantity(*(u16 *)(gs + 680), 5);
        Engine_EventSetMessage((s32)MsgLobbyWonNumberConsecutive);
    } else {
        Engine_EventSetMessage((s32)MsgLobbyCantWaitSee);
    }
    Engine_EventOpenMessage(id, 0);
    return Engine_EventEnd();
}
