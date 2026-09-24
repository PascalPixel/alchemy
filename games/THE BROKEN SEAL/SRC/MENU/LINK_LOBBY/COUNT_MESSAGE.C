#include "TYPES.H"

extern u8 Data_02000240[];

void Engine_EventBegin();
void Engine_ActorFaceActor();
void Main_08015120();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
s32 Engine_EventEnd();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Link lobby: face the speaker and report the stored count, or the empty
 * message when there is none. */
s32 LinkLobby_ShowCountMessage(s32 id)
{
    u8 *gs;

    Engine_EventBegin();
    gs = Data_02000240;
    Call3(Engine_ActorFaceActor, id, *(s32 *)(gs + 500), 0);
    if (*(u16 *)(gs + 680) != 0) {
        Main_08015120(*(u16 *)(gs + 680), 5);
        Engine_EventSetMessage(0x298a);
    } else {
        Engine_EventSetMessage(0x298b);
    }
    Engine_EventOpenMessage(id, 0);
    return Engine_EventEnd();
}
