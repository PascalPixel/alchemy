#include "TYPES.H"
#include "KANPAN.H"
#include "FIELD_EVENT.H"
extern u8 FuneKanpan_CrewScriptE[];
extern struct MapRenderWork *gMapWork;

void Engine_EventBegin();
void Event_CallWithLastActiveObjectId();
void Engine_TaskWait();
void Engine_ActorDestroy();
void Engine_ActorSetPosition();
s32 Engine_GameFlagIsSet();
void FuneKanpan_RunRobinTalk();
void Engine_EventEnd();

/* Ship deck: place actors 21 to 23 for the crossing, facing them by flag
 * 0x903, and close the encounter when the state is 6. */
void FuneKanpan_PlaceDeckActors(s32 a0, s32 a1)
{
    s32 record;
    s32 v5;

    *(s32 *)(*(s32 *)&gMapWork + 236) = 0x410000;
    Engine_EventBegin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScriptE);
    Engine_TaskWait(1);
    Engine_ActorDestroy(24);
    Call3(Engine_ActorSetPosition, 23, 0xee0000, 0x2720000);
    v5 = 192;
    record = (s32)Engine_ActorGet(23);
    *(u16 *)(record + 6) = (v5 << 6);
    if (Value1(Engine_GameFlagIsSet, 0x903) != 0) {
        Call3(Engine_ActorSetPosition, 22, 0xa20000, 0x27a0000);
        record = (s32)Engine_ActorGet(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa20000, 0x2a40000);
        record = (s32)Engine_ActorGet(21);
        {
            s32 facing = 0xd000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            *(u16 *)(record + 6) = facing;
        }
    } else {
        Call3(Engine_ActorSetPosition, 22, 0xa00000, 0x28c0000);
        record = (s32)Engine_ActorGet(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa60000, 0x29c0000);
        record = (s32)Engine_ActorGet(21);
        {
            s32 facing = 0xb000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            *(u16 *)(record + 6) = facing;
        }
    }
    if (gGameState.entrance == 6) {
        FuneKanpan_RunRobinTalk();
    }
    Engine_EventEnd();
}
